#include <ros/ros.h>
#include <std_msgs/Float32.h>

#include <actionlib/server/simple_action_server.h>
#include <planner/ComputeTrajectoryAction.h>
#include <planner/ComputeTrajectoryGoal.h>
#include <planner/ComputeTrajectoryResult.h>
#include <planner/ComputeTrajectoryFeedback.h>

#include <TrajectoryPlanner.h>



class TrajectoryActionServer
{
   public:

   TrajectoryActionServer(std::string name):
      action_name_(name),
      as_(nh_, action_name_, boost::bind(&TrajectoryActionServer::ComputeTrajectory_cb, this, _1), false)
   {
      //register the goal and feeback callbacks
     // as_.registerGoalCallback(boost::bind(&ComputeTrajectoryAction::goalCB, this));
      as_.start();
   }

   ~TrajectoryActionServer(void)
   {
   }


   void ComputeTrajectory_cb(const planner::ComputeTrajectoryGoalConstPtr& goal)
   {
      TrajectoryPlanner new_traj;


      for(int i=0; i< goal->InputPoint.poses.size(); i++)
      {
         new_traj.InitPoint.poses.push_back(goal->InputPoint.poses[i]);
         new_traj.InitLabel.push_back(goal->InitLabel[i]);
      }
      feedback_.output = "Input point readen";
      as_.publishFeedback(feedback_);

      new_traj.TrajectoryPlanning();
      as_.publishFeedback(feedback_);
      new_traj.FromEE2Link8();
      feedback_.output = "Trajectory planned";

      
      geometry_msgs::PoseArray PoseArray_flag;
      for(int i=0; i< new_traj.SecondaryTrajectories.size(); i++)
      {
         for(int j=0; j<new_traj.SecondaryTrajectories[i].poses.size(); j++)
            PoseArray_flag.poses.push_back(new_traj.SecondaryTrajectories[i].poses[j]);

         result_.FinalTrajectories.push_back(PoseArray_flag);
         result_.FinalLabel.push_back(new_traj.label_list[i]);

         PoseArray_flag.poses.clear();
      }
      

      as_.setSucceeded(result_);
      ROS_INFO("TrajectoryPlanner: Completed");

      return ;
   }


   protected:

   ros::NodeHandle nh_;

   actionlib::SimpleActionServer<planner::ComputeTrajectoryAction> as_;
   
   planner::ComputeTrajectoryGoal goal_;
   planner::ComputeTrajectoryFeedback feedback_;
   planner::ComputeTrajectoryResult result_;

   std::string action_name_;
   
};

int main (int argc, char** argv)
{
   ros::init(argc, argv, "trajectory_planner_action_server");

   TrajectoryActionServer trajectoryserver(ros::this_node::getName());
   ros::spin();

   return 0;
}