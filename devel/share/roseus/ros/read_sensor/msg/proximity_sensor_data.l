;; Auto-generated. Do not edit!


(when (boundp 'read_sensor::proximity_sensor_data)
  (if (not (find-package "READ_SENSOR"))
    (make-package "READ_SENSOR"))
  (shadow 'proximity_sensor_data (find-package "READ_SENSOR")))
(unless (find-package "READ_SENSOR::PROXIMITY_SENSOR_DATA")
  (make-package "READ_SENSOR::PROXIMITY_SENSOR_DATA"))

(in-package "ROS")
;;//! \htmlinclude proximity_sensor_data.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass read_sensor::proximity_sensor_data
  :super ros::object
  :slots (_header _proximity_sensor_data ))

(defmethod read_sensor::proximity_sensor_data
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:proximity_sensor_data __proximity_sensor_data) (make-array 0 :initial-element 0 :element-type :char))
    )
   (send-super :init)
   (setq _header __header)
   (setq _proximity_sensor_data __proximity_sensor_data)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:proximity_sensor_data
   (&optional __proximity_sensor_data)
   (if __proximity_sensor_data (setq _proximity_sensor_data __proximity_sensor_data)) _proximity_sensor_data)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; uint8[] _proximity_sensor_data
    (* 1    (length _proximity_sensor_data)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; uint8[] _proximity_sensor_data
     (write-long (length _proximity_sensor_data) s)
     (princ _proximity_sensor_data s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; uint8[] _proximity_sensor_data
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _proximity_sensor_data (make-array n :element-type :char))
     (replace _proximity_sensor_data buf :start2 ptr-) (incf ptr- n))
   ;;
   self)
  )

(setf (get read_sensor::proximity_sensor_data :md5sum-) "3a2387fb9f4f2007e1f416de15deda17")
(setf (get read_sensor::proximity_sensor_data :datatype-) "read_sensor/proximity_sensor_data")
(setf (get read_sensor::proximity_sensor_data :definition-)
      "Header header
uint8[] proximity_sensor_data

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



(provide :read_sensor/proximity_sensor_data "3a2387fb9f4f2007e1f416de15deda17")


