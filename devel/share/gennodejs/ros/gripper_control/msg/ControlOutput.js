// Auto-generated. Do not edit!

// (in-package gripper_control.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class ControlOutput {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.output = null;
      this.output_sat = null;
      this.gripper_position = null;
      this.error = null;
      this.reference = null;
      this.lower_bound = null;
      this.upper_bound = null;
      this.actual_value = null;
    }
    else {
      if (initObj.hasOwnProperty('output')) {
        this.output = initObj.output
      }
      else {
        this.output = 0.0;
      }
      if (initObj.hasOwnProperty('output_sat')) {
        this.output_sat = initObj.output_sat
      }
      else {
        this.output_sat = 0.0;
      }
      if (initObj.hasOwnProperty('gripper_position')) {
        this.gripper_position = initObj.gripper_position
      }
      else {
        this.gripper_position = 0.0;
      }
      if (initObj.hasOwnProperty('error')) {
        this.error = initObj.error
      }
      else {
        this.error = 0.0;
      }
      if (initObj.hasOwnProperty('reference')) {
        this.reference = initObj.reference
      }
      else {
        this.reference = 0.0;
      }
      if (initObj.hasOwnProperty('lower_bound')) {
        this.lower_bound = initObj.lower_bound
      }
      else {
        this.lower_bound = 0.0;
      }
      if (initObj.hasOwnProperty('upper_bound')) {
        this.upper_bound = initObj.upper_bound
      }
      else {
        this.upper_bound = 0.0;
      }
      if (initObj.hasOwnProperty('actual_value')) {
        this.actual_value = initObj.actual_value
      }
      else {
        this.actual_value = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type ControlOutput
    // Serialize message field [output]
    bufferOffset = _serializer.float64(obj.output, buffer, bufferOffset);
    // Serialize message field [output_sat]
    bufferOffset = _serializer.float64(obj.output_sat, buffer, bufferOffset);
    // Serialize message field [gripper_position]
    bufferOffset = _serializer.float64(obj.gripper_position, buffer, bufferOffset);
    // Serialize message field [error]
    bufferOffset = _serializer.float64(obj.error, buffer, bufferOffset);
    // Serialize message field [reference]
    bufferOffset = _serializer.float64(obj.reference, buffer, bufferOffset);
    // Serialize message field [lower_bound]
    bufferOffset = _serializer.float64(obj.lower_bound, buffer, bufferOffset);
    // Serialize message field [upper_bound]
    bufferOffset = _serializer.float64(obj.upper_bound, buffer, bufferOffset);
    // Serialize message field [actual_value]
    bufferOffset = _serializer.float64(obj.actual_value, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type ControlOutput
    let len;
    let data = new ControlOutput(null);
    // Deserialize message field [output]
    data.output = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [output_sat]
    data.output_sat = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [gripper_position]
    data.gripper_position = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [error]
    data.error = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [reference]
    data.reference = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [lower_bound]
    data.lower_bound = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [upper_bound]
    data.upper_bound = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [actual_value]
    data.actual_value = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 64;
  }

  static datatype() {
    // Returns string type for a message object
    return 'gripper_control/ControlOutput';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '3d113725e502b53b0e53b00acafaa65f';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float64 output
    float64 output_sat
    float64 gripper_position
    float64 error
    float64 reference
    float64 lower_bound
    float64 upper_bound
    float64 actual_value
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new ControlOutput(null);
    if (msg.output !== undefined) {
      resolved.output = msg.output;
    }
    else {
      resolved.output = 0.0
    }

    if (msg.output_sat !== undefined) {
      resolved.output_sat = msg.output_sat;
    }
    else {
      resolved.output_sat = 0.0
    }

    if (msg.gripper_position !== undefined) {
      resolved.gripper_position = msg.gripper_position;
    }
    else {
      resolved.gripper_position = 0.0
    }

    if (msg.error !== undefined) {
      resolved.error = msg.error;
    }
    else {
      resolved.error = 0.0
    }

    if (msg.reference !== undefined) {
      resolved.reference = msg.reference;
    }
    else {
      resolved.reference = 0.0
    }

    if (msg.lower_bound !== undefined) {
      resolved.lower_bound = msg.lower_bound;
    }
    else {
      resolved.lower_bound = 0.0
    }

    if (msg.upper_bound !== undefined) {
      resolved.upper_bound = msg.upper_bound;
    }
    else {
      resolved.upper_bound = 0.0
    }

    if (msg.actual_value !== undefined) {
      resolved.actual_value = msg.actual_value;
    }
    else {
      resolved.actual_value = 0.0
    }

    return resolved;
    }
};

module.exports = ControlOutput;
