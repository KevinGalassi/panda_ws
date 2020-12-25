
#ifndef TACTILESENSOR_H
#define TACTILESENSOR_H


#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_srvs/Empty.h>
#include "read_sensor.h"

#include "ros/ros.h"


#define INIT 0
#define CALIBRATION 1
#define WORK 2
#define START_COM 3
#define WAIT_GRASP 4
#define GRASP_CALI 5
#define START_CALI 6
#define FIRST_CALI 8


class TactileSensor
{
    public: 

    TactileSensor();
    ~TactileSensor();

    int no_x_cell;
    int no_y_cell;
    int no_calibration_data;

    bool Orientation;

    float ul_move;
    float ul_tens;
    float ll_move;
    float ll_tens;

    std::string SensorName;
    std::string TopicName; 

    std::vector<std_msgs::Float32MultiArray> calibration_vector;
    std_msgs::Float32MultiArray New_Data;

    std_msgs::Float32MultiArray     Diff_Border;        // DIfference between the two side of the sensor
    std_msgs::Float32               Mean_Diff_Border;   // Mean value of the diff. of the border.
    std_msgs::Float32MultiArray     OffsetValue;        // 
    std_msgs::Float32               MeanOffsetValue;    //

    std_msgs::Float32               SensorOutput;


    bool SensorInit();
    void ControlLoop();
    
    private:

    int SensorState;
    int counter;
    int user_input;
    float flag_value;

    ros::NodeHandle n;
    ros::NodeHandle nh_priv_;

    ros::Publisher BoundDiff_pub;
    ros::Publisher MeanBoundDiff_pub;
    ros::Publisher OffsetDataValue_pub;
    ros::Publisher MeanOffsetDataValue_pub;


    ros::Publisher OffsetInit_pub;
    ros::Publisher OffsetGrasp_pub;

    ros::ServiceClient Calibration_srv;

    ros::Subscriber TactileData_sub;

    int sensor_size;
    int NodeState;

    std::vector<float> mean_value;

    std_msgs::Float32MultiArray offset_value;
    std_msgs::Float32MultiArray offset_init;
    std_msgs::Float32MultiArray offset_grasp;



    void Calibration(std_msgs::Float32MultiArray& new_offset);
    // void Calibration();

    void Data_MultiArray_CallBack(const std_msgs::Float32MultiArray &msg);

    void Data_tactile_CallBack(const read_sensor::tactile_sensor_data &msg);

    bool Cali_Callback(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
    
    void UpdateBoundDifference(std_msgs::Float32MultiArray New_Data);

    void PublishBoundDiff(std_msgs::Float32MultiArray msg);
    void PublishMeanBoundDiff(float msg);
    void PublishOffsetValue(std_msgs::Float32MultiArray msg);
    void PublishMeanOffsetValue(float msg);

    void PublishAllData();
};

#endif
