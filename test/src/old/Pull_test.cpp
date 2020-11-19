
#include "ros/ros.h"
#include <moveit_visual_tools/moveit_visual_tools.h>
#include <geometry_msgs/Pose.h>
#include "MyFunc.h"
#include <collision_object_lib.h>

#include "std_msgs/Int32.h"


#define INPUT_RPY 1

int main(int argc, char** argv)
{

    ros::init(argc,argv, "Pull_Test_node");
    ros::NodeHandle n;

    float scaling_factor = 8;
    ros::Duration T_offset = ros::Duration(0.3);

    namespace rvt = rviz_visual_tools;

    std::string file_path1 = "/home/panda/ros/Franka-Kev/src/Test/src/PointList/First";
    std::string file_path2 = "/home/panda/ros/Franka-Kev/src/Test/src/PointList/Second";


    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

    moveit_visual_tools::MoveItVisualTools vt("panda_link0");
    vt.deleteAllMarkers();

    std::vector<std::string> collision_obj_name;
    collision_obj_name.resize(1);
    collision_obj_name[0] = addRobotBase(planning_scene_interface);

    ros::Publisher path_code_pub = n.advertise<std_msgs::Int32>("/PathExecuted",1000);
    std_msgs::Int32 path_code_msg;

/*** Path Creation ***/

    TrajectoryVector waypoints1, waypoints2;
    
    TrajectoryPlanner_param param;
    param.radius = 0.02;       // Radius of the semi-circle for fixing part
    param.heigh = 0.03;// Heigh of upward movement
    param.circ_point = 5; // Point generated in the semi-circle
    param.res = 0.001;  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;    // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;    // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;    // Distance of the second point
    param.corner_points = 100;     // Numver of point generated in the corner.

    geometry_msgs::Pose grasp;

    ReadFileTxt(INPUT_RPY, waypoints1.point, waypoints1.pt_label, file_path1, grasp);
    ReadFileTxt(INPUT_RPY, waypoints2.point, waypoints2.pt_label, file_path2, grasp);

    MyTrajectoryPlanner(param, waypoints1);
    MyTrajectoryPlanner(param, waypoints2);


    
    geometry_msgs::PoseArray waypoints_final1, waypoints_final2;
    for(size_t i=0; i< waypoints1.SecondaryTrajectory.size(); i++)
    {
        for (size_t k=0; k< waypoints1.SecondaryTrajectory[i].poses.size(); k++)
        {
            waypoints_final1.poses.push_back(waypoints1.SecondaryTrajectory[i].poses[k]);
        }
    }
    for(size_t i=0; i< waypoints2.SecondaryTrajectory.size(); i++)
    {
        for (size_t k=0; k< waypoints2.SecondaryTrajectory[i].poses.size(); k++)
        {
            waypoints_final2.poses.push_back(waypoints2.SecondaryTrajectory[i].poses[k]);
        }
    }

    FromEE2Link8(waypoints_final1);
    FromEE2Link8(waypoints_final2);


    vt.publishPath(waypoints_final1.poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    vt.publishPath(waypoints_final2.poses, rvt::RED, rvt::XXXSMALL);
    vt.trigger();


    moveit_msgs::RobotTrajectory trajectory_cartesian;
    const double jump_threshold = 0.0;
    const double eef_step = 0.001;
    double fraction;   
    moveit::planning_interface::MoveGroupInterface::Plan Plan1, Plan2;


    move_group.setMaxVelocityScalingFactor(0.2);
    move_group.setMaxAccelerationScalingFactor(0.2);
    move_group.setPoseTarget(waypoints_final1.poses[0]);
    move_group.move();

    ROS_INFO("fwqfwqf");
    move_group.computeCartesianPath(waypoints_final1.poses, eef_step, jump_threshold, trajectory_cartesian);
    ROS_INFO("ComputeCartesian path");
    Plan1.trajectory_ = FactorScaling(trajectory_cartesian, scaling_factor, T_offset);
    ROS_INFO("Esecuzione prova primo tratto");
    move_group.execute(Plan1);


    

    move_group.computeCartesianPath(waypoints_final2.poses, eef_step, jump_threshold, trajectory_cartesian);
    Plan2.trajectory_ = FactorScaling(trajectory_cartesian, scaling_factor, T_offset);
    ROS_INFO("Esecuzione secondo tratto");
    move_group.execute(Plan2);
    hand_group.setJointValueTarget(hand_ready_state);


/*** ESECUZIONE ***/
    while(ros::ok())
    {
        path_code_msg.data = 0;
        vt.prompt("Move to start State");
        path_code_pub.publish(path_code_msg);
        move_group.setPoseTarget(waypoints_final1.poses[0]);
        move_group.move();

        path_code_msg.data = 1;
        vt.prompt("Eseguire tensionamento cavo");
        path_code_pub.publish(path_code_msg);
        move_group.execute(Plan1);

        path_code_msg.data = 2;
        vt.prompt("Eseguire trazione cavo");
        path_code_pub.publish(path_code_msg);
        move_group.execute(Plan2);

        hand_group.setJointValueTarget(hand_ready_state);

    }

    // Remove collision object
    planning_scene_interface.removeCollisionObjects(collision_obj_name);


    return 1;
}



