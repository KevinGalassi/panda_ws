;; Auto-generated. Do not edit!


(when (boundp 'read_sensor::tactile_sensor_data)
  (if (not (find-package "READ_SENSOR"))
    (make-package "READ_SENSOR"))
  (shadow 'tactile_sensor_data (find-package "READ_SENSOR")))
(unless (find-package "READ_SENSOR::TACTILE_SENSOR_DATA")
  (make-package "READ_SENSOR::TACTILE_SENSOR_DATA"))

(in-package "ROS")
;;//! \htmlinclude tactile_sensor_data.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass read_sensor::tactile_sensor_data
  :super ros::object
  :slots (_header _tactile_sensor_data ))

(defmethod read_sensor::tactile_sensor_data
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:tactile_sensor_data __tactile_sensor_data) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _header __header)
   (setq _tactile_sensor_data __tactile_sensor_data)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:tactile_sensor_data
   (&optional __tactile_sensor_data)
   (if __tactile_sensor_data (setq _tactile_sensor_data __tactile_sensor_data)) _tactile_sensor_data)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float32[] _tactile_sensor_data
    (* 4    (length _tactile_sensor_data)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float32[] _tactile_sensor_data
     (write-long (length _tactile_sensor_data) s)
     (dotimes (i (length _tactile_sensor_data))
       (sys::poke (elt _tactile_sensor_data i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float32[] _tactile_sensor_data
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _tactile_sensor_data (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _tactile_sensor_data i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get read_sensor::tactile_sensor_data :md5sum-) "c3cd94cf027646e3561f63fb3799d7db")
(setf (get read_sensor::tactile_sensor_data :datatype-) "read_sensor/tactile_sensor_data")
(setf (get read_sensor::tactile_sensor_data :definition-)
      "Header header
float32[] tactile_sensor_data

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

")



(provide :read_sensor/tactile_sensor_data "c3cd94cf027646e3561f63fb3799d7db")


