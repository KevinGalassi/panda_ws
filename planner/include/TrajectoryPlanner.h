
#ifndef TRAJECTORYPLANNER_H
#define TRAJECTORYPLANNER_h


#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_visual_tools/moveit_visual_tools.h>


#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion

#include <math.h>

#include "parameters.h"

namespace rvt = rviz_visual_tools;


struct PlannerParameter
{
   double radius;
   double heigh;
   double clip_height;
   int circ_point;
   double res;
   double distance_approach;
   double Ctrl_pt_d1;
   double Ctrl_pt_d2;
   int corner_points; 
   double dist_init[3];        // da modificare nome
   double dist_final[3];
   double gripper_offset[3];
   bool orientation;
   double z_offset; // Offset con cui si abbassa il punto iniziale
   double cornerfix_lateral_shift;
   double cornerfix_additional_height;
   tf2::Quaternion EE_shift;
   tf2::Matrix3x3 EE_R;
};

/*
    geometry_msgs::PoseArray point;                 -> InitPoint
    std::vector<std::string>  pt_label;             -> InitLabel    
    std::vecto...eArray>     SecondaryTrajectory    ->  SecondaryTrajectories
*/

float ComputeDistance(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2);
float ComputePathLength(geometry_msgs::PoseArray waypoint);

class TrajectoryPlanner{

public:
   TrajectoryPlanner();
   ~TrajectoryPlanner();

   void PlannerInit();
   void PlannerReset();

   bool ReadFileTxt();             // Acquire data fom the txt file
   void TrajectoryPlanning();      // Plan senza abbassamento
   void FromEE2Link8();
   void FromLink82EE();

   // Parameters
   std::string file_path;
   std::string visualizer;

   bool input_rpy = true;
   PlannerParameter param;

   geometry_msgs::PoseArray InitPoint;
   geometry_msgs::PoseArray Trajectory;

   std::vector<std::string> InitLabel;
   std::vector<std::string> label_list;
   
   std::vector<geometry_msgs::PoseArray> SecondaryTrajectories;
    

   void PublishInputPoint(moveit_visual_tools::MoveItVisualTools& visual_tools);   
   void PublishTrajectory(moveit_visual_tools::MoveItVisualTools& visual_tools);


private:



   void addFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints);   // Non usare
   void addPreFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints);
   void addPostFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints);
   
  
   void CornerDetection();
   void addCornerFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints, geometry_msgs::Pose corner_point, geometry_msgs::Pose starting_point);
   void addCornerRound(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::Pose Point3, double distance1, double distance2, geometry_msgs::PoseArray& waypoints);
   int ComputePatch(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::PoseArray& waypoints);

   void CheckCornerDistance(std::vector<std::string> Traj_id, std::vector<geometry_msgs::PoseArray> Traj_list, int i, double& distance1, double& distance2);


};


#endif