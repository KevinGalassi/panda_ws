

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

#include <actionlib/client/simple_action_client.h>

#include <schunk_pg70/set_position.h>

#include <MyFunc.h>


#define PLANNING_ATTEMPTS_NO 500
#define INPUT_RPY 1

int main(int argc, char** argv)
{
   ros::init(argc, argv, "routing_app");
   ros::NodeHandle nh;

   ros::AsyncSpinner spinner(1);
   spinner.start();

   ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_state_request",100);
   ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/gripper_width_request", 100);

   std_msgs::Float32 width_msg;
   std_msgs::Float32 cmd_msg;

   ros::ServiceClient set_position = nh.serviceClient<schunk_pg70::set_position>("schunk_pg70/set_position");
      
   schunk_pg70::set_position schunk_pos_srv;

   schunk_pos_srv.request.goal_position = 60;      // 69
   schunk_pos_srv.request.goal_velocity = 60;      // 82
   schunk_pos_srv.request.goal_acceleration = 200; // 320

   namespace rvt = rviz_visual_tools;

   float T_O;
   std::string path1;
   float velocity_fix;
   float velocity_round;
   float velocity_pass;
   std::string group_name;
   std::string hand_name;
   std::string rviz_name;

   TrajectoryPlanner_param param;
   std::string node_name = "/PointPlot";
   nh.getParam(node_name + "/radius", param.radius);
   nh.getParam(node_name + "/heigh", param.heigh);
   nh.getParam(node_name + "/circ_point", param.circ_point);
   nh.getParam(node_name + "/resolution", param.res);
   nh.getParam(node_name + "/distance_approach", param.distance_approach);
   nh.getParam(node_name + "/ctrl_pt_1", param.Ctrl_pt_d1);
   nh.getParam(node_name + "/ctrl_pt_2",  param.Ctrl_pt_d2);
   nh.getParam(node_name + "/corner_points",  param.corner_points);
   
   //nh.getParam(node_name + "/fix_init_dist", param.fix_init_dist);
   //nh.getParam(node_name + "/fix_final_dist", param.fix_final_dist);
 //  nh.getParam(node_name + "/cornerfix_init_dist", param.cornerfix_init_dist);
   //nh.getParam(node_name + "/cornerfix_final_dist", param.cornerfix_final_dist);
   //nh.getParam(node_name + "/cornerfix_lateral_shift", param.cornerfix_lateral_shift);
   //nh.getParam(node_name + "/cornerfix_round_height", param.cornerfix_round_height);

   ros::Duration T_offset = ros::Duration(0.4);
   moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
   moveit::planning_interface::MoveGroupInterface hand_group("hand");
   moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
   visual_tools.deleteAllMarkers();

   ROS_INFO("Reach Ready Position");
   move_group.setMaxVelocityScalingFactor(0.2);

   hand_group.setJointValueTarget(hand_ready_state);
   hand_group.move();
   move_group.setJointValueTarget(arm_ready_state);
   move_group.move();

   
/***************  SYSTEM PARAMETERS **************************/
   
   TrajectoryVector waypointsA;
 
/**************** COMPUTE TRAJECTORY PARTS     ******************/


   path1 = "/home/kevin/ros/panda_ws/src/robot_task/src/PointList/CableA";
   geometry_msgs::Pose grasp1;
   ReadFileTxt(INPUT_RPY, waypointsA.point, waypointsA.pt_label, path1, grasp1);
   MyTrajectoryPlanner4(param, waypointsA); 
   visual_tools.deleteAllMarkers();
   
   for(int i=0; i<waypointsA.SecondaryTrajectory.size(); i++)
      visual_tools.publishPath(waypointsA.SecondaryTrajectory[i].poses, rvt::BLUE, rvt::XXXSMALL);

   for(int i=0; i<waypointsA.pt_label.size(); ++i)
   {
      std::cout << i <<" : " << waypointsA.pt_label[i] << "\n";
   }


   visual_tools.trigger();
   visual_tools.prompt("next to B");

   path1 = "/home/kevin/ros/panda_ws/src/robot_task/src/PointList/CableB";
   TrajectoryVector waypointsB;
   ReadFileTxt(INPUT_RPY, waypointsB.point, waypointsB.pt_label, path1, grasp1);
   MyTrajectoryPlanner4(param, waypointsB); 
   visual_tools.deleteAllMarkers();
   
   for(int i=0; i<waypointsB.SecondaryTrajectory.size(); i++)
   {
      visual_tools.publishPath(waypointsB.SecondaryTrajectory[i].poses, rvt::RED, rvt::XXXSMALL);
   }
   visual_tools.trigger();
   visual_tools.prompt("next to C");

   path1 = "/home/kevin/ros/panda_ws/src/robot_task/src/PointList/CableC";
   TrajectoryVector waypointsC;
   ReadFileTxt(INPUT_RPY, waypointsC.point, waypointsC.pt_label, path1, grasp1);
   MyTrajectoryPlanner4(param, waypointsC);  
   visual_tools.deleteAllMarkers();
   
   for(int i=0; i<waypointsC.SecondaryTrajectory.size(); i++)
   {
      visual_tools.publishPath(waypointsC.SecondaryTrajectory[i].poses, rvt::GREEN, rvt::XXXSMALL);
   }
   visual_tools.trigger();





   ros::shutdown();
   return 0;
}

