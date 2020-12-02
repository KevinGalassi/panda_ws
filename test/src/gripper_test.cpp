#include <ros/init.h>
#include <ros/node_handle.h>
#include <ros/rate.h>
#include <ros/subscriber.h>

#include <std_msgs/Float32.h>
#include <std_msgs/String.h>

#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/WrenchStamped.h>
#include <geometry_msgs/Wrench.h>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_visual_tools/moveit_visual_tools.h>


//#include <franka_gripper/franka_gripper.h>
#include <franka/gripper.h>

#include <franka_gripper/GraspAction.h>
#include <franka_gripper/HomingAction.h>
#include <franka_gripper/MoveAction.h>
#include <franka_gripper/StopAction.h>
#include <actionlib/client/simple_action_client.h>


#include <iostream>
#include <stdio.h>

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion


#include <gripper_control/ControlOutput.h>



int rate = 50; // franka state controller loop at 500


int main(int argc, char** argv)
{
    ros::init(argc, argv, "GripperClient_Node");

    ros::NodeHandle n;

    float velocity;
    float reference;
    float force;
    float inner, outer;

    int pause1, pause2;



    n.getParam("/GripperControl/velocity", velocity);
    n.getParam("/GripperControl/reference", reference);
    n.getParam("/GripperControl/force", force);
    n.getParam("/GripperControl/pause1", pause1);
    n.getParam("/GripperControl/pause2", pause2);
    n.getParam("/GripperControl/innter", inner);
    n.getParam("/GripperControl/outer", outer);


  
    actionlib::SimpleActionClient<franka_gripper::MoveAction> move_client("/franka_gripper/move", true);
    actionlib::SimpleActionClient<franka_gripper::GraspAction> grasp_client("/franka_gripper/grasp", true);
    actionlib::SimpleActionClient<franka_gripper::StopAction> stop_client("/franka_gripper/stop", true);


    ros::Rate loop_rate(rate);

    float actual_value = reference;

    franka_gripper::MoveGoal move_goal;
    franka_gripper::StopGoal stop_goal;
    franka_gripper::GraspGoal grasp_goal;


    move_goal.speed = velocity;
    move_goal.width = reference;

    grasp_goal.speed = velocity;
    grasp_goal.width = reference;
    grasp_goal.force = force;
    grasp_goal.epsilon.inner = inner;
    grasp_goal.epsilon.outer = outer;

/*
    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");

    
    namespace rvt = rviz_visual_tools;
*/

    move_client.sendGoal(move_goal);
    // visual_tools.prompt("NExt grasp");

    ros::Duration(pause2).sleep();

    move_goal.width = 0.8;
    move_client.sendGoal(move_goal);
    
    ros::Duration(pause2).sleep();

    std::cout << "grasp \n";
    grasp_client.sendGoal(grasp_goal);

    ros::Duration(pause2).sleep();

    move_goal.width = 0.8;
    while(ros::ok())
    {
        grasp_client.sendGoal(grasp_goal);
        grasp_client.waitForResult(ros::Duration(10));
        ros::Duration(1).sleep();

        move_client.sendGoal(move_goal);
        move_client.waitForResult(ros::Duration(10));

    }

    //visual_tools.prompt("Conclude");
    

    return 0;
}