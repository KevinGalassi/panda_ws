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

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"
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


geometry_msgs::Quaternion orientationConversion(geometry_msgs::Quaternion input_quat, float roll, float pitch, float yaw);
geometry_msgs::Pose PointConversion(geometry_msgs::Pose input_quat, float roll, float pitch, float yaw);

   /*
   world to gripper -> orientationConversion(target_pose.orientation, M_PI, 0, 0);
   world to camera  -> M_PI 0 -M_PI/2
   camera to world  -> -M_PI/2 0 M_PI
   */


int main(int argc, char** argv)
{
   ros::init(argc, argv, "ol_tes2t2");
   ros::NodeHandle nh;

   ros::AsyncSpinner spinner(1);
   spinner.start();

   namespace rvt = rviz_visual_tools;

   ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_state_request",100);
   ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/gripper_width_request", 100);

   std_msgs::Float32 width_msg;
   std_msgs::Float32 cmd_msg;

   const std::vector<double> arm_ready_state       = {0, -0.785, 0, -2.356, 0, 1.571, 0.785};
   const std::vector<double> hand_ready_state      = {0.03, 0.03};
   const std::vector<double> hand_open_position    = {0.037, 0.037};
   const std::vector<double> hand_closed_position  = {0.012, 0.012};
   const std::vector<double> hand_grasp_position   = {0.004, 0.004};

   geometry_msgs::Point camera_position;
   nh.getParam("/StaticGrasp/camera_x", camera_position.x);
   nh.getParam("/StaticGrasp/camera_y", camera_position.y);
   nh.getParam("/StaticGrasp/camera_z", camera_position.z);

   std::string file_path;
   nh.getParam("/StaticGrasp/file_path", file_path);

   float gripper_width;
   nh.getParam("/StaticGrasp/gripper_width", gripper_width);


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



   ros::ServiceClient getGraspPose_client = nh.serviceClient<vision::getGraspPose>("/get_grasp_pose");
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
   schunk_closed_pos_srv.request.goal_position = 10; // 69
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
   
   gripper_client.call(schunk_open_pos_srv);
   move_group.setJointValueTarget(arm_ready_state);
   move_group.move();

   std::cout << "Exec compelted \n";

   geometry_msgs::Pose target_pose;
   geometry_msgs::Pose picture_pose;
   geometry_msgs::Pose ee_pose;
   geometry_msgs::Pose camera_pose;


   // Service to get point
   getGraspPose_client.call(grasp_srv);
   picture_pose = grasp_srv.response.target_pose;
	std::cout <<"Dal servizio" << picture_pose << "\n";


   // Fake point generate
   /*
	picture_pose.position.x = 0.05;
   picture_pose.position.y = -0.03;
   picture_pose.position.z = 0.2;
   picture_pose.orientation.x = 0.0;
   picture_pose.orientation.y = 0.0;
   picture_pose.orientation.z = 0.0;
   picture_pose.orientation.w = 1.0;
	*/
   //picture_pose.orientation = orientationConversion(picture_pose.orientation, -M_PI/2, 0, M_PI); 
	picture_pose = PointConversion(picture_pose, -M_PI/2, 0, M_PI); 
	std::cout <<"Conversione" << picture_pose << "\n";

   visual_tools.publishAxisLabeled(picture_pose, "Target Point in camera frame", rvt::SMALL);
   visual_tools.trigger();
   visual_tools.prompt("Next to convert the orientation");

/*
   // TEST ORIENTATION
   camera_pose.position = picture_pose.position; 
   camera_pose.position.x += 0.2;
   camera_pose.orientation = orientationConversion(picture_pose.orientation, -M_PI/2, 0, M_PI); 
   visual_tools.publishAxisLabeled(camera_pose, "Camera orientation", rvt::SMALL);
   
   camera_pose.position.x += 0.2;
   camera_pose.orientation = orientationConversion(camera_pose.orientation, -M_PI/2, 0, M_PI); 
   visual_tools.publishAxisLabeled(camera_pose, "Back to world orientation", rvt::SMALL);

   camera_pose.position.x += 0.2;
   camera_pose.orientation = orientationConversion(camera_pose.orientation, M_PI, 0, 0); 
   visual_tools.publishAxisLabeled(camera_pose, " Gripper Orientation", rvt::SMALL);

   visual_tools.trigger();
   visual_tools.prompt("CHeck correction");
*/


   // Conversion to Gripper position

   target_pose.position.x = picture_pose.position.x + camera_position.x;
   target_pose.position.y = picture_pose.position.y + camera_position.y;
   target_pose.position.z = picture_pose.position.z + camera_position.z;
   target_pose.orientation = orientationConversion(picture_pose.orientation, -M_PI/2, 0, M_PI); 
   target_pose.orientation = orientationConversion(target_pose.orientation, M_PI, 0, 0); 
	std::cout <<"Final Point" << target_pose << "\n";

   visual_tools.publishAxisLabeled(target_pose, "Point to grasp", rvt::SMALL);
   visual_tools.trigger();
   visual_tools.prompt("Next to pre-grasp");

	target_pose.position.z += 0.1;
	move_group.setPoseTarget(target_pose, "schunk_pg70_object_link");
   move_group.move();

	visual_tools.prompt("Next to grasp");
	target_pose.position.z -= 0.1;
	move_group.setPoseTarget(target_pose, "schunk_pg70_object_link");
   move_group.move();

	ros::Duration(1).sleep();
   if(gripper_client.call(schunk_closed_pos_srv));

   //hand_group.setJointValueTarget(hand_closed_position);
   //hand_group.move();
	visual_tools.prompt("Next to raise");
	target_pose.position.z += 0.1;
	move_group.setPoseTarget(target_pose, "schunk_pg70_object_link");
   move_group.move();

    ros::shutdown();
    return 0;
}


geometry_msgs::Quaternion orientationConversion(geometry_msgs::Quaternion input_quat, float roll, float pitch, float yaw)
{

   tf2::Quaternion quat_rot, quat_out;
   tf2::Quaternion quat_in(input_quat.x, input_quat.y, input_quat.z, input_quat.w);
   
   /*
   quat_in.x = input_quat.x;
   quat_in.y = input_quat.y;
   quat_in.z = input_quat.z;
   quat_in.w = input_quat.w;
   */

   quat_rot.setRPY(roll, pitch, yaw);

   quat_out = quat_in * quat_rot;

   geometry_msgs::Quaternion output_orientation = tf2::toMsg(quat_out);

   return output_orientation;
}

geometry_msgs::Pose PointConversion(geometry_msgs::Pose input_quat, float roll, float pitch, float yaw)
{

   tf2::Quaternion quat_rot, quat_out;
   tf2::Quaternion quat_in(input_quat.orientation.x, input_quat.orientation.y, input_quat.orientation.z, input_quat.orientation.w);
   
   /*
   quat_in.x = input_quat.x;
   quat_in.y = input_quat.y;
   quat_in.z = input_quat.z;
   quat_in.w = input_quat.w;
   */

   quat_rot.setRPY(roll, pitch, yaw);

   quat_out = quat_in * quat_rot;
	tf2::Matrix3x3 R;
	R.setRotation(quat_out);

   geometry_msgs::Pose output_pose;

	output_pose.orientation = tf2::toMsg(quat_out);
   output_pose.position.x = R[0][0]*input_quat.position.x + R[0][1]*input_quat.position.y + R[0][2]*input_quat.position.z ;
   output_pose.position.y = R[1][0]*input_quat.position.x + R[1][1]*input_quat.position.y + R[1][2]*input_quat.position.z ;
   output_pose.position.z = R[2][0]*input_quat.position.x + R[2][1]*input_quat.position.y + R[2][2]*input_quat.position.z ;

   return output_pose;
}
