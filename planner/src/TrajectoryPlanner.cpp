#include "TrajectoryPlanner.h"


TrajectoryPlanner::TrajectoryPlanner(){}
TrajectoryPlanner::~TrajectoryPlanner(){}
// Public

void TrajectoryPlanner::PlannerInit()
{
   PlannerReset();

   param.radius = 0.05;                // Radius of the semi-circle for fixing part
   param.heigh = 0.03;                 // Heigh of upward movement
   param.clip_height = 0.03;
   param.circ_point = 1000;             // Point generated in the semi-circle
   param.res = 0.001;                  // Distance beetween two following points in the final trajectory
   param.distance_approach = 0.05;     // Distance from which starts the grasping operation
   param.Ctrl_pt_d1 = 0.05;            // Distance of the first control point in the rounding function
   param.Ctrl_pt_d2 = 0.05;            // Distance of the second point
   param.corner_points = 100;          // Numver of point generated in the corner.
   
   param.dist_init[0] = 0.05;
   param.dist_init[1] = 0;
   param.dist_init[2] = 0;

   param.dist_final[0] = 0.05;
   param.dist_final[1] = 0;
   param.dist_final[2] = 0;

   param.doubleFixDist = 0.035;

   param.gripper_offset[0] = 0;
   param.gripper_offset[1] = 0;
   param.gripper_offset[2] = 0;

   param.cornerfix_lateral_shift = 0.03;
   param.cornerfix_additional_height = 0.015;
   
   param.orientation = false;    // True passa a destra dell'ostacolo, false a simnistra
   param.z_offset = 0.00; //0.02; // Offset con cui si abbassa il punto iniziale
   param.EE_shift.setRPY(0,0,M_PI/4);
   param.EE_R.setRotation(param.EE_shift);

}
void TrajectoryPlanner::PlannerReset()
{
    InitPoint.poses.clear();
    InitLabel.clear();
    SecondaryTrajectories.clear();
    Trajectory.poses.clear();
    label_list.clear();

}
bool TrajectoryPlanner::ReadFileTxt()
{
    std::ifstream txt_file;
    geometry_msgs::Pose target_goal;
    std::string pose_identifie;

    txt_file.open(file_path);
    if( txt_file.is_open())
    {
        ROS_INFO("File succesfully open");
    }
    else
    {
        ROS_ERROR("file not open");
        return false;
    }

    if(input_rpy)
    {
        float input_RPY[3] = {0, 0, 0};
        tf2::Quaternion input_quaternion;
        while ( !txt_file.eof() )
        { 
            std::cout << "data readen \n";
            txt_file >> target_goal.position.x;
            txt_file >> target_goal.position.y;
            txt_file >> target_goal.position.z;
            txt_file >> input_RPY[0];
            txt_file >> input_RPY[1];
            txt_file >> input_RPY[2];
            input_quaternion.setRPY(input_RPY[0], input_RPY[1], input_RPY[2]);
            target_goal.orientation = tf2::toMsg(input_quaternion);
            txt_file >> pose_identifie;

            InitPoint.poses.push_back(target_goal);
            InitLabel.push_back(pose_identifie);
        }
    }
    else
    {
        while ( !txt_file.eof() )
        { 
            std::cout << "data readen \n";
            txt_file >> target_goal.position.x;
            txt_file >> target_goal.position.y;
            txt_file >> target_goal.position.z;
            txt_file >> target_goal.orientation.w;
            txt_file >> target_goal.orientation.x;
            txt_file >> target_goal.orientation.y;
            txt_file >> target_goal.orientation.z;
            txt_file >> pose_identifie;

            InitPoint.poses.push_back(target_goal);
            InitLabel.push_back(pose_identifie);
        }
    }
    txt_file.close();
    return true;
}


 bool TrajectoryPlanner::ReadFiletXTF2()
 {
    // Format File:

    // Grasp pose
    // Connector Insertion
    // --- Separator
    // Start Point
    // point 1
    // point 2 ... 
    // point n
    // ---
    // Repeat


   std::ifstream txt_file;
   geometry_msgs::Pose target_goal;
   std::string pose_identifie;

   txt_file.open(file_path);
   if( txt_file.is_open())
   {
      ROS_INFO("File succesfully open");
   }
   else
   {
      ROS_ERROR("file not open");
      return false;
   }

   

   if(input_rpy)
   {
      float input_RPY[3] = {0, 0, 0};
      tf2::Quaternion input_quaternion;

      // Read of grasp point
      txt_file >> target_goal.position.x;
      txt_file >> target_goal.position.y;
      txt_file >> target_goal.position.z;
      txt_file >> input_RPY[0];
      txt_file >> input_RPY[1];
      txt_file >> input_RPY[2];
      input_quaternion.setRPY(input_RPY[0], input_RPY[1], input_RPY[2]);
      target_goal.orientation = tf2::toMsg(input_quaternion);
      GraspPoint = target_goal;

      // Read of Connector Point
      txt_file >> target_goal.position.x;
      txt_file >> target_goal.position.y;
      txt_file >> target_goal.position.z;
      txt_file >> input_RPY[0];
      txt_file >> input_RPY[1];
      txt_file >> input_RPY[2];
      input_quaternion.setRPY(input_RPY[0], input_RPY[1], input_RPY[2]);
      target_goal.orientation = tf2::toMsg(input_quaternion);
      ConnectorPoint = target_goal;

      std::string str_flag;
      int counter = 0;

      while ( !txt_file.eof() )
      {
         txt_file >> str_flag;

         if(str_flag == "---")
         {
            counter++;
            txt_file >> target_goal.position.x;
         }
         else
         {
            target_goal.position.x = (float)str_flag;
         }

         txt_file >> target_goal.position.y;
         txt_file >> target_goal.position.z;
         txt_file >> input_RPY[0];
         txt_file >> input_RPY[1];
         txt_file >> input_RPY[2];
         input_quaternion.setRPY(input_RPY[0], input_RPY[1], input_RPY[2]);
         target_goal.orientation = tf2::toMsg(input_quaternion);
         txt_file >> pose_identifie;

         VectorInitPoint[counter-1].poses.push_back(target_goal);
         VectorInitLabel[counter-1].push_back(pose_identifie);
      }
   }

   txt_file.close();
   return true;

 }


void TrajectoryPlanner::FromEE2Link8()
{
    geometry_msgs::Pose target_pose;
    tf2::Matrix3x3 R, R_shift, R_final;
    tf2::Quaternion quat;
    tf2::Vector3 axis;

    quat.setRPY(0,0,M_PI/4);
    R_shift.setRotation(quat);

    for(size_t j=0; j<SecondaryTrajectories.size(); j++)
    {
        for (size_t i=0; i < SecondaryTrajectories[j].poses.size(); i++)
        {
            quat.setX(SecondaryTrajectories[j].poses[i].orientation.x);
            quat.setY(SecondaryTrajectories[j].poses[i].orientation.y);
            quat.setZ(SecondaryTrajectories[j].poses[i].orientation.z);
            quat.setW(SecondaryTrajectories[j].poses[i].orientation.w);
            R.setRotation(quat);

            target_pose.position.x = SecondaryTrajectories[j].poses[i].position.x -( R[0][0]*param.gripper_offset[0] + R[0][1]*param.gripper_offset[1] + R[0][2]*param.gripper_offset[2]) ;
            target_pose.position.y = SecondaryTrajectories[j].poses[i].position.y -( R[1][0]*param.gripper_offset[0] + R[1][1]*param.gripper_offset[1] + R[1][2]*param.gripper_offset[2]) ;
            target_pose.position.z = SecondaryTrajectories[j].poses[i].position.z -( R[2][0]*param.gripper_offset[0] + R[2][1]*param.gripper_offset[1] + R[2][2]*param.gripper_offset[2]) ;

            R_final = R*param.EE_R;
            R_final.getRotation(quat);
            quat.normalize();
            target_pose.orientation = tf2::toMsg(quat);

            SecondaryTrajectories[j].poses[i].position    = target_pose.position;
            SecondaryTrajectories[j].poses[i].orientation = target_pose.orientation;    
        }
    }
    return;
}
void TrajectoryPlanner::FromLink82EE(){}

void TrajectoryPlanner::PublishInputPoint(moveit_visual_tools::MoveItVisualTools& visual_tools)
{
    for(size_t i=0; i<InitPoint.poses.size(); i++ )
        visual_tools.publishAxisLabeled(InitPoint.poses[i], InitLabel[i] , rviz_visual_tools::XXXSMALL);
    visual_tools.trigger();        
}
void TrajectoryPlanner::PublishTrajectory(moveit_visual_tools::MoveItVisualTools& visual_tools)
{
    for(size_t i = 0; i<SecondaryTrajectories.size(); i++)
    {
        if(SecondaryTrajectories.size() > 2)
            visual_tools.publishPath(SecondaryTrajectories[i].poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    }    
    visual_tools.trigger();
}

// PRIVATE F
void TrajectoryPlanner::addFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints)
{
   // Select if the shif occur to the left of the clip or to the right
   float left_sign;
   if(param.orientation)
      left_sign = 1.0;
   else
      left_sign = -1.0;
   

   geometry_msgs::Pose target_pose;
   geometry_msgs::Point center;

   tf2::Matrix3x3 R;
   tf2::Quaternion quat;

   quat.setX(fixing_point.orientation.x);
   quat.setY(fixing_point.orientation.y);
   quat.setZ(fixing_point.orientation.z);
   quat.setW(fixing_point.orientation.w);
   R.setRotation(quat);

   // Starting point from which the EE move laterally
   target_pose.position.x = fixing_point.position.x - (R[0][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][2]*param.dist_init[2]);
   target_pose.position.y = fixing_point.position.y - (R[1][0]*param.dist_init[0] + R[1][1]*param.dist_init[1] + R[1][2]*param.dist_init[2]);
   target_pose.position.z = fixing_point.position.z - (R[2][0]*param.dist_init[0] + R[2][1]*param.dist_init[1] + R[2][2]*param.dist_init[2]) - param.z_offset;
   target_pose.orientation = fixing_point.orientation;
   waypoints.poses.push_back(target_pose);

   // lateral shift of the diameter of the circle
   target_pose.position.x = fixing_point.position.x + left_sign*R[0][1]*param.radius*2;
   target_pose.position.y = fixing_point.position.y + left_sign*R[1][1]*param.radius*2;
   target_pose.position.z = fixing_point.position.z + left_sign*R[2][1]*param.radius*2 - param.z_offset;
   target_pose.orientation = fixing_point.orientation;
   waypoints.poses.push_back(target_pose);
       
    // Upshift (Earlier point + heigh)
   target_pose.position.x = target_pose.position.x - R[0][2]*param.heigh;
   target_pose.position.y = target_pose.position.y - R[1][2]*param.heigh;
   target_pose.position.z = target_pose.position.z - R[2][2]*param.heigh;
   waypoints.poses.push_back(target_pose);

   // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
   center.x = target_pose.position.x + left_sign*R[0][1]*param.radius;
   center.y = target_pose.position.y + left_sign*R[1][1]*param.radius;
   center.z = target_pose.position.z + left_sign*R[2][1]*param.radius;
        

   // Rotation
   int point_no;
   geometry_msgs::Pose first_circle_point;
   first_circle_point.position.x = target_pose.position.x - R[0][2]*param.heigh;
   first_circle_point.position.y = target_pose.position.y - R[1][2]*param.heigh;
   first_circle_point.position.z = target_pose.position.z - R[2][2]*param.heigh;
   geometry_msgs::Pose last_circle_point;
   last_circle_point.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
   last_circle_point.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
   last_circle_point.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
  
   point_no = (ComputeDistance(first_circle_point, last_circle_point)*param.radius*M_PI)/param.res;
   for(int k = 0; k < point_no; ++k)
   {
      target_pose.position.x = center.x + left_sign*(R[0][1]*param.radius*cos(M_PI/point_no*k) + R[0][2]*param.radius*sin(M_PI/point_no*k)) + (R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2])*k/point_no;
      target_pose.position.y = center.y + left_sign*(R[1][1]*param.radius*cos(M_PI/point_no*k) + R[1][2]*param.radius*sin(M_PI/point_no*k)) + (R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2])*k/point_no;
      target_pose.position.z = center.z + left_sign*(R[2][1]*param.radius*cos(M_PI/point_no*k) + R[2][2]*param.radius*sin(M_PI/point_no*k)) + (R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2])*k/point_no;
      waypoints.poses.push_back(target_pose);
   }

   // Final point of the circle
   target_pose.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
   target_pose.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
   target_pose.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
   waypoints.poses.push_back(target_pose);

   // Downshift
   target_pose.position.x = fixing_point.position.x + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
   target_pose.position.y = fixing_point.position.y + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
   target_pose.position.z = fixing_point.position.z + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
   waypoints.poses.push_back(target_pose);
}

void TrajectoryPlanner::addPreFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints)
{

   float left_sign;
   if(param.orientation)
      left_sign = 1.0;
   else
      left_sign = -1.0;
   

   geometry_msgs::Pose target_pose;
   geometry_msgs::Point center;

   tf2::Matrix3x3 R;
   tf2::Quaternion quat;

   quat.setX(fixing_point.orientation.x);
   quat.setY(fixing_point.orientation.y);
   quat.setZ(fixing_point.orientation.z);
   quat.setW(fixing_point.orientation.w);
   R.setRotation(quat);

   target_pose.position.x = fixing_point.position.x - (R[0][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][2]*param.dist_init[2]);
   target_pose.position.y = fixing_point.position.y - (R[1][0]*param.dist_init[0] + R[1][1]*param.dist_init[1] + R[1][2]*param.dist_init[2]);
   target_pose.position.z = fixing_point.position.z - (R[2][0]*param.dist_init[0] + R[2][1]*param.dist_init[1] + R[2][2]*param.dist_init[2]) - param.z_offset;
   target_pose.orientation = fixing_point.orientation;
   waypoints.poses.push_back(target_pose);

   // lateral shift of the diameter of the circle
   target_pose.position.x = fixing_point.position.x + left_sign*R[0][1]*param.radius*2;
   target_pose.position.y = fixing_point.position.y + left_sign*R[1][1]*param.radius*2;
   target_pose.position.z = fixing_point.position.z + left_sign*R[2][1]*param.radius*2 - param.z_offset;
   target_pose.orientation = fixing_point.orientation;
   waypoints.poses.push_back(target_pose);
       
    // Upshift (Earlier point + heigh)
   target_pose.position.x = target_pose.position.x - R[0][2]*param.heigh;
   target_pose.position.y = target_pose.position.y - R[1][2]*param.heigh;
   target_pose.position.z = target_pose.position.z - R[2][2]*param.heigh;
   waypoints.poses.push_back(target_pose);

   // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
   center.x = target_pose.position.x + left_sign*R[0][1]*param.radius;
   center.y = target_pose.position.y + left_sign*R[1][1]*param.radius;
   center.z = target_pose.position.z + left_sign*R[2][1]*param.radius;


}
void TrajectoryPlanner::addPostFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints)
{

   float left_sign;
   if(param.orientation)
      left_sign = 1.0;
   else
      left_sign = -1.0;
   
   geometry_msgs::Pose target_pose;
   geometry_msgs::Point center;

   tf2::Matrix3x3 R;
   tf2::Quaternion quat;

   quat.setX(fixing_point.orientation.x);
   quat.setY(fixing_point.orientation.y);
   quat.setZ(fixing_point.orientation.z);
   quat.setW(fixing_point.orientation.w);
   R.setRotation(quat);

   // lateral shift of the diameter of the circle
   target_pose.position.x = fixing_point.position.x + left_sign*R[0][1]*param.radius*2;
   target_pose.position.y = fixing_point.position.y + left_sign*R[1][1]*param.radius*2;
   target_pose.position.z = fixing_point.position.z + left_sign*R[2][1]*param.radius*2 - param.z_offset;
   target_pose.orientation = fixing_point.orientation;
       
    // Upshift (Earlier point + heigh)
   target_pose.position.x = target_pose.position.x - R[0][2]*param.heigh;
   target_pose.position.y = target_pose.position.y - R[1][2]*param.heigh;
   target_pose.position.z = target_pose.position.z - R[2][2]*param.heigh;

   // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
   center.x = target_pose.position.x - left_sign*R[0][1]*param.radius;
   center.y = target_pose.position.y - left_sign*R[1][1]*param.radius;
   center.z = target_pose.position.z - left_sign*R[2][1]*param.radius;
        

   int point_no;
   geometry_msgs::Pose first_circle_point;
   geometry_msgs::Pose last_circle_point;
   first_circle_point.position.x = target_pose.position.x - R[0][2]*param.heigh;
   first_circle_point.position.y = target_pose.position.y - R[1][2]*param.heigh;
   first_circle_point.position.z = target_pose.position.z - R[2][2]*param.heigh;

   last_circle_point.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
   last_circle_point.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
   last_circle_point.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
  
   point_no = (ComputeDistance(first_circle_point, last_circle_point)*param.radius*M_PI)/param.res + 1;

   for(int k = 0; k < point_no; ++k)
   {
      target_pose.position.x = center.x + left_sign*(R[0][1]*param.radius*cos(M_PI/point_no*k)) + R[0][2]*param.radius*sin(M_PI/point_no*k) + (R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2])*k/point_no;
      target_pose.position.y = center.y + left_sign*(R[1][1]*param.radius*cos(M_PI/point_no*k)) + R[1][2]*param.radius*sin(M_PI/point_no*k) + (R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2])*k/point_no;
      target_pose.position.z = center.z +  ( - R[2][1]*param.radius*cos(M_PI/point_no*k) - R[2][2]*param.radius*sin(M_PI/point_no*k)) + (R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2])*k/point_no;
      waypoints.poses.push_back(target_pose);
   }
   /*
   target_pose.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
   target_pose.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
   target_pose.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
   waypoints.poses.push_back(target_pose);
   */

   // Downshift
   target_pose.position.x = fixing_point.position.x + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
   target_pose.position.y = fixing_point.position.y + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
   target_pose.position.z = fixing_point.position.z + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
   waypoints.poses.push_back(target_pose);

}

// PRIVATE F
void TrajectoryPlanner::addDoubleFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints)
{
   // Select if the shif occur to the left of the clip or to the right
   float right_sign;
   if(param.orientation)
      right_sign = 1.0;
   else
      right_sign = -1.0;
   

   geometry_msgs::Pose target_pose;
   geometry_msgs::Point center;

   tf2::Matrix3x3 R;
   tf2::Quaternion quat;

   quat.setX(fixing_point.orientation.x);
   quat.setY(fixing_point.orientation.y);
   quat.setZ(fixing_point.orientation.z);
   quat.setW(fixing_point.orientation.w);
   R.setRotation(quat);

   // Starting point from which the EE move laterally
   geometry_msgs::Pose startPose;
   startPose.position.x = fixing_point.position.x - R[0][0]*param.doubleFixDist - (R[0][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][2]*param.dist_init[2]);
   startPose.position.y = fixing_point.position.y - R[1][0]*param.doubleFixDist - (R[1][0]*param.dist_init[0] + R[1][1]*param.dist_init[1] + R[1][2]*param.dist_init[2]);
   startPose.position.z = fixing_point.position.z - R[2][0]*param.doubleFixDist - (R[2][0]*param.dist_init[0] + R[2][1]*param.dist_init[1] + R[2][2]*param.dist_init[2]) - param.z_offset;
   startPose.orientation = fixing_point.orientation;
   waypoints.poses.push_back(startPose);


   float angle = atan(param.radius/(sqrt(pow(param.dist_init[0],2) + pow(param.dist_init[1],2) + pow(param.dist_init[2],2))));
   float radius = ComputeDistance(fixing_point, startPose);

   // Lateral Shift 
   target_pose.position.x = startPose.position.x + R[0][0]*radius*sin(angle) + right_sign*R[0][1]*radius*cos(angle);
   target_pose.position.y = startPose.position.y + R[1][0]*radius*sin(angle) + right_sign*R[1][1]*radius*cos(angle);
   target_pose.position.z = startPose.position.z + R[2][0]*radius*sin(angle) + right_sign*R[2][1]*radius*cos(angle);
   waypoints.poses.push_back(target_pose);


   // Rise the cable

   geometry_msgs::PoseArray points_flag;
   geometry_msgs::Pose Pose_2;

   Pose_2.position.x = target_pose.position.x - R[0][2]*param.heigh;
   Pose_2.position.y = target_pose.position.y - R[1][2]*param.heigh;
   Pose_2.position.z = target_pose.position.z - R[2][2]*param.heigh;
   ComputePatch(Pose_2, target_pose, points_flag);


   // lateral shift of the diameter of the circle
   target_pose.position.x = fixing_point.position.x - right_sign*R[0][1]*param.radius*2;
   target_pose.position.y = fixing_point.position.y - right_sign*R[1][1]*param.radius*2;
   target_pose.position.z = fixing_point.position.z - right_sign*R[2][1]*param.radius*2 - param.z_offset;
   target_pose.orientation = fixing_point.orientation;
   points_flag.poses.push_back(target_pose);
       

   // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
   center.x = target_pose.position.x - right_sign*R[0][1]*param.radius;
   center.y = target_pose.position.y - right_sign*R[1][1]*param.radius;
   center.z = target_pose.position.z - right_sign*R[2][1]*param.radius;

   // Rotation
   int point_no;
   geometry_msgs::Pose first_circle_point;
   first_circle_point.position.x = target_pose.position.x - R[0][2]*param.heigh;
   first_circle_point.position.y = target_pose.position.y - R[1][2]*param.heigh;
   first_circle_point.position.z = target_pose.position.z - R[2][2]*param.heigh;

   geometry_msgs::Pose last_circle_point;
   last_circle_point.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
   last_circle_point.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
   last_circle_point.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
  
   point_no = (ComputeDistance(first_circle_point, last_circle_point)*param.radius*M_PI)/param.res;
   for(int k = 0; k < point_no; ++k)
   {
      target_pose.position.x = center.x - right_sign*(R[0][1]*param.radius*cos(M_PI/point_no*k) + R[0][2]*param.radius*sin(M_PI/point_no*k)) + (R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2])*k/point_no;
      target_pose.position.y = center.y - right_sign*(R[1][1]*param.radius*cos(M_PI/point_no*k) + R[1][2]*param.radius*sin(M_PI/point_no*k)) + (R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2])*k/point_no;
      target_pose.position.z = center.z - right_sign*(R[2][1]*param.radius*cos(M_PI/point_no*k) + R[2][2]*param.radius*sin(M_PI/point_no*k)) + (R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2])*k/point_no;
      points_flag.poses.push_back(target_pose);
   }

   // Final point of the circle
   target_pose.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
   target_pose.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
   target_pose.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
   points_flag.poses.push_back(target_pose);


   sphereProjection(startPose, radius, points_flag);

   // Downshift
   geometry_msgs::Pose endPose;
   endPose.position.x = fixing_point.position.x + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
   endPose.position.y = fixing_point.position.y + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
   endPose.position.z = fixing_point.position.z + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
   ComputePatch(endPose, points_flag.poses[points_flag.poses.size()-1], points_flag);

   for(int i=0; i<points_flag.poses.size(); i++)
      waypoints.poses.push_back(points_flag.poses[i]);


   return;
}



void TrajectoryPlanner::CornerDetection()
{
   float AB, AC, BC;
   for(int i = 1; i < (InitPoint.poses.size()-1); i++)
   {
      if(InitLabel[i] == "pass" && InitLabel[i-1] != "grasp")
      {
         AB = ComputeDistance(InitPoint.poses[i], InitPoint.poses[i-1]);
         AC = ComputeDistance(InitPoint.poses[i+1], InitPoint.poses[i-1]);
         BC = ComputeDistance(InitPoint.poses[i], InitPoint.poses[i+1]);
         if (AB + BC != AC) // Aggiungere un eventuale valore minimo per cui non è necessario eseguire la curva
            InitLabel[i] = "corner";
      }
   }
   return;
}
void TrajectoryPlanner::addCornerFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints, geometry_msgs::Pose corner_point, geometry_msgs::Pose starting_point)
{
    // Cornerfix, no lowering
   float P12, P23, P13;
   P12 = ComputeDistance(starting_point, corner_point);
   P23 = ComputeDistance(corner_point, fixing_point);
   P13 = ComputeDistance(starting_point, fixing_point);
   float corner_length = P12 + P23 + param.dist_final[0];

   tf2::Quaternion quat;
   tf2::fromMsg(starting_point.orientation, quat);
   tf2::Matrix3x3 Rot;
   Rot.setRotation(quat);
   geometry_msgs::Pose target_pose;
   geometry_msgs::Pose intermediate_pose;

   float angle;


   // Suppose to be on a plane

   geometry_msgs::Pose flag_pose;

   flag_pose.position.x = corner_point.position.x + Rot[0][1]*0.1;
   flag_pose.position.y = corner_point.position.y + Rot[0][1]*0.1;
   flag_pose.position.z = corner_point.position.z + Rot[0][1]*0.1;


   float xa = flag_pose.position.x - corner_point.position.x;
   float ya = flag_pose.position.y - corner_point.position.y;
   
   float xb = fixing_point.position.x - corner_point.position.x;
   float yb = fixing_point.position.y - corner_point.position.y;

   std::cout << " "<< flag_pose.position.x << "\n";
   std::cout << "" << flag_pose.position.y << "\n";
   std::cout << "" << flag_pose.position.z << "\n";


   angle = acos((xa*xb + ya*yb) / (sqrt(pow(xa,2)+pow(ya,2) + sqrt(pow(xb,2) + (pow(yb,2))))));

   std::cout<< "angle" << angle <<"\n";


   float diff = fixing_point.position.y - corner_point.position.y;





   // WARNING Ricontrollare bene
   bool turn_right = false;
   //if (angle < M_PI)
   
   std::cout << "ffefe" << Rot[1][1]*diff << "\n";

   if( Rot[1][1]*diff> 0 )
      turn_right = true;

   if(turn_right)
      intermediate_pose.position.y = corner_point.position.y + param.cornerfix_lateral_shift;
   else
      intermediate_pose.position.y = corner_point.position.y - param.cornerfix_lateral_shift;
   intermediate_pose.position.x = corner_point.position.x;
   intermediate_pose.position.z = corner_point.position.z;
   intermediate_pose.orientation = starting_point.orientation;
   ComputePatch(starting_point, intermediate_pose, waypoints);


   P12 = ComputeDistance(starting_point, corner_point);
   P23 = ComputeDistance(corner_point, fixing_point);
   P13 = ComputeDistance(starting_point, fixing_point);
   angle = M_PI - acos((pow(P13,2) - pow(P12,2) - pow(P23,2)) / (-2*P12*P23));



   corner_length = P12 + P23 + param.dist_final[0];
   target_pose.position.x = starting_point.position.x + Rot[0][0]*corner_length;
   target_pose.position.y = starting_point.position.y + Rot[1][0]*corner_length;
   target_pose.position.z = starting_point.position.z + Rot[2][0]*corner_length;
   target_pose.orientation = starting_point.orientation;
   ComputePatch(intermediate_pose, target_pose, waypoints);

   // Rotation
   geometry_msgs::Point center;
   center.x = corner_point.position.x;
   center.y = corner_point.position.y;
   center.z = corner_point.position.z;

   corner_length = P23 + param.dist_final[0]; 
   float point_numbers = (angle*corner_length)/param.res;
   float t;    

   tf2::Quaternion StartPoseOrientation, FixPoseOrientation;
   tf2::fromMsg(starting_point.orientation, StartPoseOrientation);
   tf2::fromMsg(fixing_point.orientation, FixPoseOrientation);


   if(turn_right)
   {
      for(int i=1; i < point_numbers; i++)
      {
         t=1/point_numbers*i;
         target_pose.position.x = center.x + corner_length*Rot[0][0]*cos(M_PI/2*t) + corner_length*Rot[0][1]*sin(M_PI/2*t);
         target_pose.position.y = center.y + corner_length*Rot[1][0]*cos(M_PI/2*t) + corner_length*Rot[1][1]*sin(M_PI/2*t);
         target_pose.position.z = center.z + corner_length*Rot[2][0]*cos(M_PI/2*t) + corner_length*Rot[2][1]*sin(M_PI/2*t) + (param.clip_height + param.cornerfix_additional_height)*t;
         target_pose.orientation = tf2::toMsg(StartPoseOrientation.slerp(FixPoseOrientation,t));
         waypoints.poses.push_back(target_pose);
      }
   }
   else
   {
      for(int i=1; i < point_numbers; i++)
      {
         t=1/point_numbers*i;
         target_pose.position.x = center.x + corner_length*Rot[0][0]*cos(M_PI/2*t) + corner_length*Rot[0][1]*sin(M_PI/2*t);
         target_pose.position.y = center.y - corner_length*Rot[1][0]*cos(M_PI/2*t) - corner_length*Rot[1][1]*sin(M_PI/2*t);
         target_pose.position.z = center.z + corner_length*Rot[2][0]*cos(M_PI/2*t) + corner_length*Rot[2][1]*sin(M_PI/2*t) + (param.clip_height + param.cornerfix_additional_height)*t;
         target_pose.orientation = tf2::toMsg(StartPoseOrientation.slerp(FixPoseOrientation,t));
         waypoints.poses.push_back(target_pose);
      }      
   }
   

   // Downshift
   tf2::fromMsg(fixing_point.orientation, quat);
   Rot.setRotation(quat);
   target_pose.position.x = fixing_point.position.x + Rot[0][0]*param.dist_final[0] + Rot[0][1]*param.dist_final[1] + Rot[0][2]*param.dist_final[2];
   target_pose.position.y = fixing_point.position.y + Rot[1][0]*param.dist_final[0] + Rot[1][1]*param.dist_final[1] + Rot[1][2]*param.dist_final[2];
   target_pose.position.z = fixing_point.position.z + Rot[2][0]*param.dist_final[0] + Rot[2][1]*param.dist_final[1] + Rot[2][2]*param.dist_final[2];
   waypoints.poses.push_back(target_pose);

}

void TrajectoryPlanner::addCornerRound(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::Pose Point3, double distance1, double distance2, geometry_msgs::PoseArray& waypoints)
{
   geometry_msgs::Pose point1_shift, point2_shift, target_point;
   geometry_msgs::Point delta_1, delta_2;
   double norm_1, norm_2;
   double d1, d2;
   float t;                // Used in for cycle €[0,1]

   // Compute the distance beetween the points with respect to x,y,z
   delta_1.x = Point2.position.x - Point1.position.x;
   delta_1.y = Point2.position.y - Point1.position.y;
   delta_1.z = Point2.position.z - Point1.position.z;
   delta_2.x = Point3.position.x - Point2.position.x;
   delta_2.y = Point3.position.y - Point2.position.y;
   delta_2.z = Point3.position.z - Point2.position.z;    

   // Check if the shifting distance to start the cornering is greater than the distance beetween the two points to avoid to go backward
   norm_1 = sqrt(delta_1.x*delta_1.x + delta_1.y*delta_1.y + delta_1.z*delta_1.z);    
   if (norm_1 > distance1)
   {
      d1 = distance1; 
      point1_shift.position.x = Point1.position.x + (norm_1-d1)/norm_1*(Point2.position.x - Point1.position.x);
      point1_shift.position.y = Point1.position.y + (norm_1-d1)/norm_1*(Point2.position.y - Point1.position.y);
      point1_shift.position.z = Point1.position.z + (norm_1-d1)/norm_1*(Point2.position.z - Point1.position.z);
      point1_shift.orientation = Point1.orientation;
      waypoints.poses.push_back(point1_shift);
   }
   else
   {
      d1 = norm_1;
      point1_shift = Point1;
   }

   norm_2 = sqrt(delta_2.x*delta_2.x + delta_2.y*delta_2.y + delta_2.z*delta_2.z);    
   if (norm_2 > distance2)
   {
      d2 = distance2;
      point2_shift.position.x = Point2.position.x + d2/norm_2*(Point3.position.x - Point2.position.x);
      point2_shift.position.y = Point2.position.y + d2/norm_2*(Point3.position.y - Point2.position.y);
      point2_shift.position.z = Point2.position.z + d2/norm_2*(Point3.position.z - Point2.position.z);
      point2_shift.orientation = Point3.orientation;
      // No push_back here becouse id done later
   }
   else
   {
      d2 = norm_2;
      point2_shift = Point3;
   }

   tf2::Quaternion Tg_Pose_Orientation1, Tg_Pose_Orientation2;
    tf2::fromMsg(Point1.orientation, Tg_Pose_Orientation1);
    tf2::fromMsg(Point3.orientation, Tg_Pose_Orientation2);

    // Cornering function using bèzier quadratic function
    for (int i = 0; i < param.corner_points; i++)
    {
        t = 1/param.corner_points*i;
        // P1 + (1-t)^2(P0-P1)+t^2(P2-P1)
        target_point.position.x = Point2.position.x + (1-t)*(1-t)*(point1_shift.position.x - Point2.position.x) + t*t*(point2_shift.position.x - Point2.position.x);
        target_point.position.y = Point2.position.y + (1-t)*(1-t)*(point1_shift.position.y - Point2.position.y) + t*t*(point2_shift.position.y - Point2.position.y);
        target_point.position.z = Point2.position.z + (1-t)*(1-t)*(point1_shift.position.z - Point2.position.z) + t*t*(point2_shift.position.z - Point2.position.z);
        target_point.orientation = tf2::toMsg(Tg_Pose_Orientation1.slerp(Tg_Pose_Orientation2,t));
        waypoints.poses.push_back(target_point);
    }

    if (norm_2 > d2)
        waypoints.poses.push_back(point2_shift);
}

int TrajectoryPlanner::ComputePatch(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::PoseArray& waypoints)
{
    /*
    *   Given two points as input returns a series of points with a distance of res beetwen 
    *   the two points included.
    */ 

    ROS_INFO("COMPUTE PATCH");
    geometry_msgs::Pose target_pose;
    geometry_msgs::Point delta;
    tf2::Quaternion Tg_Pose_Orientation1, Tg_Pose_Orientation2;
    double distance;
    double t;                // Used in for cycle €[0,1]
    double step;

    int patch_added = 0;

    // Compute the distance beetween the points with respect to x,y,z
    delta.x = Point2.position.x - Point1.position.x;
    delta.y = Point2.position.y - Point1.position.y;
    delta.z = Point2.position.z - Point1.position.z;

    distance = sqrt(delta.x*delta.x + delta.y*delta.y + delta.z*delta.z);      
    step = distance/param.res;

    tf2::fromMsg(Point1.orientation, Tg_Pose_Orientation1);
    tf2::fromMsg(Point2.orientation, Tg_Pose_Orientation2);

    for(int i = 1; i < step; i++)
    {
        t = 1/(step)*i;
        target_pose.position.x = Point1.position.x + t*delta.x;
        target_pose.position.y = Point1.position.y + t*delta.y;
        target_pose.position.z = Point1.position.z + t*delta.z;
        target_pose.orientation = Point2.orientation;
        target_pose.orientation = tf2::toMsg(Tg_Pose_Orientation1.slerp(Tg_Pose_Orientation2,t));
        waypoints.poses.push_back(target_pose);
        patch_added++;
    }
    return patch_added;
}

void TrajectoryPlanner::TrajectoryPlanning()
{    
   std::vector<geometry_msgs::PoseArray> Trajectories_list, Trajectory_list_with_patch;        // Store the trajectory of each point of the file

   geometry_msgs::PoseArray TrajectoryPart;    
   std::vector<std::string> Trajectory_id;            
   geometry_msgs::PoseArray Patch_To_Add;
   float PP_distance;          

/* Pre-Corner Detection */ 
   CornerDetection();
   
/* Secondary Trajectory Elaboration */
   for (std::size_t i = 0; i < InitPoint.poses.size(); i ++)
   {  
      ROS_INFO("Reading of label no: %d, value = ", i, InitLabel[i]);
        
        if (InitLabel[i] == "fix")
        {   
            if(i > 2)
            {
               if(InitLabel[i-1] == "corner")
               {
                  ROS_INFO("Detection of a Corner and a Fix, substituition with CornerFIx \n");
                  addCornerFixPoint(InitPoint.poses[i], TrajectoryPart, InitPoint.poses[i-1], Trajectories_list[Trajectories_list.size()-1].poses[Trajectories_list[Trajectories_list.size()-1].poses.size()-1]);
                  Trajectories_list.push_back(TrajectoryPart);
                  TrajectoryPart.poses.clear();
                  InitLabel[i-1] == "cornerfix";
                  Trajectory_id.push_back("cornerfix");
               }
               else
               {
                  // Cambiare dopo aver sistemato PreFix
                  addPreFixPoint(InitPoint.poses[i], TrajectoryPart);
                  Trajectories_list.push_back(TrajectoryPart);
                  TrajectoryPart.poses.clear();
                  Trajectory_id.push_back("pre-fix");

                  addPostFixPoint(InitPoint.poses[i], TrajectoryPart);
                  Trajectories_list.push_back(TrajectoryPart);
                  TrajectoryPart.poses.clear();
                  Trajectory_id.push_back("fix");
               }
            }
         else
         {
            addPreFixPoint(InitPoint.poses[i], TrajectoryPart);
            Trajectories_list.push_back(TrajectoryPart);
            TrajectoryPart.poses.clear();
            Trajectory_id.push_back("pre-fix");

            addPostFixPoint(InitPoint.poses[i], TrajectoryPart);
            Trajectories_list.push_back(TrajectoryPart);
            TrajectoryPart.poses.clear();
            Trajectory_id.push_back("fix");
         }
      }

      if(InitLabel[i] == "pass")
      {
         TrajectoryPart.poses.push_back(InitPoint.poses[i]);
         Trajectories_list.push_back(TrajectoryPart);
         TrajectoryPart.poses.clear();   
         Trajectory_id.push_back("pass");
      }

      if(InitLabel[i] == "corner")
      {
         if( i < InitPoint.poses.size()-1)
         {
            if(InitLabel[i+1] == "fix")
            {
               /* DO NOTHING*/
            }
            else
            {
               TrajectoryPart.poses.push_back(InitPoint.poses[i]);
               Trajectories_list.push_back(TrajectoryPart);
               TrajectoryPart.poses.clear();   
               Trajectory_id.push_back("corner");
            }     
         }
      }
   } 

/********* Add Corner **************/
   geometry_msgs::Pose P1,P2,P3;
   double new_Ctrl_Pt_d1;
   double new_Ctrl_Pt_d2;

   for(size_t i = 1; i < (Trajectories_list.size()-1); i++)
   {
      if(Trajectory_id[i] == "corner" && Trajectory_id[i+1] != "cornerfix")
      {
         P1 = Trajectories_list[i-1].poses[Trajectories_list[i-1].poses.size()];
         P2 = Trajectories_list[i].poses[0];
         P3 = Trajectories_list[i+1].poses[0];

         CheckCornerDistance(Trajectory_id, Trajectories_list, i, new_Ctrl_Pt_d1, new_Ctrl_Pt_d2);
         addCornerRound(P1, P2, P3, new_Ctrl_Pt_d1, new_Ctrl_Pt_d2, TrajectoryPart);
         Trajectories_list[i].poses.clear();
         Trajectories_list[i].poses = TrajectoryPart.poses;
         Trajectory_id[i] = "corner";
         TrajectoryPart.poses.clear();
      }
   }

   
/******** INCLUDE PATCH ***********/  

   std::vector<std::string> LabelVector_flag;
   SecondaryTrajectories.push_back(Trajectories_list[0]);
   LabelVector_flag.push_back(Trajectory_id[0]);

   for(size_t i=1; i<Trajectories_list.size(); i++)
   {
      if (ComputePatch(Trajectories_list[i-1].poses.back(), Trajectories_list[i].poses[0], Patch_To_Add) > 1 )
      {
         SecondaryTrajectories.push_back(Patch_To_Add);
         LabelVector_flag.push_back("pass");
      }    
      SecondaryTrajectories.push_back(Trajectories_list[i]);   
      LabelVector_flag.push_back(Trajectory_id[i]);     
      Patch_To_Add.poses.clear();
   }

   label_list.clear();
   for(int i=0; i< LabelVector_flag.size(); i++)
      label_list.push_back(LabelVector_flag[i]);
}

void TrajectoryPlanner::CheckCornerDistance(std::vector<std::string> Traj_id, std::vector<geometry_msgs::PoseArray> Traj_list, int i, double& distance1, double& distance2)
{
    int trajectory_size = Traj_list[i-1].poses.size();
    geometry_msgs::Pose P1 = Traj_list[i-1].poses[trajectory_size-1];
    geometry_msgs::Pose P2 = Traj_list[i].poses[0];
    geometry_msgs::Pose P3 = Traj_list[i+1].poses[0];

    double PP_distance = ComputeDistance(P1, P2);
    
    if( Traj_id[i-1] != "corner")
    {
        if(PP_distance < param.Ctrl_pt_d1)
            distance1 = PP_distance;
        else
            distance1 =  param.Ctrl_pt_d1;
    }
    else
        distance1 = PP_distance/2;
    PP_distance = ComputeDistance(P2, P3);
    if(InitLabel[i+1] != "corner")
    {
        if (PP_distance < param.Ctrl_pt_d2)
            distance2 = PP_distance;
        else 
            distance2 =  param.Ctrl_pt_d2;
    }
    else
        distance2 = PP_distance/2;
}



void TrajectoryPlanner::FinalTrajectoryRescaling(
                            moveit::planning_interface::MoveGroupInterface& move_group,
                            std::string robot_model_name,
                            float velocity_round, float velocity_pass, float velocity_fix, ros::Duration T_offset )
{

   PlannedSecondaryTrajectories.clear();
   PlannedLabelVector.clear();

   moveit_msgs::RobotTrajectory trajectory_cartesian;
   moveit_msgs::RobotTrajectory Traj_flag;
   std::vector<moveit_msgs::RobotTrajectory> TrajVector;


   robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
   robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
   robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
   robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup(robot_model_name);
   robot_state::RobotState start_state(*move_group.getCurrentState());

   trajectory_msgs::JointTrajectoryPoint Traj_point;
   Traj_point.positions.resize(7);
   Traj_point.accelerations.resize(7);
   Traj_point.velocities.resize(7);

   double joints_Cartesian[7];
   int size_plan;

   float mean_velocity;

   geometry_msgs::PoseArray PoseFlag;
   std::string last_label;
   last_label = " ";

   for(int k=0; k < SecondaryTrajectories[0].poses.size(); k++) 
      PoseFlag.poses.push_back(SecondaryTrajectories[0].poses[k]);

   last_label = label_list[0];

   for(int i=1; i < SecondaryTrajectories.size(); i++)
   {
      if(label_list[i] == last_label)
      {
         for(int k=0; k<SecondaryTrajectories[i].poses.size(); k++)
               PoseFlag.poses.push_back(SecondaryTrajectories[i].poses[k]);
      }
      else
      {
         // Nuovo nome da confrontare
         // Scegliere la velocità per il planner
         if(label_list[i-1] == "pass")
               mean_velocity = velocity_pass;
         if(label_list[i-1] == "round")
               mean_velocity = velocity_round;
         if(label_list[i-1] == "fix" || label_list[i-1] == "cornerfix" || label_list[i-1] == "pre-fix")
               mean_velocity = velocity_fix;
         else
               mean_velocity = velocity_fix;
         
         
         PlannedLabelVector.push_back(label_list[i-1]);
         move_group.computeCartesianPath(PoseFlag.poses, eef_step, jump_threshold, trajectory_cartesian);
         
         if (trajectory_cartesian.joint_trajectory.points.size() == 0)
         {
            // Do Nothing 
         }
         else
         {   
               Traj_flag = VelocityScaling(trajectory_cartesian, PoseFlag,  mean_velocity, T_offset);  
               
               if(label_list[i] == "pre-fix")
               {
                  Traj_point = Traj_flag.joint_trajectory.points[Traj_flag.joint_trajectory.points.size()-1];
                  
                  for(int i=0; i<7; i++)
                  {
                     Traj_point.accelerations[i] = 0;
                     Traj_point.velocities[i] = 0;
                  }
                  Traj_point.time_from_start += T_offset;
                  Traj_flag.joint_trajectory.points.push_back(Traj_point);
               }

               TrajVector.push_back(Traj_flag);             
               size_plan = trajectory_cartesian.joint_trajectory.points.size();
               for (int j=0; j<7; j++)
                  joints_Cartesian[j] = trajectory_cartesian.joint_trajectory.points[size_plan-1].positions[j];

               start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);       
               move_group.setStartState(start_state);
               PoseFlag.poses.clear();
         }

         for(int k=0; k<SecondaryTrajectories[i].poses.size(); k++)
               PoseFlag.poses.push_back(SecondaryTrajectories[i].poses[k]);
         last_label = label_list[i];
      }
   }

   PlannedLabelVector.push_back(label_list[label_list.size()-1]);
   move_group.computeCartesianPath(PoseFlag.poses, eef_step, jump_threshold, trajectory_cartesian);

   if (trajectory_cartesian.joint_trajectory.points.size() > 0)
      TrajVector.push_back(VelocityScaling(trajectory_cartesian, PoseFlag,  mean_velocity, T_offset));
   else
      ROS_INFO("SKIP");

   PlannedSecondaryTrajectories.resize(TrajVector.size());
   for(int i=0; i<TrajVector.size();i++)
      PlannedSecondaryTrajectories[i].trajectory_ = TrajVector[i];

}

moveit_msgs::RobotTrajectory VelocityScaling(moveit_msgs::RobotTrajectory trajectory_cartesian, geometry_msgs::PoseArray waypoint, float mean_velocity, ros::Duration T_offset)
{
   /*  Functon Input:
   
      trajectory_cartesina = Trajectory to rescale
      waypoint = punti nello spazion cartesiano, da cui calcoare la distanza per avere la T duration totale
      mean_velocity = velocità richiesta
   */


   float distance;
   
   moveit_msgs::RobotTrajectory new_trajectory;
   new_trajectory = trajectory_cartesian;
   distance = ComputePathLength(waypoint);
   ros::Duration overall_time = ros::Duration(distance/mean_velocity);

   float scaling_factor = overall_time.toSec()/(new_trajectory.joint_trajectory.points[(new_trajectory.joint_trajectory.points.size()-1)].time_from_start.toSec());
   new_trajectory = FactorScaling(new_trajectory, scaling_factor, T_offset);

   return new_trajectory;
}

moveit_msgs::RobotTrajectory FactorScaling(moveit_msgs::RobotTrajectory trajectory_cartesian, float scaling_factor, ros::Duration T_offset)
{
   moveit_msgs::RobotTrajectory new_trajectory;
   trajectory_msgs::JointTrajectoryPoint Traj_Point;

   for(size_t i=0; i<trajectory_cartesian.joint_trajectory.points.size(); i++)
   {
      new_trajectory.joint_trajectory.points.push_back(trajectory_cartesian.joint_trajectory.points[i]);
   }
   new_trajectory.joint_trajectory.joint_names.resize(7);
   new_trajectory.joint_trajectory.joint_names = trajectory_cartesian.joint_trajectory.joint_names;

   for (size_t i=0; i<new_trajectory.joint_trajectory.points.size(); i++)
   {
      for(int k=0; k<7; k++)
      {
         new_trajectory.joint_trajectory.points[i].velocities[k] /= scaling_factor;
         new_trajectory.joint_trajectory.points[i].accelerations[k] /= (scaling_factor*scaling_factor);
      }
      new_trajectory.joint_trajectory.points[i].time_from_start *= scaling_factor;
   }


   Traj_Point.time_from_start = ros::Duration(0);
   Traj_Point.positions.resize(7);
   Traj_Point.velocities.resize(7);
   Traj_Point.accelerations.resize(7);

   for(int i=0; i<7; i++)
   {
      Traj_Point.positions[i] = new_trajectory.joint_trajectory.points[0].positions[i];
      Traj_Point.velocities[i] = 0;
      Traj_Point.accelerations[i] = 0;
   }

   for (size_t i=0; i<new_trajectory.joint_trajectory.points.size(); i++)
   {
      new_trajectory.joint_trajectory.points[i].time_from_start += T_offset;
   }

   for(size_t i=1; i<new_trajectory.joint_trajectory.points.size(); i++)
   {
      if(new_trajectory.joint_trajectory.points[i].time_from_start <= new_trajectory.joint_trajectory.points[i-1].time_from_start)
      {
         new_trajectory.joint_trajectory.points[i].time_from_start = new_trajectory.joint_trajectory.points[i-1].time_from_start + ros::Duration(0.00001);
      }
   }

   new_trajectory.joint_trajectory.points.insert(new_trajectory.joint_trajectory.points.begin(), Traj_Point);
   new_trajectory.joint_trajectory.points[new_trajectory.joint_trajectory.points.size()-1].time_from_start += T_offset;

   return new_trajectory;
}

float ComputeDistance(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2)
{
   float distance;

   distance = sqrt( 
                  pow(Point2.position.x - Point1.position.x, 2) + 
                  pow(Point2.position.y - Point1.position.y, 2) +
                  pow(Point2.position.z - Point1.position.z, 2)) ;

   return distance;
}
float ComputePathLength(geometry_msgs::PoseArray waypoint)
{
   float distance;
   distance = 0.0;

   for(int i=1; i<waypoint.poses.size(); i++)
      distance += ComputeDistance(waypoint.poses[i-1], waypoint.poses[i]);
   
   return distance;
}

void sphereProjection(geometry_msgs::Pose center, float radius, geometry_msgs::PoseArray waypoint)
{

   for(int i=0; i<waypoint.poses.size(); ++i)
   {
      waypoint.poses[i].position.x = center.position.x + radius*(waypoint.poses[i].position.x - center.position.x)/abs((waypoint.poses[i].position.x - center.position.x));
      waypoint.poses[i].position.y = center.position.y + radius*(waypoint.poses[i].position.y - center.position.y)/abs((waypoint.poses[i].position.y - center.position.y));
      waypoint.poses[i].position.z = center.position.z + radius*(waypoint.poses[i].position.z - center.position.z)/abs((waypoint.poses[i].position.z - center.position.z));
   }
   return;
}