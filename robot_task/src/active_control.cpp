#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

#include <controller_manager_msgs/SwitchController.h>

#include <std_msgs/Float32.h>
#include <sensor_msgs/JointState.h>
#include <PID.h>
#include <MyFunc.h>


#define INPUT_RPY 1

geometry_msgs::Pose CheckEEPosition(robot_state::RobotStatePtr& kinematic_state);
bool CheckExitCondition(float distance, geometry_msgs::Pose start_pose, geometry_msgs::Pose EE_Pos);


// Callback
void fo_F101_callback(const std_msgs::Float32MultiArray& msg);
void fo_F102_callback(const std_msgs::Float32MultiArray& msg);
void so_F101_callback(const std_msgs::Float32MultiArray& msg);
void so_F102_callback(const std_msgs::Float32MultiArray& msg);
void joint_callback(const sensor_msgs::JointState& data);

std_msgs::Float32MultiArray fo_F101_msg, fo_F102_msg, so_F101_msg, so_F102_msg;

float UpdatePID(float ref_value, float actual_value, float Kp, float Ki, float Kd, float& integral, float dt, float& pre_error, float zero_width, int verse);
float CheckBounds(float max, float min, float output);

std::vector<double> joint_value;


int main(int argc, char** argv)
{
    ros::init(argc, argv, "Active_controller");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(1);
    spinner.start();

    joint_value.resize(7);

    fo_F101_msg.data.resize(2);
    fo_F102_msg.data.resize(2);
    so_F101_msg.data.resize(3);
    so_F102_msg.data.resize(3);

    ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_state_request",100);
    ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/gripper_width_request", 100);

    ros::Publisher vel_pub = nh.advertise<std_msgs::Float32MultiArray>("/cartesian_velocity_output", 1);
    ros::Publisher vel_filter_pub = nh.advertise<std_msgs::Float32MultiArray>("/cartesian_velocity_request", 1);

    ros::Subscriber joint_pos_sub = nh.subscribe("/franka_state_controller/joint_states", 10, joint_callback);
    sensor_msgs::JointState joint_msg;
    joint_msg.position.resize(7);



    //ros::Publisher vel_n = nh.advertise<std_msgs::Float32>("/cartesian_velocity_normal", 1);

    ros::Subscriber fo_F101_sub = nh.subscribe("/first_order_params_F101", 100, fo_F101_callback);
 //   ros::Subscriber fo_F102_sub = nh.subscribe("/first_order_params_F102", 100, fo_F102_callback);

//    ros::Subscriber so_F101_sub = nh.subscribe("/second_order_params_F101", 1, so_F101_callback);
//    ros::Subscriber so_F102_sub = nh.subscribe("/second_order_params_F102", 1, so_F102_callback);
    
    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;
    std_msgs::Float32MultiArray vel_msg, vel_filter_msg;
    std_msgs::Float32 vel_n_msg;

    namespace rvt = rviz_visual_tools;

    // PARAMEERS READING FROM LAUNH FILE 


    std::string path;
    float P, I, D;
    float reference, upper_bound, lower_bound;
    float integral = 0;
    float offset = 0.0;
    float actual_pos;
    float old_pos;
    float delta_time = 1.0/100.0;
    float pre_error = 0;
    float actual_value;

    int rate;


    if (not (nh.getParam("/Active/PathName", path)))    path = "/home/panda/ros/Franka-Kev/src/robot_task/src/PointList/Active";
    if (not (nh.getParam("/Active/P", P))) P = 0.01;
    if (not (nh.getParam("/Active/I", I))) I = 0.0;
    if (not (nh.getParam("/Active/D", D))) D = 0.001;
    if (not (nh.getParam("/Active/reference", reference))) reference = 0;
    if (not (nh.getParam("/Active/upper_bound", upper_bound))) upper_bound = 0.02;
    if (not (nh.getParam("/Active/lower_bound", lower_bound))) lower_bound = -0.02;

    std::string pos_control, vel_control;
    if (not (nh.getParam("/Active/pos_control", pos_control))) pos_control = "position_joint_trajectory_controller";
    if (not (nh.getParam("/Active/vel_control", vel_control))) vel_control = "joint_velocity_example_controller";
    if (not (nh.getParam("/Active/rate", rate))) rate = 100;


    int filter_size = 3;
    int index = 0;
    double vel_filter = 0.0;

    if (not (nh.getParam("/Active/filter_size", filter_size))) filter_size = 10;

    ROS_INFO("Filter size: %d", filter_size );

    std::vector<double> FilterVector;
    FilterVector.resize(filter_size);
    for(int i=0; i<filter_size; i++)
        FilterVector[i] = 0.0;


    ros::Duration T_offset = ros::Duration(0.4);
    ros::Rate loop_rate(rate);


    std::cout << "P " << P << "\n";
    std::cout << "I " << I << "\n";
    std::cout << "D " << D << "\n";

    float velocity_fix = 0.012;
    float velocity_round = 0.005;
    float velocity_pass = 0.025;


    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();


    ros::ServiceClient switch_client = nh.serviceClient<controller_manager_msgs::SwitchController>("/controller_manager/switch_controller");
    controller_manager_msgs::SwitchController::Request switch_req;
    controller_manager_msgs::SwitchController::Response switch_resp;
    switch_resp.ok = false;



    switch_req.start_controllers.resize(1);
    switch_req.stop_controllers.resize(1);
    switch_req.start_controllers[0] = pos_control;
    switch_req.stop_controllers[0] = vel_control; 
    if(switch_client.call(switch_req,switch_resp))
    {
        ROS_INFO("Switch controll requested");
        if (switch_resp.ok)
            ROS_INFO("Switch completed with success");
        else
        {
            ROS_ERROR("Error during the switch");
            return 1;
        }
    }
    else
    {
        ROS_ERROR("Error during the call of the client");
        return 1;
    }

    ROS_INFO("Reach Ready Position");
    move_group.setMaxVelocityScalingFactor(0.3);
    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    
/***************  SYSTEM PARAMETERS **************************/
    
    TrajectoryVector waypoints;
    TrajectoryPlanner_param param;


    param.radius = 0.02;                // Radius of the semi-circle for fixing part
    param.heigh = 0.02;   

    nh.getParam("/Active/radius", param.radius);
    nh.getParam("/Active/heigh", param.heigh);


    param.circ_point = 100;             // Point generated in the semi-circle
    param.res = 0.001;                  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;     // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;            // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;            // Distance of the second point
    param.corner_points = 100;          // Numver of point generated in the corner.

/**************** COMPUTE TRAJECTORY PARTS      ******************/


    geometry_msgs::Pose grasp1;
    geometry_msgs::PoseArray waypoints_final;

    ReadFileTxt(INPUT_RPY, waypoints.point, waypoints.pt_label, path, grasp1);
    MyTrajectoryPlanner3(param, waypoints);
    for(int i=0; i<waypoints.SecondaryTrajectory.size(); i++)
    {
        FromEE2Link8(waypoints.SecondaryTrajectory[i]);
            visual_tools.publishPath(waypoints.SecondaryTrajectory[i].poses, rvt::CYAN, rvt::SMALL);
    }
    visual_tools.trigger();


/****************** MOTION INIT *****************************************/
    move_group.setNumPlanningAttempts(500); 
    move_group.setPlanningTime(10);
    move_group.setMaxVelocityScalingFactor(0.3);
    visual_tools.prompt("Press 'next' to reach the init position \n");

/*************************   REACH STARTING POSITION ***********************************/
    moveit::planning_interface::MoveGroupInterface::Plan Plan_SetPoseTarget;
    move_group.setPoseTarget(waypoints.SecondaryTrajectory[0].poses[0]);
    move_group.plan(Plan_SetPoseTarget);
    move_group.execute(Plan_SetPoseTarget);

/********* PLAN OF THE TRAJECTORIES ****************/
    std::vector<std::string> LabelVector;
    std::vector<moveit::planning_interface::MoveGroupInterface::Plan> PlansVector;      // potrebbe non essere usato
    std::vector<geometry_msgs::PoseArray> WaypointsVector;

    FinalTrajectoryRescaling(PlansVector, WaypointsVector, LabelVector, move_group, waypoints, 
                            velocity_round, velocity_pass, velocity_fix, T_offset);
    
    std::cout << "PlansVectorSize:   "  << PlansVector.size() << "\n";
    std::cout << "WaypointsVector:   "  << WaypointsVector.size() << "\n";
    std::cout << "LabelVector:   "      << LabelVector.size() << "\n";


    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
    robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("panda_arm");


    bool loop_flag = true;
    float Vx = velocity_pass;

    vel_msg.data.resize(6);
    for(int i=0;i<6;i++)
        vel_msg.data[i] = 0;

    geometry_msgs::Pose EE_Position, final_pose, starting_pose;

    float distance;
    float wire_distance;





   
/*** ESECUZIONE ***/

    visual_tools.prompt("Press 'Next' to Grasp the wire");
    hand_group.setJointValueTarget(hand_grasp_position);
    hand_group.move();

    cmd_msg.data = 1;
    bool command_y = false;

    vel_msg.data[0] = velocity_pass;
    vel_filter_msg.data.resize(6);

    for(int i=0; i<6; i++)
        vel_filter_msg.data[i] = vel_msg.data[i];

    std::cout << "Plan to execute: \n";
    for(int i=0; i<PlansVector.size(); i++)
        std::cout << "Plan "<< i << " : " << LabelVector[i] << "\n";

    visual_tools.prompt("Press 'Next' to start the execution of the trajectory");

    for(int i=0; i<PlansVector.size(); i++)
    {
        if(LabelVector[i] == "pass")
        {
            ROS_INFO("Switch controll to velocity");
            switch_req.start_controllers[0] = vel_control;
            switch_req.stop_controllers[0] = pos_control; 
            if(switch_client.call(switch_req,switch_resp))
            {
                if (switch_resp.ok)
                    ROS_INFO("Switch completed with success");
                else
                    ROS_ERROR("Error during the switch");
            }
            else
                ROS_ERROR("Error during the call of the client");

            starting_pose = WaypointsVector[i].poses[0];
            final_pose = WaypointsVector[i].poses[WaypointsVector[i].poses.size()-1];

            distance = sqrt(pow((final_pose.position.x - starting_pose.position.x ),2) + 
                pow((final_pose.position.y - starting_pose.position.y ),2) +
                pow((final_pose.position.z - starting_pose.position.z ),2));
            std::cout<< "distance to move:  " << distance << "\n";

                if(i>1)
                {
                    if(LabelVector[i-1] == "cornerfix")
                        command_y = true;
                }

                if(command_y)
                    cmd_msg.data = 4;
                else
                    cmd_msg.data = 1;
                
                std::cout << "Send Gripper command \n";
                cmd_pub.publish(cmd_msg);
                ros::Duration(0.5).sleep();   

            while(loop_flag)
            {
                //wire_distance = (fo_F101_msg.data[1] - fo_F102_msg.data[1])/2;
                ros::spinOnce();

                reference = 0.0; 
                actual_value = fo_F101_msg.data[1];
                
                vel_msg.data[2] = UpdatePID(reference, actual_value, P, I, D, integral, delta_time, pre_error, offset, true);
                vel_msg.data[2] = CheckBounds(upper_bound, lower_bound, vel_msg.data[2]);
           
                FilterVector[index] = vel_msg.data[2];
                index = index + 1;

                if(index >= filter_size)
                    index = 0;

                for(int i=0; i<filter_size; i++)
                {
                    vel_filter += FilterVector[i];
                }
                vel_filter_msg.data[2] = vel_filter/filter_size;
                vel_filter = 0.0;

                vel_pub.publish(vel_msg);
                vel_filter_pub.publish(vel_filter_msg); // published

                //joint_value =  move_group.getCurrentJointValues();
                
                kinematic_state->setJointGroupPositions(joint_model_group, joint_value);
                EE_Position = CheckEEPosition(kinematic_state);
               // std::cout << EE_Position << "\n";


            //    EE_Position = move_group.getCurrentPose().pose;
                loop_flag = CheckExitCondition(distance, starting_pose, EE_Position);

                loop_rate.sleep();
            }

            ROS_INFO("point reached, vel set to 0.0");
            for(int i=0; i<6; i++)
                vel_msg.data[i] = 0.0;
            vel_pub.publish(vel_msg);
            loop_flag = true;

            cmd_msg.data = 2;
            //cmd_msg.data = 3; // loop
            cmd_pub.publish(cmd_msg);
            ros::Duration(1).sleep();

            // Ragigungere prossimo punto
            if(i<PlansVector.size())
            {
                switch_req.start_controllers[0] = pos_control;
                switch_req.stop_controllers[0] = vel_control; 
                if(switch_client.call(switch_req,switch_resp))
                {
                    ROS_INFO("Switch controll to position");
                    if (switch_resp.ok)
                        ROS_INFO("Switch completed");
                    else
                        ROS_ERROR("Error during the switch");
                }
                else
                    ROS_ERROR("Error during the call of the client");

                visual_tools.prompt("Move to next position");
                move_group.setMaxVelocityScalingFactor(0.01);
                move_group.setJointValueTarget(WaypointsVector[i+1].poses[0]);
                move_group.move();
            }
        }
        else
        {
            visual_tools.prompt("Start fix");
            cmd_msg.data = 2;
            //cmd_msg.data = 3; // loop
            cmd_pub.publish(cmd_msg);
            ros::Duration(0.5).sleep();
            width_msg.data = 0.004;
            width_pub.publish(width_msg);

            switch_req.start_controllers[0] = pos_control;
            switch_req.stop_controllers[0] = vel_control; 
            if(switch_client.call(switch_req,switch_resp))
            {
                ROS_INFO("Switch controll to position");
                if (switch_resp.ok)
                    ROS_INFO("Switch completed");
                else
                    ROS_ERROR("Error during the switch");
            }
            else
                ROS_ERROR("Error during the call of the client");
            move_group.execute(PlansVector[i]);
        }
    }

    switch_req.start_controllers[0] = pos_control;
    switch_req.stop_controllers[0] = vel_control; 
    if(switch_client.call(switch_req,switch_resp))
    {
        ROS_INFO("Switch controll to position");
        if (switch_resp.ok)
            ROS_INFO("Switch completed");
        else
            ROS_ERROR("Error during the switch");
    }
    else
        ROS_ERROR("Error during the call of the client");
        
    
    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setMaxVelocityScalingFactor(0.3);

    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    ros::shutdown();
    return 0;
}

geometry_msgs::Pose CheckEEPosition(robot_state::RobotStatePtr& kinematic_state)
{
    geometry_msgs::Pose EE_Pose;
    const Eigen::Isometry3d& end_effector_state = kinematic_state->getGlobalLinkTransform("panda_link8");
    Eigen::Matrix<double, 3, 1> EE_Translation = end_effector_state.translation();

    EE_Pose.position.x = EE_Translation(0);
    EE_Pose.position.y = EE_Translation(1);
    EE_Pose.position.z = EE_Translation(2);

    return EE_Pose;
}

bool CheckExitCondition(float distance, geometry_msgs::Pose start_pose, geometry_msgs::Pose EE_Pos)
{
    float actual_dist;

    actual_dist = ComputeDistance(start_pose, EE_Pos);

    if(distance - actual_dist < 0.01)
        return false;       // Exit from the loop
    else
        return true;
}

void fo_F101_callback(const std_msgs::Float32MultiArray& msg)
{
    for(int i=0; i<2; i++)
        fo_F101_msg.data[i] = msg.data[i];
}
void fo_F102_callback(const std_msgs::Float32MultiArray& msg)
{
    for(int i=0; i<2; i++)
        fo_F102_msg.data[i] = msg.data[i];
}
void so_F101_callback(const std_msgs::Float32MultiArray& msg)
{
    for(int i=0; i<3; i++)
        so_F101_msg.data[i] = msg.data[i];
}
void so_F102_callback(const std_msgs::Float32MultiArray& msg)
{
    for(int i=0; i<3; i++)
        so_F102_msg.data[i] = msg.data[i];
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
    float output = zero_width + (Pout + Iout + Dout);

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


void joint_callback(const sensor_msgs::JointState& data)
{
    for(int i=0; i<7; i++)
        joint_value[i] = data.position[i];
}