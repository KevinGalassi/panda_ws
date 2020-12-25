
#include <ros/init.h>
#include <ros/node_handle.h>
#include <ros/rate.h>
#include <ros/subscriber.h>

#include <std_msgs/Float32.h>

// #include <franka_gripper/franka_gripper.h>

#include <franka_gripper/GraspAction.h>
#include <franka_gripper/HomingAction.h>
#include <franka_gripper/MoveAction.h>
#include <franka_gripper/StopAction.h>

#include <actionlib/client/simple_action_client.h>

#include <iostream>
#include <stdio.h>

// TO LAUNCH THE GRIPPER with the hMi: gripper_control.launch

float target_width = 0.08;
float new_target_width = 0.08;
float old_target_width =  0.08;
bool reset = false;

int start = 0;

float P;
float I;
float D;
float reference;

bool grasp_ok = false;



void Request_Callback(const std_msgs::Float32& msg)
{
    ROS_INFO("Message received");
    new_target_width = msg.data;
    start = 1;
}  



int main(int argc, char** argv)
{
    ros::init(argc, argv, "GripperClient_Node");

    ros::NodeHandle n;

    actionlib::SimpleActionClient<franka_gripper::MoveAction> move_client("/franka_gripper/move", true);
    actionlib::SimpleActionClient<franka_gripper::GraspAction> grasp_client("/franka_gripper/grasp", true);
    actionlib::SimpleActionClient<franka_gripper::StopAction> stop_client("/franka_gripper/stop", true);    
        

    float grasp_force;
    if(n.getParam("/Control/GraspForce",grasp_force))
        std::cout << "grasp_force readen\n";
    else
        grasp_force = 0.01;

    float velocity;

    if(n.getParam("/Control/Velocity", velocity))
        std::cout << "Velocity readen \n";
    else
        velocity = 0.02;

    ros::Rate loop_rate(60);

    ros::Subscriber user_sub = n.subscribe("/My_new_input", 100, Request_Callback);

    ROS_INFO("Wait for server");
    move_client.waitForServer();
    ROS_INFO("Gripepr controller ready");

    franka_gripper::MoveGoal move_goal;
    franka_gripper::GraspGoal grasp_goal;
    franka_gripper::StopGoal stop_goal;

    franka_gripper::GraspEpsilon grasp_msg;

    move_goal.speed = velocity;

    grasp_msg.inner = 0.001;
    grasp_msg.outer = 0.001;
    
    grasp_goal.force = grasp_force;
    grasp_goal.speed = velocity;
    grasp_goal.epsilon = grasp_msg;

    std::vector<double> finger_width;
    finger_width.resize(2);
    float user_input;
    
    if(move_client.isServerConnected())
    {
        ROS_INFO("ServerOK");
    }
    else
    {
        ROS_INFO("Server not ok :(");
        return 9;
    }


    while(ros::ok() )
    {   
        // Prova 1:

        /*
        if(start)
        {
            move_client.waitForServer();            
            move_client.cancelAllGoals();

            move_goal.width = new_target_width;
            grasp_goal.width = new_target_width;

            ROS_INFO("Send goal!");
            move_client.sendGoal(move_goal);
            
            ROS_INFO("Wait result");            
            if (move_client.waitForResult(ros::Duration(15)))
                ROS_INFO("Operation completed");
            else
                ROS_INFO("Operation not completed!");
            
            loop_rate.sleep();
            
            start = 0;
        }
        */

        // Prova 2 

        if(new_target_width == target_width)
        {
            ROS_INFO("Status Completed");
        }
        else
        {
            if(grasp_ok)
            {
                if(new_target_width != target_width)
                {
                    grasp_client.cancelAllGoals();
                    target_width = new_target_width;
                    grasp_goal.width = target_width;

                    stop_client.sendGoal(stop_goal);
                    grasp_client.sendGoal(grasp_goal);
                }
            }
            else
            {
                if(new_target_width != target_width)
                {
                    move_client.cancelAllGoals();
                    target_width = new_target_width;
                    move_goal.width = target_width;

                    stop_client.sendGoal(stop_goal);
                    move_client.sendGoal(move_goal);
                }
            }
        }
    
        ros::spinOnce();
    }

    ros::shutdown();
    return 1;
}


/*********************
MOVE ACTION

float64 width  # [m]
float64 speed  # [m/s]
---
bool success
string error
---

********/