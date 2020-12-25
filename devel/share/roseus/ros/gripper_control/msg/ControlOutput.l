;; Auto-generated. Do not edit!


(when (boundp 'gripper_control::ControlOutput)
  (if (not (find-package "GRIPPER_CONTROL"))
    (make-package "GRIPPER_CONTROL"))
  (shadow 'ControlOutput (find-package "GRIPPER_CONTROL")))
(unless (find-package "GRIPPER_CONTROL::CONTROLOUTPUT")
  (make-package "GRIPPER_CONTROL::CONTROLOUTPUT"))

(in-package "ROS")
;;//! \htmlinclude ControlOutput.msg.html


(defclass gripper_control::ControlOutput
  :super ros::object
  :slots (_output _output_sat _gripper_position _error _reference _lower_bound _upper_bound _actual_value ))

(defmethod gripper_control::ControlOutput
  (:init
   (&key
    ((:output __output) 0.0)
    ((:output_sat __output_sat) 0.0)
    ((:gripper_position __gripper_position) 0.0)
    ((:error __error) 0.0)
    ((:reference __reference) 0.0)
    ((:lower_bound __lower_bound) 0.0)
    ((:upper_bound __upper_bound) 0.0)
    ((:actual_value __actual_value) 0.0)
    )
   (send-super :init)
   (setq _output (float __output))
   (setq _output_sat (float __output_sat))
   (setq _gripper_position (float __gripper_position))
   (setq _error (float __error))
   (setq _reference (float __reference))
   (setq _lower_bound (float __lower_bound))
   (setq _upper_bound (float __upper_bound))
   (setq _actual_value (float __actual_value))
   self)
  (:output
   (&optional __output)
   (if __output (setq _output __output)) _output)
  (:output_sat
   (&optional __output_sat)
   (if __output_sat (setq _output_sat __output_sat)) _output_sat)
  (:gripper_position
   (&optional __gripper_position)
   (if __gripper_position (setq _gripper_position __gripper_position)) _gripper_position)
  (:error
   (&optional __error)
   (if __error (setq _error __error)) _error)
  (:reference
   (&optional __reference)
   (if __reference (setq _reference __reference)) _reference)
  (:lower_bound
   (&optional __lower_bound)
   (if __lower_bound (setq _lower_bound __lower_bound)) _lower_bound)
  (:upper_bound
   (&optional __upper_bound)
   (if __upper_bound (setq _upper_bound __upper_bound)) _upper_bound)
  (:actual_value
   (&optional __actual_value)
   (if __actual_value (setq _actual_value __actual_value)) _actual_value)
  (:serialization-length
   ()
   (+
    ;; float64 _output
    8
    ;; float64 _output_sat
    8
    ;; float64 _gripper_position
    8
    ;; float64 _error
    8
    ;; float64 _reference
    8
    ;; float64 _lower_bound
    8
    ;; float64 _upper_bound
    8
    ;; float64 _actual_value
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _output
       (sys::poke _output (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _output_sat
       (sys::poke _output_sat (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _gripper_position
       (sys::poke _gripper_position (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _error
       (sys::poke _error (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _reference
       (sys::poke _reference (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _lower_bound
       (sys::poke _lower_bound (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _upper_bound
       (sys::poke _upper_bound (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _actual_value
       (sys::poke _actual_value (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _output
     (setq _output (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _output_sat
     (setq _output_sat (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _gripper_position
     (setq _gripper_position (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _error
     (setq _error (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _reference
     (setq _reference (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _lower_bound
     (setq _lower_bound (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _upper_bound
     (setq _upper_bound (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _actual_value
     (setq _actual_value (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get gripper_control::ControlOutput :md5sum-) "3d113725e502b53b0e53b00acafaa65f")
(setf (get gripper_control::ControlOutput :datatype-) "gripper_control/ControlOutput")
(setf (get gripper_control::ControlOutput :definition-)
      "float64 output
float64 output_sat
float64 gripper_position
float64 error
float64 reference
float64 lower_bound
float64 upper_bound
float64 actual_value

")



(provide :gripper_control/ControlOutput "3d113725e502b53b0e53b00acafaa65f")


