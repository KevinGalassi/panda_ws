#include "ros/ros.h"

#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float32MultiArray.h"

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

#include <controller_manager_msgs/SwitchController.h>

#include <MyFunc.h>




#define PLANNING_ATTEMPTS_NO 500
#define INPUT_RPY 1

int main(int argc, char** argv)
{
    ros::init(argc, argv, "PickPlace_Wire");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(1);
    spinner.start();

    ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_state_request",100);
    ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/gripper_width_request", 100);
    ros::Publisher cartesian_vel_pub = nh.advertise<std_msgs::Float32MultiArray>("/cartesian_velocity_request",100);


    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;
    std_msgs::Float32MultiArray crt_vel_msg;
    crt_vel_msg.data.resize(6);
    for(int i=0;i<6;i++)
        crt_vel_msg.data[i] = 0;


    /* Action Server */

    ros::ServiceClient switch_client = nh.serviceClient<controller_manager_msgs::SwitchController>("/controller_manager/switch_controller");

    controller_manager_msgs::SwitchController::Request switch_req;
    controller_manager_msgs::SwitchController::Response switch_resp;

    std::string pos_control = "position_joint_trajectory_controller";
    std::string vel_control = "joint_velocity_example_controller";

    namespace rvt = rviz_visual_tools;


// Creation of the moveit group
    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();


    move_group.setMaxVelocityScalingFactor(0.2);


/*** ESECUZIONE ***/

    geometry_msgs::Pose start_pose;

    start_pose.position.x = 0.1;
    start_pose.position.y = 0.4;
    start_pose.position.z = 0.4;

    tf2::Quaternion quat;

    quat.setRPY(M_PI,0,5*M_PI/4);
    start_pose.orientation = tf2::toMsg(quat);   


    // Cambiare controllo -> controllo di velocità
    switch_req.start_controllers.resize(1);
    switch_req.stop_controllers.resize(1);

    switch_req.start_controllers[0] = vel_control;
    switch_req.stop_controllers[0] = pos_control;       // Rivedere i nomi

    if(switch_client.call(switch_req,switch_resp))
    {
        ROS_INFO("Switch controll requested");
        if (switch_resp.ok)
            ROS_INFO("Switch completed with success");
        else
        {
            ROS_ERROR("Error during the switch");
            return 1;
        }
    }
    else
    {
        ROS_ERROR("Error during the call of the client");
        return 1;
    }

    float Vx = 0.01;


    // pubblicare velocità
    visual_tools.prompt("Next to publish the velocity");
    crt_vel_msg.data[0] = Vx;
    cartesian_vel_pub.publish(crt_vel_msg);
    
    ROS_INFO("Start counter");
    ros::Duration(5).sleep();
    ROS_INFO("Stop velocity");


    crt_vel_msg.data[0] = 0.0;
    cartesian_vel_pub.publish(crt_vel_msg);

    visual_tools.prompt("Velocity stopped, 'Next' to turn at ready state");

    crt_vel_msg.data[1] = Vx;
    cartesian_vel_pub.publish(crt_vel_msg);

    ros::Duration(5).sleep();

    crt_vel_msg.data[1] = 0;
    crt_vel_msg.data[2] = Vx;
    cartesian_vel_pub.publish(crt_vel_msg);

    ros::Duration(5).sleep();

    for(int i=0; i<6; i++)
        crt_vel_msg.data[i] = 0;
    cartesian_vel_pub.publish(crt_vel_msg);



    // Cambiare Controllo -> Posizione
    switch_req.start_controllers[0] = pos_control;
    switch_req.stop_controllers[0] = vel_control;       // Rivedere i nomi

    if(switch_client.call(switch_req,switch_resp))
    {
        ROS_INFO("Switch controll requested");
        if (switch_resp.ok)
            ROS_INFO("Switch completed with success");
        else
        {
            ROS_ERROR("Error during the switch");
            return 1;
        }
    }
    else
    {
        ROS_ERROR("Error during the call of the client");
        return 1;
    }


    // Ritorno alla posizione originale

    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();
    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();



    ros::shutdown();
    return 0;
}

