// Auto-generated. Do not edit!

// (in-package object_msgs.msg)


"use strict";

let _serializer = require('../base_serialize.js');
let _deserializer = require('../base_deserialize.js');
let _finder = require('../find.js');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class object_msgs {
  constructor() {
    this.header = new std_msgs.msg.Header();
    this.types = [];
    this.center_x = [];
    this.center_y = [];
    this.edge1_x = [];
    this.edge1_y = [];
    this.edge2_x = [];
    this.edge2_y = [];
  }

  static serialize(obj, bufferInfo) {
    // Serializes a message object of type object_msgs
    // Serialize message field [header]
    bufferInfo = std_msgs.msg.Header.serialize(obj.header, bufferInfo);
    // Serialize the length for message field [types]
    bufferInfo = _serializer.uint32(obj.types.length, bufferInfo);
    // Serialize message field [types]
    obj.types.forEach((val) => {
      bufferInfo = _serializer.int16(val, bufferInfo);
    });
    // Serialize the length for message field [center_x]
    bufferInfo = _serializer.uint32(obj.center_x.length, bufferInfo);
    // Serialize message field [center_x]
    obj.center_x.forEach((val) => {
      bufferInfo = _serializer.float32(val, bufferInfo);
    });
    // Serialize the length for message field [center_y]
    bufferInfo = _serializer.uint32(obj.center_y.length, bufferInfo);
    // Serialize message field [center_y]
    obj.center_y.forEach((val) => {
      bufferInfo = _serializer.float32(val, bufferInfo);
    });
    // Serialize the length for message field [edge1_x]
    bufferInfo = _serializer.uint32(obj.edge1_x.length, bufferInfo);
    // Serialize message field [edge1_x]
    obj.edge1_x.forEach((val) => {
      bufferInfo = _serializer.float32(val, bufferInfo);
    });
    // Serialize the length for message field [edge1_y]
    bufferInfo = _serializer.uint32(obj.edge1_y.length, bufferInfo);
    // Serialize message field [edge1_y]
    obj.edge1_y.forEach((val) => {
      bufferInfo = _serializer.float32(val, bufferInfo);
    });
    // Serialize the length for message field [edge2_x]
    bufferInfo = _serializer.uint32(obj.edge2_x.length, bufferInfo);
    // Serialize message field [edge2_x]
    obj.edge2_x.forEach((val) => {
      bufferInfo = _serializer.float32(val, bufferInfo);
    });
    // Serialize the length for message field [edge2_y]
    bufferInfo = _serializer.uint32(obj.edge2_y.length, bufferInfo);
    // Serialize message field [edge2_y]
    obj.edge2_y.forEach((val) => {
      bufferInfo = _serializer.float32(val, bufferInfo);
    });
    return bufferInfo;
  }

  static deserialize(buffer) {
    //deserializes a message object of type object_msgs
    let tmp;
    let len;
    let data = new object_msgs();
    // Deserialize message field [header]
    tmp = std_msgs.msg.Header.deserialize(buffer);
    data.header = tmp.data;
    buffer = tmp.buffer;
    // Deserialize array length for message field [types]
    tmp = _deserializer.uint32(buffer);
    len = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [types]
    data.types = new Array(len);
    for (let i = 0; i < len; ++i) {
      tmp = _deserializer.int16(buffer);
      data.types[i] = tmp.data;
      buffer = tmp.buffer;
    }
    // Deserialize array length for message field [center_x]
    tmp = _deserializer.uint32(buffer);
    len = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [center_x]
    data.center_x = new Array(len);
    for (let i = 0; i < len; ++i) {
      tmp = _deserializer.float32(buffer);
      data.center_x[i] = tmp.data;
      buffer = tmp.buffer;
    }
    // Deserialize array length for message field [center_y]
    tmp = _deserializer.uint32(buffer);
    len = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [center_y]
    data.center_y = new Array(len);
    for (let i = 0; i < len; ++i) {
      tmp = _deserializer.float32(buffer);
      data.center_y[i] = tmp.data;
      buffer = tmp.buffer;
    }
    // Deserialize array length for message field [edge1_x]
    tmp = _deserializer.uint32(buffer);
    len = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [edge1_x]
    data.edge1_x = new Array(len);
    for (let i = 0; i < len; ++i) {
      tmp = _deserializer.float32(buffer);
      data.edge1_x[i] = tmp.data;
      buffer = tmp.buffer;
    }
    // Deserialize array length for message field [edge1_y]
    tmp = _deserializer.uint32(buffer);
    len = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [edge1_y]
    data.edge1_y = new Array(len);
    for (let i = 0; i < len; ++i) {
      tmp = _deserializer.float32(buffer);
      data.edge1_y[i] = tmp.data;
      buffer = tmp.buffer;
    }
    // Deserialize array length for message field [edge2_x]
    tmp = _deserializer.uint32(buffer);
    len = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [edge2_x]
    data.edge2_x = new Array(len);
    for (let i = 0; i < len; ++i) {
      tmp = _deserializer.float32(buffer);
      data.edge2_x[i] = tmp.data;
      buffer = tmp.buffer;
    }
    // Deserialize array length for message field [edge2_y]
    tmp = _deserializer.uint32(buffer);
    len = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [edge2_y]
    data.edge2_y = new Array(len);
    for (let i = 0; i < len; ++i) {
      tmp = _deserializer.float32(buffer);
      data.edge2_y[i] = tmp.data;
      buffer = tmp.buffer;
    }
    return {
      data: data,
      buffer: buffer
    }
  }

  static datatype() {
    // Returns string type for a message object
    return 'object_msgs/object_msgs';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4168c556534593784663dd7042fab0b2';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
    int16[] types
    float32[] center_x
    float32[] center_y
    float32[] edge1_x
    float32[] edge1_y
    float32[] edge2_x
    float32[] edge2_y
    
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

module.exports = object_msgs;
