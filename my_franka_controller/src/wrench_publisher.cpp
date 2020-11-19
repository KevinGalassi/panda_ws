// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include <wrench_publisher.h>

#include <cmath>
#include <memory>
#include <mutex>
#include <string>

#include <franka/errors.h>
#include <franka_hw/franka_cartesian_command_interface.h>
#include <franka_msgs/Errors.h>
#include <hardware_interface/hardware_interface.h>
#include <pluginlib/class_list_macros.h>
#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_datatypes.h>

#include "pseudo_inversion.h"


namespace franka_robot_controllers {

bool WrenchPublisher::init(hardware_interface::RobotHW* robot_hardware,
                                 ros::NodeHandle& root_node_handle,
                                 ros::NodeHandle& controller_node_handle) {
  franka_state_interface_ = robot_hardware->get<franka_hw::FrankaStateInterface>();
  if (franka_state_interface_ == nullptr) {
    ROS_ERROR("FrankaStateController: Could not get Franka state interface from hardware");
    return false;
  }
  if (!controller_node_handle.getParam("arm_id", arm_id_)) {
    ROS_ERROR("FrankaStateController: Could not get parameter arm_id");
    return false;
  }
  double publish_rate(100);

  if (!controller_node_handle.getParam("publish_rate", publish_rate)) {
    ROS_INFO_STREAM("FrankaStateController: Did not find publish_rate. Using default "
                    << publish_rate << " [Hz].");
  }

  publish_rate = 60;
  trigger_publish_ = franka_hw::TriggerRate(publish_rate);

  if (!controller_node_handle.getParam("joint_names", joint_names_) ||
      joint_names_.size() != robot_state_.q.size()) {
    ROS_ERROR(
        "FrankaStateController: Invalid or no joint_names parameters provided, aborting "
        "controller init!");
    return false;
  }
  try {
    franka_state_handle_ = std::make_unique<franka_hw::FrankaStateHandle>(
        franka_state_interface_->getHandle(arm_id_ + "_robot"));
  } catch (const hardware_interface::HardwareInterfaceException& ex) {
    ROS_ERROR_STREAM("FrankaStateController: Exception getting franka state handle: " << ex.what());
    return false;
  }

  {   // Add For get Estimation of the forces

    auto* model_interface = robot_hardware->get<franka_hw::FrankaModelInterface>();
  if (model_interface == nullptr) {
    ROS_ERROR_STREAM("ForceExampleController: Error getting model interface from hardware");
    return false;
  }
  try {
    model_handle_ = std::make_unique<franka_hw::FrankaModelHandle>(
        model_interface->getHandle(arm_id_ + "_model"));
  } catch (hardware_interface::HardwareInterfaceException& ex) {
    ROS_ERROR_STREAM(
        "ForceExampleController: Exception getting model handle from interface: " << ex.what());
    return false;
  }
  // FIne aggiunta
  }
  publisher_external_force_.init(controller_node_handle, "External_Wrench",1);
  publisher_external_force_filter.init(controller_node_handle, "External_Wrench_Filter",1);

  { // Publisher brackets
  {
    std::lock_guard<realtime_tools::RealtimePublisher<geometry_msgs::WrenchStamped> > lock(
        publisher_external_force_);
    publisher_external_force_.msg_.header.frame_id = arm_id_ + "_K";
    publisher_external_force_.msg_.wrench.force.x = 0.0;
    publisher_external_force_.msg_.wrench.force.y = 0.0;
    publisher_external_force_.msg_.wrench.force.z = 0.0;
    publisher_external_force_.msg_.wrench.torque.x = 0.0;
    publisher_external_force_.msg_.wrench.torque.y = 0.0;
    publisher_external_force_.msg_.wrench.torque.z = 0.0;
  }

  {
    std::lock_guard<realtime_tools::RealtimePublisher<geometry_msgs::WrenchStamped> > lock(
        publisher_external_force_);
    publisher_external_force_.msg_.header.frame_id = arm_id_ + "_External_Wrench";
    publisher_external_force_.msg_.wrench.force.x = 0.0;
    publisher_external_force_.msg_.wrench.force.y = 0.0;
    publisher_external_force_.msg_.wrench.force.z = 0.0;
    publisher_external_force_.msg_.wrench.torque.x = 0.0;
    publisher_external_force_.msg_.wrench.torque.y = 0.0;
    publisher_external_force_.msg_.wrench.torque.z = 0.0;
  }  
  {
    std::lock_guard<realtime_tools::RealtimePublisher<geometry_msgs::WrenchStamped> > lock(
        publisher_external_force_filter);
    publisher_external_force_filter.msg_.header.frame_id = arm_id_ + "Wrench_Filtered100";
    publisher_external_force_filter.msg_.wrench.force.x = 0.0;
    publisher_external_force_filter.msg_.wrench.force.y = 0.0;
    publisher_external_force_filter.msg_.wrench.force.z = 0.0;
    publisher_external_force_filter.msg_.wrench.torque.x = 0.0;
    publisher_external_force_filter.msg_.wrench.torque.y = 0.0;
    publisher_external_force_filter.msg_.wrench.torque.z = 0.0;
  }

  }

  return true;
}


void WrenchPublisher::starting(const ros::Time& time ) {
  ROS_INFO("Controller Start!");
  robot_state_ = franka_state_handle_->getRobotState();
  std::array<double, 7> gravity_array = model_handle_->getGravity();
  Eigen::Map<Eigen::Matrix<double, 7, 1>> tau_measured(robot_state_.tau_J.data());
  Eigen::Map<Eigen::Matrix<double, 7, 1>> gravity(gravity_array.data());
  // Bias correction for the current external torque
  tau_ext_initial_ = tau_measured - gravity;

  filter_size = 25;
  filter_index = 0;
  ForceDataVector.resize(filter_size);
  for(int i=0; i<filter_size; i++)
    ForceDataVector[i] << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
  MeanMatrix << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;

  ROS_INFO("STARTING COMPLETED");
}


void WrenchPublisher::update(const ros::Time& time, const ros::Duration& /* period */) {
  
  if (trigger_publish_()) {
    robot_state_ = franka_state_handle_->getRobotState();

    std::array<double, 42> jacobian_array = model_handle_->getZeroJacobian(franka::Frame::kEndEffector);
    std::array<double, 7> gravity_array = model_handle_->getGravity();

    Eigen::Map<Eigen::Matrix<double, 6, 7>> jacobian(jacobian_array.data());
    Eigen::Map<Eigen::Matrix<double, 7, 1>> tau_measured(robot_state_.tau_J.data());
    Eigen::Map<Eigen::Matrix<double, 7, 1>> gravity(gravity_array.data());

    Eigen::MatrixXd jacobian_transpose_pinv;

    pseudoInverse(jacobian.transpose(), jacobian_transpose_pinv);

    tau_ext = tau_measured - gravity - tau_ext_initial_;
    Forces = jacobian_transpose_pinv * tau_ext;
  

    ForceDataVector[filter_index] = Forces;
    filter_index ++;
    if (filter_index >= filter_size)
      filter_index = 0;
    for(int i=0; i< filter_size; i++)
      MeanMatrix += ForceDataVector[i];
    Forces_filter = MeanMatrix/(double)filter_size;  
    MeanMatrix << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;
    
 
    publishExternalForce(time);
    publishExternalForceFilter(time);
    
    sequence_number_++;
  }
}

void WrenchPublisher::publishExternalForce(const ros::Time& time) {
  if (publisher_external_force_.trylock()) {
    publisher_external_force_.msg_.header.frame_id = arm_id_ + "_external_force";
    publisher_external_force_.msg_.header.stamp = time;
    publisher_external_force_.msg_.wrench.force.x = (float) Forces(0);
    publisher_external_force_.msg_.wrench.force.y = (float)Forces(1);
    publisher_external_force_.msg_.wrench.force.z = (float)Forces(2);
    publisher_external_force_.msg_.wrench.torque.x = (float)Forces(3);
    publisher_external_force_.msg_.wrench.torque.y = (float)Forces(4);
    publisher_external_force_.msg_.wrench.torque.z = (float)Forces(5);
    publisher_external_force_.unlockAndPublish();
  }
}

void WrenchPublisher::publishExternalForceFilter(const ros::Time& time) {
  
  if (publisher_external_force_filter.trylock()) {
    publisher_external_force_filter.msg_.header.frame_id = arm_id_ + "filtered";
    publisher_external_force_filter.msg_.header.stamp = time;
    publisher_external_force_filter.msg_.wrench.force.x = (float)Forces_filter(0);
    publisher_external_force_filter.msg_.wrench.force.y = (float)Forces_filter(1);
    publisher_external_force_filter.msg_.wrench.force.z = (float)Forces_filter(2);
    publisher_external_force_filter.msg_.wrench.torque.x = (float)Forces_filter(3);
    publisher_external_force_filter.msg_.wrench.torque.y = (float)Forces_filter(4);
    publisher_external_force_filter.msg_.wrench.torque.z = (float)Forces_filter(5);
    publisher_external_force_filter.unlockAndPublish();
  }

}


}  // namespace my_franka_control

PLUGINLIB_EXPORT_CLASS(franka_robot_controllers::WrenchPublisher, controller_interface::ControllerBase)
