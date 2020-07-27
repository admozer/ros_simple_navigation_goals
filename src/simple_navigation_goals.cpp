#include <ros/ros.h>
#include "nav_msgs/Odometry.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_datatypes.h>
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

double konum = 0.0;
float tum_Konumlar[3][3];

class Listener
{
public:
  void odometryCallback(const nav_msgs::Odometry::ConstPtr &msg);
};

void Listener::odometryCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
  double z = msg->pose.pose.position.z;
  konum = z;
  //ROS_INFO("z: %f", z);
}

int main(int argc, char **argv)
{

  tum_Konumlar[0][0] = 6.3123;
  tum_Konumlar[0][1] = -4.4526;
  tum_Konumlar[0][2] = -50;

  tum_Konumlar[1][0] = 0.8100;
  tum_Konumlar[1][1] = 1.6451;
  tum_Konumlar[1][2] = 90;

  tum_Konumlar[2][0] = -6.9616;
  tum_Konumlar[2][1] = -3.1483;
  tum_Konumlar[2][2] = -50;

  ros::init(argc, argv, "simple_navigation_goals");

  ros::NodeHandle nh;

  Listener listener;
  //ros::Subscriber sub = n.subscribe("chatter", 1000, &Listener::callback, &listener);

  ros::Subscriber subscriber = nh.subscribe("odom", 10, &Listener::odometryCallback, &listener);

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while (!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;
  int i = 0;
  //we'll send a goal to the robot to move 1 meter forward
  while (ros::ok())
  {
    ros::spinOnce();

    goal.target_pose.header.frame_id = "odom";
    goal.target_pose.header.stamp = ros::Time::now();

    goal.target_pose.pose.position.x = tum_Konumlar[i][0];
    goal.target_pose.pose.position.y = tum_Konumlar[i][1];
    goal.target_pose.pose.position.z = konum;
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(tum_Konumlar[i][2] * M_PI / 180);
    //goal.target_pose.pose.orientation.w = 100.0;
    i++;
    if(i==3){
      i=0;
    }
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);

    ac.waitForResult();

    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("The base moved");
    else
      ROS_INFO("The base failed to move");
  }

  return 0;
}
