; Auto-generated. Do not edit!


(cl:in-package read_sensor-msg)


;//! \htmlinclude proximity_sensor_data.msg.html

(cl:defclass <proximity_sensor_data> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (proximity_sensor_data
    :reader proximity_sensor_data
    :initarg :proximity_sensor_data
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 0 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass proximity_sensor_data (<proximity_sensor_data>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <proximity_sensor_data>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'proximity_sensor_data)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name read_sensor-msg:<proximity_sensor_data> is deprecated: use read_sensor-msg:proximity_sensor_data instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <proximity_sensor_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader read_sensor-msg:header-val is deprecated.  Use read_sensor-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'proximity_sensor_data-val :lambda-list '(m))
(cl:defmethod proximity_sensor_data-val ((m <proximity_sensor_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader read_sensor-msg:proximity_sensor_data-val is deprecated.  Use read_sensor-msg:proximity_sensor_data instead.")
  (proximity_sensor_data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <proximity_sensor_data>) ostream)
  "Serializes a message object of type '<proximity_sensor_data>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'proximity_sensor_data))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream))
   (cl:slot-value msg 'proximity_sensor_data))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <proximity_sensor_data>) istream)
  "Deserializes a message object of type '<proximity_sensor_data>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'proximity_sensor_data) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'proximity_sensor_data)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<proximity_sensor_data>)))
  "Returns string type for a message object of type '<proximity_sensor_data>"
  "read_sensor/proximity_sensor_data")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'proximity_sensor_data)))
  "Returns string type for a message object of type 'proximity_sensor_data"
  "read_sensor/proximity_sensor_data")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<proximity_sensor_data>)))
  "Returns md5sum for a message object of type '<proximity_sensor_data>"
  "3a2387fb9f4f2007e1f416de15deda17")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'proximity_sensor_data)))
  "Returns md5sum for a message object of type 'proximity_sensor_data"
  "3a2387fb9f4f2007e1f416de15deda17")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<proximity_sensor_data>)))
  "Returns full string definition for message of type '<proximity_sensor_data>"
  (cl:format cl:nil "Header header~%uint8[] proximity_sensor_data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'proximity_sensor_data)))
  "Returns full string definition for message of type 'proximity_sensor_data"
  (cl:format cl:nil "Header header~%uint8[] proximity_sensor_data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <proximity_sensor_data>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'proximity_sensor_data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <proximity_sensor_data>))
  "Converts a ROS message object to a list"
  (cl:list 'proximity_sensor_data
    (cl:cons ':header (header msg))
    (cl:cons ':proximity_sensor_data (proximity_sensor_data msg))
))
