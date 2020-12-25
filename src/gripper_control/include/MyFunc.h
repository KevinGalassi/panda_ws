#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Accel.h"

#include <moveit/move_group_interface/move_group_interface.h>
// #include <moveit_tutorials/planning_interface/move_group_interface/Plan.h>
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


#include "GP_Functions.h"

namespace rvt = rviz_visual_tools;

// Robot known positions
const std::vector<double> arm_ready_state = {0, -0.785, 0, -2.356, 0, 1.571, 0.785};
const std::vector<double> hand_ready_state = {0.04, 0.04};
const std::vector<double> hand_open_position = {0.04, 0.04};
const std::vector<double> hand_closed_position = {0.0, 0.0};

// Denavit-Hartenberg Parameters of Panda
const float DH_param_a[8] = {0, 0, 0, 0.0825, -0.0825, 0, 0.088, 0 };
const float DH_param_d[8] = {0.333, 0, 0.316, 0, 0.384, 0,  0, 0.107};
const float DH_param_alpha[8] = {0, -M_PI/2, M_PI/2, M_PI/2, -M_PI/2, M_PI/2, M_PI/2, 0};

// Panda limits

const double qd_max[7]	= {2.1750,	2.1750,	2.1750,	2.1750,	2.6100,	2.6100,	2.6100};
const double qdd_max[7]	= {15,	7.5,	10,	12.5,	15,	20,	20};
const double qddd_max[7] = {7500,	3750,	5000,	6250,	7500,	10000,	10000};

const double gripper_offset[3] = {0, 0, 0.1};

struct TrajectoryVector 
{
    geometry_msgs::PoseArray point;
    std::vector<std::string>  pt_label;
    std::vector<geometry_msgs::PoseArray> SecondaryTrajectory;
    std::vector<std::string> info;
};

struct RescaleVelocityTrajectory
{
    geometry_msgs::PoseArray ws_Pose;
    std::vector<geometry_msgs::Twist> ws_Twist;
    std::vector<geometry_msgs::Accel> ws_Accel;
    std::vector<trajectory_msgs::JointTrajectoryPoint> jointspace_points;
};

struct Jacobian_Computation
{
    double A[4][4];
    double T[4][4];
};

struct Jacobian_Computation_eigen
{
    Eigen::Matrix<double,4,4> A;
    Eigen::Matrix<double,4,4> T;
};

struct TrajectoryPlanner_param
{
    double radius;
    double heigh;
    int circ_point;
    double res;
    double distance_approach;
    double Ctrl_pt_d1;
    double Ctrl_pt_d2;
    int corner_points; 
};

void MyTrajectoryPlanner(TrajectoryPlanner_param param, TrajectoryVector& waypoints);

void FromEE2Link8(geometry_msgs::PoseArray& waypoints);

void FromLink82EE(geometry_msgs::PoseArray& waypoints);

void addFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints, double radius, double heigh, int res);

void CornerDetection(geometry_msgs::PoseArray waypoints, std::vector<std::string>& id_list);
void CornerDetection_test(std::vector<geometry_msgs::PoseArray> waypoints, std::vector<std::string>& id_list);

void CornerRounding(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::Pose Point3, double distance1, double distance2, geometry_msgs::PoseArray& waypoints, double res);


void GraspWire(moveit::planning_interface::MoveGroupInterface& move_group, moveit::planning_interface::MoveGroupInterface& hand_group,
            geometry_msgs::Pose grasp_pose, double d, double res, geometry_msgs::PoseArray& waypoints);

int ComputePatch(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::PoseArray& waypoints, double res);
// void Patching(WiresWaypoints waypoints, WiresWaypoints& waypoints_final, double res);

// void ComputeTmatrix_eigen (Eigen::Matrix<double, 4,4>& A, double DH_param_a, double DH_param_alpha, double DH_param_d, double Joint_value);
void ComputeTmatrix_float (Eigen::Matrix<float, 4,4>& A, float DH_param_a, float DH_param_alpha, float DH_param_d, float Joint_value);

// void getJacobian(Eigen::Matrix<double,6,7 >& Jacobian, std::vector<double> Joint_Pos);
void getJacobianInverse(Eigen::Matrix<double,6,7>  Jacobian, Eigen::Matrix<double, 7, 6>& Jacobian_inverse);
void getJacobianDerivate (Eigen::Matrix<double,6,7> Jacobian, Eigen::Matrix<double,6,7>& Jacobian_der, std::vector<double> vel_in);

void computeEE_Vel_Acc(geometry_msgs::PoseArray waypoints, std::vector<geometry_msgs::Twist>& EE_velocity, std::vector<geometry_msgs::Accel>& EE_accel,  float velocity, double res);

void getJointSpaceVelocity(std::vector<trajectory_msgs::JointTrajectoryPoint>& joints_configuration, std::vector<geometry_msgs::Twist> EE_velocity);
void setJointSpaceVel_Acc(std::vector<trajectory_msgs::JointTrajectoryPoint>& joints_configuration, std::vector<geometry_msgs::Twist> EE_velocity, std::vector<geometry_msgs::Accel> EE_acceleration);



void PlanCartesianMotion(robot_state::JointModelGroup* joint_model_group, geometry_msgs::PoseArray waypoints,  
        moveit::planning_interface::MoveGroupInterface::Plan& Plan_Cartesian, 
        moveit::planning_interface::MoveGroupInterface& move_group,
        RescaleVelocityTrajectory& coordinates_to_elaborate, geometry_msgs::PoseArray& failed_points,
        std::vector<geometry_msgs::PoseArray>& Plan_vector );

void PlanRescaledCartesianMotion(robot_state::JointModelGroup* joint_model_group, 
                                geometry_msgs::PoseArray waypoints,  
                                moveit::planning_interface::MoveGroupInterface& move_group,
                                RescaleVelocityTrajectory& coordinates_to_elaborate, 
                                geometry_msgs::PoseArray& failed_points,
                                float velocity,
                                moveit_msgs::RobotTrajectory& RobotTrajectory,
                                TrajectoryPlanner_param param);


void PlanSetPoseMotion(robot_state::JointModelGroup* joint_model_group, geometry_msgs::PoseArray waypoints,  
        moveit::planning_interface::MoveGroupInterface::Plan& Plan_SP, moveit::planning_interface::MoveGroupInterface& move_group,
        RescaleVelocityTrajectory& coordinates_to_elaborate, std::vector<geometry_msgs::Quaternion> constraints_vector);


void PlanSetPoseMotion2(robot_state::JointModelGroup* joint_model_group, geometry_msgs::PoseArray waypoints,  
            moveit::planning_interface::MoveGroupInterface::Plan& Plan_SP, 
            moveit::planning_interface::MoveGroupInterface& move_group, RescaleVelocityTrajectory& coordinates_to_elaborate, std::vector<geometry_msgs::Quaternion> constraints_vector);



void PlanIKMotion(robot_state::JointModelGroup* joint_model_group, robot_state::RobotStatePtr kinematic_state, 
        geometry_msgs::PoseArray waypoints,  moveit::planning_interface::MoveGroupInterface::Plan& Plan_IK, 
        moveit::planning_interface::MoveGroupInterface& move_group, RescaleVelocityTrajectory& coordinates_to_elaborate, std::vector<geometry_msgs::Quaternion> constraints_vector);


void computeEE_Vel_Acc2(geometry_msgs::PoseArray waypoints,float velocity, double res, std::vector<std::vector<trajectory_msgs::JointTrajectoryPoint>>& Plan_vector,
                        std::vector<std::vector<geometry_msgs::Twist>>& EE_vel_vector, std::vector <std::vector<geometry_msgs::Accel>>& EE_acc_vector );


bool CheckEEPosition(geometry_msgs::Pose TargetPose, float Joint_Pos[7], double res);



moveit_msgs::RobotTrajectory FactorScaling(moveit_msgs::RobotTrajectory trajectory_cartesian, float scaling_factor, ros::Duration T_offset);


bool ReadFileTxt(bool input_rpy, geometry_msgs::PoseArray& point, std::vector<std::string>& label, std::string file_path, geometry_msgs::Pose& grasp_pose)
{
    std::ifstream pose_list;
    geometry_msgs::Pose target_goal;
    std::string pose_identifie;

    pose_list.open(file_path);
    if( pose_list.is_open())
    {
        ROS_INFO("File succesfully open");
    }
    else
    {
        ROS_INFO("file not open");
        return false;
    }

    if(input_rpy)
    {
        float input_RPY[3] = {0, 0, 0};
        tf2::Quaternion input_quaternion;
        while ( !pose_list.eof() )
        { 
            std::cout << "data readen \n";
            pose_list >> target_goal.position.x;
            pose_list >> target_goal.position.y;
            pose_list >> target_goal.position.z;
            pose_list >> input_RPY[0];
            pose_list >> input_RPY[1];
            pose_list >> input_RPY[2];
            input_quaternion.setRPY(input_RPY[0], input_RPY[1], input_RPY[2]);
            target_goal.orientation = tf2::toMsg(input_quaternion);
            pose_list >> pose_identifie;

            if (pose_identifie == "grasp")
            {
                grasp_pose = target_goal;
            }
            else
            {
                point.poses.push_back(target_goal);
                label.push_back(pose_identifie);
            }

        }
    }
    else
    {
    while ( !pose_list.eof() )
        { 
            std::cout << "data readen \n";
            pose_list >> target_goal.position.x;
            pose_list >> target_goal.position.y;
            pose_list >> target_goal.position.z;
            pose_list >> target_goal.orientation.w;
            pose_list >> target_goal.orientation.x;
            pose_list >> target_goal.orientation.y;
            pose_list >> target_goal.orientation.z;
            pose_list >> pose_identifie;

            if (pose_identifie == "grasp")
            {
                grasp_pose = target_goal;
            }
            else
            {
                point.poses.push_back(target_goal);
                label.push_back(pose_identifie);
            }
        }
    }
    pose_list.close();
    return true;
}

bool ReadFileTxt_M(bool input_rpy, std::vector<geometry_msgs::PoseArray>& point, std::vector<std::vector<std::string>>& label, std::string file_path, std::vector<geometry_msgs::Pose>& grasp_pose)
{
    std::ifstream pose_list;
    geometry_msgs::Pose target_goal;
    std::string pose_identifie;

    geometry_msgs::PoseArray wire_point;
    std::vector<std::string> label_list;
    int i = 0;

    pose_list.open(file_path);
    if( pose_list.is_open())
    {
        ROS_INFO("File succesfully open");
    }
    else
    {
        ROS_INFO("file not open");
        return false;
    }

    if(input_rpy)
    {
        float input_RPY[3] = {0, 0, 0};
        tf2::Quaternion input_quaternion;

        while ( !pose_list.eof() )
        { 
            std::cout << "data readen \n";
            pose_list >> target_goal.position.x;
            pose_list >> target_goal.position.y;
            pose_list >> target_goal.position.z;
            pose_list >> input_RPY[0];
            pose_list >> input_RPY[1];
            pose_list >> input_RPY[2];
            input_quaternion.setRPY(input_RPY[0], input_RPY[1], input_RPY[2]);
            target_goal.orientation = tf2::toMsg(input_quaternion);
            pose_list >> pose_identifie;

            if (pose_identifie == "grasp")
            {   
                std::cout << "wire acquired \n";
                point.push_back(wire_point);
                label.push_back(label_list);
                label_list.clear();
                wire_point.poses.clear();
                grasp_pose.push_back(target_goal);
                i++;
            }
            else
            {
                std::cout << "data saved \n";
                wire_point.poses.push_back(target_goal);
                label_list.push_back(pose_identifie);
            }
        }

        std::cout << "End of file \n";
    }
    else
    {
    while ( !pose_list.eof() )
        { 
            std::cout << "data readen \n";
            pose_list >> target_goal.position.x;
            pose_list >> target_goal.position.y;
            pose_list >> target_goal.position.z;
            pose_list >> target_goal.orientation.w;
            pose_list >> target_goal.orientation.x;
            pose_list >> target_goal.orientation.y;
            pose_list >> target_goal.orientation.z;
            pose_list >> pose_identifie;

            if (pose_identifie == "grasp")
            {
                point.push_back(wire_point);
                label.push_back(label_list);
                label_list.clear();
                wire_point.poses.clear();
                grasp_pose.push_back(target_goal);
                i++;
            }
            else
            {
                wire_point.poses.push_back(target_goal);
                label_list.push_back(pose_identifie);
            }
        }
    }
    pose_list.close();
    return true;
}

void MyTrajectoryPlanner(TrajectoryPlanner_param param, TrajectoryVector& waypoints)
{    
    std::vector<geometry_msgs::PoseArray> Trajectories_list, Trajectory_list_with_patch;        // Store the trajectory of each point of the file

    geometry_msgs::PoseArray Trajectory_part;    
    std::vector<std::string> Trajectory_id;            
    geometry_msgs::PoseArray Patch_To_Add;

    float PP_distance;          
    double new_Ctrl_Pt_d1;
    double new_Ctrl_Pt_d2;

    geometry_msgs::Pose P1,P2,P3;
    int trajectory_size;
    
    for (std::size_t i = 0; i < waypoints.point.poses.size(); i ++)
    {  
        if (waypoints.pt_label[i] == "fix")
        {
            addFixPoint(waypoints.point.poses[i], Trajectory_part, param.radius, param.heigh, param.circ_point);
            Trajectories_list.push_back(Trajectory_part);
            Trajectory_part.poses.clear();
            Trajectory_id.push_back("fix");
        }

        if (waypoints.pt_label[i] == "pass")
        {
            Trajectory_part.poses.push_back(waypoints.point.poses[i]);
            Trajectories_list.push_back(Trajectory_part);
            Trajectory_part.poses.clear();   
            Trajectory_id.push_back("pass");
        }
    } 


    CornerDetection_test(Trajectories_list, Trajectory_id);

    for (size_t i = 1; i < (Trajectories_list.size()-1); i++)
    {
        if(Trajectory_id[i] == "corner")
        {
            trajectory_size = Trajectories_list[i-1].poses.size();
            P1 = Trajectories_list[i-1].poses[trajectory_size-1];
            P2 = Trajectories_list[i].poses[0];
            P3 = Trajectories_list[i+1].poses[0];

            PP_distance = ComputeDistance(P1, P2);
            if( Trajectory_id[i-1] != "corner")
            {
                if(PP_distance < param.Ctrl_pt_d1)
                    new_Ctrl_Pt_d1 = PP_distance;
                else
                    new_Ctrl_Pt_d1 =  param.Ctrl_pt_d1;
            }
            else
            {
                new_Ctrl_Pt_d1 = PP_distance/2;
            }

            PP_distance = ComputeDistance(P2, P3);
            if(waypoints.pt_label[i+1] != "corner")
            {
                if (PP_distance < param.Ctrl_pt_d2)
                    new_Ctrl_Pt_d2 = PP_distance;
                else 
                    new_Ctrl_Pt_d2 =  param.Ctrl_pt_d2;
            }
            else
            {
                new_Ctrl_Pt_d2 = PP_distance/2;
            }
            CornerRounding(P1, P2, P3, new_Ctrl_Pt_d1, new_Ctrl_Pt_d2, Trajectory_part, param.corner_points);
            Trajectories_list[i].poses.clear();
            Trajectories_list[i].poses = Trajectory_part.poses;
            Trajectory_id[i] = "corner";
            Trajectory_part.poses.clear();
        }
    }

/**************** INCLUDE PATCH ******************/  
    waypoints.SecondaryTrajectory.push_back(Trajectories_list[0]);
    for(size_t i=1; i<Trajectories_list.size(); i++)
    {
        ComputePatch(Trajectories_list[i-1].poses.back(), Trajectories_list[i].poses[0], Patch_To_Add, param.res);
        waypoints.SecondaryTrajectory.push_back(Patch_To_Add);
        waypoints.SecondaryTrajectory.push_back(Trajectories_list[i]);        
        Patch_To_Add.poses.clear();
    }
}

void FromEE2Link8(geometry_msgs::PoseArray& waypoints)
{
    /*****
     * Convert the position from EE to Link8
     * 
     *  The offset is specified by gripper_offset (defined outside the function at the beginning)
     *  And a rotation matrix of M_PI / 4 
     * 
     * The Pose is updated directly in the same vector passed
     * 
     */

    geometry_msgs::Pose target_pose;

    tf2::Matrix3x3 R, R_shift, R_final;

    float R_2[3][3];
    getRZ(R_2, M_PI/4);

    float R_3[3][3];


    tf2::Quaternion quat, quat_shift, quat_new;
    tf2::Vector3 axis;

    double roll, pitch, yaw;

    geometry_msgs::Transform T_shift;


    //quat.setEulerZYX(M_PI/4, 0, 0);

    quat.setRPY(0,0,M_PI/4);
    R_shift.setRotation(quat);

    for (size_t i=0; i < waypoints.poses.size(); i++)
    {
        quat.setX(waypoints.poses[i].orientation.x);
        quat.setY(waypoints.poses[i].orientation.y);
        quat.setZ(waypoints.poses[i].orientation.z);
        quat.setW(waypoints.poses[i].orientation.w);
        R.setRotation(quat);

        target_pose.position.x = waypoints.poses[i].position.x -( R[0][0]*gripper_offset[0] + R[0][1]*gripper_offset[1] + R[0][2]*gripper_offset[2]) ;
        target_pose.position.y = waypoints.poses[i].position.y -( R[1][0]*gripper_offset[0] + R[1][1]*gripper_offset[1] + R[1][2]*gripper_offset[2]) ;
        target_pose.position.z = waypoints.poses[i].position.z -( R[2][0]*gripper_offset[0] + R[2][1]*gripper_offset[1] + R[2][2]*gripper_offset[2]) ;

        quat_new = quat*quat_shift;
        quat_new.normalize();

        R_final = R*R_shift;

      //  R_final.getEulerYPR(roll, pitch, yaw);
      //  quat_new.setRPY(roll, pitch, yaw);
      //  quat.setRotation(axis, M_PI/2);
      /*
        axis.setW(quat.getW());
        axis.setX(quat.getZ());
        axis.setY(quat.getX());
        axis.setZ(quat.getY());


        for(int j=0; j<3;j++)
        {
            for (int k=0;k<3;k++)
            {
                R_3[j][k] = R[j][k];
            }
        }

        
        MatrixMul(R_3, R_2, R_3);

        for(int j=0; j<3;j++)
        {
            for (int k=0;k<3;k++)
            {
                R[j][k] = R_3[j][k];
            }
        }


        R.getRotation(quat);
        */
        R_final.getRotation(quat);
        quat.normalize();
        target_pose.orientation = tf2::toMsg(quat);

        waypoints.poses[i].position    = target_pose.position;
        waypoints.poses[i].orientation = target_pose.orientation;    

    }
    return ;
}

void FromLink82EE(geometry_msgs::PoseArray& waypoints)
{

    // DA SISTEMARE
    geometry_msgs::Pose target_pose;

    tf2::Matrix3x3 R, R_shift, R_final;
    tf2::Quaternion quat, quat_shift, quat_new;
    tf2::Vector3 axis;

    double roll, pitch, yaw;
    tf2::Transform T, T_origin, T_final;

    geometry_msgs::Transform T_shift;
/*
    T_shift.rotation = tf2::toMsg(quat.setRPY(0,0,M_PI/4));
    T_shift.translation.x = 0;
    T_shift.translation.y = 0;
    T_shift.translation.z = -0.1;

    quat_shift.setRPY(0,0,M_PI/4);
    T.setRotation(quat_shift);
*/
    quat.setRPY(0,0,M_PI/4);
    R_shift.setRotation(quat);

    for (size_t i=0; i < waypoints.poses.size(); i++)
    {

        quat.setX(waypoints.poses[i].orientation.x);
        quat.setY(waypoints.poses[i].orientation.y);
        quat.setZ(waypoints.poses[i].orientation.z);
        quat.setW(waypoints.poses[i].orientation.w);
        R.setRotation(quat);
     //   T_origin.setRotation(quat);

        target_pose.position.x = waypoints.poses[i].position.x +( R[0][0]*gripper_offset[0] + R[0][1]*gripper_offset[1] + R[0][2]*gripper_offset[2]) ;
        target_pose.position.y = waypoints.poses[i].position.y +( R[1][0]*gripper_offset[0] + R[1][1]*gripper_offset[1] + R[1][2]*gripper_offset[2]) ;
        target_pose.position.z = waypoints.poses[i].position.z +( R[2][0]*gripper_offset[0] + R[2][1]*gripper_offset[1] + R[2][2]*gripper_offset[2]) ;

    //    quat_new = quat*quat_shift;
        quat_new.normalize();

    //    T_final = T_origin*T;
    //    R_final = R_shift*R;
    //    R_final.getEulerZYX(roll, pitch, yaw);
    //    quat_new.setRPY(roll, pitch, yaw);
        
        axis.setW(quat.getW());
        axis.setX(quat.getX());
        axis.setY(quat.getY());
        axis.setZ(quat.getZ());



        quat.setRotation(axis, M_PI);
        target_pose.orientation = tf2::toMsg(quat);
        std::cout << quat << "\n";

        waypoints.poses[i].position    = target_pose.position;
        waypoints.poses[i].orientation = target_pose.orientation;      
    }
    return ;    

}

void RecomputeRotation(geometry_msgs::PoseArray& waypoints)
{


    tf2::Quaternion quat, quat_shift;
    tf2::Matrix3x3 R, R_shift, R_final;


    quat_shift.setRPY(M_PI,0,0);
    R_shift.setRotation(quat_shift);


    Eigen::Vector3f vect, A,B;
    Eigen::Matrix3f ssc;
    Eigen::Matrix3f eye;

    A << 0.01, 0, 0;


    Eigen::Matrix3f R_eigen;

    float m[3][3];
    float v1[3], v2[3];

    v1[0] = 0.01;
    v1[1] = 0;
    v1[2] = 0;

    eye <<  1,0,0,
            0,1,0,
            0,0,1;

    float sum;

    for(size_t i = 1; i< waypoints.poses.size(); i++)
    {

        /*
        sum = sqrt((waypoints.poses[i].position.x - waypoints.poses[i-1].position.x)*(waypoints.poses[i].position.x - waypoints.poses[i-1].position.x) +
               (waypoints.poses[i].position.y - waypoints.poses[i-1].position.y)*(waypoints.poses[i].position.y - waypoints.poses[i-1].position.y) +
               (waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)*(waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)); 

        v2[0] = (waypoints.poses[i].position.x - waypoints.poses[i-1].position.x)*(waypoints.poses[i].position.x - waypoints.poses[i-1].position.x)/sum;
        v2[1] = (waypoints.poses[i].position.y - waypoints.poses[i-1].position.y)*(waypoints.poses[i].position.y - waypoints.poses[i-1].position.y)/sum;
        v2[2] = (waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)*(waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)/sum;

        
        v2[0] = (waypoints.poses[i].position.x - waypoints.poses[i-1].position.x);
        v2[1] = (waypoints.poses[i].position.y - waypoints.poses[i-1].position.y);
        v2[2] = (waypoints.poses[i].position.z - waypoints.poses[i-1].position.z);
        
        */
        /*
        rotation_between_vecs_to_mat3(m, v1,  v2);

        for (int k=0; k<2; k++)
        {
            for(int j=0; j<2; j++)
            {
                R[k][j] = m[k][j];
            }
        }
        */
               sum = sqrt((waypoints.poses[i].position.x - waypoints.poses[i-1].position.x)*(waypoints.poses[i].position.x - waypoints.poses[i-1].position.x) +
               (waypoints.poses[i].position.y - waypoints.poses[i-1].position.y)*(waypoints.poses[i].position.y - waypoints.poses[i-1].position.y) +
               (waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)*(waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)); 

        B(0) = (waypoints.poses[i].position.x - waypoints.poses[i-1].position.x)*(waypoints.poses[i].position.x - waypoints.poses[i-1].position.x)/sum;
        B(1) = (waypoints.poses[i].position.y - waypoints.poses[i-1].position.y)*(waypoints.poses[i].position.y - waypoints.poses[i-1].position.y)/sum;
        B(2) = (waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)*(waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)/sum;
/*
        B(0) = (waypoints.poses[i].position.x - waypoints.poses[i-1].position.x);
        B(1) = (waypoints.poses[i].position.y - waypoints.poses[i-1].position.y);
        B(2) = (waypoints.poses[i].position.z - waypoints.poses[i-1].position.z);
*/
        /*
        B << waypoints.poses[i].position.x - waypoints.poses[i-1].position.x, 
            (waypoints.poses[i].position.y - waypoints.poses[i-1].position.y), 
            (waypoints.poses[i].position.z - waypoints.poses[i-1].position.z);
        */

        vect = B.cross(A);

        ssc(0,0) = 0;
        ssc(0,1) = -vect(2);
        ssc(0,2) = vect(1);
        
        ssc(1,0) = vect(2);
        ssc(1,1) = 0;
        ssc(1,2) = - vect(0);
        
        ssc(2,0) = -vect(1);
        ssc(2,1) = vect(0);
        ssc(2,2) = 0;
     

        R_eigen = eye + ssc + ssc*ssc*(1-B.dot(A))/(sqrt(vect(0)*vect(0) + vect(1)*vect(1) + vect(2)*vect(2)) * sqrt(vect(0)*vect(0) + vect(1)*vect(1) + vect(2)*vect(2)) );

        for (int k=0; k<2; k++)
        {
            for(int j=0; j<2; j++)
            {
                R[k][j] = R_eigen(k,j);
            }
        }


        // R = R*R_shift;
        R.getRotation(quat);
        quat.normalize();

        waypoints.poses[i].position = waypoints.poses[i].position;

        //        std::cout<<waypoints.poses[i].orientation<< "\n";
        waypoints.poses[i].orientation = tf2::toMsg(quat);

      //  std::cout<<waypoints.poses[i].orientation<< "\n";
    }

    return;
}

void addFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints, double radius, double heigh, int res)
{
    /**
     * Create a trajectory to fix a point
     * 
     * The trajectory is created starting from the pose "fixing_point" specified and added in "waypoints" through .push_back
     * The movements performed are:
     *      lateral right shift of "2*radius" distance
     *      raise of "heigh"
     *      Circular movement to position over the fix point
     *      lower
     */

    geometry_msgs::Pose target_pose;
    geometry_msgs::Point center;

    tf2::Matrix3x3 R;
    tf2::Quaternion quat;

    quat.setX(fixing_point.orientation.x);
    quat.setY(fixing_point.orientation.y);
    quat.setZ(fixing_point.orientation.z);
    quat.setW(fixing_point.orientation.w);
    R.setRotation(quat);

    double dist_init[3] = {+0.01, 0, 0};

    target_pose.position.x = fixing_point.position.x - (R[0][0]*dist_init[0] + R[0][1]*dist_init[1] + R[0][1]*dist_init[2]);
    target_pose.position.y = fixing_point.position.y - (R[1][0]*dist_init[0] + R[0][1]*dist_init[1] + R[0][1]*dist_init[2]);
    target_pose.position.z = fixing_point.position.z - (R[2][0]*dist_init[0] + R[0][1]*dist_init[1] + R[0][1]*dist_init[2]);
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);
    
    target_pose.position.x = fixing_point.position.x + R[0][1]*radius*2;
    target_pose.position.y = fixing_point.position.y + R[1][1]*radius*2;
    target_pose.position.z = fixing_point.position.z + R[2][1]*radius*2;
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);


    // lateral shift of the diameter of the circle
    target_pose.position.x = fixing_point.position.x + R[0][1]*radius*2;
    target_pose.position.y = fixing_point.position.y + R[1][1]*radius*2;
    target_pose.position.z = fixing_point.position.z + R[2][1]*radius*2;
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);

    // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
    center.x = fixing_point.position.x + R[0][1]*radius - R[0][2]*heigh;
    center.y = fixing_point.position.y + R[1][1]*radius - R[1][2]*heigh;
    center.z = fixing_point.position.z + R[2][1]*radius - R[2][2]*heigh;

    /*
    center.x = fixing_point.position.x + R[0][0]*radius[0] + R[0][1]*radius[1] + R[0][1]*radius[2] - (R[0][0]*heigh[0] + R[0][1]*heigh[1] + R[0][1]*heigh[2]);
    center.y = fixing_point.position.y + R[1][0]*radius[0] + R[0][1]*radius[1] + R[0][1]*radius[2] - (R[0][0]*heigh[0] + R[0][1]*heigh[1] + R[0][1]*heigh[2]);
    center.z = fixing_point.position.z + R[2][0]*radius[0] + R[0][1]*radius[1] + R[0][1]*radius[2] - (R[0][0]*heigh[0] + R[0][1]*heigh[1] + R[0][1]*heigh[2]);
    */

    // Upshift
    target_pose.position.x = target_pose.position.x - R[0][2]*heigh;
    target_pose.position.y = target_pose.position.y - R[1][2]*heigh;
    target_pose.position.z = target_pose.position.z - R[2][2]*heigh;
    waypoints.poses.push_back(target_pose);

    for(int k = 0; k < res; k++)
    {
        target_pose.position.x = center.x + R[0][1]*radius*cos(M_PI/res*k) - R[0][2]*radius*sin(M_PI/res*k);
        target_pose.position.y = center.y + R[1][1]*radius*cos(M_PI/res*k) - R[1][2]*radius*sin(M_PI/res*k);
        target_pose.position.z = center.z + R[2][1]*radius*cos(M_PI/res*k) - R[2][2]*radius*sin(M_PI/res*k);
        waypoints.poses.push_back(target_pose);
    }

    target_pose.position.x = fixing_point.position.x - R[0][2]*heigh;
    target_pose.position.y = fixing_point.position.y - R[1][2]*heigh;
    target_pose.position.z = fixing_point.position.z - R[2][2]*heigh;
    waypoints.poses.push_back(target_pose);

    // Downshift
    target_pose.position.x = fixing_point.position.x;
    target_pose.position.y = fixing_point.position.y;
    target_pose.position.z = fixing_point.position.z;
    waypoints.poses.push_back(target_pose);

    target_pose.position.x = fixing_point.position.x + (R[0][0]*dist_init[0] + R[0][1]*dist_init[1] + R[0][1]*dist_init[2]);
    target_pose.position.y = fixing_point.position.y + (R[1][0]*dist_init[0] + R[0][1]*dist_init[1] + R[0][1]*dist_init[2]);
    target_pose.position.z = fixing_point.position.z + (R[2][0]*dist_init[0] + R[0][1]*dist_init[1] + R[0][1]*dist_init[2]);
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);

    return ;
}

void CornerDetection(geometry_msgs::PoseArray waypoints, std::vector<std::string>& id_list)
{
    /*
    *  Detech a corner checking the condition over the determinant diven three points
    *  Only a point with the label "Pass" can become a round corner
    * 
    * */
    float det1, det2;
    for(int i = 1; i < (waypoints.poses.size()-1); i++)
    {
        // Condizione sul determinante
        det1 =  (waypoints.poses[i].position.x-waypoints.poses[i-1].position.x)*(waypoints.poses[i+1].position.y-waypoints.poses[i-1].position.y) -
                (waypoints.poses[i+1].position.x-waypoints.poses[i-1].position.x)*(waypoints.poses[i].position.y-waypoints.poses[i-1].position.y);
        
        det2 =  (waypoints.poses[i].position.x-waypoints.poses[i-1].position.x)*(waypoints.poses[i+1].position.z-waypoints.poses[i-1].position.z) -
                (waypoints.poses[i+1].position.x-waypoints.poses[i-1].position.x)*(waypoints.poses[i].position.z-waypoints.poses[i-1].position.z);

        // abs(det1) return always 0 by an error becouse the function enter in conflict 
        // since is available in multiple library called and use the ones that returns 1 in case of double value

        if (det1<0) det1 = -det1;
        if (det2<0) det2 = -det2;
        // When both are = 0 the points are alligned, the condition is denied and then add 0.01 as safety in case of numerical error rounding of the number
        
        if(det1 != 0.01 || det2 != 0.01)
        {
            if(id_list[i] == "pass")
            {
                id_list[i] = "rounding";
            }
        }
    }
}


void CornerDetection_test(std::vector<geometry_msgs::PoseArray> waypoints, std::vector<std::string>& id_list)
{
    float det1, det2;
    geometry_msgs::Pose P1, P3;
    int Traj1_size, Traj3_size;

    for(int i = 1; i < (waypoints.size()-1); i++)
    {
        if(id_list[i] == "pass" && id_list[i-1] != "grasp")
        {
            Traj1_size = waypoints[i-1].poses.size();
            Traj3_size = waypoints[i+1].poses.size();

            // L'operazione di controllo viene effettuata tra l'ultimo punto della traiettoria precedente e il primo punto della traiettoria successiva.
            P1 = waypoints[i-1].poses[Traj1_size - 1];
            P3 = waypoints[i+1].poses[0];

            // Condizione sul determinante
            det1 =  (waypoints[i].poses[0].position.x - P1.position.x)*(P3.position.y - P1.position.y) -
                    (P3.position.x - P1.position.x)*(waypoints[i].poses[0].position.y-P1.position.y);
            
            det2 =  (waypoints[i].poses[0].position.x - P1.position.x)*(P3.position.z - P1.position.z) -
                    (P3.position.x - P1.position.x)*(waypoints[i].poses[0].position.z - waypoints[i-1].poses[0].position.z);

            // abs(det1) restituiva sempre 0 per errore, l'ipotesi è che non venisse usata la funzione di cmath, 
            // ma di un altra libreria che funzionava solo con valori interger, per cui quando usata su double restituiva 0
            if (det1<0) det1 = -det1;
            if (det2<0) det2 = -det2;
            // Quando entrambi sono = 0 allora i punti sono allineati, viene usato 0.01 per sicurezza nel caso di errori di approsimazione con double
            
            if(det1 !=0 || det2 !=0)
            //if(det1 > 0.01 || det2 > 0.01)
            {
                id_list[i] = "corner";
            }
        }
    }

    return;
}

void CornerRounding(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::Pose Point3, double distance1, double distance2, geometry_msgs::PoseArray& waypoints, double res)
{
    /*
    *   The function read three points, in which "Point2" is the one in the middle
    * 
    *   It is applied a " Quadratic Bézier" curves aproach using as starting point the Point2 shifted by "distance1" in the direction of "Point1"
    *   The terminal point will be Point2 shifted by "distance2" in the direction of Point3
    * 
    *   The point are saved via .push_back in "waypoints"
    * 
    *   To compute the intermediate point is made an interpolation of the xyz coordinates (see Bèzier curves)
    *   The orientation updated through the slerp algorithm 
    * 
    * */

    geometry_msgs::Pose point1_shift, point2_shift, target_point;
    geometry_msgs::Point delta_1, delta_2;
    geometry_msgs::Point P_1, P_2;
    tf2::Quaternion Tg_Pose_Orientation1, Tg_Pose_Orientation2;
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
    tf2::fromMsg(Point1.orientation, Tg_Pose_Orientation1);
    tf2::fromMsg(Point3.orientation, Tg_Pose_Orientation2);

    // Cornering function using bèzier quadratic function
    for (int i = 0; i < res; i++)
    {
        t = 1/res*i;
        // P1 + (1-t)^2(P0-P1)+t^2(P2-P1)
        target_point.position.x = Point2.position.x + (1-t)*(1-t)*(point1_shift.position.x - Point2.position.x) + t*t*(point2_shift.position.x - Point2.position.x);
        target_point.position.y = Point2.position.y + (1-t)*(1-t)*(point1_shift.position.y - Point2.position.y) + t*t*(point2_shift.position.y - Point2.position.y);
        target_point.position.z = Point2.position.z + (1-t)*(1-t)*(point1_shift.position.z - Point2.position.z) + t*t*(point2_shift.position.z - Point2.position.z);
//        target_point.orientation = slerp(Point1.orientation, Point2.orientation,t); 
        target_point.orientation = tf2::toMsg(Tg_Pose_Orientation1.slerp(Tg_Pose_Orientation2,t));
        waypoints.poses.push_back(target_point);
    }


    if (norm_2 > d2)
    {
        waypoints.poses.push_back(point2_shift);
    }
}

void GraspWire(moveit::planning_interface::MoveGroupInterface& move_group, moveit::planning_interface::MoveGroupInterface& hand_group,
            geometry_msgs::Pose grasp_pose, double d, double res, geometry_msgs::PoseArray& waypoints)
{
    /*
    *   Compute the trajectory for perform a grasp in the position specified in "grasp_pose"
    *   res = number of point created 
    *   d = approaching distance
    * 
    */
    double distance[3] = {0, 0, d};
    double step;
    
    geometry_msgs::Pose target_pose, init_pose;
    geometry_msgs::Quaternion prova_quat;


    geometry_msgs::PoseArray grasp_seq_go;
    geometry_msgs::PoseArray grasp_seq_back;

    moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian;
    moveit_msgs::RobotTrajectory trajectory_cartesian;
    const double jump_threshold = 0.0;
    const double eef_step = 0.5;
    double fraction;   

    tf2::Matrix3x3 R;
    tf2::Quaternion quat;

    quat.setX(grasp_pose.orientation.x);
    quat.setY(grasp_pose.orientation.y);
    quat.setZ(grasp_pose.orientation.z);
    quat.setW(grasp_pose.orientation.w);
    R.setRotation(quat);

	init_pose.position.x = grasp_pose.position.x -( R[0][0]*distance[0] + R[0][1]*distance[1] + R[0][2]*distance[2]) ;
	init_pose.position.y = grasp_pose.position.y -( R[1][0]*distance[0] + R[1][1]*distance[1] + R[1][2]*distance[2]) ;
	init_pose.position.z = grasp_pose.position.z -( R[2][0]*distance[0] + R[2][1]*distance[1] + R[2][2]*distance[2]) ;
	init_pose.orientation = grasp_pose.orientation;
    
    step = d/res;

    double t;

    for (int k = 0; k < step; k++)
	{
        t = 1/(step-1)*k;
		target_pose.position.x = init_pose.position.x + t*(R[0][0]*distance[0] + R[0][1]*distance[1] + R[0][2]*distance[2]);
		target_pose.position.y = init_pose.position.y + t*(R[1][0]*distance[0] + R[1][1]*distance[1] + R[1][2]*distance[2]);
		target_pose.position.z = init_pose.position.z + t*(R[2][0]*distance[0] + R[2][1]*distance[1] + R[2][2]*distance[2]);
		target_pose.orientation = init_pose.orientation;
        grasp_seq_go.poses.push_back(target_pose);
		waypoints.poses.push_back(target_pose);
	}


    for(int k=0; k<grasp_seq_go.poses.size(); k++)
    {
        grasp_seq_back.poses.push_back(grasp_seq_go.poses[grasp_seq_go.poses.size()-1-k]);
    }
/*

	for (int k = 0; k < step; k++)
	{
        t = 1/(step-1)*k;
		target_pose.position.x = grasp_pose.position.x - t*(R[0][0]*distance[0] + R[0][1]*distance[1] + R[0][2]*distance[2]);
		target_pose.position.y = grasp_pose.position.y - t*(R[1][0]*distance[0] + R[1][1]*distance[1] + R[1][2]*distance[2]);
		target_pose.position.z = grasp_pose.position.z - t*(R[2][0]*distance[0] + R[2][1]*distance[1] + R[2][2]*distance[2]);
		target_pose.orientation = grasp_pose.orientation;
        grasp_seq_back.push_back(target_pose);
		waypoints.push_back(target_pose);
	}
*/
    FromEE2Link8(grasp_seq_go);
    FromEE2Link8(grasp_seq_back);

    std::cout<< "Open_ grip \n";
    hand_group.setJointValueTarget(hand_open_position);
    hand_group.move();
    ros::Duration(1).sleep();

    std::cout << "get close \n";
    move_group.computeCartesianPath(grasp_seq_go.poses, eef_step, jump_threshold, trajectory_cartesian);
    Plan_Cartesian.trajectory_ = trajectory_cartesian;
    move_group.execute(Plan_Cartesian);
    ros::Duration(1).sleep();

    std::cout<< "close_grip \n";
    hand_group.setJointValueTarget(hand_closed_position);
    hand_group.move();
    ros::Duration(1).sleep();

    std::cout << "move out \n";
    move_group.computeCartesianPath(grasp_seq_back.poses, eef_step, jump_threshold, trajectory_cartesian);
    Plan_Cartesian.trajectory_ = trajectory_cartesian;
    move_group.execute(Plan_Cartesian);

    ros::Duration(1).sleep();
    return;
}

int ComputePatch(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::PoseArray& waypoints, double res)
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
    step = distance/res;

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
/*
void Patching(WiresWaypoints waypoints, WiresWaypoints& waypoints_final, double res)
{
    std::vector<geometry_msgs::Pose> Patch;
    std::vector<int> InsertPos;
    std::vector<int> NoElement;
    std::vector<std::vector<geometry_msgs::Pose> > PatchVector;
    int PatchIndex = 0;
    bool check_info;

    int no_patch;
    for (size_t i = 1; i< waypoints.pose.size(); i++)
    {


        no_patch = ComputePatch(waypoints.pose[i-1], waypoints.pose[i], Patch, res);
        if (no_patch > 0)
        {
            InsertPos.push_back(i);
            NoElement.push_back(Patch.size());
            PatchVector.push_back(Patch);
            Patch.clear(); 
        }
    }

    for (size_t i=0; i< waypoints.pose.size(); i++)
    {
        if(i == InsertPos[PatchIndex])
        {    
            for(size_t j = 0; j < NoElement[PatchIndex]; j++)
            {
                waypoints_final.pose.push_back(PatchVector[PatchIndex][j]);
                waypoints_final.id_info.push_back(waypoints.id_info[i-1]);
            }
            PatchIndex++;
        }
        else
        {
            waypoints_final.pose.push_back(waypoints.pose[i]);
            waypoints_final.id_info.push_back(waypoints.id_info[i]);
            
        }
    }
    return ;
}
*/
/*
void getJacobian(Eigen::Matrix<double,6,7>& Jacobian, std::vector<double> Joint_Pos)
{
    Jacobian_Computation_eigen M_AT[8];

    M_AT[0].T(0,0) = 0;
    M_AT[0].T << 1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,0,0,1;
                        
    
    Eigen::Vector3d z;
    Eigen::Vector3d cross;
    Eigen::Vector3d P_EE;
    Eigen::Vector3d p;
    p << 0,0,0;
    
    for(int i=0; i<7; i++)
    {
        ComputeTmatrix_eigen(M_AT[i].A, DH_param_a[i], DH_param_alpha[i], DH_param_d[i], Joint_Pos[i]);
    }
    ComputeTmatrix_eigen(M_AT[7].A, DH_param_a[7], DH_param_alpha[7], DH_param_d[7], 0.0);     

    M_AT[0].T = M_AT[0].A;
    M_AT[1].T = M_AT[0].A * M_AT[1].A;
    M_AT[2].T = M_AT[0].A * M_AT[1].A * M_AT[2].A;
    M_AT[3].T = M_AT[0].A * M_AT[1].A * M_AT[2].A * M_AT[3].A;
    M_AT[4].T = M_AT[0].A * M_AT[1].A * M_AT[2].A * M_AT[3].A * M_AT[4].A;
    M_AT[5].T = M_AT[0].A * M_AT[1].A * M_AT[2].A * M_AT[3].A * M_AT[4].A * M_AT[5].A;
    M_AT[6].T = M_AT[0].A * M_AT[1].A * M_AT[2].A * M_AT[3].A * M_AT[4].A * M_AT[5].A * M_AT[6].A*M_AT[7].A;

//    M_AT[6].T = M_AT[0].A * M_AT[1].A * M_AT[2].A * M_AT[3].A * M_AT[4].A * M_AT[5].A * M_AT[6].A;
//    M_AT[7].T = M_AT[0].A * M_AT[1].A * M_AT[2].A * M_AT[3].A * M_AT[4].A * M_AT[5].A * M_AT[6].A * M_AT[7].A;

    P_EE << M_AT[7].T(0,3) - p(0), M_AT[7].T(1,3) - p(1),  M_AT[7].T(2,3) - p(2);

    cross = z.cross(P_EE);

    Jacobian(0,0) = cross(0);
    Jacobian(1,0) = cross(1);
    Jacobian(2,0) = cross(2);

    Jacobian(3,0) = 0;
    Jacobian(4,0) = 0;
    Jacobian(5,0) = 1;

    //sostituito 8 con i<7
    for(int i = 1; i<7; i++)
    {
        p    << M_AT[i].T(0,3),  M_AT[i].T(1,3), M_AT[i].T(2,3);
        P_EE << M_AT[6].T(0,3) - p(0),           M_AT[6].T(1,3) - p(0),  M_AT[6].T(2,3) - p(0);
    //    P_EE << M_AT[7].T(0,3) - p(0),           M_AT[7].T(1,3) - p(0),  M_AT[7].T(2,3) - p(0);
        z    << M_AT[i].T(0,2),  M_AT[i].T(1,2),  M_AT[i].T(2,2);

        cross = z.cross(P_EE);

        Jacobian(0,i) = cross(0);
        Jacobian(1,i) = cross(1);
        Jacobian(2,i) = cross(2);

        Jacobian(3,i) = z(0);
        Jacobian(4,i) = z(1);
        Jacobian(5,i) = z(2);
    }
}
*/

/*
void ComputeTmatrix_eigen (Eigen::Matrix<double, 4,4>& A, float DH_param_a, double DH_param_alpha, double DH_param_d, double Joint_value)
{
    A << cos(Joint_value), -sin(Joint_value)*cos(DH_param_alpha) , sin(Joint_value)*sin(DH_param_alpha)  , DH_param_a*cos(Joint_value),
         sin(Joint_value), cos(Joint_value)*cos(DH_param_alpha)  , -cos(Joint_value)*sin(DH_param_alpha) , DH_param_a*sin(Joint_value),
         0               , sin(DH_param_alpha)                   , cos(DH_param_alpha)                   , DH_param_d                 ,
         0               , 0                                     ,0                                      ,  1                         ;

    return;
}
*/
void ComputeTmatrix_float (Eigen::Matrix<float, 4,4>& A, float DH_param_a, float DH_param_alpha, float DH_param_d, float Joint_value)
{
    A << cos(Joint_value), -sin(Joint_value)*cos(DH_param_alpha) , sin(Joint_value)*sin(DH_param_alpha)  , DH_param_a*cos(Joint_value),
         sin(Joint_value), cos(Joint_value)*cos(DH_param_alpha)  , -cos(Joint_value)*sin(DH_param_alpha) , DH_param_a*sin(Joint_value),
         0               , sin(DH_param_alpha)                   , cos(DH_param_alpha)                   , DH_param_d                 ,
         0               , 0                                     ,0                                      ,  1                         ;

    A(0,0) = cos(Joint_value);
    A(0,1) = -sin(Joint_value)*cos(DH_param_alpha);
    A(0,2) = sin(Joint_value)*sin(DH_param_alpha);
    A(0,3) = DH_param_a*cos(Joint_value);

    A(1,0) = sin(Joint_value);
    A(1,1) = cos(Joint_value)*cos(DH_param_alpha);
    A(1,2) = -cos(Joint_value)*sin(DH_param_alpha);
    A(1,3) = DH_param_a*sin(Joint_value);

    A(2,0) = 0;
    A(2,1) = sin(DH_param_alpha)  ;
    A(2,2) = cos(DH_param_alpha) ; 
    A(2,3) = DH_param_d ;

    A(3,0) = 0;
    A(3,1) = 0;
    A(3,2) = 0;
    A(3,3) = 1;

    return;
}

void getJacobianDerivate (Eigen::Matrix<double,6,7> Jacobian, Eigen::Matrix<double,6,7>& Jacobian_der, std::vector<double> vel_in)
{      
    const int size = 6;
      
    Eigen::Matrix<double,3,3> rot,vel;      
    Eigen::Matrix<double,size,size> tmp = Eigen::MatrixXd::Zero(size, size);
    geometry_msgs::Twist jac_column;      
    
    for(int i=0; i < 7; i++)
    {
        jac_column.linear.x = Jacobian(0,i);
        jac_column.linear.y = Jacobian(1,i);
        jac_column.linear.z = Jacobian(2,i);
        jac_column.angular.x = Jacobian(3,i);
        jac_column.angular.y = Jacobian(4,i);
        jac_column.angular.z = Jacobian(5,i);
		  
		rot << 0, -jac_column.angular.z, jac_column.angular.y, 
		    jac_column.angular.z, 0, -jac_column.angular.x, 
			-jac_column.angular.y, jac_column.angular.x, 0;

		vel << 0, -jac_column.linear.z, jac_column.linear.y, 
			    jac_column.linear.z, 0, -jac_column.linear.x, 
		    	-jac_column.linear.y, jac_column.linear.x, 0;

		tmp.topLeftCorner(size/2, size/2)     += rot*vel_in[i];
		tmp.topRightCorner(size/2, size/2)    += vel*vel_in[i];      
	    tmp.bottomRightCorner(size/2, size/2) += rot*vel_in[i];
    }
	  
    Jacobian_der = tmp*Jacobian;  
}

void getJacobianInverse(Eigen::Matrix<double,6,7>  Jacobian, Eigen::Matrix<double, 7, 6>& Jacobian_inverse)
{
    /*
    *   Moore-Penrose pseudoInverse, using the single value decomposition
    *   given A = mxn, if A = U Sigma V*    (with U [m,m], Sigma [m,n], V* [n,n] )
    *   Then   A+ = V Sigma+ U*
    */

   int dim = 7;

    Eigen::MatrixXd U(6,6), U_inv(6,6);
    Eigen::MatrixXd Sigma_inv(dim,6), flag(dim,6);
    Eigen::MatrixXd V(dim,dim), V_inv(dim,dim);
    Eigen::VectorXd Singular_values(6);

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(Jacobian, Eigen::ComputeFullU | Eigen::ComputeFullV);

    U = svd.matrixU();
    Singular_values = svd.singularValues();
    V_inv = svd.matrixV();

    V = V_inv.transpose();
    U_inv = U.transpose();
    Sigma_inv <<       1/Singular_values(0),     0,0,0,0,0,
                0,       1/Singular_values(1),     0,0,0,0,
                0,0,       1/Singular_values(2),     0,0,0,
                0,0,0,       1/Singular_values(3),     0,0,
                0,0,0,0,       1/Singular_values(4),     0,
                0,0,0,0,0,       1/Singular_values(5),     
                0,0,0,0,0,0;

    Jacobian_inverse = V * Sigma_inv * U_inv;

    return ;
}

void computeEE_Vel_Acc(geometry_msgs::PoseArray waypoints, std::vector<geometry_msgs::Twist>& EE_velocity, std::vector<geometry_msgs::Accel>& EE_accel,  float velocity, double res)
{
    RPY angle_final, angle_init, delta_angular;
    

    EE_velocity[0].linear.x = 0;
    EE_velocity[0].linear.y = 0;
    EE_velocity[0].linear.z = 0;
    EE_velocity[0].angular.x = 0;
    EE_velocity[0].angular.y = 0;
    EE_velocity[0].angular.z = 0;

    EE_accel[0].linear.x = 0;
    EE_accel[0].linear.y = 0;
    EE_accel[0].linear.z = 0;
    EE_accel[0].angular.x = 0;
    EE_accel[0].angular.y = 0;
    EE_accel[0].angular.z = 0;

    double delta_time = res/velocity;
    double w[4];

    for( size_t i = 1; i < waypoints.poses.size(); i++)
    {
        EE_velocity[i].linear.x = (waypoints.poses[i].position.x - waypoints.poses[i-1].position.x)/delta_time;
        EE_velocity[i].linear.y = (waypoints.poses[i].position.y - waypoints.poses[i-1].position.y)/delta_time;
        EE_velocity[i].linear.z = (waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)/delta_time;

        angle_init = Quat2RPY(waypoints.poses[i-1].orientation);
        angle_final = Quat2RPY(waypoints.poses[i].orientation);

        delta_angular.pitch = (angle_final.pitch - angle_init.pitch)/delta_time;
        delta_angular.roll  = (angle_final.roll - angle_init.roll)/delta_time;
        delta_angular.yaw   = (angle_final.yaw - angle_init.yaw)/delta_time;

        EE_velocity[i].angular.x = delta_angular.roll + 0 - delta_angular.yaw * sinf(angle_final.pitch);
        EE_velocity[i].angular.y = 0 + delta_angular.pitch * cosf(angle_final.roll) + delta_angular.yaw * sinf(angle_final.roll) * cosf(angle_final.pitch);
        EE_velocity[i].angular.z = 0 - delta_angular.pitch * sinf(angle_final.roll) + delta_angular.yaw * cosf(angle_final.roll) * cosf(angle_final.pitch);

        EE_accel[i].linear.x = (EE_velocity[i].linear.x - EE_velocity[i-1].linear.x) /delta_time;
        EE_accel[i].linear.y = (EE_velocity[i].linear.y - EE_velocity[i-1].linear.y) /delta_time;
        EE_accel[i].linear.z = (EE_velocity[i].linear.z - EE_velocity[i-1].linear.z) /delta_time;
        EE_accel[i].angular.x = (EE_velocity[i].angular.x - EE_velocity[i-1].angular.x) /delta_time;
        EE_accel[i].angular.y = (EE_velocity[i].angular.y - EE_velocity[i-1].angular.y) /delta_time;
        EE_accel[i].angular.z = (EE_velocity[i].angular.z - EE_velocity[i-1].angular.z) /delta_time;
    }
}

void getJointSpaceVelocity(std::vector<trajectory_msgs::JointTrajectoryPoint>& joints_configuration, std::vector<geometry_msgs::Twist> EE_velocity)
{   
    Eigen::Matrix<double, 6, 7> Jacobian;
    Eigen::Matrix<double, 7, 6> Jacobian_inv;

    for(int k=0; k<EE_velocity.size(); k++)
    {
       // getJacobian(Jacobian, joints_configuration[k].positions);
        getJacobianInverse(Jacobian, Jacobian_inv);

        for (size_t i = 0; i < joints_configuration[k].velocities.size(); i++)
        {
            joints_configuration[k].velocities[i] = 0; // reset old value

            joints_configuration[k].velocities[i] = joints_configuration[k].velocities[i] + Jacobian_inv(i,0)*EE_velocity[k].linear.x
                                                  + Jacobian_inv(i,1)*EE_velocity[k].linear.y
                                                  + Jacobian_inv(i,2)*EE_velocity[k].linear.z

                                                  + Jacobian_inv(i,3)*EE_velocity[k].angular.x
                                                  + Jacobian_inv(i,4)*EE_velocity[k].angular.y
                                                  + Jacobian_inv(i,5)*EE_velocity[k].angular.z;
        }
    }

    return ;
}

void setJointSpaceVel_Acc(std::vector<trajectory_msgs::JointTrajectoryPoint>& joints_configuration, std::vector<geometry_msgs::Twist> EE_velocity, std::vector<geometry_msgs::Accel> EE_acceleration)
{   
    Eigen::Matrix<double, 6, 7> Jacobian;
    Eigen::Matrix<double, 7, 6> Jacobian_inv;
    Eigen::Matrix<double, 6, 7> Jacobian_der;

    Eigen::Matrix<double,7,1> q_dot, q_dotdot;
    Eigen::Matrix<double,6,1> x_dotdot;


    for(int k=0; k<EE_velocity.size(); k++)
    {
     //   getJacobian(Jacobian, joints_configuration[k].positions);
        getJacobianInverse(Jacobian, Jacobian_inv);

        for (size_t i = 0; i < joints_configuration[k].velocities.size(); i++)
        {
            joints_configuration[k].velocities[i] = 0; // reset old value
            joints_configuration[k].accelerations[i] = 0;
            joints_configuration[k].velocities[i] = joints_configuration[k].velocities[i] + Jacobian_inv(i,0)*EE_velocity[k].linear.x
                                                  + Jacobian_inv(i,1)*EE_velocity[k].linear.y
                                                  + Jacobian_inv(i,2)*EE_velocity[k].linear.z

                                                  + Jacobian_inv(i,3)*EE_velocity[k].angular.x
                                                  + Jacobian_inv(i,4)*EE_velocity[k].angular.y
                                                  + Jacobian_inv(i,5)*EE_velocity[k].angular.z;
        }

        getJacobianDerivate(Jacobian, Jacobian_der, joints_configuration[k].velocities);

        
        x_dotdot << EE_acceleration[k].linear.x,
                    EE_acceleration[k].linear.y,
                    EE_acceleration[k].linear.z,
                    EE_acceleration[k].angular.x,
                    EE_acceleration[k].angular.y,
                    EE_acceleration[k].angular.z;

        q_dot << joints_configuration[k].velocities[0],
                 joints_configuration[k].velocities[1],
                 joints_configuration[k].velocities[2],
                 joints_configuration[k].velocities[3],
                 joints_configuration[k].velocities[4],
                 joints_configuration[k].velocities[5],
                 joints_configuration[k].velocities[6];

        q_dotdot = Jacobian_inv*(x_dotdot-Jacobian_der*q_dot);

        for (size_t i=0; i < joints_configuration[k].accelerations.size(); i++)
        {
            joints_configuration[k].accelerations[i] = q_dotdot(i,0);
        }
    }

    return ;
}

void TrapezoidalMotion(double T, double Ta, double Td, double v, double qi, double qf, std::vector<double> Positions, std::vector<double>& Time )
{
    /* 
    *   Already in progress, and probably not needed
    *   
    *   Read the position in workspace, belongin to a trapezoidal velocity profile
    *   Then compute the time of the trajectory using the velocity
    * 
    */

    // T Period of motion
    // Ta time of aceleration
    // Tf time at decelleration
    // v constant velocity
    

    double a0, a1, a2;
    double b0, b1;
    double c0, c1, c2;

    double q_ta, q_tf; 
    double new_time;
    double v_temp;
    double a_temp;

    std::vector<double> T_array;
    std::vector<double> Velocity;
    std::vector<double> Acceleration;

    double Jacobian[6][7];
    double d_Jacobian[6][7];
    
    a0 = qi;
    a1 = 0;
    a2 = v/(2*Ta);

    b0 = qi - v*Ta/2;
    b1 = v;
    
        // VERIFICA CHE TD sia corretto
    c0 = qf - (v*Td*Td)/(2*Ta);
    c1 = v*Td*Td/Ta;
    c2 = -v/(2*Ta);

    q_ta = b0 + b1*Ta;
    q_tf = b0 + b1*Td;

    Time.clear();

    for (size_t i = 0; i < Positions.size(); i++)
    {
        if (Positions[i] < q_ta)
        {
            new_time = (-a1 + sqrt(a1*a1-4*a2*(a0-Positions[i])) )/(2*a2);
            Time.push_back(new_time);

            v_temp = a1 + 2*a2*Time[i];
            a_temp = 2*a2;

            Velocity.push_back(v_temp);
            Acceleration.push_back(a_temp);
        }
        else
        {
            if(Positions[i] < q_tf)
            {
                new_time = (Positions[i] - b0)/b1;
                Time.push_back(new_time);

                v_temp = b1;
                a_temp = 0;

                Velocity.push_back(v_temp);
                Acceleration.push_back(a_temp);
            }
            else
            {
                new_time = (-c1 + sqrt(c1*c1-4*c2*(c0-Positions[i])) )/(2*c2) ;
                Time.push_back(new_time);

                v_temp = c1 + 2*c2*Time[i];
                a_temp = 2*c2;

                Velocity.push_back(v_temp);
                Acceleration.push_back(a_temp);
            }
        }
    }
}

void PlanCartesianMotion(robot_state::JointModelGroup* joint_model_group, geometry_msgs::PoseArray waypoints,  
                        moveit::planning_interface::MoveGroupInterface::Plan& Plan_Cartesian, 
                        moveit::planning_interface::MoveGroupInterface& move_group,
                        RescaleVelocityTrajectory& coordinates_to_elaborate, geometry_msgs::PoseArray& failed_points,
                        std::vector<std::vector<trajectory_msgs::JointTrajectoryPoint>>& Plan_vector)
{
	moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    int size_plan;
    ros::Duration T_offset;
    robot_state::RobotState start_state(*move_group.getCurrentState());
    moveit_msgs::RobotTrajectory trajectory_cartesian;
    const double jump_threshold = 0.0;
    const double eef_step = 0.5;
    double fraction;   


    std::vector<double[7] > joints_list_Cartesian;
    double joints_Cartesian[7];
    geometry_msgs::PoseArray waypoints_border; // Used to copy the two border pose for the cartesian ppath computation.
    waypoints_border.poses.resize(2);

    T_offset = ros::Duration(0);
    std::vector<trajectory_msgs::JointTrajectoryPoint> Joints_points;

        
    int fail = 0;

    move_group.setPoseTarget(waypoints.poses[0]);
    move_group.plan(Plan_Cartesian);

    size_plan = Plan_Cartesian.trajectory_.joint_trajectory.points.size();
    T_offset = Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1].time_from_start;
    
    Plan_vector.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points);          //*********** NEW************//
    
    Joints_points.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1]);

    for (int j=0; j<7; j++)
    {
        joints_Cartesian[j] = Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
    }

    /*** Points to elaborate ***/
    coordinates_to_elaborate.ws_Pose.poses.push_back(waypoints.poses[0]);
    coordinates_to_elaborate.jointspace_points.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1]);


    start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);       
    move_group.setStartState(start_state);    


    for (size_t i = 1; i < waypoints.poses.size(); i++)
    {
        waypoints_border.poses[0] = waypoints.poses[i-1];
        waypoints_border.poses[1] = waypoints.poses[i];
        fraction = move_group.computeCartesianPath(waypoints_border.poses, eef_step, jump_threshold, trajectory_cartesian);

        if (fraction > 0)
        {
            std::cout << "Plan " << i << " succesfull \n";
            size_plan = trajectory_cartesian.joint_trajectory.points.size();

            Plan_vector.push_back(trajectory_cartesian.joint_trajectory.points);  //*****//
            for (size_t k = 0; k < size_plan; k++)
            {
                trajectory_cartesian.joint_trajectory.points[k].time_from_start += T_offset; 
                Plan_Cartesian.trajectory_.joint_trajectory.points.push_back(trajectory_cartesian.joint_trajectory.points[k]);
            }

            Joints_points.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1]);

            for (int j=0; j<7; j++)
            {
                joints_Cartesian[j] = trajectory_cartesian.joint_trajectory.points[size_plan-1].positions[j];
            }
            start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);       
            move_group.setStartState(start_state);

            size_plan = Plan_Cartesian.trajectory_.joint_trajectory.points.size();
            T_offset = Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1].time_from_start;

            /*** Points to elaborate ***/
            coordinates_to_elaborate.ws_Pose.poses.push_back(waypoints_border.poses[1]);
            coordinates_to_elaborate.jointspace_points.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1]);
            
        }
        else
        {
            std::cout << "plan " << i << " failed \n";
            fail++;
            failed_points.poses.push_back(waypoints.poses[i]);
        }
    }

    std::cout<< fail << " failed points out of " << waypoints.poses.size() << "\n";
}

void PlanRescaledCartesianMotion(robot_state::JointModelGroup* joint_model_group, 
                                geometry_msgs::PoseArray waypoints,  
                                moveit::planning_interface::MoveGroupInterface& move_group,
                                RescaleVelocityTrajectory& coordinates_to_elaborate, 
                                geometry_msgs::PoseArray& failed_points,
                                float velocity,
                                moveit_msgs::RobotTrajectory& RobotTrajectory,
                                TrajectoryPlanner_param param)
{
	moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    int size_plan;
    ros::Duration T_offset;
    robot_state::RobotState start_state(*move_group.getCurrentState());
    moveit_msgs::RobotTrajectory trajectory_cartesian;
    moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian;

    std::vector<std::vector<trajectory_msgs::JointTrajectoryPoint>> Plan_vector;
    std::vector<std::vector<geometry_msgs::Twist>> EE_vel_vector;
    std::vector <std::vector  <geometry_msgs::Accel>> EE_acc_vector;

    const double jump_threshold = 0.0;
    const double eef_step = 0.5;
    double fraction;   

    std::vector<double[7] > joints_list_Cartesian;
    double joints_Cartesian[7];
    geometry_msgs::PoseArray waypoints_border; // Used to copy the two border pose for the cartesian ppath computation.
    waypoints_border.poses.resize(2);

    T_offset = ros::Duration(0);
    std::vector<trajectory_msgs::JointTrajectoryPoint> Joints_points;


        
    int fail = 0;

    move_group.setPoseTarget(waypoints.poses[0]);
    move_group.plan(Plan_Cartesian);

    size_plan = Plan_Cartesian.trajectory_.joint_trajectory.points.size();
    T_offset = Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1].time_from_start;
    
    Plan_vector.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points);          //*********** NEW************//
    
    Joints_points.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1]);

    for (int j=0; j<7; j++)
    {
        joints_Cartesian[j] = Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
    }


    coordinates_to_elaborate.ws_Pose.poses.push_back(waypoints.poses[0]);
    coordinates_to_elaborate.jointspace_points.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1]);


    start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);       
    move_group.setStartState(start_state);    

    // Plan motion point by point
    for (size_t i = 1; i < waypoints.poses.size(); i++)
    {
        waypoints_border.poses[0] = waypoints.poses[i-1];
        waypoints_border.poses[1] = waypoints.poses[i];
        fraction = move_group.computeCartesianPath(waypoints_border.poses, eef_step, jump_threshold, trajectory_cartesian);

        if (fraction > 0)
        {
            std::cout << "Plan " << i << " succesfull \n";
            size_plan = trajectory_cartesian.joint_trajectory.points.size();

            Plan_vector.push_back(trajectory_cartesian.joint_trajectory.points);  //*****//
            for (size_t k = 0; k < size_plan; k++)
            {
                trajectory_cartesian.joint_trajectory.points[k].time_from_start += T_offset; 
                Plan_Cartesian.trajectory_.joint_trajectory.points.push_back(trajectory_cartesian.joint_trajectory.points[k]);
            }

            Joints_points.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1]);

            for (int j=0; j<7; j++)
            {
                joints_Cartesian[j] = trajectory_cartesian.joint_trajectory.points[size_plan-1].positions[j];
            }
            start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);       
            move_group.setStartState(start_state);

            size_plan = Plan_Cartesian.trajectory_.joint_trajectory.points.size();
            T_offset = Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1].time_from_start;

            coordinates_to_elaborate.ws_Pose.poses.push_back(waypoints_border.poses[1]);
            coordinates_to_elaborate.jointspace_points.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1]);
            
        }
        else
        {
            std::cout << "plan " << i << " failed \n";
            fail++;
            failed_points.poses.push_back(waypoints.poses[i]);
        }
    }


    coordinates_to_elaborate.ws_Twist.resize(coordinates_to_elaborate.ws_Pose.poses.size());
    coordinates_to_elaborate.ws_Accel.resize(coordinates_to_elaborate.ws_Pose.poses.size());

    // Copy of joint name
    RobotTrajectory.joint_trajectory.joint_names.resize(7);
    for(size_t i=0; i<7; i++)
    {
        RobotTrajectory.joint_trajectory.joint_names[i] = Plan_Cartesian.trajectory_.joint_trajectory.joint_names[i];
    }


    EE_vel_vector.resize(Plan_vector.size());
    for (size_t i=0; i<Plan_vector.size(); i++)
        EE_vel_vector[i].resize(Plan_vector[i].size());
        
    EE_acc_vector.resize(Plan_vector.size());
    for (size_t i=0; i<Plan_vector.size(); i++)
        EE_acc_vector[i].resize(Plan_vector[i].size());

    // calcolo della velocità e dell'accelerazione lungo il workspace
    computeEE_Vel_Acc2(coordinates_to_elaborate.ws_Pose , velocity, param.res, Plan_vector, EE_vel_vector, EE_acc_vector);

    coordinates_to_elaborate.jointspace_points.clear();
    coordinates_to_elaborate.ws_Accel.clear();
    coordinates_to_elaborate.ws_Twist.clear();

    // "SROTOLO DEI VETTORI CONTENENTI LE VEL E ACC"
    for(size_t i=0; i< Plan_vector.size(); i++)
    {
        for(size_t k=0; k<Plan_vector[i].size(); k++)
            coordinates_to_elaborate.jointspace_points.push_back(Plan_vector[i][k]);
    }
    for(size_t i=0; i< EE_vel_vector.size(); i++)
    {
        for(size_t k=0; k<EE_vel_vector[i].size(); k++)
            coordinates_to_elaborate.ws_Twist.push_back(EE_vel_vector[i][k]);
    }
    for(size_t i=0; i< EE_acc_vector.size(); i++)
    {
        for(size_t k=0; k<EE_acc_vector[i].size(); k++)
            coordinates_to_elaborate.ws_Accel.push_back(EE_acc_vector[i][k]);
    }

    // conversione da ws a jointspace velocity and aceleration
    setJointSpaceVel_Acc(coordinates_to_elaborate.jointspace_points, coordinates_to_elaborate.ws_Twist, coordinates_to_elaborate.ws_Accel);

    for(size_t i=0; i<coordinates_to_elaborate.jointspace_points.size(); i++)
       RobotTrajectory.joint_trajectory.points.push_back(coordinates_to_elaborate.jointspace_points[i]);


    return;    
}

void PlanSetPoseMotion(robot_state::JointModelGroup* joint_model_group, geometry_msgs::PoseArray waypoints,  
            moveit::planning_interface::MoveGroupInterface::Plan& Plan_SP, 
            moveit::planning_interface::MoveGroupInterface& move_group, RescaleVelocityTrajectory& coordinates_to_elaborate, std::vector<geometry_msgs::Quaternion> constraints_vector)
{
    double joints_SP[7];
    int fail_no = 0;
    bool success;
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    robot_state::RobotState start_state(*move_group.getCurrentState());

    int size_plan, final_plan_size;
    ros::Duration T_offset;

    move_group.setPoseTarget(waypoints.poses[0]);
    move_group.plan(Plan_SP);
    size_plan = Plan_SP.trajectory_.joint_trajectory.points.size();
    T_offset = Plan_SP.trajectory_.joint_trajectory.points[size_plan-1].time_from_start;



    move_group.setPlanningTime(20);

    for (int j=0; j<7; j++)
    {
        joints_SP[j] = Plan_SP.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
    }

    start_state.setJointGroupPositions(joint_model_group, joints_SP);           
    move_group.setStartState(start_state);

    for (size_t i=1; i < waypoints.poses.size(); i++)
    {

        move_group.setPoseTarget(waypoints.poses[i]);
        success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        if (success)
        {
            std::cout << "Plan Success \n";
            size_plan = my_plan.trajectory_.joint_trajectory.points.size();
            final_plan_size = Plan_SP.trajectory_.joint_trajectory.points.size();
            T_offset = Plan_SP.trajectory_.joint_trajectory.points[final_plan_size-1].time_from_start;
            for (size_t k = 0; k < size_plan; k++)
            {
                my_plan.trajectory_.joint_trajectory.points[k].time_from_start += T_offset; 
                Plan_SP.trajectory_.joint_trajectory.points.push_back(my_plan.trajectory_.joint_trajectory.points[k]);
            }

            for (int j=0; j<7; j++)
            {
                joints_SP[j] = my_plan.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
            }
            start_state.setJointGroupPositions(joint_model_group, joints_SP);           
            move_group.setStartState(start_state);

            coordinates_to_elaborate.ws_Pose.poses.push_back(waypoints.poses[i]);
            coordinates_to_elaborate.jointspace_points.push_back(Plan_SP.trajectory_.joint_trajectory.points.back());
        }
        else
        {
            std::cout << "Plan fallito" << i << "\n";
            fail_no++;
        }
    }
    std::cout << fail_no<< " points failed out of " << waypoints.poses.size() << "\n";
}


void PlanSetPoseMotion2(robot_state::JointModelGroup* joint_model_group, geometry_msgs::PoseArray waypoints,  
            moveit::planning_interface::MoveGroupInterface::Plan& Plan_SP, 
            moveit::planning_interface::MoveGroupInterface& move_group, RescaleVelocityTrajectory& coordinates_to_elaborate, std::vector<geometry_msgs::Quaternion> constraints_vector)
{
    double joints_SP[7];
    int fail_no = 0;
    bool success;
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    robot_state::RobotState start_state(*move_group.getCurrentState());

    int size_plan, final_plan_size;
    ros::Duration T_offset;

    move_group.setPoseTarget(waypoints.poses[0]);
    move_group.plan(Plan_SP);
    size_plan = Plan_SP.trajectory_.joint_trajectory.points.size();
    T_offset = Plan_SP.trajectory_.joint_trajectory.points[size_plan-1].time_from_start;



    // Constraint definition
    //ocm
    moveit_msgs::OrientationConstraint ocm;

    ocm.link_name = "panda_link8";
    ocm.header.frame_id = "panda_link0";

    ocm.orientation.x = 0.210738;
    ocm.orientation.y = 0.672778;
    ocm.orientation.z = 0.677635;
    ocm.orientation.w = 0.209212;

    ocm.absolute_x_axis_tolerance = 0.01;
    ocm.absolute_y_axis_tolerance = 0.01;
    ocm.absolute_z_axis_tolerance = 3.14;
    ocm.weight = 1.0;
    moveit_msgs::Constraints test_constraints;
    test_constraints.orientation_constraints.push_back(ocm);



    move_group.setPlanningTime(20);
    move_group.setNumPlanningAttempts(100);

    for (int j=0; j<7; j++)
    {
        joints_SP[j] = Plan_SP.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
    }

    start_state.setJointGroupPositions(joint_model_group, joints_SP);           
    move_group.setStartState(start_state);

    for (size_t i=1; i < waypoints.poses.size(); i++)
    {

        test_constraints.orientation_constraints[0].orientation = constraints_vector[i];
        move_group.setPathConstraints(test_constraints);
        move_group.setPoseTarget(waypoints.poses[i]);
        success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        if (success)
        {
            std::cout << "Plan Success \n";
            size_plan = my_plan.trajectory_.joint_trajectory.points.size();
            final_plan_size = Plan_SP.trajectory_.joint_trajectory.points.size();
            T_offset = Plan_SP.trajectory_.joint_trajectory.points[final_plan_size-1].time_from_start;
            for (size_t k = 0; k < size_plan; k++)
            {
                my_plan.trajectory_.joint_trajectory.points[k].time_from_start += T_offset; 
                Plan_SP.trajectory_.joint_trajectory.points.push_back(my_plan.trajectory_.joint_trajectory.points[k]);
            }

            for (int j=0; j<7; j++)
            {
                joints_SP[j] = my_plan.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
            }
            start_state.setJointGroupPositions(joint_model_group, joints_SP);           
            move_group.setStartState(start_state);

            coordinates_to_elaborate.ws_Pose.poses.push_back(waypoints.poses[i]);
            coordinates_to_elaborate.jointspace_points.push_back(Plan_SP.trajectory_.joint_trajectory.points.back());

        }
        else
        {
            std::cout << "Plan fallito" << i << "\n";
            fail_no++;
        }
    }
    std::cout << fail_no<< " points failed out of " << waypoints.poses.size() << "\n";
}

void PlanIKMotion(robot_state::JointModelGroup* joint_model_group, robot_state::RobotStatePtr kinematic_state, 
        geometry_msgs::PoseArray waypoints,  moveit::planning_interface::MoveGroupInterface::Plan& Plan_IK, 
        moveit::planning_interface::MoveGroupInterface& move_group, RescaleVelocityTrajectory& coordinates_to_elaborate, std::vector<geometry_msgs::Quaternion> constraints_vector)
{
    std::vector<double> joint_values;
    std::vector<std::vector<double> > joint_values_list_IK;
    std::size_t attempts = 1000;
    double timeout = 5;
    bool found_ik, success;

    int size_plan, final_plan_size;
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    ros::Duration T_offset;

    sensor_msgs::JointState JointSet;
    JointSet.position.resize(7);
    moveit_msgs::RobotState MyRobotStarState;
    MyRobotStarState.joint_state.position.resize(7);

    robot_state::RobotState start_state(*move_group.getCurrentState());

    found_ik = kinematic_state->setFromIK(joint_model_group, waypoints.poses[0], attempts, timeout);            
    if(found_ik)
    {
        kinematic_state->copyJointGroupPositions(joint_model_group, joint_values);
        success = (move_group.plan(Plan_IK) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    }


    moveit_msgs::OrientationConstraint ocm;

    ocm.link_name = "panda_link8";
    ocm.header.frame_id = "panda_link0";

    ocm.orientation.x = 0.210738;
    ocm.orientation.y = 0.672778;
    ocm.orientation.z = 0.677635;
    ocm.orientation.w = 0.209212;

    ocm.absolute_x_axis_tolerance = 0.01;
    ocm.absolute_y_axis_tolerance = 0.01;
    ocm.absolute_z_axis_tolerance = 3.14;
    ocm.weight = 1.0;
    moveit_msgs::Constraints test_constraints;
    test_constraints.orientation_constraints.push_back(ocm);

    for (size_t i = 1; i < waypoints.poses.size(); i++)
    {
        found_ik = kinematic_state->setFromIK(joint_model_group, waypoints.poses[i], attempts, timeout);            
        if(found_ik)
        {
            test_constraints.orientation_constraints[0].orientation = constraints_vector[i];
            move_group.setPathConstraints(test_constraints);

            std::cout << "IK founded \n";
            kinematic_state->copyJointGroupPositions(joint_model_group, joint_values);
            move_group.setJointValueTarget(joint_values);
            success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            if (success)
            {
            std::cout << "Plan succeeded \n";
            size_plan = my_plan.trajectory_.joint_trajectory.points.size();
            final_plan_size = Plan_IK.trajectory_.joint_trajectory.points.size();
            T_offset = Plan_IK.trajectory_.joint_trajectory.points[final_plan_size-1].time_from_start;
            for (size_t k = 0; k < size_plan; k++)
            {
                my_plan.trajectory_.joint_trajectory.points[k].time_from_start += T_offset; 
                Plan_IK.trajectory_.joint_trajectory.points.push_back(my_plan.trajectory_.joint_trajectory.points[k]);
            }
            start_state.setJointGroupPositions(joint_model_group, joint_values);       
            move_group.setStartState(start_state);
            }
        }
        else
        {
            ROS_INFO("Did not find IK solution");
        }
    }
}


void computeEE_Vel_Acc2(geometry_msgs::PoseArray waypoints, float velocity, double res, std::vector<std::vector<trajectory_msgs::JointTrajectoryPoint>>& Plan_vector,
                        std::vector<std::vector<geometry_msgs::Twist>>& EE_vel_vector, std::vector <std::vector  <geometry_msgs::Accel>>& EE_acc_vector ) 
{
    RPY angle_final, angle_init, delta_angular;

    EE_vel_vector[0][0].linear.x = 0;
    EE_vel_vector[0][0].linear.y = 0;
    EE_vel_vector[0][0].linear.z = 0;
    EE_vel_vector[0][0].angular.x = 0;
    EE_vel_vector[0][0].angular.y = 0;
    EE_vel_vector[0][0].angular.z = 0;

    EE_acc_vector[0][0].linear.x = 0;
    EE_acc_vector[0][0].linear.y = 0;
    EE_acc_vector[0][0].linear.z = 0;
    EE_acc_vector[0][0].angular.x = 0;
    EE_acc_vector[0][0].angular.y = 0;
    EE_acc_vector[0][0].angular.z = 0;

    double delta_time = res/velocity;
    int size_trajectory_point;
    double new_time;

    int sol = 0;

    geometry_msgs::Twist EE_vel_flag;

    // Time Rescaling

    size_trajectory_point = Plan_vector[0].size();
    ros::Duration time_from_start_max;

    time_from_start_max = Plan_vector[0][size_trajectory_point-1].time_from_start;

    for(size_t i=0; i <size_trajectory_point; i++)
    {
        new_time = Plan_vector[0][i].time_from_start.toSec() * delta_time / (time_from_start_max.toSec());
        Plan_vector[0][i].time_from_start = ros::Duration(new_time); 
    }

    for(size_t i = 1; i < Plan_vector.size(); i++)
    {
        size_trajectory_point = Plan_vector[i].size();
        time_from_start_max = Plan_vector[i][size_trajectory_point-1].time_from_start;

        for(size_t k=0; k < size_trajectory_point; k++)
        {
            if(time_from_start_max.toSec() == 0)
            {
                Plan_vector[i][k].time_from_start = Plan_vector[i-1].back().time_from_start;
                sol++;
            }
            else
            {
                new_time = Plan_vector[i][k].time_from_start.toSec() * delta_time / time_from_start_max.toSec() + Plan_vector[i-1].back().time_from_start.toSec();
                Plan_vector[i][k].time_from_start = ros::Duration(new_time);
            }
        }
    }

    for (size_t i = 1; i <Plan_vector.size(); i++)
    {
        EE_vel_flag.linear.x = (waypoints.poses[i].position.x - waypoints.poses[i-1].position.x)/delta_time;
        EE_vel_flag.linear.y = (waypoints.poses[i].position.y - waypoints.poses[i-1].position.y)/delta_time;
        EE_vel_flag.linear.z = (waypoints.poses[i].position.z - waypoints.poses[i-1].position.z)/delta_time;
        
        angle_init = Quat2RPY(waypoints.poses[i-1].orientation);
        angle_final = Quat2RPY(waypoints.poses[i].orientation);

        delta_angular.pitch = (angle_final.pitch - angle_init.pitch)/delta_time;
        delta_angular.roll  = (angle_final.roll - angle_init.roll)/delta_time;
        delta_angular.yaw   = (angle_final.yaw - angle_init.yaw)/delta_time;

        EE_vel_flag.angular.x = delta_angular.roll + 0 - delta_angular.yaw * sinf(angle_final.pitch);
        EE_vel_flag.angular.y = 0 + delta_angular.pitch * cosf(angle_final.roll) + delta_angular.yaw * sinf(angle_final.roll) * cosf(angle_final.pitch);
        EE_vel_flag.angular.z = 0 - delta_angular.pitch * sinf(angle_final.roll) + delta_angular.yaw * cosf(angle_final.roll) * cosf(angle_final.pitch);

        for(size_t k=1; k < Plan_vector[i].size(); k++)
        {
            EE_vel_vector[i][k].linear.x = EE_vel_flag.linear.x * k/Plan_vector[i].size() ;
            EE_vel_vector[i][k].linear.y = EE_vel_flag.linear.y * k/Plan_vector[i].size() ;
            EE_vel_vector[i][k].linear.z = EE_vel_flag.linear.z * k/Plan_vector[i].size() ;

            EE_acc_vector[i][k].linear.x = (EE_vel_vector[i][k].linear.x - EE_vel_vector[i][k-1].linear.x) /delta_time;
            EE_acc_vector[i][k].linear.y = (EE_vel_vector[i][k].linear.y - EE_vel_vector[i][k-1].linear.y) /delta_time;
            EE_acc_vector[i][k].linear.z = (EE_vel_vector[i][k].linear.z - EE_vel_vector[i][k-1].linear.z) /delta_time;

            EE_vel_vector[i][k].angular.x = EE_vel_flag.angular.x * k/Plan_vector[i].size() ;
            EE_vel_vector[i][k].angular.y = EE_vel_flag.angular.y * k/Plan_vector[i].size() ;
            EE_vel_vector[i][k].angular.z = EE_vel_flag.angular.z * k/Plan_vector[i].size() ;

            EE_acc_vector[i][k].angular.x = (EE_vel_vector[i][k].angular.x - EE_vel_vector[i][k-1].angular.x) /delta_time;
            EE_acc_vector[i][k].angular.y = (EE_vel_vector[i][k].angular.y - EE_vel_vector[i][k-1].angular.y) /delta_time;
            EE_acc_vector[i][k].angular.z = (EE_vel_vector[i][k].angular.z - EE_vel_vector[i][k-1].angular.z) /delta_time;
        }
    }

}

bool CheckEEPosition(geometry_msgs::Pose TargetPose, float Joint_Pos[7], double res)
{
    bool check_pos;
    
    std::vector<Eigen::Matrix<float, 4,4>> T_matricies;
    T_matricies.resize(8);
    Eigen::Matrix<float, 4,4> T_final;

    T_final <<  1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,0,0,1;
    

    for(int i=0; i<7; i++)
    {
        ComputeTmatrix_float(T_matricies[i], DH_param_a[i], DH_param_alpha[i], DH_param_d[i], Joint_Pos[i]);
    }
    ComputeTmatrix_float(T_matricies[7], DH_param_a[7], DH_param_alpha[7], DH_param_d[7], 0.0); 

    for(int i=0; i<8; i++)
    {
        T_final = T_final * T_matricies[i];
    }

    if ( (((TargetPose.position.x - T_final(0,3))*(TargetPose.position.x - T_final(0,3))) + 
         ((TargetPose.position.y - T_final(1,3))*(TargetPose.position.y - T_final(1,3))) + 
         ((TargetPose.position.z - T_final(2,3))*(TargetPose.position.z - T_final(2,3)))) < (res*res) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void getRealPath(float Joint_Pos[7], geometry_msgs::PoseArray& waypoints)
{
    bool check_pos;
    
    std::vector<Eigen::Matrix<float, 4,4>> T_matricies;
    T_matricies.resize(8);
    Eigen::Matrix<float, 4,4> T_final;

    Eigen::Matrix<float, 4,1> p, pf;

    p << 0,0,0,1;

    geometry_msgs::Pose new_point;
    tf2::Quaternion Quat;

    T_final <<  1,0,0,0,
                 0,1,0,0,
                 0,0,1,0,
                 0,0,0,1;
    

    for(int i=0; i<7; i++)
    {
        ComputeTmatrix_float(T_matricies[i], DH_param_a[i], DH_param_alpha[i], DH_param_d[i], Joint_Pos[i]);
            }

    ComputeTmatrix_float(T_matricies[7], DH_param_a[7], DH_param_alpha[7], DH_param_d[7], 0.0); 



    for(int i=0; i<8; i++)
    {
        T_final = T_final * T_matricies[i];
    }

   // T_final = T_matricies[0]*T_matricies[1]*T_matricies[2]*T_matricies[3]*T_matricies[4]*T_matricies[5]*T_matricies[6]*T_matricies[7];
   // T_final = T_matricies[7]*T_matricies[6]*T_matricies[5]*T_matricies[4]*T_matricies[3]*T_matricies[2]*T_matricies[1]*T_matricies[0];

    pf = T_final * p;
    
    new_point.position.x = T_final(0,3);
    new_point.position.y = T_final(2,3);
    new_point.position.z = T_final(1,3);
    /*
    new_point.position.x = pf(0);
    new_point.position.y = pf(1);
    new_point.position.z = pf(2);
    */
    Quat.setRPY(0,0,0);
    new_point.orientation = tf2::toMsg(Quat);    
    waypoints.poses.push_back(new_point);

    return;
}

moveit_msgs::RobotTrajectory rescalePathDavide( moveit_msgs::RobotTrajectory trajectory_cartesian, float velocity, bool save_t)
{
    /****
     * 
     *  V(t) = v0 + at
     *  a(t) = 1/2 a *t^2 +v0t+s0;
     * 
     * ********/

    moveit_msgs::RobotTrajectory traj_scaled;
    trajectory_msgs::JointTrajectoryPoint Traj_Point;

    float new_accel;
    float new_vel;
    ros::Duration delta_t;
    double time_d;

    traj_scaled.joint_trajectory.joint_names = trajectory_cartesian.joint_trajectory.joint_names;

    Traj_Point.positions.resize(7);
    Traj_Point.velocities.resize(7);
    Traj_Point.accelerations.resize(7);
    Traj_Point.effort.resize(7);    



    ros::Duration T_offset = ros::Duration(0.3);

    //SEt Initial Point;
    for(int k=0; k<7; k++)
    {
        Traj_Point.positions[k] = trajectory_cartesian.joint_trajectory.points[0].positions[k];
        Traj_Point.velocities[k] = 0; 
        Traj_Point.accelerations[k] = 0; 
    }
    Traj_Point.time_from_start = ros::Duration(0);
    traj_scaled.joint_trajectory.points.push_back(Traj_Point);


    for (size_t i = 1; i< trajectory_cartesian.joint_trajectory.points.size(); i++)
    {
        delta_t = trajectory_cartesian.joint_trajectory.points[i].time_from_start - trajectory_cartesian.joint_trajectory.points[i-1].time_from_start;
        time_d = delta_t.toSec();
        for(size_t k=0; k<7;k++)
        {
            Traj_Point.positions[k] = trajectory_cartesian.joint_trajectory.points[i].positions[k];

            new_accel = 2*(   trajectory_cartesian.joint_trajectory.points[i].positions[k] 
                            - trajectory_cartesian.joint_trajectory.points[i-1].positions[k] 
                            - trajectory_cartesian.joint_trajectory.points[i-1].velocities[k]*time_d ) / pow(time_d,2);
            
            new_vel = new_accel*time_d + traj_scaled.joint_trajectory.points[i-1].velocities[k]; 

            Traj_Point.accelerations[k] = new_accel;
            Traj_Point.velocities[k] = new_vel + trajectory_cartesian.joint_trajectory.points[i-1].velocities[k];
        }

        Traj_Point.time_from_start = trajectory_cartesian.joint_trajectory.points[i].time_from_start + T_offset;

        traj_scaled.joint_trajectory.points.push_back(Traj_Point);
    }

    if(save_t)
    {
        std::string Log_Path = "/home/kevin/ScaledDavide";
        std::ofstream plan_output_c;
        plan_output_c.open(Log_Path);
        
        for (size_t i = 0; i < traj_scaled.joint_trajectory.points.size(); i++)
        { 
            plan_output_c << "Positions\n"; 
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[0] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[1] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[2] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[3] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[4] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[5] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[6] << "\n";

            plan_output_c << "Velocities\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[0] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[1] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[2] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[3] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[4] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[5] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[6] << "\n";

            plan_output_c << "Accelerations\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[0] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[1] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[2] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[3] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[4] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[5] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[6] << "\n";

            plan_output_c << "Time_From_Start\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].time_from_start << "\n";                
        }

        plan_output_c.close();
    }

    return traj_scaled; 
}


moveit_msgs::RobotTrajectory RescaleDerivative(moveit_msgs::RobotTrajectory trajectory_cartesian, float velocity, bool save_t)
{
    moveit_msgs::RobotTrajectory traj_scaled;
    trajectory_msgs::JointTrajectoryPoint Traj_Point;
    ros::Duration T_offset = ros::Duration(0.3);

    traj_scaled.joint_trajectory.joint_names = trajectory_cartesian.joint_trajectory.joint_names;

    Traj_Point.positions.resize(7);
    Traj_Point.velocities.resize(7);
    Traj_Point.accelerations.resize(7);
    Traj_Point.effort.resize(7);    


    float delta_s = 0.001;
    float delta_t = delta_s/velocity;

    ros::Duration Delta_Time = ros::Duration(delta_s/velocity);
    T_offset = ros::Duration(0.3);

    //SEt Initial Point;
    for(int k=0; k<7; k++)
    {
        Traj_Point.positions[k] = trajectory_cartesian.joint_trajectory.points[0].positions[k];
        Traj_Point.velocities[k] = 0; 
        Traj_Point.accelerations[k] = 0; 
    }
    Traj_Point.time_from_start = ros::Duration(0);
    traj_scaled.joint_trajectory.points.push_back(Traj_Point);


    for(size_t i=1; i<trajectory_cartesian.joint_trajectory.points.size(); i++ )
    {
        for(int k=0; k<7; k++)
        {
            Traj_Point.positions[k] = trajectory_cartesian.joint_trajectory.points[i].positions[k];
            Traj_Point.velocities[k] = (trajectory_cartesian.joint_trajectory.points[i].positions[k] - trajectory_cartesian.joint_trajectory.points[i-1].positions[k])/delta_t; 
            Traj_Point.accelerations[k] = (Traj_Point.velocities[k] - traj_scaled.joint_trajectory.points[i-1].velocities[k])/delta_t; 
        }

        Traj_Point.time_from_start = ros::Duration(delta_t*i) + T_offset;
        traj_scaled.joint_trajectory.points.push_back(Traj_Point);
    }

    // Aggiunta dei punti finali
    traj_scaled.joint_trajectory.points[traj_scaled.joint_trajectory.points.size()-2].time_from_start = traj_scaled.joint_trajectory.points[traj_scaled.joint_trajectory.points.size()-2].time_from_start + ros::Duration(0.2);
    traj_scaled.joint_trajectory.points[traj_scaled.joint_trajectory.points.size()-1].time_from_start = traj_scaled.joint_trajectory.points[traj_scaled.joint_trajectory.points.size()-1].time_from_start + ros::Duration(0.21);
    //Correzione punti con ugual tempo
    for(int i=1; i< traj_scaled.joint_trajectory.points.size(); i++)
    {
        if((traj_scaled.joint_trajectory.points[i].time_from_start <= traj_scaled.joint_trajectory.points[i-1].time_from_start))
        {
            std::cout << "errore in" << i << "\n";
            traj_scaled.joint_trajectory.points[i].time_from_start = traj_scaled.joint_trajectory.points[i-1].time_from_start + ros::Duration(0.0000001);
        }
    }

    if(save_t)
    {

        std::string Log_Path = "/home/kevin/ScaledDeriv";
        std::ofstream plan_output_c;
        plan_output_c.open(Log_Path);
        
        for (size_t i = 0; i < traj_scaled.joint_trajectory.points.size(); i++)
        { 
            plan_output_c << "Positions\n"; 
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[0] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[1] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[2] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[3] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[4] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[5] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].positions[6] << "\n";

            plan_output_c << "Velocities\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[0] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[1] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[2] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[3] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[4] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[5] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].velocities[6] << "\n";

            plan_output_c << "Accelerations\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[0] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[1] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[2] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[3] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[4] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[5] << "\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].accelerations[6] << "\n";

            plan_output_c << "Time_From_Start\n";
            plan_output_c << traj_scaled.joint_trajectory.points[i].time_from_start << "\n";                
        }
        plan_output_c.close();
    }


    return traj_scaled;
}


moveit_msgs::RobotTrajectory TrajFiller(moveit_msgs::RobotTrajectory& trajectory_cartesian, float Hertz, bool save_t)
{
    moveit_msgs::RobotTrajectory final_trajectory;
    trajectory_msgs::JointTrajectoryPoint TrajPoint;
    float a[8];
    float h;
    float T, t;

    float q0,q1,v0,v1,a0,a1;

    int time_step;

    final_trajectory.joint_trajectory.joint_names = trajectory_cartesian.joint_trajectory.joint_names;

    TrajPoint.positions.resize(7);
    TrajPoint.velocities.resize(7);
    TrajPoint.accelerations.resize(7);


    for(int i=0; i<trajectory_cartesian.joint_trajectory.points.size() - 1; i++)
    {

        std :: cout << i << "\n";

        T = trajectory_cartesian.joint_trajectory.points[i+1].time_from_start.toSec() - trajectory_cartesian.joint_trajectory.points[i].time_from_start.toSec();

        time_step = T/(1/Hertz);
        
        for (int k=0; k < time_step; k++)
        {
            t = k*(1/Hertz);
            TrajPoint.time_from_start = trajectory_cartesian.joint_trajectory.points[i].time_from_start + ros::Duration(k*(1/Hertz));

            for(int j=0; j<7; j++)
            {
                q0 = trajectory_cartesian.joint_trajectory.points[i].positions[j];
                q1 = trajectory_cartesian.joint_trajectory.points[i+1].positions[j];
                v0 = trajectory_cartesian.joint_trajectory.points[i].velocities[j];
                v1 = trajectory_cartesian.joint_trajectory.points[i+1].velocities[j];
                a0 = trajectory_cartesian.joint_trajectory.points[i].accelerations[j];
                a1 = trajectory_cartesian.joint_trajectory.points[i+1].accelerations[j];
                h = q1 - q0;
                
                /* POLINOMIO GRADO 5
                a[0] = q0;
                a[1] = v0;
                a[2] = 0.5*a0];
                a[3] = (1/(2*T*T*T)*(20*h - (8*v1+12*v0)*T - (3*a0-a1)*T*T);
                a[4] = (1/(2*T*T*T*T)*(-30*h + (14*v1 + 16*v0)*T + (3*a0-2*a1)*T*T);
                a[5] = (1/(2*T*T*T*T*T)*(12*h - 6(v1+v0)*T+(a1-a0)*T*T);

                TrajPoint.positions(j)      =  a0 + a1*t +a2*t*t + a3*t*t*t + a4*t*t*t*t + a5*t*t*t*t*t;
                TrajPoint.velocities(j)     =  a1 + 2*a2*t + 3*a3*t*t + 4*a4*t*t*t + 5*a5*t*t*t*t;
                TrajPoint.accelerations(j)  =  2*a2 + 6*a3*t +12*a4*t*t + 20*a5*t*t*t;
                */

               a[0] = q0;
               a[1] = v0;
               a[2] = 0.5*a0;
               a[3] = 0;
               a[4] = (210*h  - T*((30*a0-15*a1)*T + 120*v0+ 90*v1 ))/(6*T*T*T*T);
               a[5] = (-168*h + T*((20*a0-14*a1)*T + 90*v0 + 78*v1 ))/(2*T*T*T*T*T);
               a[6] = (420*h  - T*((45*a0-39*a1)*T + 216*v0+ 204*v1))/(6*T*T*T*T*T*T);
               a[7] = (-120*h + T*((12*a0-12*a1)*T + 60*v0 + 60*v1 ))/(6*T*T*T*T*T*T*T);

                TrajPoint.positions[j]      =  a[0] + a[1]*t +a[2]*t*t + a[3]*t*t*t + a[4]*t*t*t*t + a[5]*t*t*t*t*t + a[6]*t*t*t*t*t*t + a[7]*t*t*t*t*t*t*t;
                TrajPoint.velocities[j]     =  a[1] + 2*a[2]*t + 3*a[3]*t*t + 4*a[4]*t*t*t + 5*a[5]*t*t*t*t + 6*a[6]*t*t*t*t*t + 7*a[7]*t*t*t*t*t*t;
                TrajPoint.accelerations[j]  =  2*a[2] + 6*a[3]*t +12*a[4]*t*t + 20*a[5]*t*t*t + 30*a[6]*t*t*t*t + 42*a[7]*t*t*t*t*t;
            }

            final_trajectory.joint_trajectory.points.push_back(TrajPoint);
        }
    }
    final_trajectory.joint_trajectory.points.push_back(trajectory_cartesian.joint_trajectory.points[trajectory_cartesian.joint_trajectory.points.size()-1]);

    /*  CHECK IF TIME IS CORRECT
    ros::Duration Delta_Time;
    for(int i = 1; i<final_trajectory.joint_trajectory.points.size(); i++)
    {
        Delta_Time = final_trajectory.joint_trajectory.points[i].time_from_start - final_trajectory.joint_trajectory.points[i-1].time_from_start;
        std::cout << Delta_Time <<"\n";
    }
    */
    if(save_t)
    {

        std::string Log_Path = "/home/kevin/Filled";
        std::ofstream plan_output_c;
        plan_output_c.open(Log_Path);
        
        for (size_t i = 0; i < final_trajectory.joint_trajectory.points.size(); i++)
        { 
            plan_output_c << "Positions\n"; 
            plan_output_c << final_trajectory.joint_trajectory.points[i].positions[0] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].positions[1] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].positions[2] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].positions[3] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].positions[4] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].positions[5] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].positions[6] << "\n";

            plan_output_c << "Velocities\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].velocities[0] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].velocities[1] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].velocities[2] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].velocities[3] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].velocities[4] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].velocities[5] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].velocities[6] << "\n";

            plan_output_c << "Accelerations\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].accelerations[0] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].accelerations[1] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].accelerations[2] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].accelerations[3] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].accelerations[4] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].accelerations[5] << "\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].accelerations[6] << "\n";

            plan_output_c << "Time_From_Start\n";
            plan_output_c << final_trajectory.joint_trajectory.points[i].time_from_start << "\n";                
        }
        plan_output_c.close();
    }
    

    std::cout << "Initial size : " << trajectory_cartesian.joint_trajectory.points.size() << "\n";
    std::cout << "final size   : " << final_trajectory.joint_trajectory.points.size() << "\n";

    return final_trajectory;
}


moveit_msgs::RobotTrajectory FactorScaling(moveit_msgs::RobotTrajectory trajectory_cartesian, float scaling_factor, ros::Duration T_offset)
{
    
    trajectory_msgs::JointTrajectoryPoint Traj_Point;

    ROS_INFO("Rescale of the velocity");
    for (size_t i=0; i<trajectory_cartesian.joint_trajectory.points.size(); i++)
    {
        for(int k=0; k<7; k++)
        {
            trajectory_cartesian.joint_trajectory.points[i].velocities[k] /= scaling_factor;
            trajectory_cartesian.joint_trajectory.points[i].accelerations[k] /= (scaling_factor*scaling_factor);
        }
        trajectory_cartesian.joint_trajectory.points[i].time_from_start *= scaling_factor;
    }

    ROS_INFO("Creation of the first point");
    Traj_Point.time_from_start = ros::Duration(0);
    Traj_Point.positions.resize(7);
    Traj_Point.velocities.resize(7);
    Traj_Point.accelerations.resize(7);

    for(int i=0; i<7; i++)
    {
        Traj_Point.positions[i] = trajectory_cartesian.joint_trajectory.points[0].positions[i];
        Traj_Point.velocities[i] = 0;
        Traj_Point.accelerations[i] = 0;
    }

    for (size_t i=0; i<trajectory_cartesian.joint_trajectory.points.size(); i++)
    {
        trajectory_cartesian.joint_trajectory.points[i].time_from_start += T_offset;
    }

    ROS_INFO("Check for the time");
    for(size_t i=1; i<trajectory_cartesian.joint_trajectory.points.size(); i++)
    {
        if(trajectory_cartesian.joint_trajectory.points[i].time_from_start <= trajectory_cartesian.joint_trajectory.points[i-1].time_from_start)
        {
            trajectory_cartesian.joint_trajectory.points[i].time_from_start = trajectory_cartesian.joint_trajectory.points[i-1].time_from_start + ros::Duration(0.00001);
        }
    }

    trajectory_cartesian.joint_trajectory.points.insert(trajectory_cartesian.joint_trajectory.points.begin(), Traj_Point);
    trajectory_cartesian.joint_trajectory.points[trajectory_cartesian.joint_trajectory.points.size()-1].time_from_start += T_offset;

    return trajectory_cartesian;
}