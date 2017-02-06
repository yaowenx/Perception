// Auto-generated. Do not edit!

// (in-package segment_msgs.msg)


"use strict";

let _serializer = require('../base_serialize.js');
let _deserializer = require('../base_deserialize.js');
let _finder = require('../find.js');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class segment_msgs {
  constructor() {
    this.header = new std_msgs.msg.Header();
    this.segments = [];
    this.ranges = [];
  }

  static serialize(obj, bufferInfo) {
    // Serializes a message object of type segment_msgs
    // Serialize message field [header]
    bufferInfo = std_msgs.msg.Header.serialize(obj.header, bufferInfo);
    // Serialize the length for message field [segments]
    bufferInfo = _serializer.uint32(obj.segments.length, bufferInfo);
    // Serialize message field [segments]
    obj.segments.forEach((val) => {
      bufferInfo = _serializer.int16(val, bufferInfo);
    });
    // Serialize the length for message field [ranges]
    bufferInfo = _serializer.uint32(obj.ranges.length, bufferInfo);
    // Serialize message field [ranges]
    obj.ranges.forEach((val) => {
      bufferInfo = _serializer.float32(val, bufferInfo);
    });
    return bufferInfo;
  }

  static deserialize(buffer) {
    //deserializes a message object of type segment_msgs
    let tmp;
    let len;
    let data = new segment_msgs();
    // Deserialize message field [header]
    tmp = std_msgs.msg.Header.deserialize(buffer);
    data.header = tmp.data;
    buffer = tmp.buffer;
    // Deserialize array length for message field [segments]
    tmp = _deserializer.uint32(buffer);
    len = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [segments]
    data.segments = new Array(len);
    for (let i = 0; i < len; ++i) {
      tmp = _deserializer.int16(buffer);
      data.segments[i] = tmp.data;
      buffer = tmp.buffer;
    }
    // Deserialize array length for message field [ranges]
    tmp = _deserializer.uint32(buffer);
    len = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [ranges]
    data.ranges = new Array(len);
    for (let i = 0; i < len; ++i) {
      tmp = _deserializer.float32(buffer);
      data.ranges[i] = tmp.data;
      buffer = tmp.buffer;
    }
    return {
      data: data,
      buffer: buffer
    }
  }

  static datatype() {
    // Returns string type for a message object
    return 'segment_msgs/segment_msgs';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'ea86f91234554cf339ed28ced396d040';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
    int16[] segments
    float32[] ranges
    
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    # 0: no frame
    # 1: global frame
    string frame_id
    
    `;
  }

};

module.exports = segment_msgs;
