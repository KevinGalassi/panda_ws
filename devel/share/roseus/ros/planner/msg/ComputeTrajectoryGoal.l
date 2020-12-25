;; Auto-generated. Do not edit!


(when (boundp 'planner::ComputeTrajectoryGoal)
  (if (not (find-package "PLANNER"))
    (make-package "PLANNER"))
  (shadow 'ComputeTrajectoryGoal (find-package "PLANNER")))
(unless (find-package "PLANNER::COMPUTETRAJECTORYGOAL")
  (make-package "PLANNER::COMPUTETRAJECTORYGOAL"))

(in-package "ROS")
;;//! \htmlinclude ComputeTrajectoryGoal.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass planner::ComputeTrajectoryGoal
  :super ros::object
  :slots (_InputPoint _InitLabel ))

(defmethod planner::ComputeTrajectoryGoal
  (:init
   (&key
    ((:InputPoint __InputPoint) (instance geometry_msgs::PoseArray :init))
    ((:InitLabel __InitLabel) (let (r) (dotimes (i 0) (push "" r)) r))
    )
   (send-super :init)
   (setq _InputPoint __InputPoint)
   (setq _InitLabel __InitLabel)
   self)
  (:InputPoint
   (&rest __InputPoint)
   (if (keywordp (car __InputPoint))
       (send* _InputPoint __InputPoint)
     (progn
       (if __InputPoint (setq _InputPoint (car __InputPoint)))
       _InputPoint)))
  (:InitLabel
   (&optional __InitLabel)
   (if __InitLabel (setq _InitLabel __InitLabel)) _InitLabel)
  (:serialization-length
   ()
   (+
    ;; geometry_msgs/PoseArray _InputPoint
    (send _InputPoint :serialization-length)
    ;; string[] _InitLabel
    (apply #'+ (mapcar #'(lambda (x) (+ 4 (length x))) _InitLabel)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; geometry_msgs/PoseArray _InputPoint
       (send _InputPoint :serialize s)
     ;; string[] _InitLabel
     (write-long (length _InitLabel) s)
     (dolist (elem _InitLabel)
       (write-long (length elem) s) (princ elem s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; geometry_msgs/PoseArray _InputPoint
     (send _InputPoint :deserialize buf ptr-) (incf ptr- (send _InputPoint :serialization-length))
   ;; string[] _InitLabel
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _InitLabel (make-list n))
     (dotimes (i n)
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setf (elt _InitLabel i) (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
     ))
   ;;
   self)
  )

(setf (get planner::ComputeTrajectoryGoal :md5sum-) "e134294baa46c81d09a022be523a4827")
(setf (get planner::ComputeTrajectoryGoal :datatype-) "planner/ComputeTrajectoryGoal")
(setf (get planner::ComputeTrajectoryGoal :definition-)
      "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======
#goal
geometry_msgs/PoseArray InputPoint
string[] InitLabel

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

")



(provide :planner/ComputeTrajectoryGoal "e134294baa46c81d09a022be523a4827")

