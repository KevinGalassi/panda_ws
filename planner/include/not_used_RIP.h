
#include "ros/ros.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseArray.h"

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_visual_tools/moveit_visual_tools.h>


#include <tf2_geometry_msgs/tf2_geometry_msgs.h>    //TF2 to convert YPR to Quaternion

#include <math.h>

#include "parameters.h"


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
