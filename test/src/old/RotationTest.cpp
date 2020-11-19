
#include "ros/ros.h"
#include <moveit_visual_tools/moveit_visual_tools.h>
#include <geometry_msgs/Pose.h>
#include "MyFunc.h"
#include <collision_object_lib.h>

#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"


#define INPUT_RPY 1

int main(int argc, char** argv)
{

    ros::init(argc,argv, "Pull_Test_node");
    ros::NodeHandle n;

   // ros::Publisher offset_pub = n.advertise<std_msgs::Float32>("/OFfset_value",1000);

    float scaling_factor;
    ros::Duration T_offset;

    namespace rvt = rviz_visual_tools;

      static const std::string PLANNING_GROUP = "panda_arm";

    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

    //const robot_state::JointModelGroup* joint_model_group = move_group.getCurrentState()->getJointModelGroup("panda_arm");

    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
    robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("panda_arm");

    std::cout << "OK \n";

    moveit_visual_tools::MoveItVisualTools vt("panda_link0");
    vt.deleteAllMarkers();

    std::vector<std::string> collision_obj_name;
    collision_obj_name.resize(1);
    collision_obj_name[0] = addRobotBase(planning_scene_interface);

    ros::Publisher path_code_pub = n.advertise<std_msgs::Int32>("/PathExecuted",1000);
    std_msgs::Int32 path_code_msg;

/*** Path Creation ***/


    std::vector<double> config;
    config.resize(7);


    geometry_msgs::Pose target_pose;

    target_pose.position.x = 0.1;
    target_pose.position.y = 0.55;
    target_pose.position.z = 0.3;

    tf2::Quaternion quat;


    move_group.setMaxVelocityScalingFactor(0.1);
    move_group.setMaxAccelerationScalingFactor(0.1);


    if(1)
    {
        quat.setRPY(M_PI,0,-M_PI/4);
        target_pose.orientation = tf2::toMsg(quat);

        move_group.setPoseTarget(target_pose);
        move_group.move();
        double gripper_offset = 0;
        std_msgs::Float32 offset_msg;
    }
    
    moveit::core::RobotStatePtr current_state = move_group.getCurrentState();
    //
    // Next get the current set of joint values for the group.
    std::vector<double> joint_group_positions;
    std::cout << "get current state \n"
    current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);


    joint_group_positions[6] = joint_group_positions[6] + M_PI/4;  // radians
    move_group.setJointValueTarget(joint_group_positions);  

    int i = 1;


    while(ros::ok())
    {
        std::cout << "Rotation";
        move_group.move();

        if(i==1)
            joint_group_positions[6] = joint_group_positions[6] - M_PI/2;
        else
            joint_group_positions[6] = joint_group_positions[6] + M_PI/2;

        move_group.setJointValueTarget(joint_group_positions);

        i = 1 -i;
    }


    // Now, let's modify one of the joints, plan to the new joint space goal and visualize the plan.


    
/*

    vt.prompt("Second Part");

    geometry_msgs::Pose target_pose2;
    target_pose2.position.x = 0;
    target_pose2.position.y = 0.5;
    target_pose2.position.z = 0.3;

    quat.setRPY(M_PI,0,M_PI/4);
    target_pose2.orientation = tf2::toMsg(quat);
    move_group.setPoseTarget(target_pose2);
    move_group.move();

*/



/*** ESECUZIONE ***/
    /*
    while(ros::ok())
    {
        vt.prompt("Next turn");
        gripper_offset = gripper_offset + 5*M_PI/180;
        config[6] = config[6] + gripper_offset;
       // offset_msg.data = gripper_offset;
        //offset_pub.publish(offset_msg);
        move_group.setJointValueTarget(config);
        move_group.move();

    }
    */
    // Remove collision object
    planning_scene_interface.removeCollisionObjects(collision_obj_name);


    return 1;
}



