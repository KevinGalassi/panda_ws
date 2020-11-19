#include "TactileSensor.cpp"


int main(int argc, char** argv)
{

    ros::init(argc, argv, "FingerReadNode");
    ros::NodeHandle n_;

    TactileSensor SensorB;
    TactileSensor SensorD;

    SensorB.no_calibration_data = 500;
    SensorB.no_x_cell = 5;
    SensorB.no_y_cell = 5;
    SensorB.ul_move = 0.04;
    SensorB.ul_tens = 0.015;
    SensorB.ll_move = 0.04;
    SensorB.ll_tens = 0.015;
    SensorB.Orientation = true;
    SensorB.SensorName = "/SensorB";

    SensorD.no_calibration_data = 500;
    SensorD.no_x_cell = 5;
    SensorD.no_y_cell = 5;
    SensorD.ul_move = 0.04;
    SensorD.ul_tens = 0.015;
    SensorD.ll_move = 0.04;
    SensorD.ll_tens = 0.015;
    SensorD.Orientation = false;
    SensorD.SensorName = "/SensorD";


    SensorB.TopicName = "/TactileData_dev_ttyUSB1_F101";     // Porta B
    SensorD.TopicName = "/TactileData_dev_ttyUSB3_F102";     // Porta D

    std::cout << "sensor init";
    SensorB.SensorInit();
    SensorD.SensorInit();

    std_msgs::Float32MultiArray DiffX, DiffX_avg;
    std::vector<std_msgs::Float32MultiArray> DiffY;
    DiffX_avg.data.resize(5);
    DiffX.data.resize(5);
    DiffY.resize(2);
    DiffY[0].data.resize(5);
    DiffY[1].data.resize(5);

    // Message creation
    std_msgs::Float32 MeanSum_msg, MeanY0, MeanY1;
    MeanSum_msg.data = 0;
    MeanY0.data = 0;
    MeanY1.data = 0;

    std_msgs::Float32MultiArray SumDiff_msg, DiffY0_msg, DiffY1_msg;
    SumDiff_msg.data.resize(5);
    DiffY0_msg.data.resize(5);
    DiffY1_msg.data.resize(5);

    std_msgs::Float32 tactile_feedback;
    tactile_feedback.data = 0.0;       // Output to use for the gripper regulator



    // Publisher creation

    ros::Publisher tactile_pub = n_.advertise<std_msgs::Float32>("/TactileFeedback", 1000);

    ros::Publisher SumDiff_pub  = n_.advertise<std_msgs::Float32MultiArray>("/SumDiff",1000 );
    ros::Publisher MeanSumDiff_pub      = n_.advertise<std_msgs::Float32>("/SumDiff_Mean",1000 );
        
    
    
    /*
    ros::Publisher DiffY0_pub       = n_.advertise<std_msgs::Float32MultiArray>("/DiffY_0",1000 );
    ros::Publisher DiffY1_pub       = n_.advertise<std_msgs::Float32MultiArray>("/DiffY_1",1000 );
    ros::Publisher MeanDiffY0_pub   = n_.advertise<std_msgs::Float32>("/DiffY_0_Mean",1000 );
    ros::Publisher MeanDiffY1_pub   = n_.advertise<std_msgs::Float32>("/DiffY_1_Mean",1000 );
    */

    ros::Duration(2).sleep();

    ros::Rate loop_rate(60);

    std::cout << "start \n";

    while(ros::ok())
    {
        SensorB.ControlLoop();
        SensorD.ControlLoop();
    
        tactile_feedback.data = (-SensorD.SensorOutput.data + SensorB.SensorOutput.data)/2;
        for(int i=0; i<5; i++)
        {
            DiffX.data[i] = SensorD.Diff_Border.data[i] - SensorB.Diff_Border.data[i];              // Differenza dei due valori
            DiffX_avg.data[i] = (SensorD.Diff_Border.data[i] + SensorB.Diff_Border.data[i])/2;

            MeanSum_msg.data = MeanSum_msg.data + DiffX.data[i];
            /*
            DiffY[0].data[i] = SensorB.New_Data.data[i*5] - SensorD.New_Data.data[4+i*5];
            DiffY[1].data[i] = SensorB.New_Data.data[4+i*5] - SensorD.New_Data.data[i*5];
            MeanY0.data = MeanY0.data + DiffY[0].data[i];
            MeanY1.data = MeanY1.data + DiffY[1].data[i];
            */
        }

        for(int i=0; i<5; i++)
        {
            SumDiff_msg.data[i] = DiffX.data[i]; 
           // DiffY0_msg.data[i] = DiffY[0].data[i];
            //DiffY1_msg.data[i] = DiffY[1].data[i];
        }

        SumDiff_pub.publish(SumDiff_msg);
        //DiffY0_pub.publish(DiffY0_msg);
        //DiffY1_pub.publish(DiffY1_msg);
    
    
        MeanSum_msg.data = MeanSum_msg.data/5;      // Media delle differenze

        tactile_pub.publish(tactile_feedback);



        /*
        MeanY0.data = MeanY0.data/5;
        MeanY1.data = MeanY1.data/5;
        MeanDiffY0_pub.publish(MeanY0);
        MeanDiffY1_pub.publish(MeanY1);
        */

        

        if(MeanSum_msg.data > 0.1)
            std::cout << "Pull front \n";
        else if(MeanSum_msg.data > 0.05)
            std::cout << "Tension front \n";
        if(MeanSum_msg.data < -0.1)
            std::cout << "Pull front \n";
        else if(MeanSum_msg.data < -0.05)
            std::cout << "Tension front \n";
        
        MeanSum_msg.data = 0.0;
        


        ros::spinOnce();
        loop_rate.sleep();
    }

    ros::shutdown();
    return 0;
}
