;; Auto-generated. Do not edit!


(when (boundp 'gripper_control::MoveGoal)
  (if (not (find-package "GRIPPER_CONTROL"))
    (make-package "GRIPPER_CONTROL"))
  (shadow 'MoveGoal (find-package "GRIPPER_CONTROL")))
(unless (find-package "GRIPPER_CONTROL::MOVEGOAL")
  (make-package "GRIPPER_CONTROL::MOVEGOAL"))

(in-package "ROS")
;;//! \htmlinclude MoveGoal.msg.html


(defclass gripper_control::MoveGoal
  :super ros::object
  :slots (_width _speed ))

(defmethod gripper_control::MoveGoal
  (:init
   (&key
    ((:width __width) 0.0)
    ((:speed __speed) 0.0)
    )
   (send-super :init)
   (setq _width (float __width))
   (setq _speed (float __speed))
   self)
  (:width
   (&optional __width)
   (if __width (setq _width __width)) _width)
  (:speed
   (&optional __speed)
   (if __speed (setq _speed __speed)) _speed)
  (:serialization-length
   ()
   (+
    ;; float64 _width
    8
    ;; float64 _speed
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _width
       (sys::poke _width (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _speed
       (sys::poke _speed (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _width
     (setq _width (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _speed
     (setq _speed (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get gripper_control::MoveGoal :md5sum-) "d16050a0fe85f1c2cb8347c99678362e")
(setf (get gripper_control::MoveGoal :datatype-) "gripper_control/MoveGoal")
(setf (get gripper_control::MoveGoal :definition-)
      "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======
float64 width  # [m]
float64 speed  # [m/s]

")



(provide :gripper_control/MoveGoal "d16050a0fe85f1c2cb8347c99678362e")

