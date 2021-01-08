/*
    Program to test the PID control
*/

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>


#include <franka_gripper/GraspAction.h>
#include <franka_gripper/HomingAction.h>
#include <franka_gripper/MoveAction.h>
#include <franka_gripper/StopAction.h>
#include <actionlib/client/simple_action_client.h>

#include <MyFunc.h>




void fo_F101_cb(const std_msgs::Float32MultiArray& msg);
void fo_F102_cb(const std_msgs::Float32MultiArray& msg);
void PID_cb(const std_msgs::Float32MultiArray& msg);
void Limit_cb(const std_msgs::Float32MultiArray& msg);

float UpdatePID(float ref_value, float actual_value, float Kp, float Ki, float Kd, float& integral, float dt, float& pre_error, float zero_width, int verse);
float CheckBounds(float max, float min, float output);

std_msgs::Float32MultiArray fo_F101_msg, fo_F102_msg;
std_msgs::Float32MultiArray new_pid_msg;
std_msgs::Float32MultiArray new_limit_msg;

float P = 0.01;
float I = 0.0;
float D = 0.0;
float reference = 0;
float upper_bound = 0.02;
float lower_bound = - 0.02;

float integral = 0;
float offset = 0.0;
float actual_pos;
float old_pos;
float delta_time = 1.0/100.0;

float pre_error = 0;
float actual_value;



int main(int argc, char** argv)
{
    ros::init(argc, argv, "ol_tes2t2");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(1);
    spinner.start();

    ros::Publisher cmd_pub      = nh.advertise<std_msgs::Float32>("/gripper_state_request",100);
    ros::Publisher width_pub    = nh.advertise<std_msgs::Float32>("/gripper_width_request", 100);
    ros::Publisher fake_cart_pub = nh.advertise<std_msgs::Float32MultiArray>("/Fake_Cartesian_vel",100);
    ros::Publisher fake_joint_pub   = nh.advertise<std_msgs::Float32MultiArray>("Fake_joint_vel",100);

    ros::Subscriber new_PID_sub     = nh.subscribe("/new_pid_value", 1, PID_cb);
    ros::Subscriber new_limits_sub  = nh.subscribe("new_limit", 1, Limit_cb);

    ros::Subscriber fo_F101_sub = nh.subscribe("/first_order_params_F101", 100, fo_F101_cb);
    ros::Subscriber fo_F102_sub = nh.subscribe("/first_order_params_F102", 100, fo_F102_cb);

    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;
    std_msgs::Float32MultiArray fake_cart_vel_msg;
    std_msgs::Float32MultiArray fake_joint_vel_msg;
    fake_cart_vel_msg.data.resize(6);
    fake_joint_vel_msg.data.resize(7);

    new_pid_msg.data.resize(3);
    new_limit_msg.data.resize(2);
    fo_F101_msg.data.resize(2);
    fo_F102_msg.data.resize(2);


    namespace rvt = rviz_visual_tools;


    actionlib::SimpleActionClient<franka_gripper::MoveAction> move_client("/franka_gripper/move", true);
    franka_gripper::MoveGoal move_goal;
    move_goal.speed = 0.02;
    move_goal.width = 0.08;
    
    std::vector<double> finger_width;
    finger_width.resize(2);
    float user_input;

    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();


    ROS_INFO("Reach Ready Position");
    move_group.setMaxVelocityScalingFactor(0.2);
    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();


    geometry_msgs::Pose target_pose;
    target_pose.position.x = 0.260;
    target_pose.position.y = 0.405 ;
    target_pose.position.z = 0.4;
    tf2::Quaternion quat;
    quat.setRPY(M_PI,0, -M_PI/4);
    target_pose.orientation = tf2::toMsg(quat);

    visual_tools.prompt("Next to move to stating pose");
    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setPoseTarget(target_pose);
    move_group.move();

    visual_tools.prompt("Next to close the gripper");
    move_goal.width = 0.003;
    move_client.sendGoal(move_goal);

    ROS_INFO("Start PID controller");
    

    fake_cart_vel_msg.data[0] = 0.025;
    for(int i=1; i<6; i++)
        fake_cart_vel_msg.data[i] = 0.0;
    reference = 0.0;
    float wire_pos;


    Eigen::MatrixXd jacobian_inv;
    Eigen::Matrix<double,6,1> cartesian_vel;
    Eigen::Matrix<double,7,1> joint_vel;

    // std::array<double, 42> jacobian_array = model_handle_->getZeroJacobian(franka::Frame::kEndEffector);
    // Eigen::Map<Eigen::Matrix<double, 6, 7>> jacobian(jacobian_array.data());

    while(true)
    {
        wire_pos = (fo_F101_msg.data[1] - fo_F102_msg.data[1])/2;
        std::cout << "Wire distance from central position " << wire_pos << "\n";
        actual_value = wire_pos;    
        
        fake_cart_vel_msg.data[2] = UpdatePID(reference, actual_value, P, I, D, integral, delta_time, pre_error, offset, true);
        fake_cart_vel_msg.data[2] = CheckBounds(upper_bound, lower_bound, fake_cart_vel_msg.data[2]);
        fake_cart_pub.publish(fake_cart_vel_msg);
        /*
        for(int i=0; i<6; i++)
            cartesian_vel(i) = fake_cart_vel_msg.data[i];

        pseudoInverse(jacobian, jacobian_inv)
        joint_vel = jacobbian_inv*cartesian_vel;

        for(int i=0; i<7; i++)
            fake_joint_vel_msg.data[i] = joint_vel(i);

        fake_joint_pub.publish(fake_joint_vel_msg);
        */


    }

    ros::shutdown();
    return 0;
}

void fo_F101_cb(const std_msgs::Float32MultiArray& msg)
{
    for(int i=0; i<2; i++)
        fo_F101_msg.data[i] = msg.data[i];
}
void fo_F102_cb(const std_msgs::Float32MultiArray& msg)
{
    for(int i=0; i<2; i++)
        fo_F102_msg.data[i] = msg.data[i];
}

void PID_cb(const std_msgs::Float32MultiArray& msg)
{
    if(msg.data[0]==0 && msg.data[1] == 0 && msg.data[2] == 0)
    {
        ROS_INFO("PID reset");
        pre_error = 0;
        integral = 0;
        
    }
    else
    {
        P = msg.data[0];
        I = msg.data[1];
        D = msg.data[2];
    }

}
void Limit_cb(const std_msgs::Float32MultiArray& msg)
{
    upper_bound = msg.data[0];
    lower_bound = msg.data[1];
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
    float output = zero_width - (Pout + Iout + Dout);

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
