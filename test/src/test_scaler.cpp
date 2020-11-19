#include "ros/ros.h"

#include "std_msgs/String.h"
#include "std_msgs/Float32.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "geometry_msgs/Pose.h"

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




#define PLANNING_ATTEMPTS_NO 100
#define INPUT_RPY 1

int main(int argc, char** argv)
{
    ros::init(argc, argv, "PickPlace_Wire");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(1);
    spinner.start();

    ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_command",100);
    ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/My_new_input", 100);

    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;

    namespace rvt = rviz_visual_tools;

    float T_O;
    if (not (nh.getParam("/TrajectoryTest/Offset", T_O)))
        T_O = 0.4;
    ros::Duration T_offset = ros::Duration(0.4);
    ros::Duration T_round = ros::Duration(0.4);

    float scaling_factor = 6;
    float scaling_round =  8;
    float scaling_fix = 10;

    float velocity_fix = 0.012;
    float velocity_round = 0.005;
    float velocity_pass = 0.025;

    float mean_velocity;
    


// Creation of the moveing group
    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();

    std::string path1 = "/home/panda/ros/Franka-Kev/src/test/src/PointList/Test";;
    std::string Log_Path = "/home/kevin/Panda_log";
    std::ofstream plan_output_c;

    ROS_INFO("Reach Ready Position");
    move_group.setMaxVelocityScalingFactor(0.2);

    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    
/***************  SYSTEM PARAMETERS **************************/
    
    TrajectoryVector waypoints;
    TrajectoryPlanner_param param;
    param.radius = 0.02;       // Radius of the semi-circle for fixing part
    param.heigh = 0.03;// Heigh of upward movement
    param.circ_point = 100; // Point generated in the semi-circle
    param.res = 0.001;  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;    // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;    // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;    // Distance of the second point
    param.corner_points = 1000;     // Numver of point generated in the corner.

/**************** COMPUTE TRAJECTORY PARTS      ******************/
    
    geometry_msgs::Pose grasp1;
    geometry_msgs::PoseArray waypoints_final;

    ReadFileTxt(INPUT_RPY, waypoints.point, waypoints.pt_label, path1, grasp1);
    MyTrajectoryPlanner3(param, waypoints);
    for(int i=0; i<waypoints.SecondaryTrajectory.size(); i++)
    {
        FromEE2Link8(waypoints.SecondaryTrajectory[i]);
        EE_Shift(waypoints.SecondaryTrajectory[i]);
    }
  
/**** Print result ****/

    visual_tools.deleteAllMarkers();
    for(int k=0; k< waypoints.SecondaryTrajectory.size(); k++)
    {
        visual_tools.publishPath(waypoints.SecondaryTrajectory[k].poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    }
    visual_tools.trigger();


/****************** MOTION INIT *****************************************/
        

    moveit_msgs::RobotTrajectory trajectory_cartesian;
    const double jump_threshold = 0.0;
    const double eef_step = 0.001;
    double fraction;   

    moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian, Plan2;

    bool success;
    int size_plan, final_plan_size;

    move_group.setNumPlanningAttempts(PLANNING_ATTEMPTS_NO); 
    move_group.setPlanningTime(10);
    move_group.setMaxVelocityScalingFactor(0.3);
    visual_tools.prompt("Press 'next' to reach the starting position \n");

/*********************************************************************************************************/
    // Movimento alla posizione di partenza

    moveit::planning_interface::MoveGroupInterface::Plan Plan_SetPoseTarget;
    move_group.setPoseTarget(waypoints.SecondaryTrajectory[0].poses[0]);
    move_group.plan(Plan_SetPoseTarget);
    move_group.execute(Plan_SetPoseTarget);

/********* PLAN OF THE TRAJECTORIES ****************/

    
    std::vector<std::string> LabelVector;
    std::vector<moveit_msgs::RobotTrajectory> TrajVector;
    moveit_msgs::RobotTrajectory Traj_flag;
    std::vector<moveit::planning_interface::MoveGroupInterface::Plan> PlansVector;      // potrebbe non essere usato


    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
    robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("panda_arm");

    robot_state::RobotState start_state(*move_group.getCurrentState());

    std::vector<double[7] > joints_list_Cartesian;
    double joints_Cartesian[7];

    geometry_msgs::PoseArray PoseFlag;
    std::string last_label;
    last_label = " ";

    std::cout << "Second Trajectory size: " << waypoints.SecondaryTrajectory.size() << "\n";
    std::cout << "waypoint label size : " << waypoints.pt_label.size() << "\n";


   // LabelVector.push_back(waypoints.pt_label[0]);


    std::cout << waypoints.SecondaryTrajectory[0].poses.size() << "\n";
    std::cout << "start \n";
   
    for(int k=0; k < waypoints.SecondaryTrajectory[0].poses.size(); k++)
    {
        PoseFlag.poses.push_back(waypoints.SecondaryTrajectory[0].poses[k]);
    }
    last_label = waypoints.pt_label[0];

    std::cout << "start_for \n";
    for(int i=1; i < waypoints.SecondaryTrajectory.size(); i++)
    {
        std::cout << "Point: \n";
        std::cout << i << "\n";
        std::cout << waypoints.pt_label[i] << "\n";

        if(waypoints.pt_label[i] == last_label)
        {
            for(int k=0; k<waypoints.SecondaryTrajectory[i].poses.size(); k++)
            {
                PoseFlag.poses.push_back(waypoints.SecondaryTrajectory[i].poses[k]);
            }
        }
        else
        {
            // Nuovo nome da confrontare


            // Scegliere la velocità per il planner
            if(waypoints.pt_label[i-1] == "pass")
            {
                mean_velocity = velocity_pass;
            }
            if(waypoints.pt_label[i-1] == "round")
            {
                mean_velocity = velocity_round;
            }
            if(waypoints.pt_label[i-1] == "fix")
            {
                mean_velocity = velocity_fix;
            }
            LabelVector.push_back(waypoints.pt_label[i-1]);
            move_group.computeCartesianPath(PoseFlag.poses, eef_step, jump_threshold, trajectory_cartesian);
            if (trajectory_cartesian.joint_trajectory.points.size() == 0)
            {
                ROS_INFO("Skip");
            }
            else
            {   
                if(last_label == "corner")
                {
                    std::cout<< "scrittura punti";
                    Log_Path = "/home/ros/Franka-Kev/src/test/prescale";
                    plan_output_c.open(Log_Path);
                    for (size_t i = 0; i < trajectory_cartesian.joint_trajectory.points.size(); i++)
                    { 
                            plan_output_c << "Positions\n"; 
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[0] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[1] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[2]<< "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[3]<< "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[4]<< "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[5]<< "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[6]<< "\n";

                            plan_output_c << "Velocities\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[0] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[1] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[2] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[3] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[4] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[5] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[6] << "\n";

                            plan_output_c << "Accelerations\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[0] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[1] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[2] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[3] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[4] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[5] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[6] << "\n";

                            plan_output_c << "Time_From_Start\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].time_from_start << "\n";                
                    }
                    plan_output_c.close();

                    Traj_flag = FactorScaling(trajectory_cartesian, 10, T_round);

                    Log_Path = "/home/ros/Franka-Kev/src/test/scaled";
                    plan_output_c.open(Log_Path);
                    for (size_t i = 0; i < trajectory_cartesian.joint_trajectory.points.size(); i++)
                    { 
                            plan_output_c << "Positions\n"; 
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[0] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[1] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[2]<< "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[3]<< "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[4]<< "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[5]<< "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].positions[6]<< "\n";

                            plan_output_c << "Velocities\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[0] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[1] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[2] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[3] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[4] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[5] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].velocities[6] << "\n";

                            plan_output_c << "Accelerations\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[0] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[1] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[2] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[3] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[4] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[5] << "\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].accelerations[6] << "\n";

                            plan_output_c << "Time_From_Start\n";
                            plan_output_c << trajectory_cartesian.joint_trajectory.points[i].time_from_start << "\n";                
                    }
                    plan_output_c.close();

                    std::cout << "Dime duration pre scaling : " << trajectory_cartesian.joint_trajectory.points[trajectory_cartesian.joint_trajectory.points.size()-1].time_from_start << "\n";
                    std::cout << "Time Duration post scaling: " << Traj_flag.joint_trajectory.points[Traj_flag.joint_trajectory.points.size()-1].time_from_start << "\n";
                }
                else
                {
                    Traj_flag = VelocityScaling(trajectory_cartesian, PoseFlag,  mean_velocity, T_offset);  
                }
                

                TrajVector.push_back(Traj_flag);             
                size_plan = trajectory_cartesian.joint_trajectory.points.size();
                for (int j=0; j<7; j++)
                {
                    joints_Cartesian[j] = trajectory_cartesian.joint_trajectory.points[size_plan-1].positions[j];
                }

                start_state.setJointGroupPositions(joint_model_group, joints_Cartesian);       
                move_group.setStartState(start_state);
                PoseFlag.poses.clear();
            }


            for(int k=0; k<waypoints.SecondaryTrajectory[i].poses.size(); k++)
            {
                PoseFlag.poses.push_back(waypoints.SecondaryTrajectory[i].poses[k]);
            }
            last_label = waypoints.pt_label[i];
        }
        std::cout << "completed";
    }

    LabelVector.push_back(waypoints.pt_label[waypoints.pt_label.size()-1]);
    move_group.computeCartesianPath(PoseFlag.poses, eef_step, jump_threshold, trajectory_cartesian);

    if (trajectory_cartesian.joint_trajectory.points.size() > 0)
    {
        TrajVector.push_back(VelocityScaling(trajectory_cartesian, PoseFlag,  mean_velocity, T_offset));
    }
    else
    {
        ROS_INFO("SKIP");
    }
    


    PlansVector.resize(TrajVector.size());
    for(int i=0; i<TrajVector.size();i++)
        PlansVector[i].trajectory_ = TrajVector[i];

    std::cout << "TrajVector size:  " << TrajVector.size() << "\n";
    std::cout << "LabelVector size:  " << LabelVector.size() << "\n";




/*** ESECUZIONE ***/
    /* 
        cmd_msg:
            1 = PID;
            2 = MAN;
    */
    for(int i=0; i<PlansVector[0].trajectory_.joint_trajectory.joint_names.size(); i++ )
        std::cout << PlansVector[0].trajectory_.joint_trajectory.joint_names[PlansVector[0].trajectory_.joint_trajectory.joint_names.size()-1] << "\n";
    
    visual_tools.prompt("Start The execution of the trajectory");



    for(int i=0; i<PlansVector.size(); i++)
    {
        if(waypoints.pt_label[i] == "pass")
        {
            cmd_msg.data = 1;
            cmd_pub.publish(cmd_msg);
            ros::Duration(1).sleep();
        }
        else
        {
            cmd_msg.data = 2;
            //cmd_msg.data = 3; // loop
            cmd_pub.publish(cmd_msg);
            ros::Duration(1).sleep();
            width_msg.data = 0.004;
            width_pub.publish(width_msg);
        }
        move_group.execute(PlansVector[i]);
    }

    cmd_msg.data = 2;
    //cmd_msg.data = 3; // loop
    cmd_pub.publish(cmd_msg);
    ros::Duration(1).sleep();


    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    ros::shutdown();
    return 0;
}

