/*
    Test program to grasp a wire recognized by a camera running on another node
*/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include <std_msgs/Float32MultiArray.h>

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <fstream>

#include "geometry_msgs/Pose.h"
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

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

#include "read_sensor/tactile_sensor_data.h"

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
   std::string topic_name1, topic_name2;
   float gripper_width, gripper_speed, gripper_acc;
   std::string file_path, FileName;
   int test_no;
   float offset, offset_min, offset_max;
   float wire_z, wire_x;

   nh.getParam("/StaticGrasp/camera_x", camera_position.x);
   nh.getParam("/StaticGrasp/camera_y", camera_position.y);
   nh.getParam("/StaticGrasp/camera_z", camera_position.z);
   nh.getParam("/StaticGrasp/topic_name1", topic_name1);
   nh.getParam("/StaticGrasp/topic_name2", topic_name2);
   nh.getParam("/StaticGrasp/gripper_width", gripper_width);
   nh.getParam("/StaticGrasp/gripper_speed", gripper_speed);
   nh.getParam("/StaticGrasp/gripper_acc", gripper_acc);
   nh.getParam("/StaticGrasp/file_path", file_path);
   nh.getParam("/StaticGrasp/FileName", FileName);
   nh.getParam("/StaticGrasp/test_no", test_no);
   nh.getParam("/StaticGrasp/offset_min", offset_min);
   nh.getParam("/StaticGrasp/offset_max", offset_max);
   nh.getParam("/StaticGrasp/wire_z", wire_z);
   nh.getParam("/StaticGrasp/wire_x", wire_x);  

 //  ros::ServiceClient getGraspPose_client = nh.serviceClient<vision::getGraspPose>("/get_grasp_pose");
   vision::getGraspPose grasp_srv;
   ros::ServiceClient gripper_client = nh.serviceClient<schunk_pg70::set_position>("schunk_pg70/set_position");     

   schunk_pg70::set_position schunk_pos_srv;
   schunk_pos_srv.request.goal_position = gripper_width;          // 69
   schunk_pos_srv.request.goal_velocity = gripper_speed;          // 82
   schunk_pos_srv.request.goal_acceleration = gripper_acc;        // 320

   schunk_pg70::set_position schunk_open_pos_srv;
   schunk_open_pos_srv.request.goal_position = 60;                // 69
   schunk_open_pos_srv.request.goal_velocity = gripper_speed;     // 82
   schunk_open_pos_srv.request.goal_acceleration = 200;           // 320

   schunk_pg70::set_position schunk_closed_pos_srv;
   schunk_closed_pos_srv.request.goal_position = 10;              // 69
   schunk_closed_pos_srv.request.goal_velocity = gripper_speed;   // 82
   schunk_closed_pos_srv.request.goal_acceleration = gripper_acc; // 320
    
   moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
   moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
   visual_tools.deleteAllMarkers();

   ROS_INFO("Reach Ready Position");
   move_group.setMaxVelocityScalingFactor(0.2);
   gripper_client.call(schunk_open_pos_srv);
   ros::Duration(2).sleep();

   geometry_msgs::Pose start_pose;
   start_pose.position.x = 0.50;
   start_pose.position.y = 0;
   start_pose.position.z = 0.166343;
   start_pose.orientation.x = -1;
   start_pose.orientation.y = -4.37114e-08;
   start_pose.orientation.z = -4.37114e-08;
   start_pose.orientation.w = 4.37114e-08;

   move_group.setPoseTarget(start_pose, "schunk_pg70_object_link");
   move_group.move();

   geometry_msgs::Pose target_pose;
   geometry_msgs::Pose picture_pose;

   // Fake point generate
	picture_pose.position.x = wire_x;
   picture_pose.position.y = 0.0;
   picture_pose.position.z = wire_z;
   picture_pose.orientation.x = 0.0;
   picture_pose.orientation.y = 0.0;
   picture_pose.orientation.z = 0.0;
   picture_pose.orientation.w = 1.0;
   picture_pose.orientation = orientationConversion(picture_pose.orientation, -M_PI/2, 0, M_PI); 

   // Conversion to Gripper position
   target_pose.position.x = picture_pose.position.x + camera_position.x;
   target_pose.position.y = picture_pose.position.y + camera_position.y;
   target_pose.position.z = picture_pose.position.z + camera_position.z;
   target_pose.orientation = orientationConversion(picture_pose.orientation, -M_PI/2, 0, M_PI); 
   target_pose.orientation = orientationConversion(target_pose.orientation, M_PI, 0, 0); 
   geometry_msgs::Pose grasp_pose = target_pose;

   visual_tools.prompt("Calcolo per riconfigurazione");

   // Lettura dati
   std_msgs::Float32MultiArray::ConstPtr fo_params;
   std_msgs::Float32MultiArray::ConstPtr so_params;
   std_msgs::Float32MultiArray::ConstPtr fo_flag;
   std_msgs::Float32MultiArray::ConstPtr so_flag;
   float c0, m0,cf, mf, c_error, m_error;

   std::ofstream myfile;
   myfile.open(FileName);

   float random;
   
   
   for(int i=0; i<test_no; i++)
   {
      std::cout << "Test ciclico no: " << i << "\n";
      myfile << i << " ";
      ros::Duration(3).sleep();
      gripper_client.call(schunk_open_pos_srv);
      ros::Duration(3).sleep();

      target_pose = grasp_pose;
      target_pose.position.z += 0.1;
      move_group.setPoseTarget(target_pose, "schunk_pg70_object_link");
      move_group.move();

      myfile << target_pose.position.z << " ";

      ros::Duration(1).sleep();

      target_pose.position.z -= 0.1;

      random = (float)rand()/RAND_MAX;
      offset = offset_min + (offset_max - offset_min)*(float)random;
      target_pose.position.z += offset;

      move_group.setPoseTarget(target_pose, "schunk_pg70_object_link");
      move_group.move();

      ros::Duration(1).sleep();
      gripper_client.call(schunk_pos_srv);
      ros::Duration(4).sleep();
   
      fo_params = ros::topic::waitForMessage<std_msgs::Float32MultiArray>("/first_order_params_F101", ros::Duration(5));
      c0 = fo_params->data[1]/1000;
      m0 = -fo_params->data[0];
      fo_params = ros::topic::waitForMessage<std_msgs::Float32MultiArray>("/first_order_params_F102", ros::Duration(5));
      c0 += fo_params->data[1]/1000;
      m0 += fo_params->data[0];
      c0 = c0/2;
      m0 = m0/2;
      myfile << c0 << " ";
      myfile << m0 << " ";



      // Cambio della target_pose
      ROS_INFO("Calc. of correction parameter");
      target_pose.position.z = target_pose.position.z - c0;
      float rotation = atan(m0);
      target_pose.orientation = orientationConversion(target_pose.orientation, 0, -rotation, 0);
      std::cout << "New position to reach: \n" << target_pose << "\n";

      ros::Duration(1).sleep();
      gripper_client.call(schunk_open_pos_srv);
      ros::Duration(4).sleep();

      target_pose.position.z += 0.1;
      move_group.setPoseTarget(target_pose, "schunk_pg70_object_link");
      move_group.move();
      ros::Duration(1).sleep();

      target_pose.position.z -= 0.1;
      // Riposizionamento
      move_group.setPoseTarget(target_pose, "schunk_pg70_object_link");
      move_group.move();

      myfile << target_pose.position.z  << " ";

      // Chiusura Gripper
      ros::Duration(1).sleep();
      gripper_client.call(schunk_pos_srv);
      ros::Duration(4).sleep();

         
      fo_params = ros::topic::waitForMessage<std_msgs::Float32MultiArray>("/first_order_params_F101", ros::Duration(5));
      cf = fo_params->data[1]/1000;
      mf = fo_params->data[0];
      fo_params = ros::topic::waitForMessage<std_msgs::Float32MultiArray>("/first_order_params_F102", ros::Duration(5));
      cf += fo_params->data[1]/1000;
      mf -= fo_params->data[0];
      cf = cf/2;
      mf = mf/2;
      myfile << cf << " ";
      myfile << mf << " ";
      myfile << "\n";
   }

   myfile.close();

   ros::Duration(3).sleep();
   gripper_client.call(schunk_open_pos_srv);

   ros::shutdown();
   return 0;


}


geometry_msgs::Quaternion orientationConversion(geometry_msgs::Quaternion input_quat, float roll, float pitch, float yaw)
{

   tf2::Quaternion quat_rot, quat_out;
   tf2::Quaternion quat_in(input_quat.x, input_quat.y, input_quat.z, input_quat.w);


   quat_rot.setRPY(roll, pitch, yaw);

   quat_out = quat_in * quat_rot;

   geometry_msgs::Quaternion output_orientation = tf2::toMsg(quat_out);

   return output_orientation;
}

geometry_msgs::Pose PointConversion(geometry_msgs::Pose input_quat, float roll, float pitch, float yaw)
{

   tf2::Quaternion quat_rot, quat_out;
   tf2::Quaternion quat_in(input_quat.orientation.x, input_quat.orientation.y, input_quat.orientation.z, input_quat.orientation.w);
 
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


