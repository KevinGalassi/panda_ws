// Auto-generated. Do not edit!

// (in-package schunk_pg70.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------


//-----------------------------------------------------------

class stopRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
    }
    else {
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type stopRequest
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type stopRequest
    let len;
    let data = new stopRequest(null);
    return data;
  }

  static getMessageSize(object) {
    return 0;
  }

  static datatype() {
    // Returns string type for a service object
    return 'schunk_pg70/stopRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'd41d8cd98f00b204e9800998ecf8427e';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new stopRequest(null);
    return resolved;
    }
};

class stopResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.stop_result = null;
    }
    else {
      if (initObj.hasOwnProperty('stop_result')) {
        this.stop_result = initObj.stop_result
      }
      else {
        this.stop_result = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type stopResponse
    // Serialize message field [stop_result]
    bufferOffset = _serializer.float32(obj.stop_result, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type stopResponse
    let len;
    let data = new stopResponse(null);
    // Deserialize message field [stop_result]
    data.stop_result = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 4;
  }

  static datatype() {
    // Returns string type for a service object
    return 'schunk_pg70/stopResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '1dc3c304d8e1ef4b9d7103ce1e3c990a';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 stop_result
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new stopResponse(null);
    if (msg.stop_result !== undefined) {
      resolved.stop_result = msg.stop_result;
    }
    else {
      resolved.stop_result = 0.0
    }

    return resolved;
    }
};

module.exports = {
  Request: stopRequest,
  Response: stopResponse,
  md5sum() { return '1dc3c304d8e1ef4b9d7103ce1e3c990a'; },
  datatype() { return 'schunk_pg70/stop'; }
};
