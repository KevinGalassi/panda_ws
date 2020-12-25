
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


float Force;
float ForceY;
float delta;


int main(int argc, char** argv)
{
    ros::init(argc, argv, "PickPlace_Wire");
    ros::NodeHandle n;
    ros::Rate loop_rate(500);

    n.getParam("/ForceTest/Force",Force);
    n.getParam("/ForceTest/Step", delta);

   // ForceY = Force/4;

    ros::Publisher ForceRequest_pub = n.advertise<geometry_msgs::Twist>("/force_command", 1000);
    ros::Duration(2).sleep();

    geometry_msgs::Twist force_msg;

    force_msg.linear.x = 0;
    force_msg.linear.y = 0;
    force_msg.linear.z = 0;
    force_msg.angular.x = 0;
    force_msg.angular.y = 0;
    force_msg.angular.z = 0;

    float n_step;

    int count = 0;

    n_step = Force/delta;
        
    while(count < n_step)
    {
        count ++;
        std::cout << count << "\n";
        std::cout << force_msg << "\n";

        force_msg.linear.x = force_msg.linear.x + delta;
    //    force_msg.linear.y = force_msg.linear.x/2;
        ForceRequest_pub.publish(force_msg);
        loop_rate.sleep();
    }

    while(ros::ok())
    {
        force_msg.linear.x = 0;
        ForceRequest_pub.publish(force_msg);
    }



    ros::shutdown();
    return 0;
}