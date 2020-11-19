// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include <franka_controllers/force_gravity_controller.h>

#include <cmath>
#include <memory>

#include <controller_interface/controller_base.h>
#include <pluginlib/class_list_macros.h>
#include <ros/ros.h>

#include <franka/robot_state.h>


namespace franka_controllers {

bool ForceGravityController::init(hardware_interface::RobotHW* robot_hw,
                                  ros::NodeHandle& node_handle) {


  std::vector<std::string> joint_names;
  std::string arm_id;
  desired_force_torque = Eigen::VectorXd(6);
  for (int i=0;i<6;i++){
    desired_force_torque[i] =0;
  }
  force_sub=node_handle.subscribe("/panda_arm_controller/force_command",1, &ForceGravityController::force_cb,this,ros::TransportHints().reliable().tcpNoDelay());
  ROS_WARN(
      "ForceExampleController: Make sure your robot's endeffector is in contact "
      "with a horizontal surface before starting the controller!");
  if (!node_handle.getParam("arm_id", arm_id)) {
    ROS_ERROR("ForceExampleController: Could not read parameter arm_id");
    return false;
  }
  if (!node_handle.getParam("joint_names", joint_names) || joint_names.size() != 7) {
    ROS_ERROR(
        "ForceExampleController: Invalid or no joint_names parameters provided, aborting "
        "controller init!");
    return false;
  }

  auto* model_interface = robot_hw->get<franka_hw::FrankaModelInterface>();
  if (model_interface == nullptr) {
    ROS_ERROR_STREAM("ForceExampleController: Error getting model interface from hardware");
    return false;
  }
  try {
    model_handle_ = std::make_unique<franka_hw::FrankaModelHandle>(
        model_interface->getHandle(arm_id + "_model"));
  } catch (hardware_interface::HardwareInterfaceException& ex) {
    ROS_ERROR_STREAM(
        "ForceExampleController: Exception getting model handle from interface: " << ex.what());
    return false;
  }

  auto* state_interface = robot_hw->get<franka_hw::FrankaStateInterface>();
  if (state_interface == nullptr) {
    ROS_ERROR_STREAM("ForceExampleController: Error getting state interface from hardware");
    return false;
  }
  try {
    state_handle_ = std::make_unique<franka_hw::FrankaStateHandle>(
        state_interface->getHandle(arm_id + "_robot"));
  } catch (hardware_interface::HardwareInterfaceException& ex) {
    ROS_ERROR_STREAM(
        "ForceExampleController: Exception getting state handle from interface: " << ex.what());
    return false;
  }

  auto* effort_joint_interface = robot_hw->get<hardware_interface::EffortJointInterface>();
  if (effort_joint_interface == nullptr) {
    ROS_ERROR_STREAM("ForceExampleController: Error getting effort joint interface from hardware");
    return false;
  }
  for (size_t i = 0; i < 7; ++i) {
    try {
      joint_handles_.push_back(effort_joint_interface->getHandle(joint_names[i]));
    } catch (const hardware_interface::HardwareInterfaceException& ex) {
      ROS_ERROR_STREAM("ForceExampleController: Exception getting joint handles: " << ex.what());
      return false;
    }
  }

  dynamic_reconfigure_desired_mass_param_node_ =
      ros::NodeHandle("dynamic_reconfigure_desired_mass_param_node");
  dynamic_server_desired_mass_param_ = std::make_unique<
      dynamic_reconfigure::Server<franka_example_controllers::desired_mass_paramConfig>>(

      dynamic_reconfigure_desired_mass_param_node_);
  dynamic_server_desired_mass_param_->setCallback(
      boost::bind(&ForceGravityController::desiredMassParamCallback, this, _1, _2));

   wrench_pub =  dynamic_reconfigure_desired_mass_param_node_.advertise<geometry_msgs::WrenchStampede>("/prova_forza",100);
  
  
  return true;
}

void ForceGravityController::force_cb(const geometry_msgs::Twist &msg){
    desired_force_torque[0] =msg.linear.x;
    desired_force_torque[1] =msg.linear.y;
    desired_force_torque[2] =msg.linear.z;
    desired_force_torque[3] =msg.angular.x;
    desired_force_torque[4] =msg.angular.y;
    desired_force_torque[5] =msg.angular.z;
}


void ForceGravityController::starting(const ros::Time& /*time*/) {
  franka::RobotState robot_state = state_handle_->getRobotState();
  std::array<double, 7> gravity_array = model_handle_->getGravity();
  Eigen::Map<Eigen::Matrix<double, 7, 1>> tau_measured(robot_state.tau_J.data());
  Eigen::Map<Eigen::Matrix<double, 7, 1>> gravity(gravity_array.data());
  // Bias correction for the current external torque
  tau_ext_initial_ = tau_measured - gravity;
  //desired_force_torque.setZero();
  tau_error_.setZero();
}

void ForceGravityController::update(const ros::Time& /*time*/, const ros::Duration& period) {
  franka::RobotState robot_state = state_handle_->getRobotState();
  std::array<double, 42> jacobian_array =
      model_handle_->getZeroJacobian(franka::Frame::kEndEffector);
  std::array<double, 7> coriolis_array = model_handle_->getCoriolis();

  // convert to Eigen
  Eigen::Map<Eigen::Matrix<double, 7, 1>> coriolis(coriolis_array.data());
  std::array<double, 7> gravity_array = model_handle_->getGravity();
  Eigen::Map<Eigen::Matrix<double, 6, 7>> jacobian(jacobian_array.data());
  Eigen::Map<Eigen::Matrix<double, 7, 1>> tau_measured(robot_state.tau_J.data());
  Eigen::Map<Eigen::Matrix<double, 7, 1>> tau_J_d(  // NOLINT (readability-identifier-naming)
      robot_state.tau_J_d.data());
  Eigen::Map<Eigen::Matrix<double, 7, 1>> gravity(gravity_array.data());
  //desired_force_torque[3] = 2;
  Eigen::VectorXd tau_d(7), tau_cmd(7), tau_ext(7);
  Eigen::VectorXd Wrench_(6);
  //desired_force_torque(2) = desired_mass_ * -9.81;
  tau_ext = tau_measured - gravity - tau_ext_initial_;

  Wrench = jacobian*tau_ext;

  wrench_msg.wrench.force.x = Wrench(0);
  wrench_msg.wrench.force.y = Wrench(1);
  wrench_msg.wrench.force.z = Wrench(2);
  wrench_msg.wrench.torque.x = Wrench(3);
  wrench_msg.wrench.torque.y = Wrench(4);
  wrench_msg.wrench.torque.z = Wrench(5);

  wrench_pub.publish(wrench_msg);



  tau_d << jacobian.transpose() * desired_force_torque;
  tau_error_ = tau_error_ + period.toSec() * (tau_d - tau_ext);
  // FF + PI control (PI gains are initially all 0)
  tau_cmd = tau_d + k_p_ * (tau_d - tau_ext) + k_i_ * tau_error_ ;
  //tau_cmd = Zero(7);//{0,0,0,0,0,0,0};
  //tau_cmd.Zero(7);//{0,0,0,0,0,0,0};
  tau_cmd << saturateTorqueRate(tau_cmd, tau_J_d);
  //std::cout<<"start"<<std::endl;
  //std::cout<<tau_cmd<<std::endl;
  for (size_t i = 0; i < 7; ++i) {
    joint_handles_[i].setCommand(tau_cmd(i));
  }

  // Update signals changed online through dynamic reconfigure
  desired_mass_ = filter_gain_ * target_mass_ + (1 - filter_gain_) * desired_mass_;
  k_p_ = filter_gain_ * target_k_p_ + (1 - filter_gain_) * k_p_;
  k_i_ = filter_gain_ * target_k_i_ + (1 - filter_gain_) * k_i_;
}

void ForceGravityController::desiredMassParamCallback(
    franka_example_controllers::desired_mass_paramConfig& config,
    uint32_t /*level*/) {
  target_mass_ = config.desired_mass;
  target_k_p_ = config.k_p;
  target_k_i_ = config.k_i;
}

Eigen::Matrix<double, 7, 1> ForceGravityController::saturateTorqueRate(
    const Eigen::Matrix<double, 7, 1>& tau_d_calculated,
    const Eigen::Matrix<double, 7, 1>& tau_J_d) {  // NOLINT (readability-identifier-naming)
  Eigen::Matrix<double, 7, 1> tau_d_saturated{};
  for (size_t i = 0; i < 7; i++) {
    double difference = tau_d_calculated[i] - tau_J_d[i];
    tau_d_saturated[i] = tau_J_d[i] + std::max(std::min(difference, 20.0), -20.0);
  }
  return tau_d_saturated;
}

}  // namespace franka_example_controllers

PLUGINLIB_EXPORT_CLASS(franka_controllers::ForceGravityController,
                       controller_interface::ControllerBase)