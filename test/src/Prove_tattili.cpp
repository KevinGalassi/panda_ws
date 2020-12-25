/*
    Test program that open and clsoe the gripper
*/



#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>


#include <franka_gripper/GraspAction.h>
#include <franka_gripper/HomingAction.h>
#include <franka_gripper/MoveAction.h>
#include <franka_gripper/StopAction.h>
#include <actionlib/client/simple_action_client.h>

#include <MyFunc.h>


int main(int argc, char** argv)
{
    ros::init(argc, argv, "ol_tes2t2");
    ros::NodeHandle nh;

    ros::AsyncSpinner spinner(1);
    spinner.start();

    ros::Publisher cmd_pub = nh.advertise<std_msgs::Float32>("/gripper_command",100);
    ros::Publisher width_pub = nh.advertise<std_msgs::Float32>("/My_new_input", 100);

    std_msgs::Float32 width_msg;
    std_msgs::Float32 cmd_msg;

    namespace rvt = rviz_visual_tools;


    actionlib::SimpleActionClient<franka_gripper::MoveAction> move_client("/franka_gripper/move", true);
    actionlib::SimpleActionClient<franka_gripper::GraspAction> grasp_client("/franka_gripper/grasp", true);
    actionlib::SimpleActionClient<franka_gripper::StopAction> stop_client("/franka_gripper/stop", true);

    franka_gripper::MoveGoal move_goal;
    move_goal.speed = 0.02;
    move_goal.width = 0.08;
    
    std::vector<double> finger_width;
    finger_width.resize(2);
    float user_input;

    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();


    ROS_INFO("Reach Ready Position");
    move_group.setMaxVelocityScalingFactor(0.2);

    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setJointValueTarget(arm_ready_state);
    move_group.move();


    geometry_msgs::Pose target_pose;
    target_pose.position.x = 0.260;
    target_pose.position.y = 0.405 ;
    target_pose.position.z = 0.225 + 0.1;
    tf2::Quaternion quat;
    quat.setRPY(M_PI,0, -M_PI/4);
    target_pose.orientation = tf2::toMsg(quat);

    visual_tools.prompt("Next to move to stating pose");
    
    hand_group.setJointValueTarget(hand_ready_state);
    hand_group.move();
    move_group.setPoseTarget(target_pose);
    move_group.move();

    cmd_msg.data = 2;
    cmd_pub.publish(cmd_msg);
    ros::Duration(1).sleep();

float target_width = 0.08;


    while(true)
    {
    visual_tools.prompt(" ' Next' to grasp");
    width_msg.data = 0.003;

    width_pub.publish(width_msg);
                move_client.cancelAllGoals();
                target_width = 0.003;
                move_goal.width = target_width;

                move_client.sendGoal(move_goal);


    visual_tools.prompt("'Next' to open ");
                move_client.cancelAllGoals();
                target_width = 0.04;
                move_goal.width = target_width;

                move_client.sendGoal(move_goal);


    visual_tools.prompt(" ' Next' to grasp");
                    move_client.cancelAllGoals();
                target_width = 0.005;
                move_goal.width = target_width;

                move_client.sendGoal(move_goal);

    visual_tools.prompt("'Next' to open ");
                move_client.cancelAllGoals();
                target_width = 0.04;
                move_goal.width = target_width;

                move_client.sendGoal(move_goal);

    std::cout << "nuovo ciclo";

    }





    ros::shutdown();
    return 0;
}

