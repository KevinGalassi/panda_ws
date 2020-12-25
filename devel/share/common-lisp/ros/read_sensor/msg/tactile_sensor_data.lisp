; Auto-generated. Do not edit!


(cl:in-package read_sensor-msg)


;//! \htmlinclude tactile_sensor_data.msg.html

(cl:defclass <tactile_sensor_data> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (tactile_sensor_data
    :reader tactile_sensor_data
    :initarg :tactile_sensor_data
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass tactile_sensor_data (<tactile_sensor_data>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <tactile_sensor_data>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'tactile_sensor_data)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name read_sensor-msg:<tactile_sensor_data> is deprecated: use read_sensor-msg:tactile_sensor_data instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <tactile_sensor_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader read_sensor-msg:header-val is deprecated.  Use read_sensor-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'tactile_sensor_data-val :lambda-list '(m))
(cl:defmethod tactile_sensor_data-val ((m <tactile_sensor_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader read_sensor-msg:tactile_sensor_data-val is deprecated.  Use read_sensor-msg:tactile_sensor_data instead.")
  (tactile_sensor_data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <tactile_sensor_data>) ostream)
  "Serializes a message object of type '<tactile_sensor_data>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'tactile_sensor_data))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'tactile_sensor_data))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <tactile_sensor_data>) istream)
  "Deserializes a message object of type '<tactile_sensor_data>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'tactile_sensor_data) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'tactile_sensor_data)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<tactile_sensor_data>)))
  "Returns string type for a message object of type '<tactile_sensor_data>"
  "read_sensor/tactile_sensor_data")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'tactile_sensor_data)))
  "Returns string type for a message object of type 'tactile_sensor_data"
  "read_sensor/tactile_sensor_data")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<tactile_sensor_data>)))
  "Returns md5sum for a message object of type '<tactile_sensor_data>"
  "c3cd94cf027646e3561f63fb3799d7db")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'tactile_sensor_data)))
  "Returns md5sum for a message object of type 'tactile_sensor_data"
  "c3cd94cf027646e3561f63fb3799d7db")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<tactile_sensor_data>)))
  "Returns full string definition for message of type '<tactile_sensor_data>"
  (cl:format cl:nil "Header header~%float32[] tactile_sensor_data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'tactile_sensor_data)))
  "Returns full string definition for message of type 'tactile_sensor_data"
  (cl:format cl:nil "Header header~%float32[] tactile_sensor_data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <tactile_sensor_data>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'tactile_sensor_data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <tactile_sensor_data>))
  "Converts a ROS message object to a list"
  (cl:list 'tactile_sensor_data
    (cl:cons ':header (header msg))
    (cl:cons ':tactile_sensor_data (tactile_sensor_data msg))
))
