#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "armadillo"
#include "iostream"

using namespace arma;

class Segment {
public:
  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg);
  //sensor_msgs::LaserScan fakeScan;
};

void Segment::laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  double angle_min = msg->angle_min;
  double angle_max = msg->angle_max;
  double angle_increment =msg->angle_increment;
  double ranges_size = std::ceil((angle_max - angle_min) / angle_increment);
  mat A;
  A << 1 << angle_increment << endr
    << 0 << 1 << endr;
  mat Q;
  Q << 0.01 << 0 << endr
    << 0 << 0.01 << endr;
  mat C;
  C << 1 << 0 << endr;

  std::vector< int > breakPoint;


  int i_m = 0;

while (isinf(msg->ranges[i_m])) {
      i_m = i_m + 1;
      if (i_m > ranges_size) break;
  }

  breakPoint.push_back(i_m);
  ROS_INFO("first add i: %d", i_m);

  for (int i = i_m; i < ranges_size; i++) {
    mat x_hat;
    mat p_hat;
    mat K;
    if (i_m == i) {
      x_hat << msg->ranges[i] << endr << 0 << endr;
      p_hat << std::pow(1 / std::tan(10*3.14159265/180) * msg->ranges[i] * angle_increment, 2)
      << 0 << endr << 0 << std::pow(1 / std::tan(10*3.14159265/180) * msg->ranges[i], 2) << endr;
      }
    else {
      x_hat = A * x_hat;
      p_hat = A * p_hat * A.t() + Q;

      double v = msg->ranges[i] - as_scalar(C * x_hat);
      double s = as_scalar(C * p_hat * C.t()) + 0.03 * 0.03;

      if (pow(v,2) / s >= 3.84) {
        breakPoint.push_back(i-1);
        ROS_INFO("breakPoint add i: %d", i-1);
        if(isinf(v)) {
          while (isinf(msg->ranges[i])) {
            i = i + 1;
          }
          breakPoint.push_back(i);
          ROS_INFO("while add i: %d", i);
        } else {
          ROS_INFO("no inf");
          breakPoint.push_back(i);
        }
        i_m = i;
        i = i - 1;
      } else {
        K = p_hat * C.t() / s;
        x_hat = x_hat + K * v;
        p_hat = p_hat - K * C * p_hat;
      }
    }
  //ROS_INFO("loop_end, i: %d, i_m: %d", i, i_m);
}
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "segmenter");
  ros::NodeHandle n;

  std::string segments_topic = "/segments";
  std::string LaserScan_topic = "/laserScan";
  std::string fakeLaserScan_topic ="/fakeLaserScan";
  Segment segments;

  ros::Subscriber sub = n.subscribe(LaserScan_topic, 1, &Segment::laserCallback, &segments);
  ros::Publisher pub_segments =
  ros::Publisher pub = n.advertise<sensor_msgs::LaserScan>(fakeLaserScan_topic, 1);
  ros::Rate loop_rate(10);

  while (n.ok()) {
    //ROS_INFO("I'm here");
    //pub.publish(segments.fakeScan);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
