#include "ros/ros.h"

#include "std_msgs/String.h"
#include "std_msgs/Float32.h"

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

#include <franka_gripper/GraspAction.h>
#include <franka_gripper/HomingAction.h>
#include <franka_gripper/MoveAction.h>
#include <franka_gripper/StopAction.h>

#include <actionlib/client/simple_action_client.h>


#include <math.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion

#include <MyFunc.h>




#define PLANNING_ATTEMPTS_NO 500
#define INPUT_RPY 1

int main(int argc, char** argv)
{
    ros::init(argc, argv, "PickPlace_Wire");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(1);
    spinner.start();

    ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_command",100);
    ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/My_new_input", 100);

    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;

    float T_O;
    float scaling_factor;

    namespace rvt = rviz_visual_tools;

// Get Param
    if ( not (nh.getParam("/TrajectoryTest/Rescale", scaling_factor)))
        scaling_factor = 8;

    if (not (nh.getParam("/TrajectoryTest/Offset", T_O)))
        T_O = 0.06;
    ros::Duration T_offset = ros::Duration(T_O);

// Creation of the moveing group
    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();

    std::string path1 = "/home/panda/ros/Franka-Kev/src/test/src/PointList/line";
    std::string Log_Path = "/home/kevin/Panda_log";
    std::ofstream plan_output_c;


    /**** PER GRIPPER ACTION *****/


    actionlib::SimpleActionClient<franka_gripper::MoveAction> move_client("/franka_gripper/move", true);
    actionlib::SimpleActionClient<franka_gripper::GraspAction> grasp_client("/franka_gripper/grasp", true);
    actionlib::SimpleActionClient<franka_gripper::StopAction> stop_client("/franka_gripper/stop", true);

    franka_gripper::MoveGoal move_goal;
    franka_gripper::StopGoal stop_goal;

    move_goal.speed = 0.04;
   
    ROS_INFO("Wait for server");
    move_client.waitForServer();
    ROS_INFO("Gripepr controller ready");




    ROS_INFO("Reach Ready Position");
    move_group.setMaxVelocityScalingFactor(0.2);
    
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();
    

    hand_group.setJointValueTarget(hand_ready_state);

    move_goal.width = 0.08;
    move_client.sendGoal(move_goal);
    move_client.waitForResult(ros::Duration(10.0));
    

    TrajectoryVector waypoints;
    
/***************  SYSTEM PARAMETERS **************************/
    
    TrajectoryPlanner_param param;
    param.radius = 0.05;                // Radius of the semi-circle for fixing part
    param.heigh = 0.03;                 // Heigh of upward movement
    param.circ_point = 100;             // Point generated in the semi-circle
    param.res = 0.001;                  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;     // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;            // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;            // Distance of the second point
    param.corner_points = 100;          // Numver of point generated in the corner.

/**************** COMPUTE TRAJECTORY PARTS      ******************/
    

    geometry_msgs::Pose grasp1;
    geometry_msgs::PoseArray waypoints_final;

    ReadFileTxt(INPUT_RPY, waypoints.point, waypoints.pt_label, path1, grasp1);
    MyTrajectoryPlanner2(param, waypoints);
    FromVector2PoseArray(waypoints, waypoints_final);
    FromEE2Link8(waypoints_final);

/**** Print result ****/

    visual_tools.deleteAllMarkers();
    visual_tools.publishPath(waypoints_final.poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    for (std::size_t i=0; i< waypoints_final.poses.size(); i++)
        visual_tools.publishAxisLabeled(waypoints_final.poses[1], "pt" , rvt::XXXSMALL);
    visual_tools.trigger();

/****************** MOTION INIT *****************************************/
        


    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    bool success;
    int size_plan, final_plan_size;

    move_group.setNumPlanningAttempts(PLANNING_ATTEMPTS_NO); 
    move_group.setPlanningTime(10);

    moveit_msgs::RobotTrajectory trajectory_cartesian;
    const double jump_threshold = 0.0;
    const double eef_step = 0.001;
    double fraction;   
    moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian, Plan2;
    geometry_msgs::PoseArray failed_points;


    for (std::size_t i=0; i< waypoints.point.poses.size(); i++)
    {
        visual_tools.publishAxisLabeled(waypoints.point.poses[i], waypoints.pt_label[i] , rvt::XXXSMALL);
        visual_tools.publishAxisLabeled(waypoints_final.poses[i], waypoints.pt_label[i] , rvt::XXXSMALL);
    }
    visual_tools.publishPath(waypoints_final.poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    visual_tools.trigger();


    move_group.setNumPlanningAttempts(100);
    move_group.setMaxVelocityScalingFactor(0.3);
    visual_tools.prompt("Press 'next' to reach the starting position \n");

/*********************************************************************************************************/
    // Moivemento alla posizione di partenza
    moveit::planning_interface::MoveGroupInterface::Plan Plan_SetPoseTarget, Plan_s;


    move_group.setPoseTarget(waypoints_final.poses[0]);
    move_group.plan(Plan_SetPoseTarget);
    move_group.execute(Plan_SetPoseTarget);

    move_group.computeCartesianPath(waypoints_final.poses, eef_step, jump_threshold, trajectory_cartesian);
    Plan_Cartesian.trajectory_ = trajectory_cartesian;

    Plan_s.trajectory_ = FactorScaling(trajectory_cartesian, scaling_factor, T_offset);


    int user_input = 0;
    bool op_ok = true;




/*** ESECUZIONE ***/

    visual_tools.prompt("Press 'Nextì to close the gripper");
    width_msg.data = 0.006;
    ros::Duration(1).sleep();
    width_pub.publish(width_msg);
    std::cout << "Publish message";

    visual_tools.prompt("Next to start");

    cmd_msg.data = 1;
    cmd_pub.publish(cmd_msg);
    std::cout << "Publish \n";
    ros::Duration(1).sleep();


    move_group.execute(Plan_s);

    cmd_msg.data = 2;
    cmd_pub.publish(cmd_msg);
    std::cout << "Publish \n";


    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();


/*
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

*/



    ros::shutdown();
    return 1;


/*** ALTRI TEST PROVATI (NON FUNZIONANTI)  ***/
/**
    traj_scaled = RescaleDerivative (trajectory_cartesian, 0.05, save_t);
    Plan_Cartesian.trajectory_ = traj_scaled;
    Plan_Cartesian.trajectory_ = rescalePathDavide(trajectory_cartesian, 0.05, save_t);
    Plan_Cartesian.trajectory_ = TrajFiller(traj_scaled, 500, save_t);
    move_group.execute(Plan_Cartesian);
*/

    
// Print if enabled
if (0)
{
    std::cout<< "scrittura punti";

    Log_Path = "/home/kevin/Panda_log_cartesian";
    plan_output_c.open(Log_Path);
     
    for (size_t i = 0; i < Plan_Cartesian.trajectory_.joint_trajectory.points.size(); i++)
    { 
        plan_output_c << "Positions\n"; 
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[0] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[1] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[2]<< "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[3]<< "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[4]<< "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[5]<< "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[6]<< "\n";

        plan_output_c << "Velocities\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[0] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[1] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[2] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[3] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[4] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[5] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[6] << "\n";

        plan_output_c << "Accelerations\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[0] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[1] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[2] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[3] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[4] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[5] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[6] << "\n";

        plan_output_c << "Time_From_Start\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].time_from_start << "\n";                
    }

    plan_output_c.close();

}


    ros::shutdown();
    return 0;
}

