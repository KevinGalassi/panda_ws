;; Auto-generated. Do not edit!


(when (boundp 'gripper_control::GraspEpsilon)
  (if (not (find-package "GRIPPER_CONTROL"))
    (make-package "GRIPPER_CONTROL"))
  (shadow 'GraspEpsilon (find-package "GRIPPER_CONTROL")))
(unless (find-package "GRIPPER_CONTROL::GRASPEPSILON")
  (make-package "GRIPPER_CONTROL::GRASPEPSILON"))

(in-package "ROS")
;;//! \htmlinclude GraspEpsilon.msg.html


(defclass gripper_control::GraspEpsilon
  :super ros::object
  :slots (_inner _outer ))

(defmethod gripper_control::GraspEpsilon
  (:init
   (&key
    ((:inner __inner) 0.0)
    ((:outer __outer) 0.0)
    )
   (send-super :init)
   (setq _inner (float __inner))
   (setq _outer (float __outer))
   self)
  (:inner
   (&optional __inner)
   (if __inner (setq _inner __inner)) _inner)
  (:outer
   (&optional __outer)
   (if __outer (setq _outer __outer)) _outer)
  (:serialization-length
   ()
   (+
    ;; float64 _inner
    8
    ;; float64 _outer
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _inner
       (sys::poke _inner (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _outer
       (sys::poke _outer (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _inner
     (setq _inner (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _outer
     (setq _outer (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get gripper_control::GraspEpsilon :md5sum-) "95b2c5464a6f679bd1dacaf86414f095")
(setf (get gripper_control::GraspEpsilon :datatype-) "gripper_control/GraspEpsilon")
(setf (get gripper_control::GraspEpsilon :definition-)
      "float64 inner # [m]
float64 outer # [m]

")



(provide :gripper_control/GraspEpsilon "95b2c5464a6f679bd1dacaf86414f095")


