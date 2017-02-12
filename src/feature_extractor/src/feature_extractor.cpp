#include "ros/ros.h"
#include "segment_msgs/segment_msgs.h"
#include "object_msgs/object_msgs.h"
#include "iostream"

struct Object {
  int type;
  float center_x;
  float center_y;
  float edge1_x;
  float edge1_y;
  float edge2_x;
  float edge2_y;
};

class Extractor {
public:
  void segmentsCallback(const segment_msgs::segment_msgs::ConstPtr& msg);
  object_msgs::object_msgs objects;
};

class Featurex {
public:
  Featurex(std::vector<float> ranges_in, int startPoint_in, int endPoint_in,
    double angle_min_in, double angle_increment_in);
  int getFeature();
  struct Object object;
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
    double getDistance(int idx_a, int idx_b);
  std::vector <double> getCenter(int startPoint, int endPoint);
  double checkDistance(int startIdx, int endIdx, double startPoint_range,
    double startPoint_angle, double endPoint_range, double endPoint_angle);
  std::vector <double> center;
  std::vector <double> center_eg_one;
  std::vector <double> center_eg_two;
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

  std::vector <double> Featurex::getCenter(int startPoint, int endPoint) {
    double s_x = ranges[startPoint]* cos(angle_min + angle_increment * startPoint);
    double s_y = ranges[startPoint]* sin(angle_min + angle_increment * startPoint);
    double e_x = ranges[endPoint]* cos(angle_min + angle_increment * endPoint);
    double e_y = ranges[endPoint]* sin(angle_min + angle_increment * endPoint);
    std::vector <double> cen;
    cen.push_back((s_x+e_x)/2);
    cen.push_back((s_y+e_y)/2);
    return cen;
  }

  double Featurex::getDistance(int idx_a, int idx_b) {
    double a_x = ranges[idx_a]* cos(angle_min + angle_increment * idx_a);
    double a_y = ranges[idx_a]* sin(angle_min + angle_increment * idx_a);
    double b_x = ranges[idx_b]* cos(angle_min + angle_increment * idx_b);
    double b_y = ranges[idx_b]* sin(angle_min + angle_increment * idx_b);
    double distance = sqrt(pow(a_x - b_x, 2) + pow(a_y - b_y, 2));
    return distance;
  }

  double Featurex::checkAngle(int startIdx, int endIdx, double startPoint_range,
    double startPoint_angle, double endPoint_range, double endPoint_angle) {
      double angleSum = 0;
      double angleMin = 1.6;
      double k = 0;

      for (int i = startIdx + 1; i < endIdx; i ++ ) {
        double pointIt_range = ranges[i];
        //std::cout << "pointIt_range: " << pointIt_range << std::endl;
        double pointIt_angle = angle_min + angle_increment * i;
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
        double angleIt = atan2(vector_IE_y, vector_IE_x) - atan2(vector_IS_y, vector_IS_x);
        //angleIt = k*angleIt;

        //angleIt = std::arg(IE) - std::arg(IS) + 3.141592653;
        //std::cout << "angleIS: " <<  atan2(vector_IS_x, vector_IS_y) << std::endl;
        //std::cout << "angleIE: " <<  atan2(vector_IE_x, vector_IE_y) << std::endl;
      //  if (i == startPoint + 1) angleIt_old = angleIt;
        //angleIt = angleIt * 0.98 + angleIt_old * (1-0.98);
        //angleIt_old = angleIt;
        angleSum += angleIt;
      }
      double angleAve = angleSum / (endIdx - startIdx);
      return angleAve;
    }

    double Featurex::checkDistance(int startIdx, int endIdx, double startPoint_range,
      double startPoint_angle, double endPoint_range, double endPoint_angle) {

        double d_max = -1;
        double d_maxIdx = -1;

        double A= startPoint_range * sin(startPoint_angle) - endPoint_range * sin(endPoint_angle);
        double B= endPoint_range * cos(endPoint_angle) - startPoint_range * cos(startPoint_angle);
        double C= startPoint_range * cos(startPoint_angle) * endPoint_range * sin(endPoint_angle) -
        endPoint_range * cos(endPoint_angle) * startPoint_range * sin(startPoint_angle);
        //std::cout << "startPoint_x_in: " << startPoint_range * cos(startPoint_angle) << std::endl;
        //std::cout << "startPoint_y_in: " << startPoint_range * sin(startPoint_angle) << std::endl;
        //std::cout << "endPoint_x_in: " << endPoint_range * cos(endPoint_angle) << std::endl;
        //std::cout << "endPoint_y_in: " << endPoint_range * sin(endPoint_angle) << std::endl;


        for (int i = startIdx + 1; i < endIdx; i ++ ) {
          //std::cout << "A: " << A << std::endl;
          //std::cout << "B: " << B << std::endl;
          //std::cout << "C: " << C << std::endl;

          double pointIt_range = ranges[i];

          double pointIt_angle = angle_min + angle_increment * i;

          //std::cout << "pointIt_x: " << pointIt_range * cos(pointIt_angle) << std::endl;
          //std::cout << "pointIt_y: " << pointIt_range * sin(pointIt_angle)<< std::endl;

          //std::cout << "top" << abs(pointIt_range * cos(pointIt_angle)) << std::endl;
          double d = fabs(A * pointIt_range * cos(pointIt_angle) + B * pointIt_range * sin(pointIt_angle) + C) /
          sqrt(pow(A, 2) + pow(B, 2));

          if (d > d_max) {
            d_max = d;
            d_maxIdx = i;
          }
        //  std::cout << "d : " << d << std::endl;
        //  std::cout << "i : " << i << std::endl;
        }
        //std::cout << "d_max:" << d_max << std::endl;
        //std::cout << "pointIt_x: " << ranges[d_maxIdx] * cos(angle_min + angle_increment * d_maxIdx) << std::endl;
        //std::cout << "pointIt_y: " << ranges[d_maxIdx] * sin(angle_min + angle_increment * d_maxIdx)<< std::endl;
        //std::cout << "d_maxIdx:" << d_maxIdx << std::endl;
        return d_maxIdx;
      };

    int Featurex::getFeature () {
      double startPoint_range = ranges[startPoint];
      //std::cout << "startPoint_range: " << startPoint_range << std::endl;
      double startPoint_angle = angle_min + angle_increment * startPoint;
      //std::cout << "startPoint_angle: " << startPoint_angle << std::endl;
      double endPoint_range = ranges[endPoint];
      //std::cout << "endPoint_range: " << endPoint_range << std::endl;
      double endPoint_angle = angle_min + angle_increment * endPoint;
      //std::cout << "endPoint_angle: " << endPoint_angle << std::endl;

      double angleSum = 0;
      double angleMax = -1.0;
      double angleMin = 4;
      double angleMaxIdx = 0;
      double angleMinIdx = 0;
      //double angleIt_old = 0;

      double IdxOfMaxDistPoint = checkDistance( startPoint, endPoint, startPoint_range,
        startPoint_angle, endPoint_range, endPoint_angle);

      for (int i = startPoint + 1; i < endPoint; i ++ ) {
        double pointIt_range = ranges[i];
        //std::cout << "pointIt_range: " << pointIt_range << std::endl;
        double pointIt_angle = angle_min + angle_increment * i;
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

        double angleIt = atan2(vector_IE_y, vector_IE_x) - atan2(vector_IS_y, vector_IS_x);
        //angleIt = std::arg(IE) - std::arg(IS) + 3.141592653;
        //std::cout << "angleIS: " <<  atan2(vector_IS_x, vector_IS_y) << std::endl;
        //std::cout << "angleIE: " <<  atan2(vector_IE_x, vector_IE_y) << std::endl;
      //  if (i == startPoint + 1) angleIt_old = angleIt;
        //angleIt = angleIt * 0.98 + angleIt_old * (1-0.98);
        //angleIt_old = angleIt;
        angleSum += angleIt;
        //  std::cout << "angleSum: " << angleSum << std::endl;
        if (angleMax < angleIt) {
          angleMax = angleIt;
          angleMaxIdx = i;
        }
        if (i == IdxOfMaxDistPoint) {
          angleMinIdx = IdxOfMaxDistPoint;
          angleMin = angleIt;
        }
      }

      double angleAve = angleSum / (endPoint-startPoint - 2);

      std::cout << "angleAve: " <<angleAve << std::endl;
      std::cout << "angleMin: " <<angleMin << std::endl;
      std::cout << "angleMinidx: " <<angleMinIdx << std::endl;

      //std::cout << "startPoint_range: " <<startPoint_range << std::endl;
      //std::cout << "endPoint_range: " <<endPoint_range << std::endl;
        //std::cout << "middle_angle: " << startPoint_angle + angle_increment * IdxOfMaxDistPoint << std::endl;

      if (angleAve < 3.17 & angleAve > 3.12) {
        //  std::cout << "first" << std::endl;
        type = 1;
        center = getCenter(startPoint, endPoint);
        object.type = 1;
        object.center_x = center[0];
        object.center_y = center[1];
        object.edge1_x = 0;
        object.edge1_y = 0;
        object.edge2_x = 0;
        object.edge2_y = 0;
      }
      else if ( angleMin > 1.3 & angleMin < 1.9) {
        //std::cout << "angleMin:" << angleMin << std::endl;
        double angleAveFirst = checkAngle( startPoint, angleMinIdx, startPoint_range,
          startPoint_angle, ranges[angleMinIdx], angle_min + angle_increment * angleMinIdx);
          double angleAveSecond = checkAngle( angleMinIdx, endPoint, ranges[angleMinIdx],
            angle_min + angle_increment * angleMinIdx, endPoint_range, endPoint_angle);
            std::cout << angleAveFirst << std::endl;
            std::cout << angleAveSecond << std::endl;
          if (true) {//angleAveFirst < 3.3 & angleAveFirst > 2 & angleAveSecond < 3.3 & angleAveSecond > 2) {
              double dis_SI = getDistance(startPoint, IdxOfMaxDistPoint);
              double dis_EI = getDistance(endPoint, IdxOfMaxDistPoint);
              //std::cout << dis_SI << std::endl;
              //std::cout << dis_EI << std::endl;
              if ( dis_SI < 1 & dis_SI >0.1 & dis_EI < 1 & dis_EI > 0.1 ) {
                //std::cout << "Sec" << std::endl;
                type = 2;
                center = getCenter(startPoint, endPoint);
                //center_eg_one = getCenter(startPoint, angleMinIdx);
                //center_eg_two = getCenter(endPoint, angleMinIdx);
                object.type = 2;
                object.center_x = center[0];
                object.center_y = center[1];
                object.edge1_x = ranges[startPoint] * cos(angle_min + angle_increment * startPoint);
                object.edge1_y = ranges[startPoint] * sin(angle_min + angle_increment * startPoint);
                object.edge2_x = ranges[endPoint] * cos(angle_min + angle_increment * endPoint);
                object.edge2_y = ranges[endPoint] * sin(angle_min + angle_increment * endPoint);
              }
              else {
                type = 3;
                object.type = 3;
              }
            }
            else {
              type = 4;
              object.type = 4;
            }
          }
          else {
            type = 5;
            object.type = 5;
          }

          return type;
        }

void Extractor::segmentsCallback(const segment_msgs::segment_msgs::ConstPtr& msg) {
  std::vector<int> types;
  object_msgs::object_msgs objectIt;
  if (msg->segments.size() < 2) return;
  for (int i = 0; i < msg->segments.size(); i = i + 2) {
    Featurex featurex( msg->ranges,
      msg->segments[i],
      msg->segments[i+1],
      -1.57079994678,//double angle_min_in,
      0.00436940183863);//double angle_increment_in
      int type = featurex.getFeature();
      std::cout << type << std::endl;
      types.push_back(type);
      objectIt.types.push_back(featurex.object.type);
      objectIt.center_x.push_back(featurex.object.center_x);
      objectIt.center_y.push_back(featurex.object.center_y);
      objectIt.edge1_x.push_back(featurex.object.edge1_x);
      objectIt.edge1_y.push_back(featurex.object.edge1_y);
      objectIt.edge2_x.push_back(featurex.object.edge2_x);
      objectIt.edge2_y.push_back(featurex.object.edge2_y);
    }
    std::cout << "loop again" << std::endl;
    objects = objectIt;
  }

int main(int argc, char **argv)
{
  ros::init(argc, argv, "feature_extractor");
  ros::NodeHandle n;
  std::cout << "loaded" << std::endl;

  std::string segments_topic = "/segments";
  std::string object_topic = "/objects";
  Extractor extractor;

  ros::Subscriber sub_seg = n.subscribe(segments_topic, 1, &Extractor::segmentsCallback, &extractor);
  ros::Publisher pub = n.advertise<object_msgs::object_msgs>(object_topic, 1);
  ros::Rate loop_rate(10);

  while (n.ok()) {
    pub.publish(extractor.objects);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
