#include <ros/ros.h>
#include <franka_control/SetFullCollisionBehavior.h>



int main(int argc, char** argv){
    ros::init(argc, argv, "reduce_reflex");
    ros::NodeHandle n;
    float k;
    k=3;
    ros::ServiceClient client=n.serviceClient<franka_control::SetFullCollisionBehavior>("/franka_control/set_full_collision_behavior");
    franka_control::SetFullCollisionBehavior srv;
    srv.request.lower_torque_thresholds_acceleration = {20.0*k, 20.0*k, 18.0*k, 18.0*k, 16.0*k, 14.0*k, 12.0*k};
    srv.request.upper_torque_thresholds_acceleration = {20.0*k, 20.0*k, 18.0*k, 18.0*k, 16.0*k, 14.0*k, 12.0*k};
    srv.request.lower_torque_thresholds_nominal = {20.0*k, 20.0*k, 18.0*k, 18.0, 16.0*k, 14.0*k, 12.0*k};
    srv.request.upper_torque_thresholds_nominal = {20.0*k, 20.0*k, 18.0*k, 18.0, 16.0*k, 14.0*k, 12.0*k};
    srv.request.lower_force_thresholds_acceleration = {20.0*k, 20.0*k, 20.0*k, 25.0*k, 25.0*k, 25.0*k};
    srv.request.upper_force_thresholds_acceleration = {20.0*k, 20.0*k, 20.0*k, 25.0*k, 25.0*k, 25.0*k};
    srv.request.lower_force_thresholds_nominal = {20.0*k, 20.0*k, 20.0*k, 25.0*k, 25.0*k, 25.0*k};
    srv.request.upper_force_thresholds_nominal = {20.0*k, 20.0*k, 20.0*k, 25.0*k, 25.0*k, 25.0*k};
    client.call(srv);
    if (client.call(srv))
  {
    ROS_INFO("risultato", (bool)srv.response.success);
  }
  else
  {
    ROS_ERROR("Failed to call service add_two_ints");
    return 1;
  }
    return 0;
}