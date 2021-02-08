

#include "UC2_lib.h"
#include "parameters.h"


void graspWire(geometry_msgs::Pose grasp_point,
               moveit::planning_interface::MoveGroupInterface& move_group,
               moveit::planning_interface::MoveGroupInterface& hand_group)
{

   tf2::Matrix3x3 R;
   tf2::Quaternion quat;

   quat.setX(grasp_point.orientation.x);
   quat.setY(grasp_point.orientation.y);
   quat.setZ(grasp_point.orientation.z);
   quat.setW(grasp_point.orientation.w);
   R.setRotation(quat);
   
   geometry_msgs::Pose preGraspPoint;

   preGraspPoint.position.x = grasp_point.position.x - R[0][0]*0.05;
   preGraspPoint.position.y = grasp_point.position.y - R[1][0]*0.05;
   preGraspPoint.position.z = grasp_point.position.z - R[2][0]*0.05;

   ROS_INFO("Open the gripper");
   hand_group.setJointValueTarget(hand_open_position);
   hand_group.move();

   ROS_INFO("Reach pre-grasp point");
   move_group.setPoseTarget(preGraspPoint, end_effector_frame);
   move_group.move();

   ROS_INFO("Reach grasping point");
   move_group.setPoseTarget(grasp_point, end_effector_frame);
   move_group.move();

   ROS_INFO("Close the gripper");
   hand_group.setJointValueTarget(hand_grasp_position);
   hand_group.move();

   move_group.setPoseTarget(preGraspPoint, end_effector_frame);
   move_group.move();

   ROS_INFO("Wire-grasped");

   return;
}



bool connectorInsertion(ros::NodeHandle nh,
                        geometry_msgs::Pose connectorPose,
                        float effort_value,
                        float starting_distance,
                        moveit::planning_interface::MoveGroupInterface& move_group,
                        moveit::planning_interface::MoveGroupInterface& hand_group)
{
   tf2::Matrix3x3 R;
   tf2::Quaternion quat;

   quat.setX(connectorPose.orientation.x);
   quat.setY(connectorPose.orientation.y);
   quat.setZ(connectorPose.orientation.z);
   quat.setW(connectorPose.orientation.w);
   R.setRotation(quat);

   geometry_msgs::Pose PreInsertion;

   PreInsertion.position.x = connectorPose.position.x - R[0][2]*starting_distance;
   PreInsertion.position.y = connectorPose.position.y - R[1][2]*starting_distance;
   PreInsertion.position.z = connectorPose.position.z - R[2][2]*starting_distance;


   ros::ServiceClient switch_client = nh.serviceClient<controller_manager_msgs::SwitchController>("/controller_manager/switch_controller");
   controller_manager_msgs::SwitchController::Request switch_req;
   controller_manager_msgs::SwitchController::Response switch_resp;


   ROS_INFO("Reach pre-grasp point");
   move_group.setPoseTarget(PreInsertion, end_effector_frame);
   move_group.move();


   // switch to effort controller for the insertion task
   switch_req.start_controllers.resize(1);
   switch_req.stop_controllers.resize(1);
   switch_req.start_controllers[0] = effortControllerName;
   switch_req.stop_controllers[0] = MoveItControllerName; 
   if(switch_client.call(switch_req, switch_resp))
   {
      ROS_INFO("Switch controll requested");
      if (switch_resp.ok)
         ROS_INFO("Switch completed with success");
      else
      {
         ROS_ERROR("Error during the switch");
         return false;
      }
   }
   else
   {
      ROS_ERROR("Error during the call of the client");
      return false;
   }


   // Generate force through X
   ros::Publisher ForceRequest_pub = nh.advertise<geometry_msgs::Twist>("/force_command", 1000);
   geometry_msgs::Twist Force_cmd;

   Force_cmd.linear.x = 0.0;
   Force_cmd.linear.y = 0.0;
   Force_cmd.linear.z = -5.0;
   Force_cmd.angular.x = 0.0;
   Force_cmd.angular.y = 0.0;
   Force_cmd.angular.z = 0.0;



   geometry_msgs::Pose EE_Position;
   robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
   robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
   robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
   robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("panda_arm");
   ros::Subscriber joint_pos_sub = nh.subscribe("/franka_state_controller/joint_states", 10, joint_callback);
   bool cond_OK = true;

   ROS_WARN("Init. Force publishing");
   ForceRequest_pub.publish(Force_cmd);
   while(cond_OK)
   {
      kinematic_state->setJointGroupPositions(joint_model_group, joint_value);
      EE_Position = CheckEEPosition(kinematic_state);
      if(EE_Position.position.z < connectorPose.position.z + 0.01)
      {
         cond_OK = false;
         ROS_INFO("Stop to send force command");
      }
   }

   Force_cmd.linear.z = 0.0;
   ForceRequest_pub.publish(Force_cmd);



   // back to Moveit Controller
   switch_req.start_controllers.resize(1);
   switch_req.stop_controllers.resize(1);
   switch_req.start_controllers[0] = MoveItControllerName;
   switch_req.stop_controllers[0] = effortControllerName; 
   if(switch_client.call(switch_req, switch_resp))
   {
      ROS_INFO("Switch controll requested");
      if (switch_resp.ok)
         ROS_INFO("Switch completed with success");
      else
      {
         ROS_ERROR("Error during the switch");
         return false;
      }
   }
   else
   {
      ROS_ERROR("Error during the call of the client");
      return false;
   }



   hand_group.setJointValueTarget(hand_open_position);
   hand_group.move();

   move_group.setPoseTarget(PreInsertion, end_effector_frame);
   move_group.move();

   ROS_INFO("Hope that the insertion is been completed!");
}


geometry_msgs::Pose CheckEEPosition(robot_state::RobotStatePtr& kinematic_state)
{
   geometry_msgs::Pose EE_Pose;
   const Eigen::Isometry3d& end_effector_state = kinematic_state->getGlobalLinkTransform(end_effector_frame);
   Eigen::Matrix<double, 3, 1> EE_Translation = end_effector_state.translation();

   EE_Pose.position.x = EE_Translation(0);
   EE_Pose.position.y = EE_Translation(1);
   EE_Pose.position.z = EE_Translation(2);

   return EE_Pose;
}

void joint_callback(const sensor_msgs::JointState& data)
{
   for(int i=0; i<7; i++)
      joint_value[i] = data.position[i];
}

