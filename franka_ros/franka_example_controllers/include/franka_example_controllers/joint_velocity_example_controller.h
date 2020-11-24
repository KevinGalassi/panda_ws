// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#pragma once

#include <string>
#include <vector>

#include <controller_interface/multi_interface_controller.h>

#include <franka_hw/franka_state_interface.h>
#include <franka_hw/trigger_rate.h>
#include <franka_hw/franka_model_interface.h>
#include <franka_hw/franka_state_interface.h>
#include <franka_msgs/FrankaState.h>

#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/robot_hw.h>

#include <ros/node_handle.h>
#include <ros/time.h>

#include <std_msgs/Float32MultiArray.h>

#include <Eigen/Core>

#include "pseudo_inversion.h"


namespace franka_example_controllers {

class JointVelocityExampleController : public controller_interface::MultiInterfaceController<
                                           hardware_interface::VelocityJointInterface,
                                           franka_hw::FrankaModelInterface,
                                           franka_hw::FrankaStateInterface> {
 public:
    bool init(hardware_interface::RobotHW* robot_hardware, ros::NodeHandle& root_node_handle, ros::NodeHandle& controller_node_handle) override;
    void update(const ros::Time&, const ros::Duration& period) override;
    void starting(const ros::Time&) override;
    void stopping(const ros::Time&) override;

 private:
    std::string arm_id;
    franka_hw::FrankaStateInterface* franka_state_interface_{};
    std::unique_ptr<franka_hw::FrankaStateHandle> franka_state_handle_{};

    std::unique_ptr<franka_hw::FrankaModelHandle> model_handle_;

    franka_hw::TriggerRate trigger_publish_;
    franka::RobotState robot_state_;
    uint64_t sequence_number_ = 0;
    std::vector<std::string> joint_names_;



    // Velocity handler
    hardware_interface::VelocityJointInterface* velocity_joint_interface_;
    std::vector<hardware_interface::JointHandle> velocity_joint_handles_;

    Eigen::Matrix<double, 6, 1> cartesian_velocity;
    Eigen::Matrix<double, 7, 1> joint_velocity;

    // Filtro
    Eigen::Matrix<double, 6, 1> vel_filter;
    int filter_size;
    int filter_index;
    std::vector<Eigen::Matrix<double, 6, 1>> VelDataVector;
    Eigen::Matrix<double, 6, 1> MeanMatrix;

    std_msgs::Float32MultiArray vel_msg;
    ros::Publisher new_vel_pub;



    bool joint_cmd_start; 

    ros::Subscriber vel_cmd_sub;    // topic : "/cartesian_velocity_request"
    void Velocity_callback(const std_msgs::Float32MultiArray& msg);

};

}  // namespace franka_example_controllers
