#include "ros/ros.h"

#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Bool.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "geometry_msgs/Pose.h"

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_visual_tools/moveit_visual_tools.h>

#include <math.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion

#include <franka_gripper/GraspAction.h>
#include <franka_gripper/HomingAction.h>
#include <franka_gripper/MoveAction.h>
#include <franka_gripper/StopAction.h>
#include <actionlib/client/simple_action_client.h>


#include <MyFunc.h>




#define PLANNING_ATTEMPTS_NO 100
#define INPUT_RPY 1

int main(int argc, char** argv)
{
    ros::init(argc, argv, "PickPlace_Wire");
    ros::NodeHandle nh;

    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;

    namespace rvt = rviz_visual_tools;

    ros::Duration T_offset = ros::Duration(0.4);
    float mean_velocity;

    ros::Publisher start_pub = nh.advertise<std_msgs::Bool>("/StartPublish", 100);
    bool start_msg;

    

// Creation of the moveing group
    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();

    std::string path1 = "/home/panda/ros/Franka-Kev/src/robot_task/src/PointList/line";;

    actionlib::SimpleActionClient<franka_gripper::MoveAction> move_client("/franka_gripper/move", true);
    actionlib::SimpleActionClient<franka_gripper::GraspAction> grasp_client("/franka_gripper/grasp", true);
    actionlib::SimpleActionClient<franka_gripper::StopAction> stop_client("/franka_gripper/stop", true);

    franka_gripper::MoveGoal move_goal;
    franka_gripper::StopGoal stop_goal;

    move_goal.speed = 0.05;

    ROS_INFO("Reach Ready Position");
    move_group.setMaxVelocityScalingFactor(0.2);

    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    
/***************  SYSTEM PARAMETERS **************************/
    
    TrajectoryVector waypoints;
    TrajectoryPlanner_param param;
    param.radius = 0.02;       // Radius of the semi-circle for fixing part
    param.heigh = 0.03;// Heigh of upward movement
    param.circ_point = 100; // Point generated in the semi-circle
    param.res = 0.001;  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;    // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;    // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;    // Distance of the second point
    param.corner_points = 1000;     // Numver of point generated in the corner.

/**************** COMPUTE TRAJECTORY PARTS      ******************/
    
    geometry_msgs::Pose grasp1;
    geometry_msgs::PoseArray waypoints_final;


    ReadFileTxt(INPUT_RPY, waypoints.point, waypoints.pt_label, path1, grasp1);
    MyTrajectoryPlanner3(param, waypoints);
    FromVector2PoseArray(waypoints, waypoints_final);
    FromEE2Link8(waypoints_final);

/** Print result ****/

    visual_tools.deleteAllMarkers();
    visual_tools.publishPath(waypoints_final.poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    visual_tools.trigger();


/****************** MOTION INIT *****************************************/
        

    moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian, Plan2;

    bool success;
    int size_plan, final_plan_size;

    move_group.setNumPlanningAttempts(PLANNING_ATTEMPTS_NO); 
    move_group.setPlanningTime(10);
    move_group.setMaxVelocityScalingFactor(0.3);
    visual_tools.prompt("Press 'next' to reach the starting position \n");

/*********************************************************************************************************/
    // Movimento alla posizione di partenza

    moveit::planning_interface::MoveGroupInterface::Plan Plan_SetPoseTarget;
    move_group.setPoseTarget(waypoints_final.poses[0]);
    move_group.plan(Plan_SetPoseTarget);
    move_group.execute(Plan_SetPoseTarget);

/********* PLAN OF THE TRAJECTORIES ****************/

    moveit_msgs::RobotTrajectory trajectory_cartesian;
    const double jump_threshold = 0.0;
    const double eef_step = 0.001;
    move_group.computeCartesianPath(waypoints_final.poses, eef_step, jump_threshold, trajectory_cartesian);
    VelocityScaling(trajectory_cartesian, waypoints_final,  mean_velocity, T_offset);
    Plan_Cartesian.trajectory_ = trajectory_cartesian;

    move_goal.width = 0.002;

    while(ros::ok())
    {
        move_goal.width += 0.001;
        if(move_goal.width > 0.007)
            move_goal.width = 0.003;

        move_client.sendGoal(move_goal);
        move_client.waitForResult(ros::Duration(2.0));

        start_msg = true;
        start_pub.publish(start_msg);

        move_group.execute(Plan_Cartesian);

        start_msg = false;
        start_pub.publish(start_msg);

        hand_group.setJointValueTarget(hand_ready_state);
        hand_group.move();

        move_group.setPoseTarget(waypoints_final.poses[0]);
        move_group.plan(Plan_SetPoseTarget);
        move_group.execute(Plan_SetPoseTarget);
    }

    ros::shutdown();
    return 0;
}

