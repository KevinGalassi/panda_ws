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


#define VERBOSE 0           // Display the differents steps for the realization of the trajectory path
#define CARTESIAN_PATH 1    // Planning of the trajectory using .ComputeCartesianPath
#define IK_INVERSION 0      // Planning using .SetFromIJ 
#define MOVABLE 0           // Use of .SetPoseTarget, and creation of a vector of Plan
#define SET_POSE 0          // Use of .SetPoseTarget and creation of a single Plan to execute
#define SAVE_LOG 0          // Use to save data of the waypoints and the planned joint trajectory
#define PLANNER_TEST 0      // Just use it to sequentially check the other planner ID
#define INPUT_RPY 1         // 1: to read the data in RPY format
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

    addCollisionObjects(planning_scene_interface);
    
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


    std::string path1 = "/home/kevin/panda_ws/src/traj_gen/src/PointList/Punti_RPY";


    TrajectoryVector waypoints;
/**************** READ OF THE POSE FROM FILE ******************/
   
    ReadFileTxt(INPUT_RPY, waypoints.point, waypoints.pt_label, path1);
    
    if(VERBOSE)
    {
        for (std::size_t i = 0; i < waypoints.point.size(); ++i)
            visual_tools.publishAxisLabeled(waypoints.point[i], waypoints.pt_label[i], rvt::XXSMALL);
        visual_tools.trigger();
        visual_tools.prompt("reading completed, closing the file");
    }

/***************  SYSTEM PARAMETERS **************************/
    
    TrajectoryPlanner_param param;

    param.radius = 0.02;               // Radius of the semi-circle for fixing part
    param.heigh = 0.03;                // Heigh of upward movement
    param.circ_point = 5;                 // Point generated in the semi-circle
    param.res = 0.01;                  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;    // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;            // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;            // Distance of the second point
    param.corner_points = 20;             // Numver of point generated in the corner.




/**************** COMPUTE TRAJECTORY PARTS      ******************/
    MyTrajectoryPlanner(param, waypoints);

    std::vector<geometry_msgs::Pose> waypoints_final;
    std::vector<geometry_msgs::Quaternion> constraints_vector;

    for(size_t i=0; i< waypoints.SecondaryTrajectory.size(); i++)
    {
        for (size_t k=0; k< waypoints.SecondaryTrajectory[i].size(); k++)
        {
            waypoints_final.push_back(waypoints.SecondaryTrajectory[i][k]);
            constraints_vector.push_back(waypoints.SecondaryTrajectory[i][k].orientation);
            visual_tools.publishAxisLabeled(waypoints.SecondaryTrajectory[i][k], "pt" , rvt::XXXSMALL);
        }
    }
    visual_tools.trigger();
    visual_tools.prompt("JHSAJFF");
/**************** CONVERSION FROM EE TO LINK 8  ******************/

    visual_tools.deleteAllMarkers();

    FromEE2Link8(waypoints_final);
    for(size_t i=0; i<waypoints.SecondaryTrajectory.size(); i++)
    {
        FromEE2Link8(waypoints.SecondaryTrajectory[i]);
    }


    visual_tools.publishPath(waypoints_final, rvt::LIME_GREEN, rvt::XXXSMALL);

    for (std::size_t i=0; i< waypoints_final.size(); i++)
        visual_tools.publishAxisLabeled(waypoints_final[i], "pt" , rvt::XXXSMALL);

    visual_tools.trigger();
    visual_tools.prompt("continua");
/**************** MOTION                        ******************/
  
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

    Ws_JS_Coordinates Points_to_elaborate;

    if(CARTESIAN_PATH)
    {

        moveit_msgs::RobotTrajectory trajectory_cartesian, trajectory_cartesian1, trajectory_cartesian2, trajectory_cartesian3;
        const double jump_threshold = 0.0;
        const double eef_step = 0.005;
        double fraction;   
        moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian;
        std::vector<geometry_msgs::Pose> failed_points;

        std::vector<moveit_msgs::RobotTrajectory> trajectory_vector;

        int pt_out_of_trajectory = 0;
        std::vector<geometry_msgs::Pose> outer_points;

        visual_tools.deleteAllMarkers();
        
        for (std::size_t i=0; i< waypoints.point.size(); i++)
            visual_tools.publishAxisLabeled(waypoints.point[i], waypoints.pt_label[i] , rvt::XXXSMALL);


        visual_tools.publishPath(waypoints_final, rvt::LIME_GREEN, rvt::XXXSMALL);
        visual_tools.trigger();
        visual_tools.prompt("Execute Plan");

        uint i=10;


        moveit_msgs::Constraints path_constraint;

        path_constraint.position_constraints.resize(1);
        path_constraint.position_constraints[0].link_name = "panda_hand";
        // path_constraint.position_constraints[0].header.frame_id = "panda_link0";
        path_constraint.position_constraints[0].target_point_offset.z = 0.01;
        path_constraint.position_constraints[0].target_point_offset.x = 0.01;
        path_constraint.position_constraints[0].target_point_offset.y = 0;
        
        path_constraint.orientation_constraints.resize(1);
        path_constraint.orientation_constraints[0].link_name = "panda_hand";
       // path_constraint.orientation_constraints[0].header.frame_id = "panda_link8";
        path_constraint.orientation_constraints[0].orientation = waypoints_final[0].orientation;
        path_constraint.orientation_constraints[0].absolute_x_axis_tolerance = 0;
        path_constraint.orientation_constraints[0].absolute_y_axis_tolerance = 0;
        path_constraint.orientation_constraints[0].absolute_z_axis_tolerance = 2*M_PI;
        
        move_group.computeCartesianPath(waypoints_final, eef_step, jump_threshold, trajectory_cartesian, path_constraint, true);
                Plan_Cartesian.trajectory_ = trajectory_cartesian;
                move_group.execute(Plan_Cartesian);
                move_group.setNumPlanningAttempts(i);
                move_group.setJointValueTarget(arm_ready_state);
                move_group.move();

        visual_tools.prompt("jdfjef");

        while(ros::ok)
        {
            move_group.setNumPlanningAttempts(PLANNING_ATTEMPTS_NO);
            move_group.computeCartesianPath(waypoints.SecondaryTrajectory[12], eef_step, jump_threshold, trajectory_cartesian, path_constraint, true);
            fraction = move_group.computeCartesianPath(waypoints.SecondaryTrajectory[12], eef_step, jump_threshold, trajectory_cartesian);    
            if (fraction = 1)
            {
                Plan_Cartesian.trajectory_ = trajectory_cartesian;
                move_group.execute(Plan_Cartesian);
                move_group.setNumPlanningAttempts(i);
                move_group.setJointValueTarget(arm_ready_state);
                move_group.move();
            }
        }

        for (size_t i=0; i<waypoints.SecondaryTrajectory.size(); i++ ){
            move_group.computeCartesianPath(waypoints.SecondaryTrajectory[i], eef_step, jump_threshold, trajectory_cartesian);
            Plan_Cartesian.trajectory_ = trajectory_cartesian;
            move_group.execute(Plan_Cartesian);
            move_group.setJointValueTarget(arm_ready_state);
            move_group.move();
        }
 
        


        visual_tools.prompt(" press next to execute an normal speed \n");
        move_group.execute(Plan_Cartesian);
        move_group.setJointValueTarget(arm_ready_state);
        move_group.move();

        std::cout << waypoints_final.size() << "\n"; ;
        std::cout << trajectory_cartesian.joint_trajectory.points.size() << "\n";






        visual_tools.prompt("Rescaling velocity \n");

        std::vector<std::vector<trajectory_msgs::JointTrajectoryPoint>> Plan_vector;
        std::vector<std::vector<geometry_msgs::Twist>> EE_vel_vector;
        std::vector <std::vector  <geometry_msgs::Accel>> EE_acc_vector;
        moveit::planning_interface::MoveGroupInterface::Plan new_plan;
        double velocity = 0.15;
        float joint_vector_flag[7];

        PlanCartesianMotion(joint_model_group, waypoints_final, Plan_Cartesian, move_group, Points_to_elaborate, failed_points, Plan_vector);   

        if(failed_points.size() > 0)
        {
            for (std::size_t i = 0; i <failed_points.size(); i++)
                visual_tools.publishAxisLabeled(failed_points[i], "failed pt", rvt::SMALL);
            visual_tools.trigger();   
            visual_tools.prompt("failed points");
        }

        Points_to_elaborate.ws_velocity.resize(Points_to_elaborate.ws_point.size());
        Points_to_elaborate.ws_acceleration.resize(Points_to_elaborate.ws_point.size());


        new_plan.trajectory_.joint_trajectory.joint_names.resize(7);
        for(size_t i=0; i<7; i++)
        {
            new_plan.trajectory_.joint_trajectory.joint_names[i] = Plan_Cartesian.trajectory_.joint_trajectory.joint_names[i];
        }

        EE_vel_vector.resize(Plan_vector.size());
        for (size_t i=0; i<Plan_vector.size(); i++)
            EE_vel_vector[i].resize(Plan_vector[i].size());
        
        EE_acc_vector.resize(Plan_vector.size());
        for (size_t i=0; i<Plan_vector.size(); i++)
            EE_acc_vector[i].resize(Plan_vector[i].size());

        computeEE_Vel_Acc2(Points_to_elaborate.ws_point, velocity, param.res, Plan_vector, EE_vel_vector, EE_acc_vector);

        Points_to_elaborate.jointspace_points.clear();
        Points_to_elaborate.ws_acceleration.clear();
        Points_to_elaborate.ws_velocity.clear();

        for(size_t i=0; i< Plan_vector.size(); i++)
        {
            for(size_t k=0; k<Plan_vector[i].size(); k++)
            {
                Points_to_elaborate.jointspace_points.push_back(Plan_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_vel_vector.size(); i++)
        {
            for(size_t k=0; k<EE_vel_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_velocity.push_back(EE_vel_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_acc_vector.size(); i++)
        {
            for(size_t k=0; k<EE_acc_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_acceleration.push_back(EE_acc_vector[i][k]);
            }
        }

        setJointSpaceVel_Acc(Points_to_elaborate.jointspace_points, Points_to_elaborate.ws_velocity, Points_to_elaborate.ws_acceleration);

        for(size_t i=0; i<Points_to_elaborate.jointspace_points.size(); i++)
        {
            new_plan.trajectory_.joint_trajectory.points.push_back(Points_to_elaborate.jointspace_points[i]);
        }

        move_group.setJointValueTarget(new_plan.trajectory_.joint_trajectory.points[0].positions);
        move_group.move();
        std::cout << "Esecution with velocity = " << velocity << "\n";
        move_group.execute(new_plan);

        new_plan.trajectory_.joint_trajectory.points.clear();
        Plan_vector.clear();
        Points_to_elaborate.jointspace_points.clear();
        Points_to_elaborate.ws_acceleration.clear();
        Points_to_elaborate.ws_velocity.clear();
        Points_to_elaborate.ws_point.clear();


        velocity = 0.1;

        PlanCartesianMotion(joint_model_group, waypoints_final, Plan_Cartesian, move_group, Points_to_elaborate, failed_points, Plan_vector);   

        if(failed_points.size() > 0)
        {
            for (std::size_t i = 0; i <failed_points.size(); i++)
                visual_tools.publishAxisLabeled(failed_points[i], "failed pt", rvt::SMALL);
            visual_tools.trigger();   
            visual_tools.prompt("failed points");
        }

        Points_to_elaborate.ws_velocity.resize(Points_to_elaborate.ws_point.size());
        Points_to_elaborate.ws_acceleration.resize(Points_to_elaborate.ws_point.size());

        new_plan.trajectory_.joint_trajectory.joint_names.resize(7);
        for(size_t i=0; i<7; i++)
        {
            new_plan.trajectory_.joint_trajectory.joint_names[i] = Plan_Cartesian.trajectory_.joint_trajectory.joint_names[i];
        }


        EE_vel_vector.resize(Plan_vector.size());
        for (size_t i=0; i<Plan_vector.size(); i++)
            EE_vel_vector[i].resize(Plan_vector[i].size());
        
        EE_acc_vector.resize(Plan_vector.size());
        for (size_t i=0; i<Plan_vector.size(); i++)
            EE_acc_vector[i].resize(Plan_vector[i].size());

        computeEE_Vel_Acc2(Points_to_elaborate.ws_point, velocity, param.res, Plan_vector, EE_vel_vector, EE_acc_vector);

        Points_to_elaborate.jointspace_points.clear();
        Points_to_elaborate.ws_acceleration.clear();
        Points_to_elaborate.ws_velocity.clear();

        for(size_t i=0; i< Plan_vector.size(); i++)
        {
            for(size_t k=0; k<Plan_vector[i].size(); k++)
            {
                Points_to_elaborate.jointspace_points.push_back(Plan_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_vel_vector.size(); i++)
        {
            for(size_t k=0; k<EE_vel_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_velocity.push_back(EE_vel_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_acc_vector.size(); i++)
        {
            for(size_t k=0; k<EE_acc_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_acceleration.push_back(EE_acc_vector[i][k]);
            }
        }

        setJointSpaceVel_Acc(Points_to_elaborate.jointspace_points, Points_to_elaborate.ws_velocity, Points_to_elaborate.ws_acceleration);

        for(size_t i=0; i<Points_to_elaborate.jointspace_points.size(); i++)
        {
            new_plan.trajectory_.joint_trajectory.points.push_back(Points_to_elaborate.jointspace_points[i]);
        }

        move_group.setJointValueTarget(new_plan.trajectory_.joint_trajectory.points[0].positions);
        move_group.move();
        std::cout << "Esecution with velocity = " << velocity << "\n";
        move_group.execute(new_plan);

        new_plan.trajectory_.joint_trajectory.points.clear();
        Plan_vector.clear();
        Points_to_elaborate.jointspace_points.clear();
        Points_to_elaborate.ws_acceleration.clear();
        Points_to_elaborate.ws_velocity.clear();
        Points_to_elaborate.ws_point.clear();
        
        velocity = 0.05;

        PlanCartesianMotion(joint_model_group, waypoints_final, Plan_Cartesian, move_group, Points_to_elaborate, failed_points, Plan_vector);   

        if(failed_points.size() > 0)
        {
            for (std::size_t i = 0; i <failed_points.size(); i++)
                visual_tools.publishAxisLabeled(failed_points[i], "failed pt", rvt::SMALL);
            visual_tools.trigger();   
            visual_tools.prompt("failed points");
        }

        Points_to_elaborate.ws_velocity.resize(Points_to_elaborate.ws_point.size());
        Points_to_elaborate.ws_acceleration.resize(Points_to_elaborate.ws_point.size());


        new_plan.trajectory_.joint_trajectory.joint_names.resize(7);
        for(size_t i=0; i<7; i++)
        {
            new_plan.trajectory_.joint_trajectory.joint_names[i] = Plan_Cartesian.trajectory_.joint_trajectory.joint_names[i];
        }


        EE_vel_vector.resize(Plan_vector.size());
        for (size_t i=0; i<Plan_vector.size(); i++)
            EE_vel_vector[i].resize(Plan_vector[i].size());
        
        EE_acc_vector.resize(Plan_vector.size());
        for (size_t i=0; i<Plan_vector.size(); i++)
            EE_acc_vector[i].resize(Plan_vector[i].size());

        computeEE_Vel_Acc2(Points_to_elaborate.ws_point, velocity, param.res, Plan_vector, EE_vel_vector, EE_acc_vector);

        Points_to_elaborate.jointspace_points.clear();
        Points_to_elaborate.ws_acceleration.clear();
        Points_to_elaborate.ws_velocity.clear();

        for(size_t i=0; i< Plan_vector.size(); i++)
        {
            for(size_t k=0; k<Plan_vector[i].size(); k++)
            {
                Points_to_elaborate.jointspace_points.push_back(Plan_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_vel_vector.size(); i++)
        {
            for(size_t k=0; k<EE_vel_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_velocity.push_back(EE_vel_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_acc_vector.size(); i++)
        {
            for(size_t k=0; k<EE_acc_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_acceleration.push_back(EE_acc_vector[i][k]);
            }
        }

        setJointSpaceVel_Acc(Points_to_elaborate.jointspace_points, Points_to_elaborate.ws_velocity, Points_to_elaborate.ws_acceleration);

        for(size_t i=0; i<Points_to_elaborate.jointspace_points.size(); i++)
        {
            new_plan.trajectory_.joint_trajectory.points.push_back(Points_to_elaborate.jointspace_points[i]);
        }

        move_group.setJointValueTarget(new_plan.trajectory_.joint_trajectory.points[0].positions);
        move_group.move();
        std::cout << "Esecution with velocity = " << velocity << "\n";
        move_group.execute(new_plan);

        new_plan.trajectory_.joint_trajectory.points.clear();
        return 0;

/*
        \\ ESECUZIONE "CLASSICA"
        move_group.computeCartesianPath(waypoints_final,eef_step, jump_threshold, trajectory_cartesian, true);
        move_group.setJointValueTarget(trajectory_cartesian.joint_trajectory.points[0].positions);
        move_group.move();

        // visual_tools.prompt("Cartesian Path");


        Plan_Cartesian.trajectory_ = trajectory_cartesian;
        visual_tools.prompt("execute planned");
        move_group.execute(Plan_Cartesian);
*/
/*
        \\ CREAZIONE VETTORE DI CONSTRAINT
        int input_prt;
        moveit::planning_interface::MoveGroupInterface::Plan  Plan_SP;
        


        constraints_vector.clear();
        for(int k = 0; k < Trajectory_list_with_patch[12].size(); k++)
            constraints_vector.push_back(Trajectory_list_with_patch[12][k].orientation);
 */       
    /*
        // PROVAQ VINCOLI!
        moveit_msgs::Constraints paral;
        tf2::Quaternion orient;
        paral.orientation_constraints.resize(1);
        paral.orientation_constraints[0].orientation.w = 0.000000 ;
        paral.orientation_constraints[0].orientation.x = 0.382683 ;
        paral.orientation_constraints[0].orientation.y = 0.92388 ;
        paral.orientation_constraints[0].orientation.z = 0.000000;
        paral.orientation_constraints[0].absolute_z_axis_tolerance = M_PI;
    */
    
    /*
        // PROVA ESECUZIONE CONTINUATA DELLA TTRAIETTORIA 12 PER VEDERE SE È IN GRADO
        while(1)
        {
        // move_group.setPathConstraints(paral);
        move_group.computeCartesianPath(Trajectory_list_with_patch[12], eef_step, jump_threshold, trajectory_cartesian);
        Plan_Cartesian.trajectory_ = trajectory_cartesian;

        move_group.execute(Plan_Cartesian);

        move_group.setJointValueTarget(arm_ready_state);
        move_group.move();
        trajectory_cartesian.joint_trajectory.points.clear();
        Plan_Cartesian.trajectory_.joint_trajectory.points.clear();
        }
    */
    
/*
// PROVA ALTERNANZA CARTESIAN PATH CON SETPOSE TARGET NON FUNZIONA
        visual_tools.prompt("continuo programma");
        for(size_t i=0; i< Trajectory_list_with_patch.size(); i++)
        {
            if(0)
            {
            std::cout << "0 for cartsian path \n";
            // std::cin >> input_prt ;
            if (1)
            {
                std::cout << "execute no:  " << i << "\n";
                move_group.computeCartesianPath(Trajectory_list_with_patch[i], eef_step, jump_threshold, trajectory_cartesian, true);
                trajectory_vector.push_back(trajectory_cartesian);

                Plan_Cartesian.trajectory_ = trajectory_cartesian;
                
                // move_group.setJointValueTarget(trajectory_cartesian.joint_trajectory.points[0].positions);
                // move_group.move();
                
                // move_group.execute(Plan_Cartesian);
            }
            else
            {
                constraints_vector.clear();
                for(int k = 0; k < Trajectory_list_with_patch[i].size(); k++)
                    constraints_vector.push_back(Trajectory_list_with_patch[i][k].orientation);

                PlanSetPoseMotion2(joint_model_group, Trajectory_list_with_patch[i], Plan_SP, move_group, Points_to_elaborate, constraints_vector); 
                move_group.execute(Plan_SP);
            }
            }   
        }
*/
        move_group.setNumPlanningAttempts(200);
        /*
        visual_tools.prompt("Execution Cartesian Path");
        for(int i=0;i< trajectory_vector.size() ; i++)
        {
            Plan_Cartesian.trajectory_ = trajectory_vector[i];
            move_group.setJointValueTarget(trajectory_vector[i].joint_trajectory.points[0].positions);
            move_group.move();

            move_group.execute(Plan_Cartesian);
        }
        */
      //  std::vector<std::vector<trajectory_msgs::JointTrajectoryPoint>> Plan_vector;

        PlanCartesianMotion(joint_model_group, waypoints_final, Plan_Cartesian, move_group, Points_to_elaborate, failed_points, Plan_vector);   

        if(failed_points.size() > 0)
        {
            for (std::size_t i = 0; i <failed_points.size(); i++)
                visual_tools.publishAxisLabeled(failed_points[i], "failed pt", rvt::SMALL);
            visual_tools.trigger();   
            visual_tools.prompt("failed points");
        }

   //     float joint_vector_flag[7];
        
/*
PROVA CALCOLO PUNT FUORI TRAIETTORIA
        for(size_t i=0; i<waypoints_final.size(); i++)
        {
            for (size_t k=0; k<7; k++)
            {
                joint_vector_flag[k] = Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[k];
            }

            if(CheckEEPosition(waypoints_final[i],  joint_vector_flag, 0.01))
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

      std::vector<geometry_msgs::Pose> real_path;
     
        for(size_t i=0; i<Plan_Cartesian.trajectory_.joint_trajectory.points.size(); i++)
        {
            for (size_t k=0; k<7; k++)
            {
                joint_vector_flag[k] = Plan_Cartesian.trajectory_.joint_trajectory.points[i].positions[k];
            }
            
            getRealPath(joint_vector_flag, real_path);
        }

        std::cout << "real_path 1n";


        visual_tools.publishPath(real_path);
                visual_tools.trigger();
        visual_tools.prompt("print trajectory");

        move_group.execute(Plan_Cartesian);
        move_group.setJointValueTarget(arm_ready_state);
        move_group.move();
        

        if (SAVE_LOG)
        {
            SaveLog("Plan_Cartesian", Plan_Cartesian, "Waypoints_Cartesian", waypoints_final);
        }

        Points_to_elaborate.ws_velocity.resize(Points_to_elaborate.ws_point.size());
        Points_to_elaborate.ws_acceleration.resize(Points_to_elaborate.ws_point.size());

       // moveit::planning_interface::MoveGroupInterface::Plan new_plan;
        new_plan.trajectory_.joint_trajectory.joint_names.resize(7);
        for(size_t i=0; i<7; i++)
        {
            new_plan.trajectory_.joint_trajectory.joint_names[i] = Plan_Cartesian.trajectory_.joint_trajectory.joint_names[i];
        }

       // double velocity = 0.1;

       // std::vector<std::vector<geometry_msgs::Twist>> EE_vel_vector;
      //  std::vector <std::vector  <geometry_msgs::Accel>> EE_acc_vector;
        
        EE_vel_vector.resize(Plan_vector.size());
        for (size_t i=0; i<Plan_vector.size(); i++)
            EE_vel_vector[i].resize(Plan_vector[i].size());
        
        EE_acc_vector.resize(Plan_vector.size());
        for (size_t i=0; i<Plan_vector.size(); i++)
            EE_acc_vector[i].resize(Plan_vector[i].size());


        std::cout << "comput vel and acc" << "\n";
        computeEE_Vel_Acc2(Points_to_elaborate.ws_point, velocity, param.res, Plan_vector, EE_vel_vector, EE_acc_vector);
        std::cout << "comput vel and acc done" << "\n";

        Points_to_elaborate.jointspace_points.clear();
        Points_to_elaborate.ws_acceleration.clear();
        Points_to_elaborate.ws_velocity.clear();

        std::cout << "srotolo" << "\n";
        for(size_t i=0; i< Plan_vector.size(); i++)
        {
            for(size_t k=0; k<Plan_vector[i].size(); k++)
            {
                Points_to_elaborate.jointspace_points.push_back(Plan_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_vel_vector.size(); i++)
        {
            for(size_t k=0; k<EE_vel_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_velocity.push_back(EE_vel_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_acc_vector.size(); i++)
        {
            for(size_t k=0; k<EE_acc_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_acceleration.push_back(EE_acc_vector[i][k]);
            }
        }

        std::cout << "joint velocity calcolo" << "\n";
        setJointSpaceVel_Acc(Points_to_elaborate.jointspace_points, Points_to_elaborate.ws_velocity, Points_to_elaborate.ws_acceleration);

        for(size_t i=0; i<Points_to_elaborate.jointspace_points.size(); i++)
        {
            new_plan.trajectory_.joint_trajectory.points.push_back(Points_to_elaborate.jointspace_points[i]);
         //   new_plan.trajectory_.joint_trajectory.points[i].time_from_start = ros::Duration(i * res/velocity);
        }

        move_group.setJointValueTarget(new_plan.trajectory_.joint_trajectory.points[0].positions);
        move_group.move();std::cout << "Esecution with velocity = " << velocity << "\n";
        //visual_tools.prompt("Execution with v=0.1 final!");
        move_group.execute(new_plan);
        new_plan.trajectory_.joint_trajectory.points.clear();


        velocity = 0.07;
        computeEE_Vel_Acc2(Points_to_elaborate.ws_point, velocity, param.res, Plan_vector, EE_vel_vector, EE_acc_vector);


        Points_to_elaborate.jointspace_points.clear();
        Points_to_elaborate.ws_acceleration.clear();
        Points_to_elaborate.ws_velocity.clear();

        for(size_t i=0; i< Plan_vector.size(); i++)
        {
            for(size_t k=0; k<Plan_vector[i].size(); k++)
            {
                Points_to_elaborate.jointspace_points.push_back(Plan_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_vel_vector.size(); i++)
        {
            for(size_t k=0; k<EE_vel_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_velocity.push_back(EE_vel_vector[i][k]);
            }
        }

        for(size_t i=0; i< EE_acc_vector.size(); i++)
        {
            for(size_t k=0; k<EE_acc_vector[i].size(); k++)
            {
                Points_to_elaborate.ws_acceleration.push_back(EE_acc_vector[i][k]);
            }
        }
        
        setJointSpaceVel_Acc(Points_to_elaborate.jointspace_points, Points_to_elaborate.ws_velocity, Points_to_elaborate.ws_acceleration);

        for(size_t i=0; i<Points_to_elaborate.jointspace_points.size(); i++)
        {
            new_plan.trajectory_.joint_trajectory.points.push_back(Points_to_elaborate.jointspace_points[i]);
//            new_plan.trajectory_.joint_trajectory.points[i].time_from_start = ros::Duration(i * param.res/velocity);
        }

        move_group.setJointValueTarget(new_plan.trajectory_.joint_trajectory.points[0].positions);
        move_group.move();
        std::cout << "Esecution with velocity = " << velocity << "\n";
        //visual_tools.prompt("Execution with v=0.7 final!");
        move_group.execute(new_plan);
        new_plan.trajectory_.joint_trajectory.points.clear();
        /*
        velocity = 0.7;

        computeEE_Vel_Acc(Points_to_elaborate.ws_point, Points_to_elaborate.ws_velocity, Points_to_elaborate.ws_acceleration,  velocity, param.res);
        setJointSpaceVel_Acc(Points_to_elaborate.jointspace_points, Points_to_elaborate.ws_velocity, Points_to_elaborate.ws_acceleration);

        for(size_t i=0; i<Points_to_elaborate.jointspace_points.size(); i++)
        {
            new_plan.trajectory_.joint_trajectory.points.push_back(Points_to_elaborate.jointspace_points[i]);
            new_plan.trajectory_.joint_trajectory.points[i].time_from_start = ros::Duration(i * param.res/velocity);
        }

        move_group.setJointValueTarget(new_plan.trajectory_.joint_trajectory.points[0].positions);
        move_group.move();
        visual_tools.prompt("Execution with v=0.7 final!");
        move_group.execute(new_plan);
        new_plan.trajectory_.joint_trajectory.points.clear();
        */

    }

    if(MOVABLE)
    {     
        std::vector<double> joints;
        std::vector<double[7] > joints_list;
        joints.resize(7);
        /*
        visual_tools.prompt("Grasp sequence");
	    for (size_t i=0; i< grasp_pose.size(); i++)
        {
            move_group.setPoseTarget(grasp_pose[i]);
            success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            if (success)
            {
                std::cout << "Esecuzione punto " << i << "\n";
                move_group.execute(my_plan);
            }
            else
            {
                std::cout << "esecuzione fallita" << i << "\n";
            }

            if(i == 6)
            {
                std::cout << "closing finger";
                ros::Duration(1).sleep();
                hand_group.setJointValueTarget(hand_closed_position);
                hand_group.move();
            }
        }
        */

        std::vector<moveit::planning_interface::MoveGroupInterface::Plan> Plan_list;
        visual_tools.prompt("Esecuzione con setPoseTarget");

        for (size_t i = 0; i < waypoints_final.size(); i++)
        {
            
            move_group.setPoseTarget(waypoints_final[i]);
            success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            if (success)
            {
                std::cout << "Pose " << i << "planned \n" ;
                Plan_list.push_back(my_plan);

                size_plan = my_plan.trajectory_.joint_trajectory.points.size();
                for (int j=0; j<7; j++)
                {
                    std::cout << j << "\n";
                    joints[j] = my_plan.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
                }
                start_state.setJointGroupPositions(joint_model_group, joints);           
                move_group.setStartState(start_state);
            }
            else
            {
                std::cout << "Plan fallito" << i << "\n";
            }
        }
        
        std::ofstream plan_output;
        plan_output.open("/home/kevin/panda_ws/src/traj_gen/src/Plan");
        for (size_t i=0; i < Plan_list.size(); i++)
        { 
            plan_output << "Plan: " << i << "\n";
            size_plan = Plan_list[i].trajectory_.joint_trajectory.points.size(); 
            for (int j=0; j<size_plan; j++)
            {
                plan_output << Plan_list[i].trajectory_.joint_trajectory.points[j];
            }
        }

        for (size_t i=0; i<Plan_list.size(); i++)
        {
            move_group.execute(Plan_list[i]);
        }
        move_group.setJointValueTarget(arm_ready_state);
        move_group.move();
    }      

    if(SET_POSE)
    {   
        moveit::planning_interface::MoveGroupInterface::Plan  Plan_SP;
 
        visual_tools.prompt("Esecuzione con setPoseTarget");
        PlanSetPoseMotion(joint_model_group, waypoints_final, Plan_SP, move_group, Points_to_elaborate, constraints_vector);
        // PlanSetPoseMotion2(joint_model_group, waypoints_final, Plan_SP, move_group, Points_to_elaborate, constraints_vector);
        visual_tools.prompt("Motion with no velocity resized");

        if (SAVE_LOG) 
            SaveLog("Plan_SP", Plan_SP, "Waypoints_SP", waypoints_final);
    
        std::cout << Points_to_elaborate.ws_point.size() << "\n";
        std::cout << Points_to_elaborate.jointspace_points.size() << "\n";
        
        Points_to_elaborate.ws_velocity.resize(Points_to_elaborate.ws_point.size());
        Points_to_elaborate.ws_acceleration.resize(Points_to_elaborate.ws_point.size());

        moveit::planning_interface::MoveGroupInterface::Plan new_plan;
        new_plan.trajectory_.joint_trajectory.joint_names.resize(7);
        for(size_t i=0; i<7; i++)
        {
            new_plan.trajectory_.joint_trajectory.joint_names[i] = Plan_SP.trajectory_.joint_trajectory.joint_names[i];
        }

        double velocity = 0.2;
        computeEE_Vel_Acc(Points_to_elaborate.ws_point, Points_to_elaborate.ws_velocity, Points_to_elaborate.ws_acceleration,  velocity, param.res);
        setJointSpaceVel_Acc(Points_to_elaborate.jointspace_points, Points_to_elaborate.ws_velocity, Points_to_elaborate.ws_acceleration);

        for(size_t i=0; i<Points_to_elaborate.jointspace_points.size(); i++)
        {
            new_plan.trajectory_.joint_trajectory.points.push_back(Points_to_elaborate.jointspace_points[i]);
            new_plan.trajectory_.joint_trajectory.points[i].time_from_start = ros::Duration(i * param.res/velocity);
        }

        // SaveLog("test_final_plan", new_plan, "test_waypoints", Points_to_elaborate.ws_point);

        visual_tools.prompt("Planned motion with velocity computed by MoveiT");
        move_group.execute(Plan_SP);        
        move_group.setJointValueTarget(arm_ready_state);
        move_group.move();

        visual_tools.prompt("Scaled velocity, with path Velocity = 0.2 m/s");
        move_group.setJointValueTarget(new_plan.trajectory_.joint_trajectory.points[0].positions);
        move_group.move();
        move_group.execute(new_plan); 


        /*
        std::cout << "Grasp sequence";
	    for(size_t i=0; i< grasp_pose.size(); i++)
        {
            move_group.setPoseTarget(grasp_pose[i]);
            success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            if (success)
            {
                std::cout << "Esecuzione punto " << i << "\n";
                move_group.execute(my_plan);
            }
            else
            {
                std::cout << "esecuzione fallita" << i << "\n";
            }

            if(i == distance_approach/param.res)
            {
                std::cout << "closing finger";
                ros::Duration(1).sleep();
                hand_group.setJointValueTarget(hand_closed_position);
                hand_group.move();
            }
        }
        */
    }

    if(IK_INVERSION)
    {
        moveit::planning_interface::MoveGroupInterface::Plan Plan_IK;
        move_group.setPlanningTime(5);

       PlanIKMotion(joint_model_group, kinematic_state, waypoints_final, Plan_IK, move_group, Points_to_elaborate, constraints_vector);

        if (SAVE_LOG)
            SaveLog("PLAN_IK", Plan_IK, "WP_IK", waypoints_final);

        visual_tools.prompt("Execution of final plan");
        move_group.execute(Plan_IK);
        move_group.setJointValueTarget(arm_ready_state);
        move_group.move();
    }

    if(CART_SET_POSE)
    {
        visual_tools.prompt("Cartesian Path");
        moveit_msgs::RobotTrajectory trajectory_CSP;
        const double jump_threshold = 0;
        const double eef_step = 0.1;
        double fraction;   
        std::vector<double[7] > joints_list_CSP;
        double joints_CSP[7];
        std::vector<geometry_msgs::Pose> waypoints_border_CSP; // Used to copy the two border pose for the CSP ppath computation.
        waypoints_border_CSP.resize(2);

        robot_state::RobotState start_state(*move_group.getCurrentState());


        T_offset = ros::Duration(0);
        moveit::planning_interface::MoveGroupInterface::Plan Plan_CSP;

        std::vector<trajectory_msgs::JointTrajectoryPoint> Joints_points;

        
        int fail = 0;

        move_group.setPoseTarget(waypoints_final[0]);
        move_group.plan(Plan_CSP);

        size_plan = Plan_CSP.trajectory_.joint_trajectory.points.size();
        T_offset = Plan_CSP.trajectory_.joint_trajectory.points[size_plan-1].time_from_start;
        
        Joints_points.push_back(Plan_CSP.trajectory_.joint_trajectory.points[size_plan-1]);

        for (int j=0; j<7; j++)
        {
            joints_CSP[j] = Plan_CSP.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
        }

        std::cout <<"Set start State \n";
        start_state.setJointGroupPositions(joint_model_group, joints_CSP);           
        move_group.setStartState(start_state);    


        for (size_t i = 1; i < waypoints_final.size(); i++)
        {
            waypoints_border_CSP[0] = waypoints_final[i-1];
            waypoints_border_CSP[1] = waypoints_final[i];
            fraction = move_group.computeCartesianPath(waypoints_border_CSP, eef_step, jump_threshold, trajectory_CSP);

            std::cout << fraction << "\n";
            if (fraction > 0.55)
            {
                std::cout << "plan " << i << " succesfull \n";

                size_plan = trajectory_CSP.joint_trajectory.points.size();
                for (size_t k = 0; k < size_plan; k++)
                {
                    trajectory_CSP.joint_trajectory.points[k].time_from_start += T_offset; 
                    Plan_CSP.trajectory_.joint_trajectory.points.push_back(trajectory_CSP.joint_trajectory.points[k]);
                    // visual_tools.prompt("jjj");
                }

                Joints_points.push_back(Plan_CSP.trajectory_.joint_trajectory.points[size_plan-1]);

                for (int j=0; j<7; j++)
                {
                    joints_CSP[j] = trajectory_CSP.joint_trajectory.points[size_plan-1].positions[j];
                }
                start_state.setJointGroupPositions(joint_model_group, joints_CSP);           
                move_group.setStartState(start_state);

                size_plan = Plan_CSP.trajectory_.joint_trajectory.points.size();
                T_offset = Plan_CSP.trajectory_.joint_trajectory.points[size_plan-1].time_from_start;

            }
            else
            {
                std::cout << "Cartesian planner fail for point " << i <<" \n";
                std::cout << "Trying SetPoseTarget instead \n";

                size_plan = Plan_CSP.trajectory_.joint_trajectory.points.size();


                for (int j=0; j<7; j++)
                {
                    joints_CSP[j] = Plan_CSP.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
                }

                start_state.setJointGroupPositions(joint_model_group, joints_CSP);   
                

                move_group.setStartState(start_state);
                move_group.setPoseTarget(waypoints_border_CSP[1]);
                std::cout << "Start/Target Pose setted \n"; 

                success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
                if(success)
                {
                    std::cout << "Plan Success \n";

                    final_plan_size = Plan_CSP.trajectory_.joint_trajectory.points.size();
                    T_offset = Plan_CSP.trajectory_.joint_trajectory.points[final_plan_size-1].time_from_start;

                    size_plan =  my_plan.trajectory_.joint_trajectory.points.size();
                    for (size_t k = 0; k < size_plan; k++)
                    {
                        my_plan.trajectory_.joint_trajectory.points[k].time_from_start += T_offset; 
                        Plan_CSP.trajectory_.joint_trajectory.points.push_back(my_plan.trajectory_.joint_trajectory.points[k]);
                    }

                    final_plan_size = Plan_CSP.trajectory_.joint_trajectory.points.size();
                    for (int j=0; j<7; j++)
                    {
                        joints_CSP[j] = Plan_CSP.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
                    }

                    start_state.setJointGroupPositions(joint_model_group, joints_CSP);           
                    move_group.setStartState(start_state);

                    size_plan = Plan_CSP.trajectory_.joint_trajectory.points.size();
                    T_offset = Plan_CSP.trajectory_.joint_trajectory.points[size_plan-1].time_from_start;

                }
                else
                {
                    std::cout << "Plan failed with pose target" << i << "\n";
                    fail ++;
                }

            }
        }

        std::cout<< fail << " failed points out of " << waypoints_final.size() << "\n";
        
        move_group.execute(Plan_CSP);
        move_group.setJointValueTarget(arm_ready_state);
        move_group.move();

        if (SAVE_LOG)
            SaveLog("Plan_CSP", Plan_CSP, "WP_CSP", waypoints_final);

    }

    if(NEW_METHOD)
    {
        moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian;

        moveit::planning_interface::MoveGroupInterface::Plan Assembled_plan, my_plan_buffer;
        std::vector<geometry_msgs::Pose> failed_points;
        std::vector<int> failed_point_index;

        double joints_SP[7];

        int size_plan;
        int fail = 0;
        ros::Duration T_offset;
        robot_state::RobotState start_state(*move_group.getCurrentState());
        moveit_msgs::RobotTrajectory trajectory_cartesian;
        std::vector<moveit_msgs::RobotTrajectory> Trajectory_vector;
        const double jump_threshold = 0.0;
        const double eef_step = 0.5;
        double fraction;   

        std::vector<double[7] > joints_list_Cartesian;
        double joints_Cartesian[7];
        std::vector<geometry_msgs::Pose> waypoints_border; 
        waypoints_border.resize(2);

        T_offset = ros::Duration(0);
        std::vector<trajectory_msgs::JointTrajectoryPoint> Joints_points;

        moveit::planning_interface::MoveGroupInterface::Plan  Plan_SP_NEW;

        move_group.clearPathConstraints();

        /*
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
        */

        move_group.setPoseTarget(waypoints_final[0]);
        move_group.plan(Plan_Cartesian);

        size_plan = Plan_Cartesian.trajectory_.joint_trajectory.points.size();
        Joints_points.push_back(Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1]);

        for (int j=0; j<7; j++)
        {
            joints_Cartesian[j] = Plan_Cartesian.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
        }

        start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);       
        move_group.setStartState(start_state);    
        std::cout <<"Set start State \n";

        for (size_t i = 1; i < waypoints_final.size(); i++)
        {
            waypoints_border[0] = waypoints_final[i-1];
            waypoints_border[1] = waypoints_final[i];

            std::cout << waypoints_border[1].position.x - waypoints_final[i].position.x << "\n";

            fraction = move_group.computeCartesianPath(waypoints_border, eef_step, jump_threshold, trajectory_cartesian);
            if (fraction > 0.20)
            {
                std::cout << "plan " << i << " succesfull \n";
                size_plan = trajectory_cartesian.joint_trajectory.points.size();

                Trajectory_vector.push_back(trajectory_cartesian);

                for (int j=0; j<7; j++)
                {
                    joints_Cartesian[j] = trajectory_cartesian.joint_trajectory.points[size_plan-1].positions[j];
                }
                start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);       
                move_group.setStartState(start_state);
                
                // std::cout << waypoints_border[i] << "\n";

            }
            else
            {
                std::cout << "plan " << i << " failed \n";

                std::cout << waypoints_border[1].position.x - waypoints_final[i].position.x << "\n";
                std::cout << waypoints_border[1].position.y - waypoints_final[i].position.y << "\n";
                std::cout << waypoints_border[1].position.z - waypoints_final[i].position.z << "\n";
                move_group.setStartState(start_state);
                move_group.setPoseTarget(waypoints_border[i]);
                /*
                test_constraints.orientation_constraints[0].orientation = waypoints_border[i].orientation;
                std::cout << waypoints_border[i].orientation << "\n";
                ocm.absolute_x_axis_tolerance = (waypoints_border[i].orientation.x - waypoints_border[i-1].orientation.x) + 0.3*3.14/180;
                ocm.absolute_y_axis_tolerance = (waypoints_border[i].orientation.y - waypoints_border[i-1].orientation.y) + 0.3*3.14/180;
                ocm.absolute_z_axis_tolerance = (waypoints_border[i].orientation.z - waypoints_border[i-1].orientation.z) + 0.3*3.14/180;
            
                move_group.setPathConstraints(test_constraints);   
                */
                success = (move_group.plan(my_plan_buffer) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
                
                if (success)
                {
                    std::cout << "Plan Success \n";
                    size_plan = my_plan_buffer.trajectory_.joint_trajectory.points.size();

                    for (int j=0; j<7; j++)
                    {
                        joints_SP[j] = my_plan_buffer.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
                    }

                    Trajectory_vector.push_back(my_plan_buffer.trajectory_);

                    start_state.setJointGroupPositions(joint_model_group, joints_SP);           
                    move_group.setStartState(start_state);
                }
                else
                {
                    std::cout << "Plan " << i << " failed with setpoint \n";
                }
                

                failed_points.push_back(waypoints_final[i-1]);
            }   
        }
    
        move_group.setJointValueTarget(Trajectory_vector[0].joint_trajectory.points[0].positions);
        move_group.move();
    
        ros::Duration(2).sleep();



        std::ofstream waypoint_new_c;
        std::ofstream plan_output_c;
        plan_output_c.open("plan_nuovo.txt");
        for (size_t i = 0; i < Trajectory_vector.size(); i++)
        { 
            for(size_t k = 0; k< Trajectory_vector[i].joint_trajectory.points.size(); k++)
            {
                plan_output_c << Trajectory_vector[i].joint_trajectory.points[k] << "\n";  
            }       
        }
        plan_output_c.close();

        waypoint_new_c.open("waypoints_nuovo");
        for(size_t i =0; i<waypoints_final.size(); i++)
        {
            waypoint_new_c << waypoints_final[i] << "\n";
        }
        waypoint_new_c.close();

        
        for(size_t i=0; i < Trajectory_vector.size(); i++)
        {
            Plan_Cartesian.trajectory_ = Trajectory_vector[i];
            move_group.execute(Plan_Cartesian);
            for(size_t k=0; k< Trajectory_vector[k].joint_trajectory.points.size(); k++)
            {

            }
        }

        for (size_t i=0;i<failed_points.size();i++)
            visual_tools.publishAxisLabeled(failed_points[i], "Pt" + std::to_string(i), rvt::SMALL);
        visual_tools.trigger();
    }

        // Calcolo delle traiettorie che non è stato possibile includer con ComputeCartesianPath
        /*
        for(size_t i = 0; i< failed_points.size()-1; i++)
        {

            size_plan = Trajectory_vector[failed_point_index-1-i].joint_trajectory.points.size();
            for (int j=0; j<7; j++)
            {
                joints_Cartesian[j] = Trajectory_vector[failed_point_index-1-i].joint_trajectory.points[size_plan-1].positions[j];
            }
            start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);
            move_group.setStartState(start_state);

            if (failed_point_index[i+1] = failed_point_index[i]+1)
            {
                // Case in which is missing also the next point
                move_group.setPoseTarget(waypoints.pose[i]);

                test_constraints.orientation_constraints[0].orientation = waypoints.pose[i].orientation;
                ocm.absolute_x_axis_tolerance = (waypoints.pose[i].orientation.x - waypoints.pose[i-1].orientation.x) + 0.3*3.14/180;
                ocm.absolute_y_axis_tolerance = (waypoints.pose[i].orientation.y - waypoints.pose[i-1].orientation.y) + 0.3*3.14/180;
                ocm.absolute_z_axis_tolerance = (waypoints.pose[i].orientation.z - waypoints.pose[i-1].orientation.z) + 0.3*3.14/180;

                move_group.setPathConstraints(test_constraints);   
            }
            else
            {            
                // Case in which the next point is already planned
                size_plan = Trajectory_vector[failed_point_index-i].joint_trajectory.points.size();
                for (int j=0; j<7; j++)
                {
                    joints_Cartesian[j] = Trajectory_vector[failed_point_index-i].joint_trajectory.points[size_plan-1].positions[j];
                }
                start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);
                move_group.setStartState(start_state);
                
                size_plan = Trajectory_vector[failed_point_index].joint_trajectory.points.size();
                for (int j=0; j<7; j++)
                {
                    joints_Cartesian[j] = Trajectory_vector[failed_point_index].joint_trajectory.points[size_plan-1].positions[j];

                }

                move_group.setJointValueTarget(start_state);

                test_constraints.orientation_constraints[0].orientation = waypoints.pose[i].orientation;
                ocm.absolute_x_axis_tolerance = (waypoints.pose[i].orientation.x - waypoints.pose[i-1].orientation.x) + 0.3*3.14/180;
                ocm.absolute_y_axis_tolerance = (waypoints.pose[i].orientation.y - waypoints.pose[i-1].orientation.y) + 0.3*3.14/180;
                ocm.absolute_z_axis_tolerance = (waypoints.pose[i].orientation.z - waypoints.pose[i-1].orientation.z) + 0.3*3.14/180;
                move_group.setPathConstraints(test_constraints);

            }

            success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            if (success)
            {
                std::cout << "Plan Success \n";
                    
                // Funzione per inserire il punto nel posto giusto
            }
            else
            {
               
            }
        }
        
        move_group.setPlanningTime(20);
        move_group.setNumPlanningAttempts(100);

        for (int j=0; j<7; j++)
        {
            joints_SP[j] = Plan_SP_NEW.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
        }

        start_state.setJointGroupPositions(joint_model_group, joints_SP);           
        move_group.setStartState(start_state);

        for (size_t i=1; i < waypoints.pose.size(); i++)
        {

            test_constraints.orientation_constraints[0].orientation = constraints_vector[i];
            move_group.setPathConstraints(test_constraints);

            move_group.setPoseTarget(waypoints.pose[i]);
            success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
            if (success)
            {
                std::cout << "Plan Success \n";
                size_plan = my_plan.trajectory_.joint_trajectory.points.size();
                final_plan_size = Plan_SP_NEW.trajectory_.joint_trajectory.points.size();
                T_offset = Plan_SP_NEW.trajectory_.joint_trajectory.points[final_plan_size-1].time_from_start;
                for (size_t k = 0; k < size_plan; k++)
                {
                    my_plan.trajectory_.joint_trajectory.points[k].time_from_start += T_offset; 
                    Plan_SP_NEW.trajectory_.joint_trajectory.points.push_back(my_plan.trajectory_.joint_trajectory.points[k]);
                }

                for (int j=0; j<7; j++)
                {
                    joints_SP[j] = my_plan.trajectory_.joint_trajectory.points[size_plan-1].positions[j];
                }
                start_state.setJointGroupPositions(joint_model_group, joints_SP);           
                move_group.setStartState(start_state);

                coordinates_to_elaborate.ws_point.push_back(waypoints.pose[i]);
                coordinates_to_elaborate.jointspace_points.push_back(Plan_SP_NEW.trajectory_.joint_trajectory.points.back());

            }
            else
            {
                std::cout << "Plan fallito" << i << "\n";

            }
        }
        */
    

 


   
    ros::shutdown();
    return 0;
}

