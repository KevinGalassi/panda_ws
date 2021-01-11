/*
    Test program to grasp a wire recognized by a camera running on another node
*/



#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "robot_task/getGraspPose.h"

#include <MyFunc.h>


int main(int argc, char** argv)
{
    ros::init(argc, argv, "ol_tes2t2");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(1);
    spinner.start();

    ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_state_request",100);
    ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/gripper_width_request", 100);

    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;


//    ros::ServiceClient getGraspPose_client = nh.serviceClient<robot_task::getGraspPose>("/get_grasp_pose");
    robot_task::getGraspPose grasp_srv;


    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
//    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();


    ROS_INFO("Reach Ready Position");
    move_group.setMaxVelocityScalingFactor(0.2);
//    hand_group.setJointValueTarget(hand_ready_state);
//    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();

    std::cout << "Exec compelted \n";

    geometry_msgs::Pose target_pose;

    /*
    target_pose.position.x = 0.260;
    target_pose.position.y = 0.405 ;
    target_pose.position.z = 0.225 + 0.1;
    tf2::Quaternion quat;
    quat.setRPY(M_PI,0, -M_PI/4);
    target_pose.orientation = tf2::toMsg(quat);

    visual_tools.prompt("Next to move to stating pose");
*/

    target_pose.position.x = 0.31;
    target_pose.position.y = 0.0 + 0.2 ;
    target_pose.position.z = 0.4;

    target_pose.orientation.x = 0.7269;
    target_pose.orientation.y = -0.68649;
    target_pose.orientation.z = -0.01323;
    target_pose.orientation.w = 0.00818;

    tf2::Quaternion q_orig, q_rot, q_new;

    // Get the original orientation of 'commanded_pose'
    tf2::convert(target_pose.orientation , q_orig);
    q_orig.normalize();
    tf2::convert(q_orig, target_pose.orientation);




    visual_tools.publishAxis(target_pose, 0.1);
    visual_tools.trigger();

    visual_tools.prompt("Next to set new target frame");
    
    move_group.setPoseReferenceFrame("wire_link");

    std::cout << move_group.getPoseReferenceFrame() << "\n";

    move_group.setPoseTarget(target_pose);
    move_group.move();








/*
    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setPoseTarget(target_pose);
    move_group.move();

    cmd_msg.data = 2;
    cmd_pub.publish(cmd_msg);
    ros::Duration(1).sleep();

    float target_width = 0.08;



    getGraspPose_client.call(grasp_srv);

    target_pose = grasp_srv.target_pose;

    move_group.setPoseTarget(target_pose);
    move_group.move();


*/



    ros::shutdown();
    return 0;
}

