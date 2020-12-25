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

class set_positionRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.goal_position = null;
      this.goal_velocity = null;
      this.goal_acceleration = null;
    }
    else {
      if (initObj.hasOwnProperty('goal_position')) {
        this.goal_position = initObj.goal_position
      }
      else {
        this.goal_position = 0.0;
      }
      if (initObj.hasOwnProperty('goal_velocity')) {
        this.goal_velocity = initObj.goal_velocity
      }
      else {
        this.goal_velocity = 0.0;
      }
      if (initObj.hasOwnProperty('goal_acceleration')) {
        this.goal_acceleration = initObj.goal_acceleration
      }
      else {
        this.goal_acceleration = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type set_positionRequest
    // Serialize message field [goal_position]
    bufferOffset = _serializer.float32(obj.goal_position, buffer, bufferOffset);
    // Serialize message field [goal_velocity]
    bufferOffset = _serializer.float32(obj.goal_velocity, buffer, bufferOffset);
    // Serialize message field [goal_acceleration]
    bufferOffset = _serializer.float32(obj.goal_acceleration, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type set_positionRequest
    let len;
    let data = new set_positionRequest(null);
    // Deserialize message field [goal_position]
    data.goal_position = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [goal_velocity]
    data.goal_velocity = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [goal_acceleration]
    data.goal_acceleration = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 12;
  }

  static datatype() {
    // Returns string type for a service object
    return 'schunk_pg70/set_positionRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '319b6d57a431ad4ba5699f76bb8b9f71';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 goal_position
    float32 goal_velocity
    float32 goal_acceleration
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new set_positionRequest(null);
    if (msg.goal_position !== undefined) {
      resolved.goal_position = msg.goal_position;
    }
    else {
      resolved.goal_position = 0.0
    }

    if (msg.goal_velocity !== undefined) {
      resolved.goal_velocity = msg.goal_velocity;
    }
    else {
      resolved.goal_velocity = 0.0
    }

    if (msg.goal_acceleration !== undefined) {
      resolved.goal_acceleration = msg.goal_acceleration;
    }
    else {
      resolved.goal_acceleration = 0.0
    }

    return resolved;
    }
};

class set_positionResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.goal_accepted = null;
    }
    else {
      if (initObj.hasOwnProperty('goal_accepted')) {
        this.goal_accepted = initObj.goal_accepted
      }
      else {
        this.goal_accepted = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type set_positionResponse
    // Serialize message field [goal_accepted]
    bufferOffset = _serializer.bool(obj.goal_accepted, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type set_positionResponse
    let len;
    let data = new set_positionResponse(null);
    // Deserialize message field [goal_accepted]
    data.goal_accepted = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 1;
  }

  static datatype() {
    // Returns string type for a service object
    return 'schunk_pg70/set_positionResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '2a5ca3c440eebef45662a29c35d8e42e';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool goal_accepted
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new set_positionResponse(null);
    if (msg.goal_accepted !== undefined) {
      resolved.goal_accepted = msg.goal_accepted;
    }
    else {
      resolved.goal_accepted = false
    }

    return resolved;
    }
};

module.exports = {
  Request: set_positionRequest,
  Response: set_positionResponse,
  md5sum() { return 'ab6e767af4c4f9e00f0354670a02d7cb'; },
  datatype() { return 'schunk_pg70/set_position'; }
};
