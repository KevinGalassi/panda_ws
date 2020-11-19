// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <controller_interface/multi_interface_controller.h>

#include <franka_hw/franka_state_interface.h>
#include <franka_hw/trigger_rate.h>
#include <franka_hw/franka_model_interface.h>
#include <franka_hw/franka_state_interface.h>
#include <franka_msgs/FrankaState.h>


#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/robot_hw.h>

#include <geometry_msgs/WrenchStamped.h>
#include <realtime_tools/realtime_publisher.h>
#include <sensor_msgs/JointState.h>
#include <tf2_msgs/TFMessage.h>

#include <Eigen/Core>

namespace franka_robot_controllers {

/**
 * Controller to publish the robot state as ROS topics.
 */
class WrenchPublisher : public controller_interface::MultiInterfaceController<
                                  franka_hw::FrankaStateInterface, 
                                  franka_hw::FrankaModelInterface>{
 public:
  
  
  bool init(hardware_interface::RobotHW* robot_hardware, ros::NodeHandle& root_node_handle, ros::NodeHandle& controller_node_handle) override;
  void starting(const ros::Time& time);
  void update(const ros::Time& time, const ros::Duration& period) override;

 private:

  void publishExternalForce(const ros::Time& time);         // Force estimation
  void publishExternalForceFilter(const ros::Time& time);   // Filtered Force estimation


  std::string arm_id_;

  franka_hw::FrankaStateInterface* franka_state_interface_{};
  std::unique_ptr<franka_hw::FrankaStateHandle> franka_state_handle_{};

  // Add for Force estimation
  
  std::unique_ptr<franka_hw::FrankaModelHandle> model_handle_;
  Eigen::Matrix<double, 7, 1> tau_ext_initial_;
  Eigen::Matrix<double, 7, 1> tau_ext;
  Eigen::Matrix<double, 6, 1> Forces;
  Eigen::Matrix<double, 6, 1> Forces_filter;
  int filter_size;
  int filter_index;
  std::vector<Eigen::Matrix<double, 6, 1>> ForceDataVector;
  Eigen::Matrix<double, 6, 1> MeanMatrix;


  
  // PUBLISHER
  
  realtime_tools::RealtimePublisher<geometry_msgs::WrenchStamped> publisher_external_force_;
  realtime_tools::RealtimePublisher<geometry_msgs::WrenchStamped> publisher_external_force_filter;  

  franka_hw::TriggerRate trigger_publish_;
  franka::RobotState robot_state_;
  uint64_t sequence_number_ = 0;
  std::vector<std::string> joint_names_;
};

}  // namespace franka_control
