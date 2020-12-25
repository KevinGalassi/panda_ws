; Auto-generated. Do not edit!


(cl:in-package planner-msg)


;//! \htmlinclude ComputeTrajectoryResult.msg.html

(cl:defclass <ComputeTrajectoryResult> (roslisp-msg-protocol:ros-message)
  ((FinalTrajectories
    :reader FinalTrajectories
    :initarg :FinalTrajectories
    :type (cl:vector geometry_msgs-msg:PoseArray)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:PoseArray :initial-element (cl:make-instance 'geometry_msgs-msg:PoseArray)))
   (FinalLabel
    :reader FinalLabel
    :initarg :FinalLabel
    :type (cl:vector cl:string)
   :initform (cl:make-array 0 :element-type 'cl:string :initial-element "")))
)

(cl:defclass ComputeTrajectoryResult (<ComputeTrajectoryResult>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ComputeTrajectoryResult>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ComputeTrajectoryResult)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name planner-msg:<ComputeTrajectoryResult> is deprecated: use planner-msg:ComputeTrajectoryResult instead.")))

(cl:ensure-generic-function 'FinalTrajectories-val :lambda-list '(m))
(cl:defmethod FinalTrajectories-val ((m <ComputeTrajectoryResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader planner-msg:FinalTrajectories-val is deprecated.  Use planner-msg:FinalTrajectories instead.")
  (FinalTrajectories m))

(cl:ensure-generic-function 'FinalLabel-val :lambda-list '(m))
(cl:defmethod FinalLabel-val ((m <ComputeTrajectoryResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader planner-msg:FinalLabel-val is deprecated.  Use planner-msg:FinalLabel instead.")
  (FinalLabel m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ComputeTrajectoryResult>) ostream)
  "Serializes a message object of type '<ComputeTrajectoryResult>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'FinalTrajectories))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'FinalTrajectories))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'FinalLabel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((__ros_str_len (cl:length ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) ele))
   (cl:slot-value msg 'FinalLabel))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ComputeTrajectoryResult>) istream)
  "Deserializes a message object of type '<ComputeTrajectoryResult>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'FinalTrajectories) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'FinalTrajectories)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:PoseArray))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'FinalLabel) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'FinalLabel)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:aref vals i) __ros_str_idx) (cl:code-char (cl:read-byte istream))))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ComputeTrajectoryResult>)))
  "Returns string type for a message object of type '<ComputeTrajectoryResult>"
  "planner/ComputeTrajectoryResult")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ComputeTrajectoryResult)))
  "Returns string type for a message object of type 'ComputeTrajectoryResult"
  "planner/ComputeTrajectoryResult")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ComputeTrajectoryResult>)))
  "Returns md5sum for a message object of type '<ComputeTrajectoryResult>"
  "40834ff98d5d7863bdb61af7ff11587a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ComputeTrajectoryResult)))
  "Returns md5sum for a message object of type 'ComputeTrajectoryResult"
  "40834ff98d5d7863bdb61af7ff11587a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ComputeTrajectoryResult>)))
  "Returns full string definition for message of type '<ComputeTrajectoryResult>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#result~%geometry_msgs/PoseArray[] FinalTrajectories~%string[] FinalLabel~%~%================================================================================~%MSG: geometry_msgs/PoseArray~%# An array of poses with a header for global reference.~%~%Header header~%~%Pose[] poses~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ComputeTrajectoryResult)))
  "Returns full string definition for message of type 'ComputeTrajectoryResult"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#result~%geometry_msgs/PoseArray[] FinalTrajectories~%string[] FinalLabel~%~%================================================================================~%MSG: geometry_msgs/PoseArray~%# An array of poses with a header for global reference.~%~%Header header~%~%Pose[] poses~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ComputeTrajectoryResult>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'FinalTrajectories) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'FinalLabel) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4 (cl:length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ComputeTrajectoryResult>))
  "Converts a ROS message object to a list"
  (cl:list 'ComputeTrajectoryResult
    (cl:cons ':FinalTrajectories (FinalTrajectories msg))
    (cl:cons ':FinalLabel (FinalLabel msg))
))
