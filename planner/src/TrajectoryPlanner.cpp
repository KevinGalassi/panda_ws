#include "TrajectoryPlanner.h"


TrajectoryPlanner::TrajectoryPlanner(){}
TrajectoryPlanner::~TrajectoryPlanner(){}
// Public

void TrajectoryPlanner::PlannerInit()
{
    PlannerReset();

    param.radius = 0.05;                // Radius of the semi-circle for fixing part
    param.heigh = 0.03;                 // Heigh of upward movement
    param.circ_point = 1000;             // Point generated in the semi-circle
    param.res = 0.001;                  // Distance beetween two following points in the final trajectory
    param.distance_approach = 0.05;     // Distance from which starts the grasping operation
    param.Ctrl_pt_d1 = 0.05;            // Distance of the first control point in the rounding function
    param.Ctrl_pt_d2 = 0.05;            // Distance of the second point
    param.corner_points = 100;          // Numver of point generated in the corner.
    
    param.dist_init[0] = 0.05;
    param.dist_init[1] = 0;
    param.dist_init[2] = 0;

    param.dist_final[0] = 0.05;
    param.dist_final[1] = 0;
    param.dist_final[2] = 0;

    param.gripper_offset[0] = 0;
    param.gripper_offset[1] = 0;
    param.gripper_offset[2] = 0;
    
    param.orientation = true;
    param.z_offset = 0.02; // Offset con cui si abbassa il punto iniziale
    param.EE_shift.setRPY(0,0,M_PI/4);
    param.EE_R.setRotation(param.EE_shift);

}
void TrajectoryPlanner::PlannerReset()
{
    InitPoint.poses.clear();
    InitLabel.clear();
    SecondaryTrajectories.clear();
    Trajectory.poses.clear();
    label_list.clear();

}
bool TrajectoryPlanner::ReadFileTxt()
{
    std::ifstream txt_file;
    geometry_msgs::Pose target_goal;
    std::string pose_identifie;

    txt_file.open(file_path);
    if( txt_file.is_open())
    {
        ROS_INFO("File succesfully open");
    }
    else
    {
        ROS_ERROR("file not open");
        return false;
    }

    if(input_rpy)
    {
        float input_RPY[3] = {0, 0, 0};
        tf2::Quaternion input_quaternion;
        while ( !txt_file.eof() )
        { 
            std::cout << "data readen \n";
            txt_file >> target_goal.position.x;
            txt_file >> target_goal.position.y;
            txt_file >> target_goal.position.z;
            txt_file >> input_RPY[0];
            txt_file >> input_RPY[1];
            txt_file >> input_RPY[2];
            input_quaternion.setRPY(input_RPY[0], input_RPY[1], input_RPY[2]);
            target_goal.orientation = tf2::toMsg(input_quaternion);
            txt_file >> pose_identifie;

            InitPoint.poses.push_back(target_goal);
            InitLabel.push_back(pose_identifie);
        }
    }
    else
    {
        while ( !txt_file.eof() )
        { 
            std::cout << "data readen \n";
            txt_file >> target_goal.position.x;
            txt_file >> target_goal.position.y;
            txt_file >> target_goal.position.z;
            txt_file >> target_goal.orientation.w;
            txt_file >> target_goal.orientation.x;
            txt_file >> target_goal.orientation.y;
            txt_file >> target_goal.orientation.z;
            txt_file >> pose_identifie;

            InitPoint.poses.push_back(target_goal);
            InitLabel.push_back(pose_identifie);
        }
    }
    txt_file.close();
    return true;
}

void TrajectoryPlanner::FromEE2Link8()
{
    geometry_msgs::Pose target_pose;
    tf2::Matrix3x3 R, R_shift, R_final;
    tf2::Quaternion quat;
    tf2::Vector3 axis;

    quat.setRPY(0,0,M_PI/4);
    R_shift.setRotation(quat);

    for(size_t j=0; j<SecondaryTrajectories.size(); j++)
    {
        for (size_t i=0; i < SecondaryTrajectories[j].poses.size(); i++)
        {
            quat.setX(SecondaryTrajectories[j].poses[i].orientation.x);
            quat.setY(SecondaryTrajectories[j].poses[i].orientation.y);
            quat.setZ(SecondaryTrajectories[j].poses[i].orientation.z);
            quat.setW(SecondaryTrajectories[j].poses[i].orientation.w);
            R.setRotation(quat);

            target_pose.position.x = SecondaryTrajectories[j].poses[i].position.x -( R[0][0]*param.gripper_offset[0] + R[0][1]*param.gripper_offset[1] + R[0][2]*param.gripper_offset[2]) ;
            target_pose.position.y = SecondaryTrajectories[j].poses[i].position.y -( R[1][0]*param.gripper_offset[0] + R[1][1]*param.gripper_offset[1] + R[1][2]*param.gripper_offset[2]) ;
            target_pose.position.z = SecondaryTrajectories[j].poses[i].position.z -( R[2][0]*param.gripper_offset[0] + R[2][1]*param.gripper_offset[1] + R[2][2]*param.gripper_offset[2]) ;

            R_final = R*param.EE_R;
            R_final.getRotation(quat);
            quat.normalize();
            target_pose.orientation = tf2::toMsg(quat);

            SecondaryTrajectories[j].poses[i].position    = target_pose.position;
            SecondaryTrajectories[j].poses[i].orientation = target_pose.orientation;    
        }
    }
    return;
}

void TrajectoryPlanner::FromLink82EE(){}

// Private

void TrajectoryPlanner::addFixPoint1(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints)
{
    geometry_msgs::Pose target_pose;
    geometry_msgs::Point center;

    tf2::Matrix3x3 Rot, R;
    tf2::Quaternion quat;

    quat.setX(fixing_point.orientation.x);
    quat.setY(fixing_point.orientation.y);
    quat.setZ(fixing_point.orientation.z);
    quat.setW(fixing_point.orientation.w);
    R.setRotation(quat);

    //First point
    target_pose.position.x = fixing_point.position.x - (R[0][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][2]*param.dist_init[2]);
    target_pose.position.y = fixing_point.position.y - (R[1][0]*param.dist_init[0] + R[1][1]*param.dist_init[1] + R[1][2]*param.dist_init[2]);
    target_pose.position.z = fixing_point.position.z - (R[2][0]*param.dist_init[0] + R[2][1]*param.dist_init[1] + R[2][2]*param.dist_init[2]);
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);
    
    target_pose.position.x = fixing_point.position.x + R[0][1]*param.radius*2;
    target_pose.position.y = fixing_point.position.y + R[1][1]*param.radius*2;
    target_pose.position.z = fixing_point.position.z + R[2][1]*param.radius*2;
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);


    // lateral shift of the diameter of the circle
    target_pose.position.x = fixing_point.position.x + R[0][1]*param.radius*2;
    target_pose.position.y = fixing_point.position.y + R[1][1]*param.radius*2;
    target_pose.position.z = fixing_point.position.z + R[2][1]*param.radius*2;
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);

    // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
    center.x = fixing_point.position.x + R[0][1]*param.radius - R[0][2]*param.heigh;
    center.y = fixing_point.position.y + R[1][1]*param.radius - R[1][2]*param.heigh;
    center.z = fixing_point.position.z + R[2][1]*param.radius - R[2][2]*param.heigh;

    // Upshift
    target_pose.position.x = target_pose.position.x - R[0][2]*param.heigh;
    target_pose.position.y = target_pose.position.y - R[1][2]*param.heigh;
    target_pose.position.z = target_pose.position.z - R[2][2]*param.heigh;
    waypoints.poses.push_back(target_pose);

    for(int k = 0; k < param.res; k++)
    {
        target_pose.position.x = center.x + R[0][1]*param.radius*cos(M_PI/param.res*k) - R[0][2]*param.radius*sin(M_PI/param.res*k);
        target_pose.position.y = center.y + R[1][1]*param.radius*cos(M_PI/param.res*k) - R[1][2]*param.radius*sin(M_PI/param.res*k);
        target_pose.position.z = center.z + R[2][1]*param.radius*cos(M_PI/param.res*k) - R[2][2]*param.radius*sin(M_PI/param.res*k);
        waypoints.poses.push_back(target_pose);
    }

    target_pose.position.x = fixing_point.position.x - R[0][2]*param.heigh;
    target_pose.position.y = fixing_point.position.y - R[1][2]*param.heigh;
    target_pose.position.z = fixing_point.position.z - R[2][2]*param.heigh;
    waypoints.poses.push_back(target_pose);

    // Downshift
    target_pose.position.x = fixing_point.position.x;
    target_pose.position.y = fixing_point.position.y;
    target_pose.position.z = fixing_point.position.z;
    waypoints.poses.push_back(target_pose);

    target_pose.position.x = fixing_point.position.x + (R[0][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][1]*param.dist_init[2]);
    target_pose.position.y = fixing_point.position.y + (R[1][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][1]*param.dist_init[2]);
    target_pose.position.z = fixing_point.position.z + (R[2][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][1]*param.dist_init[2]);
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);

}
void TrajectoryPlanner::addFixPoint2(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints)
{
    /**
     * Create a trajectory to fix a point
     * 
     * The trajectory is created starting from the pose "fixing_point" specified and added in "waypoints" through .push_back
     * The movements performed are:
     *      lateral right shift of "2*param.radius" distance
     *      raise of "heigh"
     *      Circular movement to position over the fix point
     *      lower
     * 
     * orientation = true  avvicinamento da destra
     * orientation = false avvicinamento da sinistra
     * 
     *      * CON SPOSTAMENTO IN AVANTI

     */

    geometry_msgs::Pose target_pose;
    geometry_msgs::Point center;

    tf2::Matrix3x3 R;
    tf2::Quaternion quat;

    quat.setX(fixing_point.orientation.x);
    quat.setY(fixing_point.orientation.y);
    quat.setZ(fixing_point.orientation.z);
    quat.setW(fixing_point.orientation.w);
    R.setRotation(quat);

    target_pose.position.x = fixing_point.position.x - (R[0][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][2]*param.dist_init[2]);
    target_pose.position.y = fixing_point.position.y - (R[1][0]*param.dist_init[0] + R[1][1]*param.dist_init[1] + R[1][2]*param.dist_init[2]);
    target_pose.position.z = fixing_point.position.z - (R[2][0]*param.dist_init[0] + R[2][1]*param.dist_init[1] + R[2][2]*param.dist_init[2]) - param.z_offset;    // !!!
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);

    if (param.orientation)
    {
        // lateral shift of the diameter of the circle
        target_pose.position.x = fixing_point.position.x + R[0][1]*param.radius*2;
        target_pose.position.y = fixing_point.position.y + R[1][1]*param.radius*2;
        target_pose.position.z = fixing_point.position.z + R[2][1]*param.radius*2 - param.z_offset;
        // target_pose.position.z = fixing_point.position.z + R[2][1]*param.radius*2;
        target_pose.orientation = fixing_point.orientation;
        waypoints.poses.push_back(target_pose);

        // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
        center.x = fixing_point.position.x + R[0][1]*param.radius - R[0][2]*param.heigh;
        center.y = fixing_point.position.y + R[1][1]*param.radius - R[1][2]*param.heigh;
        center.z = fixing_point.position.z + R[2][1]*param.radius - R[2][2]*param.heigh;
        
        for(int k = 0; k < param.res; k++)
        {
            target_pose.position.x = center.x + R[0][1]*param.radius*cos(M_PI/param.res*k) - R[0][2]*param.radius*sin(M_PI/param.res*k) + (R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2])*k/param.res ; // Piccolo movimento in avanti
            target_pose.position.y = center.y + R[1][1]*param.radius*cos(M_PI/param.res*k) - R[1][2]*param.radius*sin(M_PI/param.res*k) + (R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2])*k/param.res;
            target_pose.position.z = center.z + R[2][1]*param.radius*cos(M_PI/param.res*k) - R[2][2]*param.radius*sin(M_PI/param.res*k) + (R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2])*k/param.res;
            waypoints.poses.push_back(target_pose);
        }
    }
    else
    {
        // lateral shift of the diameter of the circle
        target_pose.position.x = fixing_point.position.x - R[0][1]*param.radius*2;
        target_pose.position.y = fixing_point.position.y - R[1][1]*param.radius*2;
        target_pose.position.z = fixing_point.position.z - R[2][1]*param.radius*2 - param.z_offset;
        target_pose.orientation = fixing_point.orientation;
        waypoints.poses.push_back(target_pose);

        // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
        center.x = fixing_point.position.x - R[0][1]*param.radius - R[0][2]*param.heigh;
        center.y = fixing_point.position.y - R[1][1]*param.radius - R[1][2]*param.heigh;
        center.z = fixing_point.position.z - R[2][1]*param.radius - R[2][2]*param.heigh;
        
        // Upshift
        target_pose.position.x = target_pose.position.x - R[0][2]*param.heigh;
        target_pose.position.y = target_pose.position.y - R[1][2]*param.heigh;
        target_pose.position.z = target_pose.position.z - R[2][2]*param.heigh;
        waypoints.poses.push_back(target_pose);

        for(int k = 0; k < param.res; k++)
        {
            target_pose.position.x = center.x - R[0][1]*param.radius*cos(M_PI/param.res*k) - R[0][2]*param.radius*sin(M_PI/param.res*k) + (R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2])*k/param.res ; // Piccolo movimento in avanti
            target_pose.position.y = center.y - R[1][1]*param.radius*cos(M_PI/param.res*k) - R[1][2]*param.radius*sin(M_PI/param.res*k) + (R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2])*k/param.res;
            target_pose.position.z = center.z - R[2][1]*param.radius*cos(M_PI/param.res*k) - R[2][2]*param.radius*sin(M_PI/param.res*k) + (R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2])*k/param.res;
            waypoints.poses.push_back(target_pose);
        }
    }

    target_pose.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
    target_pose.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
    target_pose.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
    waypoints.poses.push_back(target_pose);

    // Downshift
    target_pose.position.x = fixing_point.position.x + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
    target_pose.position.y = fixing_point.position.y + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
    target_pose.position.z = fixing_point.position.z + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
    waypoints.poses.push_back(target_pose);

}


void TrajectoryPlanner::addFixPoint3(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints)
{
    /**
     * Create a trajectory to fix a point
     * 
     * The trajectory is created starting from the pose "fixing_point" specified and added in "waypoints" through .push_back
     * The movements performed are:
     *      lateral right shift of "2*param.radius" distance
     *      raise of "heigh"
     *      Circular movement to position over the fix point
     *      lower
     * 
     * orientation = true  avvicinamento da destra
     * orientation = false avvicinamento da sinistra
     * 
     * 
     */

    geometry_msgs::Pose target_pose;
    geometry_msgs::Point center;

    tf2::Matrix3x3 R;
    tf2::Quaternion quat;

    quat.setX(fixing_point.orientation.x);
    quat.setY(fixing_point.orientation.y);
    quat.setZ(fixing_point.orientation.z);
    quat.setW(fixing_point.orientation.w);
    R.setRotation(quat);

    target_pose.position.x = fixing_point.position.x - (R[0][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][2]*param.dist_init[2]);
    target_pose.position.y = fixing_point.position.y - (R[1][0]*param.dist_init[0] + R[1][1]*param.dist_init[1] + R[1][2]*param.dist_init[2]);
    target_pose.position.z = fixing_point.position.z - (R[2][0]*param.dist_init[0] + R[2][1]*param.dist_init[1] + R[2][2]*param.dist_init[2]);
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);

    // lateral shift of the diameter of the circle
    target_pose.position.x = fixing_point.position.x - R[0][1]*param.radius*2 + (R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2]);
    target_pose.position.y = fixing_point.position.y - R[1][1]*param.radius*2 + (R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2]);
    target_pose.position.z = fixing_point.position.z - R[2][1]*param.radius*2 + (R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2]);
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);
       
    // Upshift (Earlier point + heigh)
    target_pose.position.x = target_pose.position.x - R[0][2]*param.heigh;
    target_pose.position.y = target_pose.position.y - R[1][2]*param.heigh;
    target_pose.position.z = target_pose.position.z - R[2][2]*param.heigh;
    waypoints.poses.push_back(target_pose);

    // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
    center.x = target_pose.position.x + R[0][1]*param.radius;
    center.y = target_pose.position.y + R[1][1]*param.radius;
    center.z = target_pose.position.z + R[2][1]*param.radius;
        

    int point_no;
    geometry_msgs::Pose first_circle_point;
    geometry_msgs::Pose last_circle_point;
    first_circle_point.position.x = target_pose.position.x - R[0][2]*param.heigh;
    first_circle_point.position.y = target_pose.position.y - R[1][2]*param.heigh;
    first_circle_point.position.z = target_pose.position.z - R[2][2]*param.heigh;
    last_circle_point.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
    last_circle_point.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
    last_circle_point.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
  
    point_no = (ComputeDistance(first_circle_point, last_circle_point)*param.radius*M_PI)/param.res;

    for(int k = 0; k < point_no; k++)
    {
        target_pose.position.x = center.x - R[0][1]*param.radius*cos(M_PI/point_no*k) - R[0][2]*param.radius*sin(M_PI/point_no*k) + (R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2])*k/point_no;
        target_pose.position.y = center.y - R[1][1]*param.radius*cos(M_PI/point_no*k) - R[1][2]*param.radius*sin(M_PI/point_no*k) + (R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2])*k/point_no;
        target_pose.position.z = center.z - R[2][1]*param.radius*cos(M_PI/point_no*k) - R[2][2]*param.radius*sin(M_PI/point_no*k) + (R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2])*k/point_no;
        waypoints.poses.push_back(target_pose);
    }


    target_pose.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
    target_pose.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
    target_pose.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
    waypoints.poses.push_back(target_pose);

    // Downshift
    target_pose.position.x = fixing_point.position.x + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
    target_pose.position.y = fixing_point.position.y + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
    target_pose.position.z = fixing_point.position.z + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
    waypoints.poses.push_back(target_pose);

}

void TrajectoryPlanner::addFixPoint4(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints)
{
    /**
     * Create a trajectory to fix a point
     * 
     * The trajectory is created starting from the pose "fixing_point" specified and added in "waypoints" through .push_back
     * The movements performed are:
     *      lateral right shift of "2*param.radius" distance
     *      raise of "heigh"
     *      Circular movement to position over the fix point
     *      lower
     * 
     * orientation = true  avvicinamento da destra
     * orientation = false avvicinamento da sinistra
     * 
     * 
     */

    geometry_msgs::Pose target_pose;
    geometry_msgs::Point center;

    tf2::Matrix3x3 R;
    tf2::Quaternion quat;

    quat.setX(fixing_point.orientation.x);
    quat.setY(fixing_point.orientation.y);
    quat.setZ(fixing_point.orientation.z);
    quat.setW(fixing_point.orientation.w);
    R.setRotation(quat);

    target_pose.position.x = fixing_point.position.x - (R[0][0]*param.dist_init[0] + R[0][1]*param.dist_init[1] + R[0][2]*param.dist_init[2]);
    target_pose.position.y = fixing_point.position.y - (R[1][0]*param.dist_init[0] + R[1][1]*param.dist_init[1] + R[1][2]*param.dist_init[2]);
    target_pose.position.z = fixing_point.position.z - (R[2][0]*param.dist_init[0] + R[2][1]*param.dist_init[1] + R[2][2]*param.dist_init[2]) - param.z_offset;
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);

    // lateral shift of the diameter of the circle
    target_pose.position.x = fixing_point.position.x - R[0][1]*param.radius*2 + (R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2]);
    target_pose.position.y = fixing_point.position.y - R[1][1]*param.radius*2 + (R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2]);
    target_pose.position.z = fixing_point.position.z - R[2][1]*param.radius*2 + (R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2]) - param.z_offset;
    target_pose.orientation = fixing_point.orientation;
    waypoints.poses.push_back(target_pose);
       
    // Upshift (Earlier point + heigh)
    target_pose.position.x = target_pose.position.x - R[0][2]*param.heigh;
    target_pose.position.y = target_pose.position.y - R[1][2]*param.heigh;
    target_pose.position.z = target_pose.position.z - R[2][2]*param.heigh;
    waypoints.poses.push_back(target_pose);

    // Half-circle, the minus in the "heigh" is required since the z axies is taken pointing down
    center.x = target_pose.position.x + R[0][1]*param.radius;
    center.y = target_pose.position.y + R[1][1]*param.radius;
    center.z = target_pose.position.z + R[2][1]*param.radius;
        

    int point_no;
    geometry_msgs::Pose first_circle_point;
    geometry_msgs::Pose last_circle_point;
    first_circle_point.position.x = target_pose.position.x - R[0][2]*param.heigh;
    first_circle_point.position.y = target_pose.position.y - R[1][2]*param.heigh;
    first_circle_point.position.z = target_pose.position.z - R[2][2]*param.heigh;
    last_circle_point.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
    last_circle_point.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
    last_circle_point.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
  
    point_no = (ComputeDistance(first_circle_point, last_circle_point)*param.radius*M_PI)/param.res;

    for(int k = 0; k < point_no; k++)
    {
        target_pose.position.x = center.x - R[0][1]*param.radius*cos(M_PI/point_no*k) - R[0][2]*param.radius*sin(M_PI/point_no*k) + (R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2])*k/point_no;
        target_pose.position.y = center.y - R[1][1]*param.radius*cos(M_PI/point_no*k) - R[1][2]*param.radius*sin(M_PI/point_no*k) + (R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2])*k/point_no;
        target_pose.position.z = center.z - R[2][1]*param.radius*cos(M_PI/point_no*k) - R[2][2]*param.radius*sin(M_PI/point_no*k) + (R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2])*k/point_no;
        waypoints.poses.push_back(target_pose);
    }


    target_pose.position.x = fixing_point.position.x - R[0][2]*param.heigh + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
    target_pose.position.y = fixing_point.position.y - R[1][2]*param.heigh + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
    target_pose.position.z = fixing_point.position.z - R[2][2]*param.heigh + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
    waypoints.poses.push_back(target_pose);

    // Downshift
    target_pose.position.x = fixing_point.position.x + R[0][0]*param.dist_final[0] + R[0][1]*param.dist_final[1] + R[0][2]*param.dist_final[2];
    target_pose.position.y = fixing_point.position.y + R[1][0]*param.dist_final[0] + R[1][1]*param.dist_final[1] + R[1][2]*param.dist_final[2];
    target_pose.position.z = fixing_point.position.z + R[2][0]*param.dist_final[0] + R[2][1]*param.dist_final[1] + R[2][2]*param.dist_final[2];
    waypoints.poses.push_back(target_pose);

}



void TrajectoryPlanner::CornerDetection()
{
    float AB, AC, BC;
    for(int i = 1; i < (InitPoint.poses.size()-1); i++)
    {
        if(InitLabel[i] == "pass" && InitLabel[i-1] != "grasp")
        {
            AB = ComputeDistance(InitPoint.poses[i], InitPoint.poses[i-1]);
            AC = ComputeDistance(InitPoint.poses[i+1], InitPoint.poses[i-1]);
            BC = ComputeDistance(InitPoint.poses[i], InitPoint.poses[i+1]);
            if (AB + BC != AC) // Aggiungere un eventuale valore minimo per cui non è necessario eseguire la curva
            {
                InitLabel[i] = "corner";
            }
        }
    }
    return;
}
void TrajectoryPlanner::addCornerFixPoint(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints, geometry_msgs::Pose corner_point, geometry_msgs::Pose starting_point)
{
    geometry_msgs::PoseArray CornerPath;    

    float PP_distance;          
    double D1, D2;
    geometry_msgs::Pose P1,P2,P3;

    float corner_length;
       
    P1 = starting_point;
    P2 = corner_point;
    P3 = fixing_point;
    PP_distance = ComputeDistance(P1, P2);
    
    if(PP_distance < param.Ctrl_pt_d1)
        D1 = PP_distance;
    else
        D1 =  param.Ctrl_pt_d1;

    PP_distance = ComputeDistance(P2, P3);
    if (PP_distance < param.Ctrl_pt_d2)
        D2 = PP_distance;
    else 
        D2 = param.Ctrl_pt_d2;
    
    // Calcolo della lunghezza totale 
    addCornerRound(P1, P2, P3, D1, D2, CornerPath);
    corner_length = ComputeDistance(P1,P2) + ComputeDistance(P2,P3) + param.dist_final[0];


    tf2::Quaternion quat;
    tf2::fromMsg(starting_point.orientation, quat);
    tf2::Matrix3x3 Rot;
    Rot.setRotation(quat);
    
    geometry_msgs::Pose target_pose;

    corner_length = ComputeDistance(P1,P2) + param.dist_final[0] + ComputeDistance(P2,P3);
    target_pose.position.x = starting_point.position.x + Rot[0][0]*corner_length;
    target_pose.position.y = starting_point.position.y + Rot[1][0]*corner_length;
    target_pose.position.z = starting_point.position.z + Rot[2][0]*corner_length;
    target_pose.orientation = starting_point.orientation;
    ComputePatch(starting_point, target_pose, waypoints);
    
    // Rotation

   // corner_length = ComputeDistance(P2,P3) + dist_final;
    corner_length = param.dist_final[0] + ComputeDistance(P2,P3); 
    float point_numbers = (M_PI/2*corner_length)/param.res;
    float t;
    float a_angle = asin(param.radius/corner_length);


    geometry_msgs::Point center;

    center.x = corner_point.position.x;
    center.y = corner_point.position.y;
    center.z = corner_point.position.z;

    for(int i=1; i < point_numbers; i++)
    {
        t=1/point_numbers*i;
        target_pose.position.x = center.x + corner_length*Rot[0][0]*cos(M_PI/2*t) + corner_length*Rot[0][1]*sin(M_PI/2*t);
        target_pose.position.y = center.y + corner_length*Rot[1][0]*cos(M_PI/2*t) + corner_length*Rot[1][1]*sin(M_PI/2*t);
        target_pose.position.z = center.z + corner_length*Rot[2][0]*cos(M_PI/2*t) + corner_length*Rot[2][1]*sin(M_PI/2*t) + param.heigh*t + param.radius*t;
        target_pose.orientation = slerp(starting_point.orientation, fixing_point.orientation, t);

        waypoints.poses.push_back(target_pose);
    }

    // Downshift
    tf2::fromMsg(fixing_point.orientation, quat);
    Rot.setRotation(quat);
    target_pose.position.x = fixing_point.position.x + Rot[0][0]*param.dist_final[0] + Rot[0][1]*param.dist_final[1] + Rot[0][2]*param.dist_final[2];
    target_pose.position.y = fixing_point.position.y + Rot[1][0]*param.dist_final[0] + Rot[1][1]*param.dist_final[1] + Rot[1][2]*param.dist_final[2];
    target_pose.position.z = fixing_point.position.z + Rot[2][0]*param.dist_final[0] + Rot[2][1]*param.dist_final[1] + Rot[2][2]*param.dist_final[2];
    waypoints.poses.push_back(target_pose);

}
void TrajectoryPlanner::addCornerFixPoint2(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints, geometry_msgs::Pose corner_point, geometry_msgs::Pose starting_point)
{
    // modifica per evitare il piolo

    geometry_msgs::PoseArray CornerPath;    

    float PP_distance;          
    double D1;
    double D2;
    geometry_msgs::Pose P1,P2,P3;

    float corner_length;
        
    P1 = starting_point;
    P2 = corner_point;
    P3 = fixing_point;
    PP_distance = ComputeDistance(P1, P2);
    
    if(PP_distance < param.Ctrl_pt_d1)
        D1 = PP_distance;
    else
        D1 = param.Ctrl_pt_d1;

    PP_distance = ComputeDistance(P2, P3);
    if (PP_distance < param.Ctrl_pt_d2)
        D2 = PP_distance;
    else 
        D2 = param.Ctrl_pt_d2;
    
    // Calcolo della lunghezza totale 
    addCornerRound(P1, P2, P3, D1, D2, CornerPath);
    corner_length = ComputeDistance(P1,P2) + ComputeDistance(P2,P3) + param.dist_final[0];

    tf2::Quaternion quat;
    tf2::fromMsg(starting_point.orientation, quat);
    tf2::Matrix3x3 Rot;
    Rot.setRotation(quat);
    
    geometry_msgs::Pose target_pose;
    geometry_msgs::PoseArray Trajectory;
    geometry_msgs::Pose intermediate_pose;

    intermediate_pose.position.x = corner_point.position.x;
    intermediate_pose.position.y = corner_point.position.y + 0.03;
    intermediate_pose.position.z = corner_point.position.z - 0.02;
    intermediate_pose.orientation = starting_point.orientation;
    ComputePatch(starting_point, intermediate_pose, waypoints);
    
    corner_length = ComputeDistance(P1,P2) + param.dist_final[0] + ComputeDistance(P2,P3);
    target_pose.position.x = starting_point.position.x + Rot[0][0]*corner_length;
    target_pose.position.y = starting_point.position.y + Rot[1][0]*corner_length;
    target_pose.position.z = starting_point.position.z + Rot[2][0]*corner_length;
    target_pose.orientation = starting_point.orientation;
    ComputePatch(intermediate_pose, target_pose, waypoints);

    // Rotation
    corner_length = param.dist_final[0] + ComputeDistance(P2,P3); 
    float point_numbers = (M_PI/2*corner_length)/param.res;
    float t;
    float a_angle = asin(param.radius/corner_length);

    geometry_msgs::Point center;
    center.x = corner_point.position.x;
    center.y = corner_point.position.y;
    center.z = corner_point.position.z;

    for(int i=1; i < point_numbers; i++)
    {
        t=1/point_numbers*i;
        target_pose.position.x = center.x + corner_length*Rot[0][0]*cos(M_PI/2*t) + corner_length*Rot[0][1]*sin(M_PI/2*t);
        target_pose.position.y = center.y + corner_length*Rot[1][0]*cos(M_PI/2*t) + corner_length*Rot[1][1]*sin(M_PI/2*t);
        target_pose.position.z = center.z + corner_length*Rot[2][0]*cos(M_PI/2*t) + corner_length*Rot[2][1]*sin(M_PI/2*t) + param.heigh*t + 0.015*t;
        target_pose.orientation = slerp(starting_point.orientation, fixing_point.orientation, t);
        waypoints.poses.push_back(target_pose);
    }

    // Downshift
    tf2::fromMsg(fixing_point.orientation, quat);
    Rot.setRotation(quat);
    target_pose.position.x = fixing_point.position.x + Rot[0][0]*param.dist_final[0] + Rot[0][1]*param.dist_final[1] + Rot[0][2]*param.dist_final[2];
    target_pose.position.y = fixing_point.position.y + Rot[1][0]*param.dist_final[0] + Rot[1][1]*param.dist_final[1] + Rot[1][2]*param.dist_final[2];
    target_pose.position.z = fixing_point.position.z + Rot[2][0]*param.dist_final[0] + Rot[2][1]*param.dist_final[1] + Rot[2][2]*param.dist_final[2];
    waypoints.poses.push_back(target_pose);

}

void TrajectoryPlanner::addCornerFixPoint_prova(geometry_msgs::Pose fixing_point, geometry_msgs::PoseArray& waypoints, geometry_msgs::Pose corner_point, geometry_msgs::Pose starting_point, double dis_init, double dist_final, bool orientation)
{
    // modifica per evitare il piolo

    geometry_msgs::PoseArray CornerPath;    

    float PP_distance;          
    double D1;
    double D2;
    geometry_msgs::Pose P1,P2,P3;

    float corner_length;
        
    P1 = starting_point;
    P2 = corner_point;
    P3 = fixing_point;

    float P12 = ComputeDistance(P1,P2);
    float P23 = ComputeDistance(P2,P3);
    float P31 = ComputeDistance(P3,P1);

    PP_distance = ComputeDistance(P1, P2);

    if(PP_distance < param.Ctrl_pt_d1)
        D1 = PP_distance;
    else
        D1 = param.Ctrl_pt_d1;

    PP_distance = ComputeDistance(P2, P3);

    if (PP_distance < param.Ctrl_pt_d2)
        D2 = PP_distance;
    else 
        D2 = param.Ctrl_pt_d2;
    


    // Calcolo della lunghezza totale 
    addCornerRound(P1, P2, P3, D1, D2, CornerPath);
    corner_length = ComputeDistance(P1,P2) + ComputeDistance(P2,P3) + param.dist_final[0];

    tf2::Quaternion quat;
    tf2::fromMsg(starting_point.orientation, quat);
    tf2::Matrix3x3 Rot;
    Rot.setRotation(quat);
    
    geometry_msgs::Pose target_pose;
    geometry_msgs::PoseArray Trajectory;
    geometry_msgs::Pose intermediate_pose;

    intermediate_pose.position.x = corner_point.position.x;
    intermediate_pose.position.y = corner_point.position.y + 0.03;
    intermediate_pose.position.z = corner_point.position.z - 0.02;
    intermediate_pose.orientation = starting_point.orientation;
    ComputePatch(starting_point, intermediate_pose, waypoints);
    
    corner_length = ComputeDistance(P1,P2) + param.dist_final[0] + ComputeDistance(P2,P3);
    target_pose.position.x = starting_point.position.x + Rot[0][0]*corner_length;
    target_pose.position.y = starting_point.position.y + Rot[1][0]*corner_length;
    target_pose.position.z = starting_point.position.z + Rot[2][0]*corner_length;
    target_pose.orientation = starting_point.orientation;
    ComputePatch(intermediate_pose, target_pose, waypoints);

    //Compute angles of rotation

    float rot_angle = acos((pow(P12,2) + pow(P23,2) - pow(P31,2)/(2*P12*P23)));


    // Rotation
    corner_length = param.dist_final[0] + ComputeDistance(P2,P3); 
    float point_numbers = (rot_angle*corner_length)/param.res;
    float t;
    float a_angle = asin(param.radius/corner_length);

    geometry_msgs::Point center;
    center.x = corner_point.position.x;
    center.y = corner_point.position.y;
    center.z = corner_point.position.z;

    for(int i=1; i < point_numbers; i++)
    {
        t=1/point_numbers*i;
        target_pose.position.x = center.x + corner_length*Rot[0][0]*cos(rot_angle*t) + corner_length*Rot[0][1]*sin(rot_angle*t);
        target_pose.position.y = center.y + corner_length*Rot[1][0]*cos(rot_angle*t) + corner_length*Rot[1][1]*sin(rot_angle*t);
        target_pose.position.z = center.z + corner_length*Rot[2][0]*cos(rot_angle*t) + corner_length*Rot[2][1]*sin(rot_angle*t) + param.heigh*t + param.z_offset*t;
        target_pose.orientation = slerp(starting_point.orientation, fixing_point.orientation, t);
        waypoints.poses.push_back(target_pose);
    }

    // Downshift
    tf2::fromMsg(fixing_point.orientation, quat);
    Rot.setRotation(quat);
    target_pose.position.x = fixing_point.position.x + Rot[0][0]*param.dist_final[0] + Rot[0][1]*param.dist_final[1] + Rot[0][2]*param.dist_final[2];
    target_pose.position.y = fixing_point.position.y + Rot[1][0]*param.dist_final[0] + Rot[1][1]*param.dist_final[1] + Rot[1][2]*param.dist_final[2];
    target_pose.position.z = fixing_point.position.z + Rot[2][0]*param.dist_final[0] + Rot[2][1]*param.dist_final[1] + Rot[2][2]*param.dist_final[2];
    waypoints.poses.push_back(target_pose);

}

void TrajectoryPlanner::addCornerRound(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::Pose Point3, double distance1, double distance2, geometry_msgs::PoseArray& waypoints)
{
    geometry_msgs::Pose point1_shift, point2_shift, target_point;
    geometry_msgs::Point delta_1, delta_2;
    tf2::Quaternion Tg_Pose_Orientation1, Tg_Pose_Orientation2;
    double norm_1, norm_2;
    double d1, d2;
    float t;                // Used in for cycle €[0,1]

    // Compute the distance beetween the points with respect to x,y,z
    delta_1.x = Point2.position.x - Point1.position.x;
    delta_1.y = Point2.position.y - Point1.position.y;
    delta_1.z = Point2.position.z - Point1.position.z;
    delta_2.x = Point3.position.x - Point2.position.x;
    delta_2.y = Point3.position.y - Point2.position.y;
    delta_2.z = Point3.position.z - Point2.position.z;    

    // Check if the shifting distance to start the cornering is greater than the distance beetween the two points to avoid to go backward
    norm_1 = sqrt(delta_1.x*delta_1.x + delta_1.y*delta_1.y + delta_1.z*delta_1.z);    
    if (norm_1 > distance1)
    {
        d1 = distance1; 
        point1_shift.position.x = Point1.position.x + (norm_1-d1)/norm_1*(Point2.position.x - Point1.position.x);
        point1_shift.position.y = Point1.position.y + (norm_1-d1)/norm_1*(Point2.position.y - Point1.position.y);
        point1_shift.position.z = Point1.position.z + (norm_1-d1)/norm_1*(Point2.position.z - Point1.position.z);
        point1_shift.orientation = Point1.orientation;
        waypoints.poses.push_back(point1_shift);
    }
    else
    {
        d1 = norm_1;
        point1_shift = Point1;
    }

    norm_2 = sqrt(delta_2.x*delta_2.x + delta_2.y*delta_2.y + delta_2.z*delta_2.z);    
    if (norm_2 > distance2)
    {
        d2 = distance2;
        point2_shift.position.x = Point2.position.x + d2/norm_2*(Point3.position.x - Point2.position.x);
        point2_shift.position.y = Point2.position.y + d2/norm_2*(Point3.position.y - Point2.position.y);
        point2_shift.position.z = Point2.position.z + d2/norm_2*(Point3.position.z - Point2.position.z);
        point2_shift.orientation = Point3.orientation;
        // No push_back here becouse id done later
    }
    else
    {
        d2 = norm_2;
        point2_shift = Point3;
    }
    tf2::fromMsg(Point1.orientation, Tg_Pose_Orientation1);
    tf2::fromMsg(Point3.orientation, Tg_Pose_Orientation2);

    // Cornering function using bèzier quadratic function
    for (int i = 0; i < param.corner_points; i++)
    {
        t = 1/param.corner_points*i;
        // P1 + (1-t)^2(P0-P1)+t^2(P2-P1)
        target_point.position.x = Point2.position.x + (1-t)*(1-t)*(point1_shift.position.x - Point2.position.x) + t*t*(point2_shift.position.x - Point2.position.x);
        target_point.position.y = Point2.position.y + (1-t)*(1-t)*(point1_shift.position.y - Point2.position.y) + t*t*(point2_shift.position.y - Point2.position.y);
        target_point.position.z = Point2.position.z + (1-t)*(1-t)*(point1_shift.position.z - Point2.position.z) + t*t*(point2_shift.position.z - Point2.position.z);
        target_point.orientation = tf2::toMsg(Tg_Pose_Orientation1.slerp(Tg_Pose_Orientation2,t));
        waypoints.poses.push_back(target_point);
    }

    if (norm_2 > d2)
        waypoints.poses.push_back(point2_shift);
}
int TrajectoryPlanner::ComputePatch(geometry_msgs::Pose Point1, geometry_msgs::Pose Point2, geometry_msgs::PoseArray& waypoints)
{
    /*
    *   Given two points as input returns a series of points with a distance of res beetwen 
    *   the two points included.
    */ 

    ROS_INFO("COMPUTE PATCH");
    geometry_msgs::Pose target_pose;
    geometry_msgs::Point delta;
    tf2::Quaternion Tg_Pose_Orientation1, Tg_Pose_Orientation2;
    double distance;
    double t;                // Used in for cycle €[0,1]
    double step;

    int patch_added = 0;

    // Compute the distance beetween the points with respect to x,y,z
    delta.x = Point2.position.x - Point1.position.x;
    delta.y = Point2.position.y - Point1.position.y;
    delta.z = Point2.position.z - Point1.position.z;

    distance = sqrt(delta.x*delta.x + delta.y*delta.y + delta.z*delta.z);      
    step = distance/param.res;

    tf2::fromMsg(Point1.orientation, Tg_Pose_Orientation1);
    tf2::fromMsg(Point2.orientation, Tg_Pose_Orientation2);

    for(int i = 1; i < step; i++)
    {
        t = 1/(step)*i;
        target_pose.position.x = Point1.position.x + t*delta.x;
        target_pose.position.y = Point1.position.y + t*delta.y;
        target_pose.position.z = Point1.position.z + t*delta.z;
        target_pose.orientation = Point2.orientation;
        target_pose.orientation = tf2::toMsg(Tg_Pose_Orientation1.slerp(Tg_Pose_Orientation2,t));
        waypoints.poses.push_back(target_pose);
        patch_added++;
    }
    return patch_added;
}

void TrajectoryPlanner::TrajectoryPlanning()
{
    std::vector<geometry_msgs::PoseArray> Trajectories_list; 

    geometry_msgs::PoseArray Trajectory_part;    
    std::vector<std::string> Trajectory_id;            
    geometry_msgs::PoseArray Patch_To_Add;

    float PP_distance;          
    double new_Ctrl_Pt_d1;
    double new_Ctrl_Pt_d2;

    geometry_msgs::Pose P1,P2,P3;
    int trajectory_size;
    
    CornerDetection();
    
    for (std::size_t i = 0; i < InitPoint.poses.size(); i ++)
    {  
        std::cout << i << "\n";
        std::cout <<InitLabel[i] << "\n";
        if (InitLabel[i] == "fix")
        {   
            if(i > 2)
            {
                if(InitLabel[i-1] == "corner")
                {
                    std::cout << "corner fix \n";
                    std::cout << InitPoint.poses[i] << "\n";
                    std::cout << InitPoint.poses[i-1] << "\n";
                    std::cout << Trajectories_list[Trajectories_list.size()-1].poses[Trajectories_list[Trajectories_list.size()-1].poses.size()-1] << "\n";

                    addCornerFixPoint(InitPoint.poses[i], Trajectory_part, InitPoint.poses[i-1], Trajectories_list[Trajectories_list.size()-1].poses[Trajectories_list[Trajectories_list.size()-1].poses.size()-1]);
                    std::cout << "fine corner \n";
                    Trajectories_list.push_back(Trajectory_part);
                    Trajectory_part.poses.clear();
                    std::cout << "modifica \n";
                    InitLabel[i-1] == "cornerfix";
                    Trajectory_id.push_back("cornerfix");
                    
                }
                else
                {
                    addFixPoint3(InitPoint.poses[i], Trajectory_part);
                    Trajectories_list.push_back(Trajectory_part);
                    Trajectory_part.poses.clear();
                    Trajectory_id.push_back("fix");
                }
            }
            else
            {
                addFixPoint3(InitPoint.poses[i], Trajectory_part);
                Trajectories_list.push_back(Trajectory_part);
                Trajectory_part.poses.clear();
                Trajectory_id.push_back("fix");
            }
        }

        if(InitLabel[i] == "pass")
        {
            Trajectory_part.poses.push_back(InitPoint.poses[i]);
            Trajectories_list.push_back(Trajectory_part);
            Trajectory_part.poses.clear();   
            Trajectory_id.push_back("pass");
        }

        if(InitLabel[i] == "corner")
        {
            if( i < InitPoint.poses.size()-1)
            {
                if(InitLabel[i+1] == "fix")
                    std::cout << "Skip";
                else
                {
                    Trajectory_part.poses.push_back(InitPoint.poses[i]);
                    Trajectories_list.push_back(Trajectory_part);
                    Trajectory_part.poses.clear();   
                    Trajectory_id.push_back("corner");
                }     
            }
        }
    } 

    for(size_t i = 1; i < (Trajectories_list.size()-1); i++)
    {
        if(Trajectory_id[i] == "corner" && Trajectory_id[i+1] != "cornerfix")
        {
            P1 = Trajectories_list[i-1].poses[trajectory_size-1];
            P2 = Trajectories_list[i].poses[0];
            P3 = Trajectories_list[i+1].poses[0];

            CheckCornerDistance(Trajectory_id, Trajectories_list, i, new_Ctrl_Pt_d1, new_Ctrl_Pt_d2);
            addCornerRound(P1, P2, P3, new_Ctrl_Pt_d1, new_Ctrl_Pt_d2, Trajectory_part);

            Trajectories_list[i].poses.clear();
            Trajectories_list[i].poses = Trajectory_part.poses;
            Trajectory_id[i] = "corner";
            Trajectory_part.poses.clear();
        }
    }

    /**************** INCLUDE PATCH ******************/  

    std::vector<std::string> LabelVector_flag;
    SecondaryTrajectories.push_back(Trajectories_list[0]);
    LabelVector_flag.push_back(Trajectory_id[0]);

    for(size_t i=1; i<Trajectories_list.size(); i++)
    {
        if (ComputePatch(Trajectories_list[i-1].poses.back(), Trajectories_list[i].poses[0], Patch_To_Add) > 1 )
        {
            SecondaryTrajectories.push_back(Patch_To_Add);
            LabelVector_flag.push_back("pass");
        }    
        SecondaryTrajectories.push_back(Trajectories_list[i]);   
        LabelVector_flag.push_back(Trajectory_id[i]);     
        Patch_To_Add.poses.clear();
    }

   label_list.clear();
    for(int i=0; i< LabelVector_flag.size(); i++)
    {
       label_list.push_back(LabelVector_flag[i]);
    }
}
void TrajectoryPlanner::TrajectoryPlanning2()
{
    std::vector<geometry_msgs::PoseArray> Trajectories_list; 

    geometry_msgs::PoseArray Trajectory_part;    
    std::vector<std::string> Trajectory_id;            
    geometry_msgs::PoseArray Patch_To_Add;

    float PP_distance;          
    double new_Ctrl_Pt_d1;
    double new_Ctrl_Pt_d2;

    geometry_msgs::Pose P1,P2,P3;
    int trajectory_size;
    
    CornerDetection();
    
    for (std::size_t i = 0; i < InitPoint.poses.size(); i ++)
    {  
        std::cout << i << "\n";
        std::cout <<InitLabel[i] << "\n";
        if (InitLabel[i] == "fix")
        {   
            if(i > 2)
            {
                if(InitLabel[i-1] == "corner")
                {
                    std::cout << "corner fix \n";
                    std::cout << InitPoint.poses[i] << "\n";
                    std::cout << InitPoint.poses[i-1] << "\n";
                    std::cout << Trajectories_list[Trajectories_list.size()-1].poses[Trajectories_list[Trajectories_list.size()-1].poses.size()-1] << "\n";

                    addCornerFixPoint2(InitPoint.poses[i], Trajectory_part, InitPoint.poses[i-1], Trajectories_list[Trajectories_list.size()-1].poses[Trajectories_list[Trajectories_list.size()-1].poses.size()-1]);
                    std::cout << "fine corner \n";
                    Trajectories_list.push_back(Trajectory_part);
                    Trajectory_part.poses.clear();
                    std::cout << "modifica \n";
                    InitLabel[i-1] == "cornerfix";
                    Trajectory_id.push_back("cornerfix");
                    
                }
                else
                {
                    addFixPoint4(InitPoint.poses[i], Trajectory_part);
                    Trajectories_list.push_back(Trajectory_part);
                    Trajectory_part.poses.clear();
                    Trajectory_id.push_back("fix");
                }
            }
            else
            {
                addFixPoint4(InitPoint.poses[i], Trajectory_part);
                Trajectories_list.push_back(Trajectory_part);
                Trajectory_part.poses.clear();
                Trajectory_id.push_back("fix");
            }
        }

        if(InitLabel[i] == "pass")
        {
            Trajectory_part.poses.push_back(InitPoint.poses[i]);
            Trajectories_list.push_back(Trajectory_part);
            Trajectory_part.poses.clear();   
            Trajectory_id.push_back("pass");
        }

        if(InitLabel[i] == "corner")
        {
            if( i < InitPoint.poses.size()-1)
            {
                if(InitLabel[i+1] == "fix")
                    std::cout << "Skip";
                else
                {
                    Trajectory_part.poses.push_back(InitPoint.poses[i]);
                    Trajectories_list.push_back(Trajectory_part);
                    Trajectory_part.poses.clear();   
                    Trajectory_id.push_back("corner");
                }     
            }
        }
    } 

    for(size_t i = 1; i < (Trajectories_list.size()-1); i++)
    {
        if(Trajectory_id[i] == "corner" && Trajectory_id[i+1] != "cornerfix")
        {
            P1 = Trajectories_list[i-1].poses[trajectory_size-1];
            P2 = Trajectories_list[i].poses[0];
            P3 = Trajectories_list[i+1].poses[0];

            CheckCornerDistance(Trajectory_id, Trajectories_list, i, new_Ctrl_Pt_d1, new_Ctrl_Pt_d2);
            addCornerRound(P1, P2, P3, new_Ctrl_Pt_d1, new_Ctrl_Pt_d2, Trajectory_part);

            Trajectories_list[i].poses.clear();
            Trajectories_list[i].poses = Trajectory_part.poses;
            Trajectory_id[i] = "corner";
            Trajectory_part.poses.clear();
        }
    }

    /**************** INCLUDE PATCH ******************/  

    std::vector<std::string> LabelVector_flag;
    SecondaryTrajectories.push_back(Trajectories_list[0]);
    LabelVector_flag.push_back(Trajectory_id[0]);

    for(size_t i=1; i<Trajectories_list.size(); i++)
    {
        if (ComputePatch(Trajectories_list[i-1].poses.back(), Trajectories_list[i].poses[0], Patch_To_Add) > 1 )
        {
            SecondaryTrajectories.push_back(Patch_To_Add);
            LabelVector_flag.push_back("pass");
        }    
        SecondaryTrajectories.push_back(Trajectories_list[i]);   
        LabelVector_flag.push_back(Trajectory_id[i]);     
        Patch_To_Add.poses.clear();
    }

   label_list.clear();
    for(int i=0; i< LabelVector_flag.size(); i++)
    {
       label_list.push_back(LabelVector_flag[i]);
    }
}


void TrajectoryPlanner::CheckCornerDistance(std::vector<std::string> Traj_id, std::vector<geometry_msgs::PoseArray> Traj_list, int i, double& distance1, double& distance2)
{
    int trajectory_size = Traj_list[i-1].poses.size();
    geometry_msgs::Pose P1 = Traj_list[i-1].poses[trajectory_size-1];
    geometry_msgs::Pose P2 = Traj_list[i].poses[0];
    geometry_msgs::Pose P3 = Traj_list[i+1].poses[0];

    double PP_distance = ComputeDistance(P1, P2);
    
    if( Traj_id[i-1] != "corner")
    {
        if(PP_distance < param.Ctrl_pt_d1)
            distance1 = PP_distance;
        else
            distance1 =  param.Ctrl_pt_d1;
    }
    else
        distance1 = PP_distance/2;
    PP_distance = ComputeDistance(P2, P3);
    if(InitLabel[i+1] != "corner")
    {
        if (PP_distance < param.Ctrl_pt_d2)
            distance2 = PP_distance;
        else 
            distance2 =  param.Ctrl_pt_d2;
    }
    else
        distance2 = PP_distance/2;
}

void TrajectoryPlanner::PublishTrajectory(moveit_visual_tools::MoveItVisualTools& visual_tools)
{
    visual_tools.deleteAllMarkers();
    for(size_t i = 0; i<SecondaryTrajectories.size(); i++)
    {
        if(SecondaryTrajectories.size() > 2)
            visual_tools.publishPath(SecondaryTrajectories[i].poses, rvt::LIME_GREEN, rvt::XXXSMALL);
    }    
    visual_tools.trigger();
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
float ComputePathLength(geometry_msgs::PoseArray waypoint)
{
    float distance;
    distance = 0.0;

    for(int i=1; i<waypoint.poses.size(); i++)
    {
        distance += ComputeDistance(waypoint.poses[i-1], waypoint.poses[i]);
    }

    return distance;
}

geometry_msgs::Quaternion slerp( geometry_msgs::Quaternion v0, geometry_msgs::Quaternion v1, double t)
{

    /*
        REPLACED BY SLERP FUNCTION OF TF2 LIBRARY!!
    */
   
    // Only unit quaternions are valid rotations.
    // Normalize to avoid undefined behavior.

    geometry_msgs::Quaternion result;
 /*
    v0.normalize();
    v1.normalize();
 */
    // Compute the cosine of the angle between the two vectors.
    double dot = v0.w*v1.w + v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;

    // If the dot product is negative, slerp won't take
    // the shorter path. Note that v1 and -v1 are equivalent when
    // the negation is applied to all four components. Fix by 
    // reversing one quaternion.
    if (dot < 0.0f)
    {
        v1.w = -v1.w;
        v1.x = -v1.x;
        v1.y = -v1.y;
        v1.z = v1.z;
        dot = -dot;
    }

    const double DOT_THRESHOLD = 0.9995;
    if (dot > DOT_THRESHOLD) {
        // If the inputs are too close for comfort, linearly interpolate
        // and normalize the result.

        result.x = v0.x + t*(v1.x - v0.x);
        result.y = v0.y + t*(v1.y - v0.y);
        result.z = v0.z + t*(v1.z - v0.z);
        result.w = v0.w + t*(v1.w - v0.w);
        dot = result.w*result.w + result.x*result.x + result.y*result.y + result.z*result.z;
        dot = sqrt(dot);

        result.x = result.x/dot;
        result.y = result.y/dot;
        result.z = result.z/dot;
        result.w = result.w/dot;
   
        return result;
    }

    // Since dot is in range [0, DOT_THRESHOLD], acos is safe
    double theta_0 = acos(dot);        // theta_0 = angle between input vectors
    double theta = theta_0*t;          // theta = angle between v0 and result
    double sin_theta = sin(theta);     // compute this value only once
    double sin_theta_0 = sin(theta_0); // compute this value only once

    double s0 = cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
    double s1 = sin_theta / sin_theta_0;

    result.x = (s0 * v0.x) + (s1 * v1.x);
    result.y = (s0 * v0.y) + (s1 * v1.y);
    result.z = (s0 * v0.z) + (s1 * v1.z);
    result.w = (s0 * v0.w) + (s1 * v1.w);
    
    return result;
}
