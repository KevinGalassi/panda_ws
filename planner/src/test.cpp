

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>



#include <actionlib/client/simple_action_client.h>


#include <TrajectoryPlanner.h>

#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>



int main(int argc, char** argv)
{
    ros::init(argc, argv, "prova_planner");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(1);
    spinner.start();

    ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_command",100);
    ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/My_new_input", 100);

    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;

    namespace rvt = rviz_visual_tools;

    float T_O;
    if (not (nh.getParam("/TrajectoryTest/Offset", T_O)))
        T_O = 0.4;
    ros::Duration T_offset = ros::Duration(0.4);
    ros::Duration T_round = ros::Duration(0.4);

    float scaling_factor = 6;
    float scaling_round =  8;
    float scaling_fix = 10;

    float velocity_fix = 0.02;
    float velocity_round = 0.1;
    float velocity_pass = 0.05;
   

    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();

    std::string path = "/home/kevin/ros/panda_ws/src/planner/src/PointList/Test";
    std::string Log_Path = "/home/kevin/Panda_log";
    std::ofstream plan_output_c;

    ros::Duration(3).sleep();

    ROS_INFO("Reach Ready Position");
    move_group.setMaxVelocityScalingFactor(0.2);

    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    
/***************  SYSTEM PARAMETERS **************************/
    
    TrajectoryPlanner traj_test;

    traj_test.file_path = path;

    traj_test.PlannerInit();
    traj_test.ReadFileTxt();
    traj_test.PublishInputPoint(visual_tools);
    visual_tools.prompt("next");

    traj_test.TrajectoryPlanning();
    traj_test.FromEE2Link8();
    traj_test.PublishTrajectory(visual_tools);

    
 


    ros::shutdown();
    return 0;
}

