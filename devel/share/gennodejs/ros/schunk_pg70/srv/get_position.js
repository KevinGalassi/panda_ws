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

class get_positionRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
    }
    else {
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type get_positionRequest
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type get_positionRequest
    let len;
    let data = new get_positionRequest(null);
    return data;
  }

  static getMessageSize(object) {
    return 0;
  }

  static datatype() {
    // Returns string type for a service object
    return 'schunk_pg70/get_positionRequest';
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
    const resolved = new get_positionRequest(null);
    return resolved;
    }
};

class get_positionResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.actual_position = null;
    }
    else {
      if (initObj.hasOwnProperty('actual_position')) {
        this.actual_position = initObj.actual_position
      }
      else {
        this.actual_position = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type get_positionResponse
    // Serialize message field [actual_position]
    bufferOffset = _serializer.float32(obj.actual_position, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type get_positionResponse
    let len;
    let data = new get_positionResponse(null);
    // Deserialize message field [actual_position]
    data.actual_position = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 4;
  }

  static datatype() {
    // Returns string type for a service object
    return 'schunk_pg70/get_positionResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '40db4b6d09ee0b910be5d9e61aacbb7d';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 actual_position
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new get_positionResponse(null);
    if (msg.actual_position !== undefined) {
      resolved.actual_position = msg.actual_position;
    }
    else {
      resolved.actual_position = 0.0
    }

    return resolved;
    }
};

module.exports = {
  Request: get_positionRequest,
  Response: get_positionResponse,
  md5sum() { return '40db4b6d09ee0b910be5d9e61aacbb7d'; },
  datatype() { return 'schunk_pg70/get_position'; }
};
