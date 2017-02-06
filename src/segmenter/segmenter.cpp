#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Point.h"
#include "visualization_msgs/Marker.h"
#include "segment_msgs/segment_msgs.h"
#include "armadillo"
#include "iostream"

using namespace arma;

class Segment {
public:
  void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg);
  void publishBreakPionts();

  visualization_msgs::Marker points;
  segment_msgs::segment_msgs segment_msg;
private:
  std::string frame_id;
  double angle_min;
  double angle_max;
  double angle_increment;
  void visualization();
  void publishRanges();
  std::vector< int > breakPoint;
  std::vector< float >ranges;
};

void Segment::laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  frame_id = msg->header.frame_id;
  angle_min = msg->angle_min;
  angle_max = msg->angle_max;
  angle_increment =msg->angle_increment;

  double range_max = msg->range_max;
  double ranges_size = std::ceil((angle_max - angle_min) / angle_increment);
  ranges = msg->ranges;

  mat A;
  A << 1 << angle_increment << endr
    << 0 << 1 << endr;
  mat Q;
  Q << 0.01 << 0 << endr
    << 0 << 0.01 << endr;
  mat C;
  C << 1 << 0 << endr;

  std::vector< int > breakPointIt;
  int i_m = 0;
  mat x_hat;
  mat p_hat;
  mat K;

  for (int i = i_m; i < ranges_size; i++) {

    if (isinf(ranges[i])) {
      ranges[i] = range_max + 10;
    }

    if (i_m == i) {
      x_hat << ranges[i] << endr << 0 << endr;
    //  cout << x_hat << endl;
      p_hat << std::pow(1 / std::tan(10*3.14159265/180) * ranges[i] * angle_increment, 2)
      << 0 << endr << 0 << std::pow(1 / std::tan(10*3.14159265/180) * ranges[i], 2) << endr;
      //cout << i_m << endl;
      if (ranges[i_m] > range_max & i_m != 0) {
        breakPointIt.push_back(i_m-1);
      }
      else {
        if (ranges[i_m-1] < range_max & i_m != 0) breakPointIt.push_back(i_m-1);
        breakPointIt.push_back(i_m);
      }
    }
    else {
    //  cout << A << endl;
      x_hat = A * x_hat;
    //  cout << x_hat << endl;
      p_hat = A * p_hat * A.t() + Q;
    //  ROS_INFO("2");
      double v = ranges[i] - as_scalar(C * x_hat);
      double s = as_scalar(C * p_hat * C.t()) + 0.03 * 0.03;

      if (pow(v,2) / s >= 3.84) {
        i_m = i;
        i = i - 1;
      }
      else {
        K = p_hat * C.t() / s;
        x_hat = x_hat + K * v;
        p_hat = p_hat - K * C * p_hat;
      }
    }
  }
  if (ranges[ranges.size()-1] <= range_max) breakPointIt.push_back(ranges.size()-1);
  breakPoint = breakPointIt;
  visualization();
  publishBreakPionts();
  publishRanges();
}

void Segment::publishBreakPionts() {
  segment_msg.segments.clear();
  for (int i = 0;  i < breakPoint.size(); i++) {
  segment_msg.segments.push_back(breakPoint[i]);
  //cout << breakPoint[i] << endl;
  //cout << segment_msg.segments[i] << endl;
 }
 //cout << "i'm called" << endl;
}

void Segment::publishRanges() {
  segment_msg.ranges.clear();
  for (int i = 0; i < ranges.size(); i++) {
    segment_msg.ranges.push_back(ranges[i]);
  }
}

void Segment::visualization() {
  points.header.frame_id = frame_id;
  points.header.stamp = ros::Time();
  points.ns = "Points";
  points.id = 0;
  points.type = visualization_msgs::Marker::POINTS;
  points.action = 0;

  points.scale.x = 0.05;
  points.scale.y = 0.05;

  points.color.g = 1.0f;
  points.color.a = 1.0;

  //points.lifetime = ros::Duration();
  points.points.clear();

  for (int i = 0;  i < breakPoint.size(); i++) {

    geometry_msgs::Point p;
    p.x = ranges[breakPoint[i]] * cos(angle_min + angle_increment * breakPoint[i]);
    p.y = ranges[breakPoint[i]] * sin(angle_min + angle_increment * breakPoint[i]);
    p.z = 0;
    //std::cout << breakPoint[i] << std::endl;
    points.points.push_back(p);

   // The line list needs two points for each line
   }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "segmenter");
  ros::NodeHandle n;
  cout << "loaded" << endl;

  std::string segments_topic = "/segments";
  std::string LaserScan_topic = "/laserScan";
  std::string ranges_topic = "/ranges";
  Segment segments;

  ros::Subscriber sub = n.subscribe(LaserScan_topic, 1, &Segment::laserCallback, &segments);
  ros::Publisher vis_pub = n.advertise<visualization_msgs::Marker>( "visualization_marker", 1 );
  ros::Publisher pub = n.advertise<segment_msgs::segment_msgs>(segments_topic, 1);
  ros::Rate loop_rate(10);

  while (n.ok()) {
    //ROS_INFO("I'm here");
    //pub.publish(segments.fakeScan);
    vis_pub.publish(segments.points);
    pub.publish(segments.segment_msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
