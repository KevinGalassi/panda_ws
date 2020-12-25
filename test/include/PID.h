
#ifndef PID_H
#define PID_H

#include <ros/init.h>
#include <ros/node_handle.h>
#include <ros/rate.h>
#include <ros/subscriber.h>

#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/String.h>

#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/WrenchStamped.h>

#include <iostream>
#include <stdio.h>

namespace controller{

class PID
{
    public:

    PID();
    ~PID();

    float Kp;
    float Ki;
    float Kd;
    
    float upper_limit;
    float lower_limit;

    float offset;

    float ref_value;
    float actual_value;
    
    int rate;

    float output_sat;


    std::string Name;

    
    float UpdatePID();
   // void PublishOutput();
    void Reset();

private:

    ros::NodeHandle n_;

    float error;
    float pre_error;

    float Pout;
    float Iout;
    float Dout;
    float integral;
    float derivative;

    float output;

    void UpdateValue();
    void CheckBounds();
    float GetOutput();


  //  ros::Publisher output_pub;
};


PID::PID()
{
    ROS_INFO("PID Controller init");

    rate = 200;
    integral = 0.0;

    Kp = 1.0;
    Ki = 0.0;
    Kd = 0.0;

    offset = 0.0;
    output_sat = 0.0;
    pre_error = 0.0;

}


PID::~PID()
{
    ROS_INFO("PID shutdown");
}

float PID::UpdatePID()
{
    float output_value;
    UpdateValue();
    CheckBounds();
    output_value = GetOutput();
    return output_value;
}


void PID::UpdateValue()
{
    // Calculate error
    error = ref_value - actual_value;

    // Proportional term
    Pout = Kp * error;

    // Integral term
    integral += error * 0.01;
    Iout = Ki * integral;

    // Derivative term
    derivative = (error - pre_error) / 0.01;
    Dout = Kd * derivative;

    // Calculate total output
    output = offset - (Pout + Iout + Dout);
    std::cout << "Output:   " << output << "\n";

    // Save error to previous error
    pre_error = error;
}

void PID::CheckBounds()
{
    // Restrict to max/min
    if( output > upper_limit )
        output_sat = upper_limit;
    else if(output < lower_limit )
        output_sat = lower_limit;
    std::cout << "output_sat:  " << output_sat << "\n";
} 

/*
void PID::PublishOutput()
{
    Output_msg.output = output;
    Output_msg.output_sat = output_sat;
    Output_msg.error = pre_error;
    output_pub.publish(Output_msg);
}
*/

void PID::Reset()
{
    pre_error = 0;
    error = 0;
    integral = 0;
}

float PID::GetOutput()
{
    return output_sat;
}


} //namespace controller

#endif


