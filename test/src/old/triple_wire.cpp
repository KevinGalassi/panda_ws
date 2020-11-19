/****************************************************

The program compute a list of waypoints starting from the reading of a txt file.
    Input file formats: pos.x pos.y pos.z quat.w quat.x quat.y quat.z
By Setting INPUT_RPY to 1 in the #define the orienttion is described by the RPY angle and then converted using tf2 to quaternion.
    Input file formats: pos.x pos.y pos.z RPY.x RPY.y RPY.z

This version compute the Final trajectory combining together the trajectory requested from the label 

The code is divided in:
1) Data reading (create a copy of the readen file)
2) Compute and store the trajectory
3) "Patching", add points to encrease the resolution of the trajectory
4) Conversion from EE to link 8 reference system

The trajectory is created from the usage of push_back, when the label assigned to a certain operation is readen then is 
called a function that adds the list of poses for the execution of the action.

In the "LetFile_V2" the trajectory generation is addressed in another way.

**************************************/
#include "ros/ros.h"

#include "std_msgs/String.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "geometry_msgs/Pose.h"

#include <moveit/move_group_interface/move_group_interface.h>

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_visual_tools/moveit_visual_tools.h>
#include <math.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion

#include <MyFunc.h>


#define VERBOSE 0   // Display the differents steps for the realization of the trajectory path
#define CARTESIAN_PATH 1    // Planning of the trajectory using .ComputeCartesianPath
#define IK_INVERSION 0      // Planning using .SetFromIJ 
#define MOVABLE 0   // Use of .SetPoseTarget, and creation of a vector of Plan
#define SET_POSE 0  // Use of .SetPoseTarget and creation of a single Plan to execute
#define SAVE_LOG 0  // Use to save data of the waypoints and the planned joint trajectory
#define PLANNER_TEST 0      // Just use it to sequentially check the other planner ID
#define INPUT_RPY 1 // 1: to read the data in RPY format
#define CART_SET_POSE 0
#define NEW_METHOD 0
#define PLANNING_ATTEMPTS_NO 500

int main(int argc, char** argv)
{
    ros::init(argc, argv, "PickPlace_Wire");
    ros::NodeHandle node_handle;
    ros::AsyncSpinner spinner(1);
    spinner.start();

// Creation of the moveing group
    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

    
    namespace rvt = rviz_visual_tools;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();


    // visual_tools.loadRemoteControl();
    visual_tools.trigger();

    ROS_INFO("Reach Ready Position");
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();
    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();


    std::string path1 = "/home/kevin/panda_ws/src/traj_gen/src/PointList/Harness1";
    std::string path2 = "/home/kevin/panda_ws/src/traj_gen/src/PointList/Harness2";
    std::string path3 = "/home/kevin/panda_ws/src/traj_gen/src/PointList/Harness3";

    std::string path_test  = "/home/kevin/panda_ws/src/traj_gen/src/PointList/Harness_test";


    TrajectoryVector waypoints1, waypoints2, waypoints3, waypoints_test;
    geometry_msgs::PoseArray grasp_seq1, grasp_seq2, grasp_seq3;

    geometry_msgs::Pose grasp1;
    geometry_msgs::Pose grasp2;
    geometry_msgs::Pose grasp3;
    
/***************  SYSTEM PARAMETERS **************************/
    
    TrajectoryPlanner_param param;

    param.radius = 0.02;       // Radius of the semi-circle for fixing part
    param.heigh = 0.03;// Heigh of upward movement
    param.circ_point = 5; // Point generated in the semi-circle
    param.res = 0.01;  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;    // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;    // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;    // Distance of the second point
    param.corner_points = 20;     // Numver of point generated in the corner.




/**************** COMPUTE TRAJECTORY PARTS      ******************/
    
    ReadFileTxt(INPUT_RPY, waypoints1.point, waypoints1.pt_label, path1, grasp1);
    ReadFileTxt(INPUT_RPY, waypoints2.point, waypoints2.pt_label, path2, grasp2);
    ReadFileTxt(INPUT_RPY, waypoints3.point, waypoints3.pt_label, path3, grasp3);

    ReadFileTxt(INPUT_RPY, waypoints_test.point, waypoints_test.pt_label, path_test, grasp3);


    MyTrajectoryPlanner(param, waypoints1);
    MyTrajectoryPlanner(param, waypoints2);
    MyTrajectoryPlanner(param, waypoints3);

    MyTrajectoryPlanner(param, waypoints_test);



    geometry_msgs::PoseArray waypoints_final1, waypoints_final2, waypoints_final3, waypoints_final;

    geometry_msgs::PoseArray waypoints_final_test;


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
    
    for(size_t i=0; i< waypoints3.SecondaryTrajectory.size(); i++)
        {
        for (size_t k=0; k< waypoints3.SecondaryTrajectory[i].poses.size(); k++)
        {
            waypoints_final3.poses.push_back(waypoints3.SecondaryTrajectory[i].poses[k]);
        }
    }

        for(size_t i=0; i< waypoints_test.SecondaryTrajectory.size(); i++)
        {
        for (size_t k=0; k< waypoints_test.SecondaryTrajectory[i].poses.size(); k++)
        {
            waypoints_final_test.poses.push_back(waypoints_test.SecondaryTrajectory[i].poses[k]);
        }
    }




    /**************** CONVERSION FROM EE TO LINK 8  ******************/

    visual_tools.deleteAllMarkers();



    tf2::Quaternion quat;

     quat.setRPY(M_PI/2, -M_PI/2, 0 );
    // quat.setRPY(M_PI/2, -M_PI/4, M_PI/4);

    grasp1.position.x = 0.2;
    grasp1.position.y = -0.5;
    grasp1.position.z = 0.6;

    grasp2.position.x = 0.25;
    grasp2.position.y = -0.5;
    grasp2.position.z = 0.6;

    grasp3.position.x = 0.3;
    grasp3.position.y = -0.5;
    grasp3.position.z = 0.6;

    grasp1.orientation = tf2::toMsg(quat);
    grasp2.orientation = grasp1.orientation;
    grasp3.orientation = grasp1.orientation;


    visual_tools.deleteAllMarkers();


    FromEE2Link8(waypoints_final1);
    FromEE2Link8(waypoints_final2);
    FromEE2Link8(waypoints_final3);

    FromEE2Link8(waypoints_final_test);


    std::vector<geometry_msgs::Quaternion> constraints_vector;

    /*
    visual_tools.publishAxisLabeled(grasp1, "grasp1", rvt::XXXSMALL);
    visual_tools.publishAxisLabeled(grasp2, "grasp2", rvt::XXXSMALL);   
    visual_tools.publishAxisLabeled(grasp3, "grasp3", rvt::XXXSMALL);

    visual_tools.publishPath(waypoints_final1.poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    visual_tools.publishPath(waypoints_final2.poses, rvt::BLUE, rvt::XXXSMALL);
    visual_tools.publishPath(waypoints_final3.poses, rvt::RED, rvt::XXXSMALL);

    for (std::size_t i=0; i< waypoints_final1.poses.size(); i++)
        visual_tools.publishAxisLabeled(waypoints_final1.poses[i], "pt" , rvt::XXXSMALL);
    for (std::size_t i=0; i< waypoints_final2.poses.size(); i++)
        visual_tools.publishAxisLabeled(waypoints_final2.poses[i], "pt" , rvt::XXXSMALL);
    for (std::size_t i=0; i< waypoints_final3.poses.size(); i++)
        visual_tools.publishAxisLabeled(waypoints_final3.poses[i], "pt" , rvt::XXXSMALL);
    visual_tools.trigger();
    visual_tools.prompt("wait");

    */



    /**************** MOTION******************/
        
    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
    robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("panda_arm");

    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    bool success;
    int size_plan, final_plan_size;
    ros::Duration T_offset;

    robot_state::RobotState start_state(*move_group.getCurrentState());

    move_group.setNumPlanningAttempts(PLANNING_ATTEMPTS_NO); 
    move_group.setPlanningTime(10);

    RescaleVelocityTrajectory Points_to_elaborate;


    moveit_msgs::RobotTrajectory trajectory_cartesian, trajectory_cartesian1, trajectory_cartesian2, trajectory_cartesian3;

    moveit_msgs::RobotTrajectory traj_test, traj_original;

    const double jump_threshold = 0.0;
    const double eef_step = 0.001;
    double fraction;   
    moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian;
    geometry_msgs::PoseArray failed_points;


    int pt_out_of_trajectory = 0;


    visual_tools.deleteAllMarkers();

    /*
    visual_tools.publishAxisLabeled(grasp1, "grasp1", rvt::SMALL);
    visual_tools.publishAxisLabeled(grasp2, "grasp2", rvt::SMALL);   
    visual_tools.publishAxisLabeled(grasp3, "grasp3", rvt::SMALL);

    for (std::size_t i=0; i< waypoints1.point.poses.size(); i++)
        visual_tools.publishAxisLabeled(waypoints1.point.poses[i], waypoints1.pt_label[i] , rvt::XXXSMALL);
    for (std::size_t i=0; i< waypoints2.point.poses.size(); i++)
        visual_tools.publishAxisLabeled(waypoints2.point.poses[i], waypoints2.pt_label[i] , rvt::XXXSMALL);
    for (std::size_t i=0; i< waypoints3.point.poses.size(); i++)
        visual_tools.publishAxisLabeled(waypoints3.point.poses[i], waypoints3.pt_label[i] , rvt::XXXSMALL); 


    visual_tools.publishPath(waypoints_final1.poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    visual_tools.publishPath(waypoints_final2.poses, rvt::BLUE, rvt::XXXSMALL);
    visual_tools.publishPath(waypoints_final3.poses, rvt::RED, rvt::XXXSMALL);
    visual_tools.trigger();

    */

   RescaleVelocityTrajectory coordinates_to_elaborate;
   double velocity_resc = 0.01;
   moveit_msgs::RobotTrajectory RobotTrajectory;


    PlanRescaledCartesianMotion(joint_model_group, waypoints_test.point, move_group, coordinates_to_elaborate, failed_points, velocity_resc, RobotTrajectory, param);

    RobotTrajectory.joint_trajectory.points[RobotTrajectory.joint_trajectory.points.size()-1].time_from_start = RobotTrajectory.joint_trajectory.points[RobotTrajectory.joint_trajectory.points.size()-1].time_from_start;



    std::cout<< "Computedc Path \n";
    move_group.computeCartesianPath(waypoints_final_test.poses, eef_step, jump_threshold, traj_test);

    std::cout << "copy \n";
    traj_original = traj_test;
    std::cout << "rescale \n";
   //  rescalePath(traj_test,waypoints_final_test);

    traj_test.joint_trajectory.points[traj_test.joint_trajectory.points.size()-1].time_from_start = traj_test.joint_trajectory.points[traj_test.joint_trajectory.points.size()-1].time_from_start + ros::Duration(1);
    traj_original.joint_trajectory.points[traj_original.joint_trajectory.points.size()-1].time_from_start = traj_original.joint_trajectory.points[traj_test.joint_trajectory.points.size()-1].time_from_start + ros::Duration(1);

    std::cout << traj_test;





    std::string Log_Path = "/home/kevin/cartesian_log";

    std::ofstream plan_output_c;
    plan_output_c.open(Log_Path);
     
    for (size_t i = 0; i < traj_test.joint_trajectory.points.size(); i++)
    { 
       plan_output_c << "position \n";
        plan_output_c << traj_test.joint_trajectory.points[i].positions[0] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].positions[1] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].positions[2]<< "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].positions[3]<< "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].positions[4]<< "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].positions[5]<< "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].positions[6]<< "\n";
        plan_output_c << "velocity \n";
        plan_output_c << traj_test.joint_trajectory.points[i].velocities[0] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].velocities[1] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].velocities[2] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].velocities[3] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].velocities[4] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].velocities[5] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].velocities[6] << "\n";
        plan_output_c << "acceleration \n";
        plan_output_c << traj_test.joint_trajectory.points[i].accelerations[0] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].accelerations[1] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].accelerations[2] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].accelerations[3] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].accelerations[4] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].accelerations[5] << "\n";
        plan_output_c << traj_test.joint_trajectory.points[i].accelerations[6] << "\n";
        plan_output_c << "time_from_start \n";
        plan_output_c << traj_test.joint_trajectory.points[i].time_from_start << "\n";                
    }
    plan_output_c.close();


    move_group.setJointValueTarget(traj_test.joint_trajectory.points[100].positions);
    move_group.plan(Plan_Cartesian);

    std::string Log_Path2 = "/home/kevin/setJointValue";
    plan_output_c.open(Log_Path2);
     
    for (size_t i = 0; i < Plan_Cartesian.trajectory_.joint_trajectory.points.size(); i++)
    { 
       
        plan_output_c << "position \n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[0] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[1] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[2]<< "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[3]<< "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[4]<< "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[5]<< "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[6]<< "\n";
        plan_output_c << "velocity \n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[0] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[1] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[2] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[3] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[4] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[5] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].velocities[6] << "\n";
        plan_output_c << "acceleration \n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[0] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[1] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[2] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[3] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[4] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[5] << "\n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].accelerations[6] << "\n";
        plan_output_c << "time_from_start \n";
        plan_output_c << Plan_Cartesian.trajectory_.joint_trajectory.points[i].time_from_start << "\n";                
    }

    plan_output_c.close();




    Plan_Cartesian.trajectory_ = traj_original;
    visual_tools.prompt("Exec original");
    move_group.execute(Plan_Cartesian);

    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    Plan_Cartesian.trajectory_ = traj_test;
    visual_tools.prompt("Exec rescaled");
    move_group.execute(Plan_Cartesian);



    geometry_msgs::PoseArray pose_array1;

    for (size_t i= 0; i<waypoints_final1.poses.size(); i++)
    {
        pose_array1.poses.push_back(waypoints_final1.poses[i]);
    }


    move_group.computeCartesianPath(waypoints_final1.poses, eef_step, jump_threshold, trajectory_cartesian1);
    move_group.computeCartesianPath(waypoints_final2.poses, eef_step, jump_threshold, trajectory_cartesian2);
    move_group.computeCartesianPath(waypoints_final3.poses, eef_step, jump_threshold, trajectory_cartesian3);
    Plan_Cartesian.trajectory_ = trajectory_cartesian1; 




    geometry_msgs::PoseArray real_path;
    float joint_vector_flag[7];
    

    /** NON FUNZIONA

    for(size_t i=0; i<Plan_Cartesian.trajectory_.joint_trajectory.points.size(); i++)
    {
        for (size_t k=0; k<7; k++)
        {
            joint_vector_flag[k] = Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[k];
        }
        getRealPath(joint_vector_flag, real_path);
    }
    */

    visual_tools.publishPath(real_path.poses);
        visual_tools.trigger();

    move_group.setNumPlanningAttempts(20);


    visual_tools.prompt(" press next to execute an normal speed \n");


    GraspWire(move_group, hand_group, grasp1, 0.1, 0.01, grasp_seq1);
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    move_group.execute(Plan_Cartesian);
    hand_group.setJointValueTarget(hand_open_position);
    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    GraspWire(move_group, hand_group, grasp2, 0.1, 0.01, grasp_seq2);
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    Plan_Cartesian.trajectory_ = trajectory_cartesian2; 
    move_group.execute(Plan_Cartesian);
    hand_group.setJointValueTarget(hand_open_position);
    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    GraspWire(move_group, hand_group, grasp3, 0.1, 0.01, grasp_seq3);
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    Plan_Cartesian.trajectory_ = trajectory_cartesian3; 
    move_group.execute(Plan_Cartesian);
        hand_group.setJointValueTarget(hand_open_position);
    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();


    std::cout << waypoints_final1.poses.size() << "\n"; ;
    std::cout << trajectory_cartesian1.joint_trajectory.points.size() << "\n";
    std::cout << waypoints_final2.poses.size() << "\n"; ;
    std::cout << trajectory_cartesian2.joint_trajectory.points.size() << "\n";
    std::cout << waypoints_final3.poses.size() << "\n"; ;
    std::cout << trajectory_cartesian3.joint_trajectory.points.size() << "\n";




/*
        for(size_t i=0; i<waypoints_final1.size(); i++)
        {
            for (size_t k=0; k<7; k++)
            {
                joint_vector_flag[k] = Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[k]
            }

            if(CheckEEPosition(waypoints_final1.poses[i],  joint_vector_flag, 0.01))
            {
                std::cout << "point ok \n";
            }
            else
            {
                pt_out_of_trajectory++;
                outer_points.push_back(waypoints_final[i]);
            }
        }
        if(pt_out_of_trajectory > 0)
        {
            for (std::size_t i = 0; i <outer_points.size(); i++)
                visual_tools.publishAxisLabeled(outer_points[i], "failed pt", rvt::SMALL);
            visual_tools.trigger();   
            visual_tools.prompt("fuori traiettoria");
        }    

*/











    visual_tools.prompt("Rescaling velocity \n");


    moveit_msgs::RobotTrajectory ScaledRobotTrajectory;;
    moveit::planning_interface::MoveGroupInterface::Plan ScaledPlan;

    double velocity = 0.15;

    PlanRescaledCartesianMotion(joint_model_group, waypoints_final1, move_group, Points_to_elaborate, failed_points, velocity, ScaledRobotTrajectory, param);
    ScaledPlan.trajectory_ = ScaledRobotTrajectory;
    
    move_group.setJointValueTarget(ScaledRobotTrajectory.joint_trajectory.points[0].positions);
    move_group.move();
    std::cout << "Esecution with velocity = " << velocity << "\n";
    move_group.execute(ScaledPlan);

    ScaledPlan.trajectory_.joint_trajectory.points.clear();
    Points_to_elaborate.jointspace_points.clear();
    Points_to_elaborate.ws_Accel.clear();
    Points_to_elaborate.ws_Twist.clear();
    Points_to_elaborate.ws_Pose.poses.clear();


    velocity = 0.10;


    PlanRescaledCartesianMotion(joint_model_group, waypoints_final1, move_group, Points_to_elaborate, failed_points, velocity, ScaledRobotTrajectory, param);
    ScaledPlan.trajectory_ = ScaledRobotTrajectory;
    
    move_group.setJointValueTarget(ScaledRobotTrajectory.joint_trajectory.points[0].positions);
    move_group.move();
    std::cout << "Esecution with velocity = " << velocity << "\n";
    move_group.execute(ScaledPlan);

    ScaledPlan.trajectory_.joint_trajectory.points.clear();
    Points_to_elaborate.jointspace_points.clear();
    Points_to_elaborate.ws_Accel.clear();
    Points_to_elaborate.ws_Twist.clear();
    Points_to_elaborate.ws_Pose.poses.clear();    
    
    
    velocity = 0.5;
    
    
    PlanRescaledCartesianMotion(joint_model_group, waypoints_final1, move_group, Points_to_elaborate, failed_points, velocity, ScaledRobotTrajectory, param);
    ScaledPlan.trajectory_ = ScaledRobotTrajectory;
    
    move_group.setJointValueTarget(ScaledRobotTrajectory.joint_trajectory.points[0].positions);
    move_group.move();
    std::cout << "Esecution with velocity = " << velocity << "\n";
    move_group.execute(ScaledPlan);

    ScaledPlan.trajectory_.joint_trajectory.points.clear();;
    Points_to_elaborate.jointspace_points.clear();
    Points_to_elaborate.ws_Accel.clear();
    Points_to_elaborate.ws_Twist.clear();
    Points_to_elaborate.ws_Pose.poses.clear();
   
    ros::shutdown();
    return 0;
}
