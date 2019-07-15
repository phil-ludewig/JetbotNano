// In progress...!
// Node subscribes to cmd_vel topic and controls Sparkfun Stepper AutoDriver

#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include 

void control_motor(const geometry_msgs::Twist msg)
{
  //ROS_INFO("Teleop received!");
  float linear_x = msg.linear.x;
  ROS_INFO_STREAM(linear_x);
}

int main(int argc, char** argv)
{
    // Initialize node
    ros::init(argc, argv, "motor_teleop");
    ros::NodeHandle n;

    ros::Subscriber teleop_sub = n.subscribe("/cmd_vel", 10, control_motor);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
