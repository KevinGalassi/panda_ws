#include "ros/ros.h"
#include "moveit/move_group_interface/move_group_interface.h"
#include "geometric_shapes/shapes.h"
#include "geometric_shapes/mesh_operations.h"
#include "geometric_shapes/shape_operations.h"
#include "moveit/planning_scene_interface/planning_scene_interface.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "building_workspace");
    ros::NodeHandle node_handle;
    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    sleep(2.0);

    moveit_msgs::CollisionObject wall;
    shapes::Mesh* m = shapes::createMeshFromResource("package://robot_config/meshes/wall.stl");
    ROS_INFO("Wall imported!");  
    shape_msgs::Mesh wall_mesh;
    shapes::ShapeMsg wall_mesh_msg;
    shapes::constructMsgFromShape(m,wall_mesh_msg);
    wall_mesh = boost::get<shape_msgs::Mesh>(wall_mesh_msg);
    wall.meshes.resize(1);
    wall.meshes[0] = wall_mesh;
    wall.mesh_poses.resize(1);

    // ORIGINAL VALUES X = 0.0; Y = 1.0; Z = 0.0
    wall.mesh_poses[0].position.x = -0.5;
    wall.mesh_poses[0].position.y = 1.0;
    wall.mesh_poses[0].position.z = -1.0;
    wall.mesh_poses[0].orientation.w= 1.0;
    wall.mesh_poses[0].orientation.x= 0.0;
    wall.mesh_poses[0].orientation.y= 0.0;
    wall.mesh_poses[0].orientation.z= 0.0;

    wall.meshes.push_back(wall_mesh);
    wall.mesh_poses.push_back(wall.mesh_poses[0]);
    wall.operation = wall.ADD;

    std::vector<moveit_msgs::CollisionObject> collision_objects;  
    collision_objects.push_back(wall);  

    ROS_INFO("Wall created");  
    planning_scene_interface.addCollisionObjects(collision_objects);
}