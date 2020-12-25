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

class acknowledge_errorRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
    }
    else {
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type acknowledge_errorRequest
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type acknowledge_errorRequest
    let len;
    let data = new acknowledge_errorRequest(null);
    return data;
  }

  static getMessageSize(object) {
    return 0;
  }

  static datatype() {
    // Returns string type for a service object
    return 'schunk_pg70/acknowledge_errorRequest';
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
    const resolved = new acknowledge_errorRequest(null);
    return resolved;
    }
};

class acknowledge_errorResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.acknowledge_response = null;
    }
    else {
      if (initObj.hasOwnProperty('acknowledge_response')) {
        this.acknowledge_response = initObj.acknowledge_response
      }
      else {
        this.acknowledge_response = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type acknowledge_errorResponse
    // Serialize message field [acknowledge_response]
    bufferOffset = _serializer.bool(obj.acknowledge_response, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type acknowledge_errorResponse
    let len;
    let data = new acknowledge_errorResponse(null);
    // Deserialize message field [acknowledge_response]
    data.acknowledge_response = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'schunk_pg70/acknowledge_errorResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '216312424a7b13297dba83de8033144e';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool acknowledge_response
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new acknowledge_errorResponse(null);
    if (msg.acknowledge_response !== undefined) {
      resolved.acknowledge_response = msg.acknowledge_response;
    }
    else {
      resolved.acknowledge_response = false
    }

    return resolved;
    }
};

module.exports = {
  Request: acknowledge_errorRequest,
  Response: acknowledge_errorResponse,
  md5sum() { return '216312424a7b13297dba83de8033144e'; },
  datatype() { return 'schunk_pg70/acknowledge_error'; }
};
