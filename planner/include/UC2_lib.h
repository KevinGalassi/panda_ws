

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdio.h>

#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

#include <moveit_visual_tools/moveit_visual_tools.h>

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion

#include <controller_manager_msgs/SwitchController.h>
#include <geometry_msgs/Twist.h>


const std::string end_effector_frame = "EE_POint"; // Da rivedere nome
const std::string MoveItControllerName = "pos_traj_controller";
const std::string effortControllerName = "effort_joint_controller";

std::vector<double> joint_value = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}:

void graspWire(geometry_msgs::Pose grasp_point,
               moveit::planning_interface::MoveGroupInterface& move_group,
               moveit::planning_interface::MoveGroupInterface& hand_group);


bool connectorInsertion(ros::NodeHandle nh,
                        geometry_msgs::Pose connectorPose,
                        float effort_value,
                        float starting_distance,
                        moveit::planning_interface::MoveGroupInterface& move_group,
                        moveit::planning_interface::MoveGroupInterface& hand_group
                        );

geometry_msgs::Pose CheckEEPosition(robot_state::RobotStatePtr& kinematic_state);

void joint_callback(const sensor_msgs::JointState& data);