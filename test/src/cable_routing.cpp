

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

#include <actionlib/client/simple_action_client.h>


#include <MyFunc.h>




#define PLANNING_ATTEMPTS_NO 500
#define INPUT_RPY 1

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ol_tes2t2");
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

    /*
    float velocity_fix = 0.17;
    float velocity_round = 0.1;
    float velocity_pass = 0.20;
    */

   
    float velocity_fix = 0.02;
    float velocity_round = 0.1;
    float velocity_pass = 0.05;
   

    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();

    std::string path1 = "/home/kevin/ros/panda_ws/src/test/src/PointList/Test";
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

    param.radius = 0.05;                // Radius of the semi-circle for fixing part
    param.heigh = 0.03;                 // Heigh of upward movement
    param.circ_point = 1000;             // Point generated in the semi-circle
    param.res = 0.001;                  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;     // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;            // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;            // Distance of the second point
    param.corner_points = 100;          // Numver of point generated in the corner.

/**************** COMPUTE TRAJECTORY PARTS      ******************/


    geometry_msgs::Pose grasp1;
    geometry_msgs::PoseArray waypoints_final;

    ReadFileTxt(INPUT_RPY, waypoints.point, waypoints.pt_label, path1, grasp1);
    MyTrajectoryPlanner3(param, waypoints);
    visual_tools.deleteAllMarkers();
    


    for(int i=0; i<waypoints.SecondaryTrajectory.size(); i++)
    {
        visual_tools.publishPath(waypoints.SecondaryTrajectory[i].poses, rvt::BLUE, rvt::XXXSMALL);
        FromEE2Link8(waypoints.SecondaryTrajectory[i]);
    //    EE_Shift(waypoints.SecondaryTrajectory[i]);
    }

/**** Print result ****/


    visual_tools.prompt("Print Value");
    visual_tools.publishPath(waypoints.point.poses, rvt::LIME_GREEN, rvt::XXXSMALL);

    for(int i=0; i<waypoints.point.poses.size(); i++)
    {
        visual_tools.publishAxisLabeled(waypoints.point.poses[i], "init_point" , rvt::XXXSMALL);
    }

    for(int i=0; i<waypoints.SecondaryTrajectory.size(); i++)
    {
        for(int k=0; k<waypoints.SecondaryTrajectory[i].poses.size(); k++)
        {
            visual_tools.publishAxisLabeled(waypoints.SecondaryTrajectory[i].poses[k], waypoints.pt_label[i] , rvt::XXXSMALL);
        }
    }
    visual_tools.trigger();



/****************** MOTION INIT *****************************************/
        

    moveit_msgs::RobotTrajectory trajectory_cartesian;


    moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian, Plan2;

    bool success;
    int final_plan_size;

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
    std::vector<geometry_msgs::PoseArray> WaypointsVector;
    std::vector<moveit::planning_interface::MoveGroupInterface::Plan> PlansVector;      // potrebbe non essere usato

    FinalTrajectoryRescaling(PlansVector, WaypointsVector, LabelVector, move_group, waypoints, 
                            velocity_round, velocity_pass, velocity_fix, T_offset);


/*** ESECUZIONE ***/
    /* 
        cmd_msg:
            1 = PID;
            2 = MAN;
    */
    for(int i=0; i<PlansVector[0].trajectory_.joint_trajectory.joint_names.size(); i++ )
        std::cout << PlansVector[0].trajectory_.joint_trajectory.joint_names[PlansVector[0].trajectory_.joint_trajectory.joint_names.size()-1] << "\n";
    
    visual_tools.prompt("Start to grasp");

    hand_group.setJointValueTarget(hand_closed_position);
    hand_group.move();
  
    visual_tools.prompt(" Next to start");

    cmd_msg.data = 1;

    bool command_y = false;

    for(int i=0; i<PlansVector.size(); i++)
    {
        std::cout << "execute: " << LabelVector[i] << "\n";

        /*
        if(LabelVector[i] == "pass")
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
        */


        if(LabelVector[i] == "pass")
        {
            
            if(i>1)
            {
                if(LabelVector[i-1] == "cornerfix")
                {
                    command_y = true;
                }  
            }

            if(command_y)
            {
                cmd_msg.data = 4;
            }
            else
            {
                cmd_msg.data = 1;
            }
            
            cmd_pub.publish(cmd_msg);
            ros::Duration(0.5).sleep();     
        }
        else
        {
            cmd_msg.data = 2;
            //cmd_msg.data = 3; // loop
            cmd_pub.publish(cmd_msg);
            ros::Duration(0.5).sleep();
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

