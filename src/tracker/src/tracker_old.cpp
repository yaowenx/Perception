#include "ros/ros.h"
#include "visualization_msgs/Marker.h"
#include "object_msgs/object_msgs.h"
#include "iostream"
#include <vector>
#include <Eigen/Dense>
#include "kalman.hpp"
#include "kalman.cpp"

KalmanFilter getTrackerEKF() {
  int n = 4; // Number of states
  int m = 2; // Number of measurements

  double dt = 0.1; // Time step

  Eigen::MatrixXd A(n, n); // System dynamics matrix
  Eigen::MatrixXd C(m, n); // Output matrix
  Eigen::MatrixXd Q(n, n); // Process noise covariance
  Eigen::MatrixXd R(m, m); // Measurement noise covariance
  Eigen::MatrixXd P(n, n); // Estimate error covariance

  // Discrete LTI projectile motion, measuring position only
  A << 1, dt, 0, 0,     0, 1, 0, 0,     0, 0, 1, dt,    0, 0, 0, 1;
  C << 1, 0, 0, 0, 0, 0, 1, 0;

       // Reasonable covariance matrices
  Q << .05*0.05, .05, .0, .0,
       .0, .05, .0, .0,
       .0, .0, .05, .05*0.05,
       .0, .0, .0, .05;
  R << .01*0.01, .01*0.01/10,
       .01*0.01/10, 0.01*0.01;
  P << 100*100.0, 0, 0, 0,
       0, 10*10, 0, 0,
       0, 0, 25*25, 0,
       0, 0, 0, 10*10;

  //std::cout << "A: \n" << A << std::endl;
  //std::cout << "C: \n" << C << std::endl;
  //std::cout << "Q: \n" << Q << std::endl;
  //std::cout << "R: \n" << R << std::endl;
  //std::cout << "P: \n" << P << std::endl;

  // Construct the filter
  KalmanFilter kf(dt, A, C, Q, R, P);
  return kf;
}


struct Object {
    int type;
    float ID;
    float center_x;
    float center_y;
    float edge1_x;
    float edge1_y;
    float edge2_x;
    float edge2_y;
};

struct searchResult {
  int idx;
  int pointType;
  float ID;
  bool newObject;
};

struct Speed {
  float x;
  float y;
};


class Tracker {
public:
  std::vector< struct Object > objectList;
  std::vector< struct Object > objectList_new;
  std::vector< int > counters;
  std::vector< KalmanFilter > trackers;
  std::vector< struct Speed > speeds;
  void updateTracker();
  void addObject(struct Object object);
  void deleteObject(int idx);
private:
  void addTracker(float x1, float y1);
};

void Tracker::deleteObject(int idx) {
  objectList.erase(objectList.begin() + idx);
  counters.erase(counters.begin() + idx);
  trackers.erase(trackers.begin() + idx);
  speeds.erase(speeds.begin() + idx);
}

void Tracker::addObject(struct Object object) {
    objectList.push_back(object);
    counters.push_back(0);
    addTracker(object.center_x, object.center_y);
    struct Speed _speed;
    speeds.push_back(_speed);
}

void Tracker::updateTracker() {
  for (int i = 0; i < counters.size(); i++) {
    if (counters[i] > 50) {
      deleteObject(i);
    }
    else if (counters[i] > 30) {
      objectList[i].center_x = trackers[i].state()[0];
      objectList[i].center_y = trackers[i].state()[2];
    }
    Eigen::VectorXd y(2);
    y << objectList[i].center_x, objectList[i].center_y;

    trackers[i].update(y);
    speeds[i].x = trackers[i].state()[1];
    speeds[i].y = trackers[i].state()[3];
    std::cout << "speed.x:" << trackers[i].state()[1]<< std::endl;
    std::cout << "speed.y:" << trackers[i].state()[3] << std::endl;
    std::cout << "reading.x:" << objectList[i].center_x<< std::endl;
    std::cout << "reading.y:" << objectList[i].center_y << std::endl;
  }
}

void Tracker::addTracker(float x1, float y1) {

  KalmanFilter kf = getTrackerEKF();
  Eigen::VectorXd x0(4);
  x0 << x1, 0.1, y1, 0.1;
  kf.init(0.1, x0);

  trackers.push_back(kf);
}

class Associator {
public:
  Tracker tracker;
  void objectCallback(const object_msgs::object_msgs::ConstPtr& msg);
  struct searchResult getGNN(float x, float y);
private:

};

struct searchResult Associator::getGNN(float x, float y) {
  float distance = 100;
  float distanceIt;
  struct searchResult result;
  //std::cout << "here2" <<std::endl;
  for(int i = 0; i < tracker.objectList.size(); i++) {
    //std::cout << "here3" << std::endl;
    if (tracker.objectList[i].type != 1 | tracker.objectList[i].type != 2) continue;
      distanceIt = sqrt(pow(x - tracker.objectList[i].center_x,2) + pow(y - tracker.objectList[i].center_y,2));
      if (distanceIt < distance) {
        distance = distanceIt;
        result.idx = i;
        result.pointType = 1;
      }
      if (tracker.objectList[i].type == 2) {
        distanceIt = sqrt(pow(x - tracker.objectList[i].edge1_x,2) + pow(y - tracker.objectList[i].edge1_y,2));
        if (distanceIt < distance) {
          distance = distanceIt;
          result.idx = i;
          result.pointType = 2;
        }
        distanceIt = sqrt(pow(x - tracker.objectList[i].edge2_x,2) + pow(y - tracker.objectList[i].edge2_y,2));
        if (distanceIt < distance) {
          distance = distanceIt;
          result.idx = i;
          result.pointType = 3;
        }
      }
    }
    if (distance < 0.2) {
      result.newObject = false;
    } else {
      result.newObject = true;
      result.ID = ros::Time::now().toSec();
    }
    return result;
}

void Associator::objectCallback(const object_msgs::object_msgs::ConstPtr& msg) {
  struct searchResult result;
  struct Object objectIt;
  //std::cout << "here" << std::endl;
  for(int i = 0; i < msg->types.size(); i++) {
    if (msg->types[i] == 10) {
      result = getGNN(msg->center_x[i], msg->center_y[i]);
    }
    else {
      result = getGNN(msg->center_x[i], msg->center_y[i]);
      if (result.newObject) {
      result = getGNN(msg->edge1_x[i], msg->edge1_y[i]);
      }
      if (result.newObject) {
      result = getGNN(msg->edge2_x[i], msg->edge2_y[i]);
      }
    }
    if (result.newObject) {
      objectIt.type = msg->types[i];
      objectIt.center_x = msg->center_x[i];
      objectIt.center_y = msg->center_y[i];
      if (msg->types[i] == 2) {
        objectIt.edge1_x = msg->edge1_x[i];
        objectIt.edge1_y = msg->edge1_y[i];
        objectIt.edge2_x = msg->edge2_x[i];
        objectIt.edge2_y = msg->edge2_y[i];
      }
      tracker.addObject(objectIt);
    }
    else {
      tracker.objectList[result.idx].type = msg->types[i];
      tracker.objectList[result.idx].center_x = msg->center_x[i];
      tracker.objectList[result.idx].center_y = msg->center_y[i];
      if (msg->types[i] == 2) {
        tracker.objectList[result.idx].edge1_x = msg->edge1_x[i];
        tracker.objectList[result.idx].edge1_y = msg->edge1_y[i];
        tracker.objectList[result.idx].edge2_x = msg->edge2_x[i];
        tracker.objectList[result.idx].edge2_y = msg->edge2_y[i];
      }
      tracker.addObject(objectIt);
    }
  }
  tracker.updateTracker();
  //for (int i = 0; i < tracker.objectList.size(); i++)
  //std::cout << tracker.objectList[i].type << std::endl;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "tracker");
  ros::NodeHandle n;
  std::cout << "loaded" << std::endl;

  std::string object_topic = "/objects";
  Associator associator;

  ros::Subscriber sub = n.subscribe(object_topic, 1, &Associator::objectCallback, &associator);
  //ros::Publisher pub = n.advertise<object_msgs::object_msgs>(object_topic, 1);
  ros::Rate loop_rate(10);

  while (n.ok()) {
    ros::spinOnce();
    loop_rate.sleep();
  }
}
