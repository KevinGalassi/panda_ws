#include <MyFunc.h>

#define PLANNING_ATTEMPTS_NO 500
#define INPUT_RPY 1


void printEE(robot_state::RobotStatePtr kinematic_state)
{
    const Eigen::Isometry3d& end_effector_state = kinematic_state->getGlobalLinkTransform("panda_link8");
    ROS_INFO_STREAM("Translation: " << end_effector_state.translation());
}


int  main(int argc, char** argv)
{
    ros::init(argc, argv, "PickPlace_Wire");
    ros::NodeHandle nh;

    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();

    std::string path1 = "/home/panda/ros/Franka-Kev/src/test/src/PointList/Harness_test2";

    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
    const robot_state::JointModelGroup* joint_model_group = kinematic_model->getJointModelGroup("panda_arm");
    const std::vector<std::string> &joint_names = joint_model_group->getJointModelNames();
    const Eigen::Isometry3d& end_effector_state = kinematic_state->getGlobalLinkTransform("panda_link8");

    TrajectoryVector waypoints;
    
/***************  SYSTEM PARAMETERS **************************/
    
    TrajectoryPlanner_param param;
    param.radius = 0.02;       // Radius of the semi-circle for fixing part
    param.heigh = 0.03;// Heigh of upward movement
    param.circ_point = 100; // Point generated in the semi-circle
    param.res = 0.001;  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;    // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;    // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;    // Distance of the second point
    param.corner_points = 100;

    geometry_msgs::Pose grasp1;
    geometry_msgs::PoseArray waypoints_final;

    ReadFileTxt(INPUT_RPY, waypoints.point, waypoints.pt_label, path1, grasp1);
    MyTrajectoryPlanner(param, waypoints);
    FromVector2PoseArray(waypoints, waypoints_final);
    FromEE2Link8(waypoints_final);

/**** Print result ****/

    visual_tools.deleteAllMarkers();
    visual_tools.publishPath(waypoints_final.poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    for (std::size_t i=0; i< waypoints_final.poses.size(); i++)
        visual_tools.publishAxisLabeled(waypoints_final.poses[1], "pt" , rvt::XXXSMALL);
    visual_tools.trigger();

        moveit_msgs::RobotTrajectory trajectory_cartesian;
    const double jump_threshold = 0.0;
    const double eef_step = 0.001;
    double fraction;   
    moveit::planning_interface::MoveGroupInterface::Plan Plan_Cartesian, Plan2;
 

    move_group.computeCartesianPath(waypoints_final.poses, eef_step, jump_threshold, trajectory_cartesian);
    Plan_Cartesian.trajectory_ = trajectory_cartesian;

    std::vector<double> joint_value;
    std::vector<float> joint_value_f;
    joint_value.resize(7);
    joint_value_f.resize(7);


    geometry_msgs::PoseArray EE_poses;

    for(int i=0; i<trajectory_cartesian.joint_trajectory.points.size(); i++)
    {
        for (int k=0; k<8; k++)
        {
            joint_value[k] = (double)trajectory_cartesian.joint_trajectory.points[i].positions[k];
        }

        kinematic_state->setJointGroupPositions(joint_model_group, joint_value);
      //  end_effector_state = kinematic_state->getGlobalLinkTransform("panda_link8");
      //  ROS_INFO_STREAM("Translation: " << end_effector_state.translation());

        printEE(kinematic_state);


   // ROS_INFO_STREAM("Rotation: " << end_effector_state.rotation());
/*
        for(int k=0; k<7; k++)
            joint_value_f[k] = (float)joint_value[k];

        EE_poses.poses.push_back(ForwardKinematics(joint_value_f));
*/ 
    }

    
 

    /* Print end-effector pose. Remember that this is in the model frame */
    ROS_INFO_STREAM("Translation: " << end_effector_state.translation());
    ROS_INFO_STREAM("Rotation: " << end_effector_state.rotation());


    visual_tools.deleteAllMarkers();
    visual_tools.publishPath(EE_poses.poses, rvt::RED, rvt::XXXSMALL);
  //  visual_tools.publishPath(waypoints_final.poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    visual_tools.trigger();



    ros::shutdown();
    return 1;
}