#include "ros/ros.h"
#include "segment_msgs/segment_msgs.h"
#include "object_msgs/object_msgs.h"
#include "iostream"


class Featurex {
public:
  Featurex(std::vector<float> ranges_in, int startPoint_in, int endPoint_in, double angle_min_in, double angle_increment_in);
  int getFeature();
private:
  std::vector <float> ranges;
  void init();
  int startPoint;
  int endPoint;
  double angle_min;
  double angle_increment;
  int type; // Circle 0; Line 1; Box 2
  double angleMinIdx;
  double checkAngle(int startIdx, int endIdx, double startPoint_range,
  double startPoint_angle, double endPoint_range, double endPoint_angle);
};

  Featurex::Featurex (
  std::vector<float> ranges_in,
  int startPoint_in,
  int endPoint_in,
  double angle_min_in,
  double angle_increment_in
) {
  ranges = ranges_in;
  startPoint = startPoint_in;
  endPoint = endPoint_in;
  angle_min = angle_min_in;
  angle_increment = angle_increment_in;
}

double Featurex::checkAngle(int startIdx, int endIdx, double startPoint_range,
  double startPoint_angle, double endPoint_range, double endPoint_angle) {
    double angleSum = 0;
    double angleMin = 1.6;
  for (int i = startIdx + 1; i < endIdx; i ++ ) {
    double pointIt_range = ranges[i];
    double pointIt_angle = angle_min + angle_increment * i;
    double vector_IS_x = startPoint_range * cos(startPoint_angle) -
    pointIt_range * cos(pointIt_angle);
    double vector_IS_y = startPoint_range * sin(startPoint_angle) -
    pointIt_range * sin(pointIt_angle);
    double vector_IE_x = endPoint_range * cos(endPoint_angle) -
    pointIt_range * cos(pointIt_angle);
    double vector_IE_y = endPoint_range * sin(endPoint_angle) -
    pointIt_range * sin(pointIt_angle);
    double angleIt = acos( (vector_IE_x*vector_IS_x + vector_IE_y*vector_IS_y)
    / (sqrt(pow(vector_IE_x, 2) + pow(vector_IE_y, 2)) *
    sqrt(pow(vector_IS_x, 2) + pow(vector_IS_y, 2))));

    angleSum += angleIt;
    if (angleMin > angleIt) {
      angleMin = angleIt;
      angleMinIdx = i;
    }
  }
  double angleAve = angleSum / (endIdx - startIdx);
  return angleAve;
}

int Featurex::getFeature () {
  double startPoint_range = ranges[startPoint];
  double startPoint_angle = angle_min + angle_increment * startPoint;
  double endPoint_range = ranges[endPoint];
  double endPoint_angle = angle_min + angle_increment * endPoint;

  double angleSum = 0;
  double angleMax = -1.0;
  double angleMin = 3.2;
  double angleMaxIdx = 0;
  double angleMinIdx = 0;

  for (int i = startPoint + 1; i < endPoint; i ++ ) {
    double pointIt_range = ranges[i];
    //std::cout << "pointIt_range: " << pointIt_range << std::endl;
    double pointIt_angle = angle_min + angle_increment * angleMinIdx;;
    //std::cout << "pointIt_angle: " << pointIt_angle << std::endl;
    double vector_IS_x = startPoint_range * cos(startPoint_angle) -
    pointIt_range * cos(pointIt_angle);
    //std::cout << "vector_IS_x: " << vector_IS_x << std::endl;
    double vector_IS_y = startPoint_range * sin(startPoint_angle) -
    pointIt_range * sin(pointIt_angle);
    //std::cout << "vector_IS_y: " << vector_IS_y << std::endl;
    double vector_IE_x = endPoint_range * cos(endPoint_angle) -
    pointIt_range * cos(pointIt_angle);
    //std::cout << "vector_IE_x: " << vector_IE_x << std::endl;
    double vector_IE_y = endPoint_range * sin(endPoint_angle) -
    pointIt_range * sin(pointIt_angle);
    //std::cout << "vector_IE_y: " << vector_IE_y << std::endl;
    double angleIt = acos( (vector_IE_x*vector_IS_x + vector_IE_y*vector_IS_y)
    / (sqrt(pow(vector_IE_x, 2) + pow(vector_IE_y, 2)) *
    sqrt(pow(vector_IS_x, 2) + pow(vector_IS_y, 2))));
    std::cout << "angleIt: " <<  angleIt << std::endl;
    std::cout << "i: " <<  i << std::endl;
    //std::cout << "cos: " <<  (vector_IE_x*vector_IS_x + vector_IE_y*vector_IS_y)
    /// (sqrt(pow(vector_IE_x, 2) + pow(vector_IE_y, 2)) *
    //sqrt(pow(vector_IS_x, 2) + pow(vector_IS_y, 2))) << std::endl;
    angleSum += angleIt;
    //std::cout << "angleSum: " <<  (vector_IE_x*vector_IS_x + vector_IE_y*vector_IS_y)
    /// (sqrt(pow(vector_IE_x, 2) + pow(vector_IE_y, 2)) *
    //sqrt(pow(vector_IS_y, 2) + pow(vector_IS_y, 2))) << std::endl;
    if (angleMax < angleIt) {
      angleMax = angleIt;
      angleMaxIdx = i;
    }
    if (angleMin > angleIt) {
      angleMin = angleIt;
      angleMinIdx = i;
    }
  }

  double angleAve = angleSum / (ranges.size() - 2);
  std::cout << angleAve << std::endl;
  if (angleAve < 3.15 & angleAve > 3.13) {
    type = 1
    objects.center_x;
  } else if (angleAve < 1.58 & angleAve < 1.56) {
    type = 0;
  } else if (angleMin < 1.58 & angleMin < 1.56) {
    double angleAveFirst = checkAngle( startPoint, angleMinIdx, startPoint_range,
      startPoint_angle, ranges[angleMinIdx], angle_min + angle_increment * angleMinIdx);
    double angleAveSecond = checkAngle( startPoint, angleMinIdx, ranges[angleMinIdx],
      angle_min + angle_increment * angleMinIdx, endPoint_range, endPoint_angle);
      if (angleAveFirst < 3.15 & angleAveFirst > 3.13 & angleAveSecond < 3.15 & angleAveSecond > 3.13) {
        type = 2;
      }
  } else {
      type = 3;
    }
    return type;
}

void segmentsCallback(const segment_msgs::segment_msgs::ConstPtr& msg) {
  std::vector<int> types;
  if (msg->segments.size() < 2) return;
  //std::cout << msg->segments.size() << std::endl;
  for (int i = 0; i < msg->segments.size(); i = i + 2) {
    Featurex featurex( msg->ranges,
      msg->segments[i],
      msg->segments[i+1],
      -1.57079994678,//double angle_min_in,
      0.00436940183863);//double angle_increment_in
      object_msgs::object_msgs objects;
      int type = featurex.getFeature();
      std::cout << type << std::endl;
      types.push_back(type);
  }
  std::cout << "loop again" << std::endl;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "feature_extractor");
  ros::NodeHandle n;
  std::cout << "loaded" << std::endl;

  std::string segments_topic = "/segments";

  ros::Subscriber sub_seg = n.subscribe(segments_topic, 1, segmentsCallback);
  //ros::Publisher pub = n.advertise<std_msgs::Int8MultiArray>(segments_topic, 1);
  ros::Rate loop_rate(10);

  while (n.ok()) {
    //ROS_INFO("I'm here");
    //pub.publish(segments.fakeScan);
    //vis_pub.publish(segments.points);
    //pub.publish(segments.breakPoint_msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
