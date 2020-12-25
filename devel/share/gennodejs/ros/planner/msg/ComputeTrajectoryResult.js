// Auto-generated. Do not edit!

// (in-package planner.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let geometry_msgs = _finder('geometry_msgs');

//-----------------------------------------------------------

class ComputeTrajectoryResult {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.FinalTrajectories = null;
      this.FinalLabel = null;
    }
    else {
      if (initObj.hasOwnProperty('FinalTrajectories')) {
        this.FinalTrajectories = initObj.FinalTrajectories
      }
      else {
        this.FinalTrajectories = [];
      }
      if (initObj.hasOwnProperty('FinalLabel')) {
        this.FinalLabel = initObj.FinalLabel
      }
      else {
        this.FinalLabel = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type ComputeTrajectoryResult
    // Serialize message field [FinalTrajectories]
    // Serialize the length for message field [FinalTrajectories]
    bufferOffset = _serializer.uint32(obj.FinalTrajectories.length, buffer, bufferOffset);
    obj.FinalTrajectories.forEach((val) => {
      bufferOffset = geometry_msgs.msg.PoseArray.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [FinalLabel]
    bufferOffset = _arraySerializer.string(obj.FinalLabel, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type ComputeTrajectoryResult
    let len;
    let data = new ComputeTrajectoryResult(null);
    // Deserialize message field [FinalTrajectories]
    // Deserialize array length for message field [FinalTrajectories]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.FinalTrajectories = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.FinalTrajectories[i] = geometry_msgs.msg.PoseArray.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [FinalLabel]
    data.FinalLabel = _arrayDeserializer.string(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    object.FinalTrajectories.forEach((val) => {
      length += geometry_msgs.msg.PoseArray.getMessageSize(val);
    });
    object.FinalLabel.forEach((val) => {
      length += 4 + val.length;
    });
    return length + 8;
  }

  static datatype() {
    // Returns string type for a message object
    return 'planner/ComputeTrajectoryResult';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '40834ff98d5d7863bdb61af7ff11587a';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    # ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======
    #result
    geometry_msgs/PoseArray[] FinalTrajectories
    string[] FinalLabel
    
    ================================================================================
    MSG: geometry_msgs/PoseArray
    # An array of poses with a header for global reference.
    
    Header header
    
    Pose[] poses
    
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
    string frame_id
    
    ================================================================================
    MSG: geometry_msgs/Pose
    # A representation of pose in free space, composed of position and orientation. 
    Point position
    Quaternion orientation
    
    ================================================================================
    MSG: geometry_msgs/Point
    # This contains the position of a point in free space
    float64 x
    float64 y
    float64 z
    
    ================================================================================
    MSG: geometry_msgs/Quaternion
    # This represents an orientation in free space in quaternion form.
    
    float64 x
    float64 y
    float64 z
    float64 w
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new ComputeTrajectoryResult(null);
    if (msg.FinalTrajectories !== undefined) {
      resolved.FinalTrajectories = new Array(msg.FinalTrajectories.length);
      for (let i = 0; i < resolved.FinalTrajectories.length; ++i) {
        resolved.FinalTrajectories[i] = geometry_msgs.msg.PoseArray.Resolve(msg.FinalTrajectories[i]);
      }
    }
    else {
      resolved.FinalTrajectories = []
    }

    if (msg.FinalLabel !== undefined) {
      resolved.FinalLabel = msg.FinalLabel;
    }
    else {
      resolved.FinalLabel = []
    }

    return resolved;
    }
};

module.exports = ComputeTrajectoryResult;