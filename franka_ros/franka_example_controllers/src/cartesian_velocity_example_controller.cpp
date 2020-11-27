// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include <franka_example_controllers/cartesian_velocity_example_controller.h>

#include <array>
#include <cmath>
#include <memory>
#include <string>

#include <controller_interface/controller_base.h>
#include <hardware_interface/hardware_interface.h>
#include <hardware_interface/joint_command_interface.h>
#include <pluginlib/class_list_macros.h>
#include <ros/ros.h>


namespace franka_example_controllers {

bool CartesianVelocityExampleController::init(hardware_interface::RobotHW* robot_hardware,
                                              ros::NodeHandle& node_handle) {
    std::string arm_id;
    if (!node_handle.getParam("arm_id", arm_id)) {
        ROS_ERROR("CartesianVelocityExampleController: Could not get parameter arm_id");
        return false;
    }

    velocity_cartesian_interface =
        robot_hardware->get<franka_hw::FrankaVelocityCartesianInterface>();
    if (velocity_cartesian_interface == nullptr) {
        ROS_ERROR(
            "CartesianVelocityExampleController: Could not get Cartesian velocity interface from "
            "hardware");
        return false;
    }
    try {
        velocity_cartesian_handle = std::make_unique<franka_hw::FrankaCartesianVelocityHandle>(
            velocity_cartesian_interface->getHandle(arm_id + "_robot"));
    } catch (const hardware_interface::HardwareInterfaceException& e) {
        ROS_ERROR_STREAM(
            "CartesianVelocityExampleController: Exception getting Cartesian handle: " << e.what());
        return false;
    }

    auto state_interface = robot_hardware->get<franka_hw::FrankaStateInterface>();
    if (state_interface == nullptr) {
        ROS_ERROR("CartesianVelocityExampleController: Could not get state interface from hardware");
        return false;
    }

    publish_rate = 200;
    trigger_publish = franka_hw::TriggerRate(publish_rate);

    vel_cmd_sub = node_handle.subscribe("/cartesian_velocity_request", 1, &CartesianVelocityExampleController::Velocity_callback, this);
    filter_pub = node_handle.advertise<std_msgs::Float32MultiArray>("/velocity_filtered",1);


  return true;
}

void CartesianVelocityExampleController::starting(const ros::Time& /* time */) {
    elapsed_time_ = ros::Duration(0.0);
    for(int i=0; i<6;i++)
        cartesian_velocity[i] = 0.0;

    filter_size = 25;
    filter_index = 0;
    VelDataVector.resize(filter_size);
    for(int i=0; i<filter_size; i++)
        VelDataVector[i] << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
    MeanMatrix << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
    vel_msg.data.resize(6);
    
    last_cmd_time = 0.0;
    vel_cmd_timeout = 0.1;
}

void CartesianVelocityExampleController::update(const ros::Time& , const ros::Duration& period) {
    elapsed_time_ += period;

    if(ros::Time::now().toSec() - last_cmd_time > vel_cmd_timeout)
    {
        // Controllo di ave ricevuto un comando
        for(int i=0; i<6; i++)
            command[i] = 0.0;
    }
    else
    {
        VelDataVector[filter_index] = cartesian_velocity;
        filter_index ++;
        if (filter_index >= filter_size)
            filter_index = 0;
        for(int i=0; i< filter_size; i++)
            MeanMatrix += VelDataVector[i];
        cartesian_velocity = MeanMatrix/(double)filter_size;  
        MeanMatrix << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;


        cartesian_acc = (cartesian_velocity - last_cart_velocity)/period.toSec();
        cartesian_jerk = (cartesian_acc - last_cartesian_acc)/period.toSec();

        scaling_factor[0] = (cartesian_velocity/p_dot).maxCoeff();
        scaling_factor[1] = (cartesian_acc/p_ddot).maxCoeff();
        scaling_factor[2] = (cartesian_jerk/p_dddot).maxCoeff();

        Eigen::Index max,min;
        max_scaling_factor = scaling_factor.maxCoeff(&max);
        if( max_scaling_factor > 1.0)
        {
            int index = (int)max;
            for(int i=0; i<6; i++)
                command[i] = cartesian_velocity(i)/(pow(max_scaling_factor, 1/(index+1)));
        }
        else
        {
            for(int i=0; i<6; i++)
               command[i] = cartesian_velocity(i);
        }
    }
  
    velocity_cartesian_handle->setCommand(command);
    for(int i=0; i<6; i++)
    {
        last_cart_velocity(i) = command[i];
        last_cartesian_acc(i) = (command[i] - last_command[i])/period.toSec();
        last_command[i] = command[i];
    }

}


void CartesianVelocityExampleController::stopping(const ros::Time& /*time*/) {
  // WARNING: DO NOT SEND ZERO VELOCITIES HERE AS IN CASE OF ABORTING DURING MOTION
  // A JUMP TO ZERO WILL BE COMMANDED PUTTING HIGH LOADS ON THE ROBOT. LET THE DEFAULT
  // BUILT-IN STOPPING BEHAVIOR SLOW DOWN THE ROBOT.
}

void CartesianVelocityExampleController::Velocity_callback(const std_msgs::Float32MultiArray& msg)
{

    if(msg.data.size() != 6)
        ROS_ERROR("CartesianController: Cartesian velocity request not consistence! :( ");
    else
    {
        for(int i=0; i<6;i++)
        cartesian_velocity(i) = (double)msg.data[i];
    }

    last_cmd_time = ros::Time::now().toSec();


}
}  // namespace franka_example_controllers

PLUGINLIB_EXPORT_CLASS(franka_example_controllers::CartesianVelocityExampleController,
                       controller_interface::ControllerBase)
