#include <TactileSensor.hpp>

#include <iostream>


TactileSensor::TactileSensor()
{
    ROS_INFO("Tactile Sensor Node Init");
}

TactileSensor::~TactileSensor()
{
    ros::shutdown();
}


bool TactileSensor::SensorInit()
{
    sensor_size = no_x_cell*no_y_cell;
    no_calibration_data = 500;
    counter = 0;
    SensorState = 0;

    mean_value.resize(sensor_size);  
    New_Data.data.resize(sensor_size);
    offset_value.data.resize(sensor_size);
    OffsetValue.data.resize(sensor_size);
    offset_init.data.resize(sensor_size);
    offset_grasp.data.resize(sensor_size);



    Diff_Border.data.resize(no_y_cell);

    for(int i=0; i<sensor_size; i++)
    {
        mean_value[i] = 0;
        offset_value.data[i] = 0;
        offset_init.data[i] = 0;
        offset_grasp.data[i] = 0;

    }
    calibration_vector.resize(no_calibration_data);
    for(int i=0; i< no_calibration_data; i++)
    {
        calibration_vector[i].data.resize(sensor_size);
    }

    BoundDiff_pub           = n.advertise<std_msgs::Float32MultiArray>(SensorName + "/BoundDifference", 1000);
    MeanBoundDiff_pub       = n.advertise<std_msgs::Float32>(SensorName + "/BoundDifference_Mean", 1000);
    OffsetDataValue_pub     = n.advertise<std_msgs::Float32MultiArray>(SensorName + "/OffsetData", 1000);
    MeanOffsetDataValue_pub = n.advertise<std_msgs::Float32>(SensorName + "/OffsetData_Mean", 1000);

    OffsetInit_pub          = n.advertise<std_msgs::Float32MultiArray>(SensorName +"/Init_grasp",1000);
    OffsetGrasp_pub         = n.advertise<std_msgs::Float32MultiArray>(SensorName + "/GraspValue",1000);

    //TactileData_sub     = n.subscribe(TopicName, 10, &TactileSensor::Data_MultiArray_CallBack, this);
    TactileData_sub     = n.subscribe(TopicName, 10, &TactileSensor::Data_tactile_CallBack, this);

   // Calibration_srv     = n.advertiseService("Calibration", &TactileSensor::Cali_Callback);
    ros::Duration(1).sleep();
    NodeState = WAIT_GRASP;

    return true;
}

void TactileSensor::Data_MultiArray_CallBack(const std_msgs::Float32MultiArray &msg)
{
    for(int i=0; i < sensor_size; i++)
    {
        New_Data.data[i] = msg.data[i] - offset_value.data[i];  
    }
}

void TactileSensor::Data_tactile_CallBack(const read_sensor::tactile_sensor_data &msg)
{
    for(int i=0; i < sensor_size; i++)
    {
       New_Data.data[i] = msg.tactile_sensor_data[i] - offset_init.data[i] - offset_grasp.data[i];
       // New_Data.data[i] = msg.tactile_sensor_data[i] - offset_value.data[i];  
    }
}

bool TactileSensor::Cali_Callback(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res)
{
    std::cout << SensorName << ": Calibration request received \n";
    NodeState = CALIBRATION;
    return true;
}

void TactileSensor::Calibration(std_msgs::Float32MultiArray& new_offset)
{
    
    for(int i =0 ; i<no_calibration_data; i++)
    {
        for(int k=0; k<sensor_size; k++)
        {
            mean_value[k] = mean_value[k] + calibration_vector[i].data[k]; 
        }
    }
    for(int i =0; i<sensor_size; i++)
    {
        new_offset.data[i] = new_offset.data[i] + mean_value[i]/no_calibration_data;    
        mean_value[i] = 0;
    }
}


void TactileSensor::PublishBoundDiff(std_msgs::Float32MultiArray msg)
{
    BoundDiff_pub.publish(msg);
}
void TactileSensor::PublishMeanBoundDiff(float msg)
{
    std_msgs::Float32 to_send;
    to_send.data = msg;
    MeanBoundDiff_pub.publish(to_send);
}
void TactileSensor::PublishOffsetValue(std_msgs::Float32MultiArray msg)
{
    OffsetDataValue_pub.publish(msg);
}
void TactileSensor::PublishMeanOffsetValue(float msg)
{
    std_msgs::Float32 to_send;
    to_send.data = msg;
    MeanOffsetDataValue_pub.publish(to_send);
}

void TactileSensor::ControlLoop()
{
    switch (NodeState)
    {
        case START_CALI:
        {
            std::cout << "Press 1 to start cali completed";
            std::cin >> user_input;

            if(user_input == 1)
            {
                for(int k=0; k < sensor_size; k++)
                {
                    mean_value[k] = 0; 
                }
                counter = 0;
                
                NodeState = FIRST_CALI;
                std::cout << "start_cali \n";
            }
            else
            {
                ROS_ERROR("Error, retry!");
            }            
            break;
        }

        case FIRST_CALI:
        {
            counter++;
            std::cout << "calibration data:  " << counter << "\n";
            calibration_vector[counter % no_calibration_data].data = New_Data.data;

            if (counter > no_calibration_data)
            {
                // Calibration();
                Calibration(offset_init);
                std::cout << SensorName << ": Calibration Completed, start com \n";
                // NodeState = WAIT_GRASP;
                NodeState = START_COM;
            }
            break;
        }
        case WAIT_GRASP:
        {
            std::cout << "Press 1 when Grasp completed";
            std::cin >> user_input;

            if(user_input == 1)
            {
                for(int k=0; k < sensor_size; k++)
                {
                    mean_value[k] = 0; 
                }
                counter = 0;
                
                NodeState = CALIBRATION;
                std::cout << "start_cali \n";
            }
            else
            {
                ROS_ERROR("Error, retry!");
            }
            break;
        }

        case CALIBRATION:
        {
            counter++;
            std::cout << "calibration data:  " << counter << "\n";
            calibration_vector[counter % no_calibration_data].data = New_Data.data;

            if (counter > no_calibration_data)
            {
                Calibration(offset_grasp);
                std::cout << SensorName << ": Calibration Completed, start com \n";
                NodeState = START_COM;
            }
            break;
        }
        
        case START_COM:
        {
            flag_value = 0;
            for(int k=0; k < no_y_cell; k++)
            {
                Diff_Border.data[k] = New_Data.data[k*no_x_cell] - New_Data.data[no_x_cell - 1 + k*no_x_cell];

                flag_value = flag_value + Diff_Border.data[k];
            }
            Mean_Diff_Border.data = flag_value/no_y_cell;

            flag_value = 0;
            for(int i=0; i<sensor_size; i++)
            {
                OffsetValue.data[i] = New_Data.data[i];
                flag_value = flag_value + New_Data.data[i];
            }
            MeanOffsetValue.data = flag_value/sensor_size;

            SensorOutput.data = Mean_Diff_Border.data;         
            break;
        }

    }   

    BoundDiff_pub.publish(Diff_Border);         
    MeanBoundDiff_pub.publish(Mean_Diff_Border);     
    OffsetDataValue_pub.publish(OffsetValue);
    MeanOffsetDataValue_pub.publish(MeanOffsetValue);

   // OffsetInit_pub.publish(offset_init);
   // OffsetGrasp_pub.publish(offset_grasp);

                
    
    return;
}



