#include <ros/init.h>
#include <ros/node_handle.h>
#include <ros/rate.h>
#include <ros/subscriber.h>

#include <std_msgs/Float32.h>
#include <std_msgs/String.h>

#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/WrenchStamped.h>
#include <geometry_msgs/Wrench.h>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_visual_tools/moveit_visual_tools.h>

#include <schunk_pg70/set_position.h>

#include <iostream>
#include <stdio.h>

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion

#include <gripper_control/ControlOutput.h>


#define WAIT 0
#define MOVE_TO_START 1
#define MAN 2
#define PID 3
#define PIDY 9
#define RESET 10

float target_width = 0.08;
float old_target_width =  0.08;
bool reset = false;

int start = 0;

float P;
float I;
float D;
float reference;
float velocity;
float upper_bound;
float lower_bound;
float integral = 0;
float offset = 0.002;
float actual_pos;
float old_pos;
float tactile_ref;

int rate = 50; // franka state controller loop at 500
int verse = 1;
float delta_time = (float) 1/rate;
int control_state = WAIT;

int filter_size = 20;
int filter_index = 0;
std::vector<geometry_msgs::WrenchStamped> Mean_window;

float error = 0;
float pre_error = 0;

std::string TopicName;

std::vector<double> joint_value;
std::vector<float> joint_value_f;

geometry_msgs::WrenchStamped Wrench_ext;

float tactile_feedback;
float new_target_width = 60;

gripper_control::ControlOutput control_msg;

int loop_counter = 0;

void ReadWrench_cb( const geometry_msgs::WrenchStamped &msg);
void ReadStatus_cb(const sensor_msgs::JointState &msg);
void ReadTactile_cb(const std_msgs::Float32 &msg);
void new_width_cb(const std_msgs::Float32 &msg);
void new_command_cb(const std_msgs::Float32 &msg);

// Gripper orientation
Eigen::Matrix<double, 3, 3> printEE(robot_state::RobotStatePtr kinematic_state);
geometry_msgs::WrenchStamped ShiftWrench(Eigen::Matrix<double, 3, 3> EE_orientation, geometry_msgs::WrenchStamped Wrench);
geometry_msgs::Quaternion ConvertOrientation(geometry_msgs::Quaternion orientation, Eigen::Matrix<double,3,3> EE_Vector);

// PID Function
void FilterInit(std::vector<geometry_msgs::WrenchStamped>& MeanWindow, int filter_size, gripper_control::ControlOutput& control_msg);
float UpdatePID(float ref_value, float actual_value, float Kp, float Ki, float Kd, float& integral, float dt, float& pre_error, float zero_width, int verse);
float CheckBounds(float max, float min, float output);


int main(int argc, char** argv)
{
    ros::init(argc, argv, "GripperClient_Node");

    ros::NodeHandle n;

    n.getParam("/GripperControl/P", P);
    n.getParam("/GripperControl/I", I);
    n.getParam("/GripperControl/D", D);
    n.getParam("/GripperControl/velocity", velocity);
    n.getParam("/GripperControl/reference", reference);
    n.getParam("/GripperControl/upper_bound", upper_bound);
    n.getParam("/GripperControl/lower_bound", lower_bound);
    n.getParam("/GripperControl/offset", offset);
    n.getParam("/GripperControl/filter_size", filter_size);
    n.getParam("/GripperControl/control_topic", TopicName); 
    n.getParam("/GripperControl/tactile_ref", tactile_ref);    

 //   reference = tactile_ref;        // !!


    joint_value.resize(7);
    joint_value_f.resize(7);

    ros::Subscriber GripperPos_sub  = n.subscribe("/joint_states", 100, ReadStatus_cb);
    ros::Subscriber Wrench_sub      = n.subscribe(TopicName, 1, ReadWrench_cb);
    ros::Subscriber user_sub        = n.subscribe("/gripper_width_request", 100, new_width_cb);
    ros::Subscriber command_sub     = n.subscribe("/gripper_state_request", 100, new_command_cb);
    ros::Subscriber tactile_sub     = n.subscribe("/TactileFeedback", 100, ReadTactile_cb);    

    ros::Publisher control_output_pub = n.advertise<gripper_control::ControlOutput>("/gripper_control/ControlOutput", 100);
    ros::Publisher new_wrench_pub = n.advertise<geometry_msgs::WrenchStamped>("/new_wrench", 100);

    ros::ServiceClient set_position = n.serviceClient<schunk_pg70::set_position>("schunk_pg70/set_position");     

    schunk_pg70::set_position schunk_pos_srv;
    schunk_pos_srv.request.goal_position = 60; // 69
    schunk_pos_srv.request.goal_velocity = 60; // 82
    schunk_pos_srv.request.goal_acceleration = 200; // 320
    

    FilterInit(Mean_window, filter_size, control_msg);
    control_output_pub.publish(control_msg);

    ROS_INFO("Init completed");

    ros::Rate loop_rate(rate);

    float actual_value = reference;


    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");

    
    namespace rvt = rviz_visual_tools;

    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
    const robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("panda_arm");
    const std::vector<std::string> &joint_names = joint_model_group->getJointModelNames();
    const Eigen::Isometry3d& end_effector_state = kinematic_state->getGlobalLinkTransform("panda_link8");
   
    geometry_msgs::PoseStamped EE_pose;
    EE_pose.pose.position.x = 1.3;
    EE_pose.pose.position.y = 0;
    EE_pose.pose.position.z = 0;

    geometry_msgs::Pose EE;
    EE.position.x = 1;
    EE.position.y = 0;
    EE.position.z = 0;
    EE.orientation.x = 1;
    EE.orientation.y = 0;
    EE.orientation.z = 0;
    EE.orientation.w =0;

    Eigen::Matrix<double, 3, 3> EE_Vector;

    geometry_msgs::WrenchStamped New_Wrench;

    while(ros::ok())
    {   
     //   joint_value =  move_group.getCurrentJointValues();
        //for(int k=0; k<7; k++)
         //   joint_value_f[k] = (float)joint_value[k];

       // EE_pose = move_group.getCurrentPose("panda_link8");
        // EE_pose = ForwardKinematics(joint_value_f);
        // std::cout << "EE state : " << EE_pose.pose.orientation << "\n";

       // kinematic_state->setJointGroupPositions(joint_model_group, joint_value);
       // EE_Vector = printEE(kinematic_state);
        // New_Wrench = ShiftWrench(EE_Vector,Wrench_ext);
        // EE.orientation = ConvertOrientation(EE_pose.pose.orientation, EE_Vector);

       // std::cout << New_Wrench << "\n";
        //new_wrench_pub.publish(New_Wrench);

        /*
        visual_tools.deleteAllMarkers();
        visual_tools.publishAxisLabeled(EE_pose.pose, "pt" , rvt::XXXSMALL);
        visual_tools.publishAxisLabeled(EE, "pt" , rvt::XXXSMALL);
        visual_tools.trigger();
        */

        switch (control_state)
        {
        
        case WAIT:
        {
            ROS_INFO("Gripper Initialization");
            std::cout << "P = " << P << "\n" << " I = " << I << "\n" << " D = " << D << "\n" ;
            std::cout << "Target reference = " << reference << "\n";
            integral = 0;
            pre_error = 0;
            ROS_INFO("Initialization OK: Wait for instruction");
            control_state = MAN;
            break;
        }

        case MAN:
        {
            
            if(new_target_width != target_width)
            {

                target_width = new_target_width;
                schunk_pos_srv.request.goal_position = target_width*1000;
                set_position.call(schunk_pos_srv);

                control_msg.output = schunk_pos_srv.request.goal_position;
                control_msg.output_sat = schunk_pos_srv.request.goal_position;      // position of gripper
            }
            break;
        }

        case PID:
        {
            
            actual_value = Wrench_ext.wrench.force.x;       // control variable value
           // actual_value = tactile_feedback;
            
            control_msg.actual_value = actual_value;

            target_width = UpdatePID(reference, actual_value, P, I, D, integral, delta_time, pre_error, offset, verse);
            control_msg.output = target_width*1000;
            target_width = CheckBounds(upper_bound, lower_bound, target_width);

            control_msg.error = pre_error;
            control_msg.output_sat = target_width*1000;
            control_msg.gripper_position = actual_pos*1000;      // position of gripper

            schunk_pos_srv.request.goal_position = target_width*1000;
            set_position.call(schunk_pos_srv);
    
            break;
        }

        case PIDY:
        {
            actual_value = -Wrench_ext.wrench.force.y;       // control variable value
            //actual_value = tactile_feedback;

            control_msg.actual_value = actual_value;

            target_width = UpdatePID(reference, actual_value, P, I, D, integral, delta_time, pre_error, offset, verse);
            control_msg.output = target_width*1000;
            target_width = CheckBounds(upper_bound, lower_bound, target_width);

            control_msg.error = pre_error;
            control_msg.output_sat = target_width*1000;
            control_msg.gripper_position = actual_pos*1000;      // position of gripper
            control_output_pub.publish(control_msg);

            schunk_pos_srv.request.goal_position = target_width*1000;
            set_position.call(schunk_pos_srv);
        
            break;
        }

        case RESET:
        {
            integral = 0;
            pre_error = 0;
            control_state = MAN;
            new_target_width = 50;
            break;
        }

        default:
            break;
        } 
        
        control_msg.gripper_position = actual_pos*1000;
        control_output_pub.publish(control_msg);
        ros::spinOnce();
        loop_rate.sleep();
        
    }

    ros::shutdown();
    return 1;
}

void FilterInit(std::vector<geometry_msgs::WrenchStamped>& MeanWindow, int filter_size, gripper_control::ControlOutput& control_msg)
{
    geometry_msgs::Wrench Null_Wrench;

    Null_Wrench.force.x = 0.0;
    Null_Wrench.force.y = 0.0;
    Null_Wrench.force.z = 0.0;

    Null_Wrench.torque.x = 0.0;
    Null_Wrench.torque.y = 0.0;
    Null_Wrench.torque.z = 0.0;

    MeanWindow.resize(filter_size);

    for( int i=0; i< filter_size; i++)
        MeanWindow[i].wrench = Null_Wrench;

    control_msg.output              = 0;
    control_msg.output_sat          = 0;
    control_msg.gripper_position    = actual_pos;
    control_msg.error               = 0;
    control_msg.reference           = reference;
    control_msg.upper_bound         = upper_bound* 1000;
    control_msg.lower_bound         = lower_bound* 1000;
    control_msg.actual_value        = 0;
}

float UpdatePID(float ref_value, float actual_value, float Kp, float Ki, float Kd, float& integral, float dt, float& pre_error, float zero_width, int verse)
{
    // Calculate error
    float error = ref_value - actual_value;
    float Pout = Kp * error;
    integral += error * dt;
    float Iout = Ki * integral;
    float derivative = (error - pre_error) / dt;
    float Dout = Kd * derivative;

    // Calculate total output
    float output = zero_width - verse*(Pout + Iout + Dout);

    pre_error = error;

    return output;
}

float CheckBounds(float max, float min, float output)
{
    // Restrict to max/min
    if( output > max )
        output = max;
    else if(output < min )
        output = min;
    return output;
}

void ReadWrench_cb( const geometry_msgs::WrenchStamped &msg)
{
    Wrench_ext.wrench.force.x = msg.wrench.force.x ;
    Wrench_ext.wrench.force.y = msg.wrench.force.y;
    Wrench_ext.wrench.force.z = msg.wrench.force.z;
    Wrench_ext.wrench.torque.x = msg.wrench.torque.x;
    Wrench_ext.wrench.torque.y = msg.wrench.torque.y;
    Wrench_ext.wrench.torque.z = msg.wrench.torque.z;
}

void ReadStatus_cb(const sensor_msgs::JointState &msg)
{
    for(int i=0; i<7; i++)
        joint_value[i] = msg.position[i];

    old_pos = actual_pos;
    actual_pos = msg.position[7] + msg.position[8];
    
}

void ReadTactile_cb(const std_msgs::Float32 &msg)
{
    tactile_feedback = msg.data;
}

void new_width_cb(const std_msgs::Float32 &msg)
{
    ROS_INFO("Message received");
    new_target_width = msg.data;
    start = 1;
}  

void new_command_cb(const std_msgs::Float32 &msg)
{
    ROS_INFO("Message received");
    if (msg.data == 1)
    {
        error = 0;
        pre_error = 0;
        control_state = PID;
        ROS_INFO("Switch state to PID controller");
    }

    if(msg.data == 2)
    {
        control_state = RESET;
        ROS_INFO("Switch to wait instruction");
    }

    if(msg.data == 4)
    {
        loop_counter = 0;
        control_state = PIDY;
        ROS_INFO("Switch state to PID controller");
    }
}

Eigen::Matrix<double, 3, 3> printEE(robot_state::RobotStatePtr kinematic_state)
{
    const Eigen::Isometry3d& end_effector_state = kinematic_state->getGlobalLinkTransform("panda_link8");
    Eigen::Matrix<double, 3, 3> RotationMatrix = end_effector_state.rotation();

   // ROS_INFO_STREAM("Rotation: " << end_effector_state.rotation());
    //ROS_INFO_STREAM("Rotation: " << RotationMatrix );

    return RotationMatrix;
}

geometry_msgs::WrenchStamped ShiftWrench(Eigen::Matrix<double, 3, 3> EE_orientation, geometry_msgs::WrenchStamped Wrench)
 {
    geometry_msgs::WrenchStamped New_Wrench;
    tf2::Quaternion quat_shift, quat, quat_fine;
    tf2::Matrix3x3 R, R_gripper, Rf;

    Eigen::Matrix<double, 3, 3> EE_f;

    for(int i=0; i<3;i++)
    {
        for(int k=0;k<3;k++)
        {
            R_gripper[i][k] = EE_orientation(i,k);
        }
    }
    R_gripper.getRotation(quat);

    quat_shift.setRPY(M_PI, 0 , -M_PI/4);  
    quat_fine = quat*quat_shift;

    Rf.setRotation(quat_fine);


    //Rf = R_gripper*R;

    for(int i=0; i<3;i++)
    {
        for(int k=0;k<3;k++)
        {
            EE_f(i,k) = Rf[i][k];
        }
    }

    New_Wrench = Wrench;

    New_Wrench.wrench.force.x = Wrench.wrench.force.x*EE_f(0,0) + Wrench.wrench.force.y*EE_f(0,1) + Wrench.wrench.force.z*EE_f(0,2);
    New_Wrench.wrench.force.y = Wrench.wrench.force.x*EE_f(1,0) + Wrench.wrench.force.y*EE_f(1,1) + Wrench.wrench.force.z*EE_f(1,2);
    New_Wrench.wrench.force.z = Wrench.wrench.force.x*EE_f(2,0) + Wrench.wrench.force.y*EE_f(2,1) + Wrench.wrench.force.z*EE_f(2,2);


/*
    New_Wrench.wrench.force.x = Wrench.wrench.force.x*EE_orientation(0,0) + Wrench.wrench.force.y*EE_orientation(0,1) + Wrench.wrench.force.z*EE_orientation(0,2);
    New_Wrench.wrench.force.y = Wrench.wrench.force.x*EE_orientation(1,0) + Wrench.wrench.force.y*EE_orientation(1,1) + Wrench.wrench.force.z*EE_orientation(1,2);
    New_Wrench.wrench.force.z = Wrench.wrench.force.x*EE_orientation(2,0) + Wrench.wrench.force.y*EE_orientation(2,1) + Wrench.wrench.force.z*EE_orientation(2,2);
*/
     return New_Wrench;

 }

geometry_msgs::Quaternion ConvertOrientation(geometry_msgs::Quaternion orientation, Eigen::Matrix<double,3,3> EE_Vector)
{
    geometry_msgs::Quaternion new_orient;
    tf2::Quaternion quat, quat_shift;

    tf2::Matrix3x3 R, R_shift_base;

    quat_shift.setRPY(M_PI, 0 , -M_PI/4);

    for(int i=0; i<3;i++)
    {
        for(int k=0;k<3;k++)
        {
            R[i][k] = EE_Vector(i,k);
        }
    }

    R.getRotation(quat);

    quat = quat*quat_shift;
    quat.normalize();
    new_orient = tf2::toMsg(quat);
    return new_orient;
}
