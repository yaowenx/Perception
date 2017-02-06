#include "ros/ros.h"
#include "visualization_msgs/Marker.h"
#include "object_msgs/object_msgs.h"
#include "kalman/ekfilter.hpp"
#include "tracker/kalman.hpp"
#include "iostream"
//#include <cstdlib>
//#include <iostream>
//#include <fstream>

#include <cmath>

//using namespace std;
//using namespace Kalman;

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
  std::vector< TrackerEKF > trackers;
  std::vector< struct Speed > speeds;
  void updateTracker();
  void addObject(struct Object object);
  void deleteObject(int idx);
private:
  void addTrackerEKF(float x1, float y1);
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
    addTrackerEKF(object.center_x, object.center_y);
    struct Speed _speed;
    speeds.push_back(_speed);
}

void Tracker::updateTracker() {
  for (int i = 0; i < counters.size(); i++) {
    if (counters[i] > 50) {
      deleteObject(i);
    }
    else if (counters[i] > 0) {
      objectList[i].center_x = trackers[i].getX()(1);
      objectList[i].center_y = trackers[i].getX()(3);
    }
    Vector z(2);
    Vector u(1);
    z(1) = objectList[i].center_x;
    z(2) = objectList[i].center_y;
    u(1) = 0;
std::cout << "here" << std::endl;
    trackers[i].step(u, z);
    std::cout << "here2" << std::endl;
    speeds[i].x = trackers[i].getX()(2);
    speeds[i].y = trackers[i].getX()(4);
    std::cout << "here3" << std::endl;
  }
}

void Tracker::addTrackerEKF(float x1, float y1) {

  //selectKVectorContext(createKVectorContext(" ", "[ ", " ];", 4));
  //selectKMatrixContext(createKMatrixContext(" ", " ;\n  ", "[ ", " ];", 4));

  static const float _P0[] = {100.0*100.0, 0.0, 0.0, 0.0,
                                 0.0, 10.0*10.0, 0.0, 0.0,
                                 0.0, 0.0, 25.0*25.0, 0.0,
                                 0.0, 0.0, 0.0, 10.0*10.0};

  Vector x(4);
  Matrix P0(4, 4, _P0);

  x(1) = x1;
	x(2) = 0;
	x(3) = y1;
	x(4) = 0;

  TrackerEKF tracker;
  tracker.init(x, P0);
  trackers.push_back(tracker);
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
    if (msg->types[i] == 1) {
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
  for (int i = 0; i < tracker.objectList.size(); i++)
  std::cout << tracker.objectList[i].type << std::endl;
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
