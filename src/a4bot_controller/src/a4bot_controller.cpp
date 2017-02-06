#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"

class Controller {
public:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& msg);
  geometry_msgs::Twist ctrl_msg;
};

void Controller::joyCallback(const sensor_msgs::Joy::ConstPtr& msg) {
  double axes_heading = msg->axes[0];
  double axes_vel_up = - msg->axes[5];
  double axes_vel_dw = msg->axes[2];

  const int k_vel = 1;
  const int k_ang = 1;

  ctrl_msg.linear.x = k_vel * (axes_vel_up + axes_vel_dw) / 2;
  ctrl_msg.angular.z = k_ang * axes_heading;
};


int main(int argc, char **argv)
{
  ros::init(argc, argv, "a4bot_controller");
  ros::NodeHandle n;

  std::string joy_topic = "/joy";
  std::string control_topic = "/sim_p3at/cmd_vel";

  Controller controller;

  ros::Subscriber sub_joy = n.subscribe(joy_topic, 1,&Controller::joyCallback, &controller);
  ros::Publisher pub = n.advertise<geometry_msgs::Twist>(control_topic, 1);

  ros::Rate loop_rate(10);
  while (n.ok()) {
    pub.publish(controller.ctrl_msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
