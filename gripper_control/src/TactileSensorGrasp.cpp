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


float velocity;
int rate;
float upper_bound;
float lower_bound;


franka::Gripper gripper("172.16.0.2");

int main(int argc, char** argv)
{
    ros::init(argc, argv, "Grasp Test for tactile sensor");

    ros::NodeHandle n;

    n.getParam("/GripperControl/rate", rate);
    n.getParam("/GripperControl/velocity", velocity);
    n.getParam("/GripperControl/upper_bound", upper_bound);
    n.getParam("/GripperControl/lower_bound", lower_bound);    

    ros::Rate loop_rate(rate);

    actionlib::SimpleActionClient<franka_gripper::MoveAction> move_client("/franka_gripper/move", true);
    actionlib::SimpleActionClient<franka_gripper::StopAction> stop_client("/franka_gripper/stop", true);

    franka_gripper::MoveGoal move_goal;
    franka_gripper::StopGoal stop_goal;

    move_goal.speed = velocity;
    
    ROS_INFO("Wait for server");
    move_client.waitForServer();
    ROS_INFO("Gripepr controller ready");


    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");

    visual_tools.prompt("Start Test");

    move_goal.speed = 0.10;

    
    while(ros::ok())
    {   
        move_goal.width = lower_bound;
        gripper.move(move_goal.width, move_goal.speed);
        
        move_goal.width = 0.08;
        gripper.move(move_goal.width, move_goal.speed);

    }

    ros::shutdown();
    return 0;
}
