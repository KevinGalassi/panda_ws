
#include <ros/ros.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>

#include <controller_manager_msgs/SwitchController.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_visual_tools/moveit_visual_tools.h>

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion
#include <math.h>

bool CheckExitCondition(float distance, geometry_msgs::Pose start_pose, geometry_msgs::Pose EE_Pos);
moveit_msgs::RobotTrajectory VelocityScaling(moveit_msgs::RobotTrajectory trajectory_cartesian, geometry_msgs::Pose starting_point, geometry_msgs::Pose ending_point, float velocity, ros::Duration T_offset);
float ComputeDistance(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2);
moveit_msgs::RobotTrajectory FactorScaling(moveit_msgs::RobotTrajectory trajectory_cartesian, float scaling_factor, ros::Duration T_offset);


int main(int argc, char** argv)
{
    ros::init(argc, argv, "Tactile_Sensor_Testing_node");

    ros::NodeHandle n_;

    std::string file_path;
    float open_width = 0.08;
    bool moveit_on = true;
    ros::Duration T_offset;
    T_offset = ros::Duration(0.1);

    n_.getParam("TactileSensorTest/InputPath", file_path);
    n_.getParam("TactileSensorTest/GripperOpening", open_width);
    n_.getParam("TactileSensorTest/Moveit_on", moveit_on);

    ros::Rate(60);

// Moviet
    moveit::planning_interface::MoveGroupInterface move_group("panda_arm");
    moveit::planning_interface::MoveGroupInterface hand_group("hand");
    moveit_visual_tools::MoveItVisualTools visual_tools("panda_link0");
    visual_tools.deleteAllMarkers();

    std::vector<moveit_msgs::RobotTrajectory> trajectory_vector;
    moveit::planning_interface::MoveGroupInterface::Plan robot_plan;
    std::vector<double> closed_position  = {0.0, 0.0};
    std::vector<double> open_position  = {open_width/2, open_width};

    double jump_threshold = 0.0;
    double eef_step = 0.001;
    double fraction;   


// Senza Moveit

    ros::ServiceClient switch_client = n_.serviceClient<controller_manager_msgs::SwitchController>("/controller_manager/switch_controller");
    controller_manager_msgs::SwitchController::Request switch_req;
    controller_manager_msgs::SwitchController::Response switch_resp;
    switch_resp.ok = false;
    std::string pos_control = "position_joint_trajectory_controller";
    std::string vel_control = "cartesian_velocity_controller";

    ros::Publisher vel_pub = n_.advertise<std_msgs::Float32MultiArray>("/cartesian_velocity_request",1);
    std_msgs::Float32MultiArray vel_msg;
    vel_msg.data.resize(6);

    float distance;
    float exec_time;
    geometry_msgs::Pose EE_Position;
    bool loop_flag;


// continuo
    std::vector<geometry_msgs::Pose> starting_point;
    std::vector<geometry_msgs::Pose> ending_point;
    std::vector<int> repetition_no;
    int repetition_counter = 0;
    std::vector<float> gripper_width;
    std::vector<float> velocity;



    // Input reading
    std::ifstream txt_file;
    
    txt_file.open(file_path);

    if(txt_file.is_open())
        ROS_INFO("File opened correctly");
    else
    {
        ROS_ERROR("File not opened correctly");
        return false;
    }

    int counter = 0;
    int rep_no;
    float gpr_wdt;
    geometry_msgs::Pose new_pose;
    float input_RPY[3] = {0, 0, 0};
    tf2::Quaternion input_quaternion;
    while(!txt_file.eof())
    {
        counter ++;
        std::cout << "Line " << counter << " acquired! \n";

        txt_file >> new_pose.position.x;
        txt_file >> new_pose.position.y;
        txt_file >> new_pose.position.z;
        txt_file >> input_RPY[0];
        txt_file >> input_RPY[1];
        txt_file >> input_RPY[2];
        input_quaternion.setRPY(input_RPY[0], input_RPY[1], input_RPY[2]);
        new_pose.orientation = tf2::toMsg(input_quaternion);
        starting_point.push_back(new_pose);

        txt_file >> new_pose.position.x;
        txt_file >> new_pose.position.y;
        txt_file >> new_pose.position.z;
        txt_file >> input_RPY[0];
        txt_file >> input_RPY[1];
        txt_file >> input_RPY[2];
        input_quaternion.setRPY(input_RPY[0], input_RPY[1], input_RPY[2]);
        new_pose.orientation = tf2::toMsg(input_quaternion);
        ending_point.push_back(new_pose);

        txt_file >> rep_no;
        repetition_no.push_back(rep_no);
        txt_file >> gpr_wdt;
        gripper_width.push_back(gpr_wdt);
        txt_file >> gpr_wdt;
        velocity.push_back(gpr_wdt);
    }
    


    trajectory_vector.resize(starting_point.size());
    geometry_msgs::PoseArray waypoints;
    waypoints.poses.resize(2);

    for(int i=0; i<trajectory_vector.size(); i++)
    {
        waypoints.poses[0] = starting_point[i];
        waypoints.poses[1] = ending_point[i];
        move_group.computeCartesianPath(waypoints.poses, eef_step, jump_threshold, trajectory_vector[i]);
        trajectory_vector[i] = VelocityScaling(trajectory_vector[i], starting_point[i], ending_point[i], velocity[i], T_offset);

    }


    for(int i=0; i< starting_point.size(); i++)
    {
        robot_plan.trajectory_ = trajectory_vector[i];
        std::cout << "Esecuzione punto numero: " << i+1 << "\n";
        for(int j=0; j<repetition_no[i]; j++)
        {
            std::cout << "Esecuzione ripetizione numero: " << j+1 << "\n";
            
            new_pose = starting_point[i];
            new_pose.position.z += 0.05;
            move_group.setPoseTarget(new_pose);
            move_group.move();

            move_group.setPoseTarget(starting_point[i]);
            move_group.move();

            closed_position[0] = gripper_width[i]/(float)2.0;
            closed_position[1] = gripper_width[i]/(float)2.0;

            hand_group.setJointValueTarget(closed_position);
            hand_group.move();
            move_group.execute(robot_plan);
            hand_group.setJointValueTarget(open_position);
            hand_group.move();
            
            new_pose = ending_point[i];
            new_pose.position.z += 0.05;
            move_group.setPoseTarget(new_pose);
            move_group.move();
        }
    }

    // WHILE LOOP CARTESIAN CONTROLLER




    for(int i=0; i< starting_point.size(); i++)
    {
        std::cout << "Esecuzione punto numero: " << i+1 << "\n";
        for(int j=0; j<repetition_no[i]; j++)
        {
            std::cout << "Esecuzione ripetizione numero: " << j+1 << "\n";
            // Chaimata per lanciare controllo di posiione

            ROS_INFO("Switch controll to velocity");
            switch_req.start_controllers[0] = pos_control;
            switch_req.stop_controllers[0] = vel_control; 
            if(switch_client.call(switch_req,switch_resp))
            {
                if (switch_resp.ok)
                    ROS_INFO("Switch completed with success");
                else
                    ROS_ERROR("Error during the switch");
            }
            else
                ROS_ERROR("Error during the call of the client");     
            
            // raggiungere starting point
            new_pose = starting_point[i];
            new_pose.position.z += 0.05;
            move_group.setPoseTarget(new_pose);
            move_group.move();
            move_group.setPoseTarget(starting_point[i]);
            move_group.move();
            // Close gripper
            closed_position[0] = gripper_width[i]/2.0;
            closed_position[1] = gripper_width[i]/2.0;
            hand_group.setJointValueTarget(closed_position);
            hand_group.move();
            
            // Azionare controllo di velocittà
            ROS_INFO("Switch controll to velocity");
            switch_req.start_controllers[0] = vel_control;
            switch_req.stop_controllers[0] = pos_control; 
            if(switch_client.call(switch_req,switch_resp))
            {
                if (switch_resp.ok)
                    ROS_INFO("Switch completed with success");
                else
                    ROS_ERROR("Error during the switch");
            }
            else
                ROS_ERROR("Error during the call of the client");    

            // Muoversi

            distance = ComputeDistance(starting_point[i], ending_point[i]);
            exec_time = distance/velocity[i];
            vel_msg.data[0] = (ending_point[i].position.x - starting_point[i].position.x )/exec_time;
            vel_msg.data[1] = (ending_point[i].position.y - starting_point[i].position.y )/exec_time;
            vel_msg.data[2] = (ending_point[i].position.z - starting_point[i].position.z )/exec_time;

            /*
            vel_msg.data[0] = (ending_point[i]. - starting_point[i].position )/exec_time;
            vel_msg.data[0] = (ending_point[i].position. - starting_point[i].position )/exec_time;
            vel_msg.data[0] = (ending_point[i].position. - starting_point[i].position )/exec_time;
            */
            vel_pub.publish(vel_msg);

            while(loop_flag)
            {
                EE_Position = move_group.getCurrentPose().pose;
                loop_flag = CheckExitCondition(distance, starting_point[i], EE_Position);
            }
            for(int i=0; i<6; i++)
                vel_msg.data[i] = 0.0;
            vel_pub.publish(vel_msg);
            ros::Duration(0.1).sleep();
            

            // Azionare controllo di posizione
            ROS_INFO("Switch controll to velocity");
            switch_req.start_controllers[0] = pos_control;
            switch_req.stop_controllers[0] = vel_control; 
            if(switch_client.call(switch_req,switch_resp))
            {
                if (switch_resp.ok)
                    ROS_INFO("Switch completed with success");
                else
                    ROS_ERROR("Error during the switch");
            }
            else
                ROS_ERROR("Error during the call of the client");    



            // apertura
            hand_group.setJointValueTarget(open_position);
            hand_group.move();
            new_pose = ending_point[i];
            new_pose.position.z += 0.05;
            move_group.setPoseTarget(new_pose);
            move_group.move();

            // fine;
        }

    }
    

    ros::shutdown();
    return 0;
}




moveit_msgs::RobotTrajectory VelocityScaling(moveit_msgs::RobotTrajectory trajectory_cartesian, geometry_msgs::Pose starting_point, geometry_msgs::Pose ending_point, float velocity, ros::Duration T_offset)
{
    moveit_msgs::RobotTrajectory final_trajectory = trajectory_cartesian;
    float distance = ComputeDistance(starting_point, ending_point);
    ros::Duration total_time = ros::Duration(distance/velocity);
    float scaling_factor = total_time.toSec()/final_trajectory.joint_trajectory.points[(final_trajectory.joint_trajectory.points.size()-1)].time_from_start.toSec();   
    final_trajectory = FactorScaling(final_trajectory, scaling_factor, T_offset);
    
    return final_trajectory;
}

moveit_msgs::RobotTrajectory FactorScaling(moveit_msgs::RobotTrajectory trajectory_cartesian, float scaling_factor, ros::Duration T_offset)
{
    moveit_msgs::RobotTrajectory new_trajectory;
    trajectory_msgs::JointTrajectoryPoint Traj_Point;

    for(size_t i=0; i<trajectory_cartesian.joint_trajectory.points.size(); i++)
    {
        new_trajectory.joint_trajectory.points.push_back(trajectory_cartesian.joint_trajectory.points[i]);
    }
    new_trajectory.joint_trajectory.joint_names.resize(7);
    new_trajectory.joint_trajectory.joint_names = trajectory_cartesian.joint_trajectory.joint_names;

    for (size_t i=0; i<new_trajectory.joint_trajectory.points.size(); i++)
    {
        for(int k=0; k<7; k++)
        {
            new_trajectory.joint_trajectory.points[i].velocities[k] /= scaling_factor;
            new_trajectory.joint_trajectory.points[i].accelerations[k] /= (scaling_factor*scaling_factor);
        }
        new_trajectory.joint_trajectory.points[i].time_from_start *= scaling_factor;
    }


    Traj_Point.time_from_start = ros::Duration(0);
    Traj_Point.positions.resize(7);
    Traj_Point.velocities.resize(7);
    Traj_Point.accelerations.resize(7);

    for(int i=0; i<7; i++)
    {
        Traj_Point.positions[i] = new_trajectory.joint_trajectory.points[0].positions[i];
        Traj_Point.velocities[i] = 0;
        Traj_Point.accelerations[i] = 0;
    }

    for (size_t i=0; i<new_trajectory.joint_trajectory.points.size(); i++)
    {
        new_trajectory.joint_trajectory.points[i].time_from_start += T_offset;
    }

    for(size_t i=1; i<new_trajectory.joint_trajectory.points.size(); i++)
    {
        if(new_trajectory.joint_trajectory.points[i].time_from_start <= new_trajectory.joint_trajectory.points[i-1].time_from_start)
        {
            new_trajectory.joint_trajectory.points[i].time_from_start = new_trajectory.joint_trajectory.points[i-1].time_from_start + ros::Duration(0.00001);
        }
    }

    new_trajectory.joint_trajectory.points.insert(new_trajectory.joint_trajectory.points.begin(), Traj_Point);
    new_trajectory.joint_trajectory.points[new_trajectory.joint_trajectory.points.size()-1].time_from_start += T_offset;

    return new_trajectory;
}


float ComputeDistance(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2)
{
    float distance;

    distance = sqrt( 
                        pow(Point2.position.x - Point1.position.x, 2) + 
                        pow(Point2.position.y - Point1.position.y, 2) +
                        pow(Point2.position.z - Point1.position.z, 2)) ;

    return distance;
}

bool CheckExitCondition(float distance, geometry_msgs::Pose start_pose, geometry_msgs::Pose EE_Pos)
{
    float actual_dist;

    actual_dist = ComputeDistance(start_pose, EE_Pos);

    if(distance - actual_dist < 0.01)
        return false;       // Exit from the loop
    else
        return true;
}
