/*
    Test program to grasp a wire recognized by a camera running on another node
*/

#include "ros/ros.h"
#include <eigen_conversions/eigen_msg.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>


#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include <std_msgs/Float32MultiArray.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Accel.h"

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <moveit_visual_tools/moveit_visual_tools.h>
#include <math.h>

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion
#include "geometric_shapes/shapes.h"
#include "geometric_shapes/mesh_operations.h"
#include "geometric_shapes/shape_operations.h"

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

#include "vision/getGraspPose.h"

#include <schunk_pg70/set_position.h>



geometry_msgs::Pose pointTrasformToEE(geometry_msgs::TransformStamped transform, geometry_msgs::Pose target_pose, std::string init_frame, std::string final_frame);

int main(int argc, char** argv)
{
   ros::init(argc, argv, "ol_tes2t2");
   ros::NodeHandle nh;

   ros::AsyncSpinner spinner(1);
   spinner.start();

   ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_state_request",100);
   ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/gripper_width_request", 100);

   std_msgs::Float32 width_msg;
   std_msgs::Float32 cmd_msg;

   const std::vector<double> arm_ready_state       = {0, -0.785, 0, -2.356, 0, 1.571, 0.785};
   const std::vector<double> hand_ready_state      = {0.03, 0.03};
   const std::vector<double> hand_open_position    = {0.037, 0.037};
   const std::vector<double> hand_closed_position  = {0.012, 0.012};
   const std::vector<double> hand_grasp_position   = {0.004, 0.004};

   geometry_msgs::TransformStamped transform;
   tf2_ros::Buffer tfBuffer;



   geometry_msgs::TransformStamped tfCameraWorld;
   tfCameraWorld.transform.translation.x = 0.455;
   tfCameraWorld.transform.translation.y = 0.031;
   tfCameraWorld.transform.translation.z = 0.176;
   tfCameraWorld.transform.rotation.x = 0.0;
   tfCameraWorld.transform.rotation.y = -0.707;
   tfCameraWorld.transform.rotation.z = 0.707;
   tfCameraWorld.transform.rotation.w = 0.0;

   geometry_msgs::TransformStamped tfEEGripper;
   tfEEGripper.transform.translation.x = 0.0;
   tfEEGripper.transform.translation.y = 0.0;
   tfEEGripper.transform.translation.z = 0.18;
   tfEEGripper.transform.rotation.x = 0.0;
   tfEEGripper.transform.rotation.y = 0.0;
   tfEEGripper.transform.rotation.z = 0.3825;
   tfEEGripper.transform.rotation.w = 0.92396;

   geometry_msgs::TransformStamped tfZAxis;
   tfEEGripper.transform.translation.x = 0.0;
   tfEEGripper.transform.translation.y = 0.0;
   tfEEGripper.transform.translation.z = 0.0;
   tfEEGripper.transform.rotation.x = 1.0;
   tfEEGripper.transform.rotation.y = 0.0;
   tfEEGripper.transform.rotation.z = 0.0;
   tfEEGripper.transform.rotation.w = 0.0;



 //  ros::ServiceClient getGraspPose_client = nh.serviceClient<vision::getGraspPose>("/get_grasp_pose");
   vision::getGraspPose grasp_srv;


   ros::ServiceClient gripper_client = nh.serviceClient<schunk_pg70::set_position>("schunk_pg70/set_position");     

   schunk_pg70::set_position schunk_pos_srv;
   schunk_pos_srv.request.goal_position = 60; // 69
   schunk_pos_srv.request.goal_velocity = 60; // 82
   schunk_pos_srv.request.goal_acceleration = 200; // 320

   schunk_pg70::set_position schunk_open_pos_srv;
   schunk_open_pos_srv.request.goal_position = 60; // 69
   schunk_open_pos_srv.request.goal_velocity = 60; // 82
   schunk_open_pos_srv.request.goal_acceleration = 200; // 320

   schunk_pg70::set_position schunk_closed_pos_srv;
   schunk_closed_pos_srv.request.goal_position = 20; // 69
   schunk_closed_pos_srv.request.goal_velocity = 60; // 82
   schunk_closed_pos_srv.request.goal_acceleration = 200; // 320

    
   moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
   //moveit::planning_interface::MoveGroupInterface hand_group("hand");
   moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
   moveit_visual_tools::MoveItVisualTools camera_tools("camera_link");
   camera_tools.deleteAllMarkers();
   visual_tools.deleteAllMarkers();


   ROS_INFO("Reach Ready Position");
   move_group.setMaxVelocityScalingFactor(0.2);
   
   //gripper_client.call(schunk_open_pos_srv);
   move_group.setJointValueTarget(arm_ready_state);
   move_group.move();

   std::cout << "Exec compelted \n";

   geometry_msgs::Pose target_pose;

/*
   // Service to get poin
   getGraspPose_client.call(grasp_srv);
   target_pose = grasp_srv.response.target_pose;
   target_pose.orientation.w = 1;
   target_pose.orientation.x = 0;
   target_pose.orientation.y = 0;
   target_pose.orientation.z = 0;
*/

   geometry_msgs::Pose picture_pose;
   geometry_msgs::Pose camera_pose;
   geometry_msgs::Pose ee_pose;

   picture_pose.position.x = 0.05;
   picture_pose.position.y = -0.03;
   picture_pose.position.z = 0.2;
   picture_pose.orientation.x = 0.0;
   picture_pose.orientation.y = 0.0;
   picture_pose.orientation.z = 0.0;
   picture_pose.orientation.w = 1.0;

   tf2::doTransform(picture_pose, camera_pose, tfCameraWorld);

   
   
   camera_tools.publishAxis(picture_pose, 0.1);
   camera_tools.trigger();

   visual_tools.prompt("Convert fjeejf");
   visual_tools.publishAxis(picture_pose, 0.1);
   visual_tools.publishAxis(camera_pose, 0.1);
   visual_tools.trigger();

   tf2::doTransform(camera_pose, target_pose, tfZAxis);


   /*
   target_pose.position.x = 40;
   target_pose.position.y = 0;
   target_pose.position.z = 0.04;

   geometry_msgs::Pose new_target_pose = pointTrasformToEE(transform, target_pose, "camera_link", "schunk_pg70_object_link");
   visual_tools.publishAxis(new_target_pose, 0.1);

   */



// tf2::doTransform(target_pose, pose_transformed, transform);

   // visual_tools.publishAxis(target_pose, 0.1);
   // visual_tools.trigger();


   visual_tools.prompt("Next to set new target frame");
   //move_group.setPoseTarget(target_pose, "schunk_pg70_object_link");
   move_group.setPoseTarget(target_pose);
   move_group.move();

   //if(gripper_client.call(schunk_closed_pos_srv));


    ros::Duration(5).sleep();

  //  hand_group.setJointValueTarget(hand_closed_position);
    //hand_group.move();


    ros::shutdown();
    return 0;
}


geometry_msgs::Pose pointTrasformToEE(geometry_msgs::TransformStamped transform, geometry_msgs::Pose target_pose, std::string init_frame, std::string final_frame)
{
    geometry_msgs::Pose pose_transformed;

    tf2::doTransform(target_pose, pose_transformed, transform);

    return pose_transformed;
}

