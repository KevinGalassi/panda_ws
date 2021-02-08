

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

    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;

    namespace rvt = rviz_visual_tools;


    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();

    std::string path =  "/home/kvn/ros/panda_ws/src/robot_task/src/PointList/Routing";
    std::ofstream plan_output_c;

    nh.getParam("test_launch/filename", path);
    

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

