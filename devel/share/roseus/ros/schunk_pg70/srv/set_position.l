;; Auto-generated. Do not edit!


(when (boundp 'schunk_pg70::set_position)
  (if (not (find-package "SCHUNK_PG70"))
    (make-package "SCHUNK_PG70"))
  (shadow 'set_position (find-package "SCHUNK_PG70")))
(unless (find-package "SCHUNK_PG70::SET_POSITION")
  (make-package "SCHUNK_PG70::SET_POSITION"))
(unless (find-package "SCHUNK_PG70::SET_POSITIONREQUEST")
  (make-package "SCHUNK_PG70::SET_POSITIONREQUEST"))
(unless (find-package "SCHUNK_PG70::SET_POSITIONRESPONSE")
  (make-package "SCHUNK_PG70::SET_POSITIONRESPONSE"))

(in-package "ROS")





(defclass schunk_pg70::set_positionRequest
  :super ros::object
  :slots (_goal_position _goal_velocity _goal_acceleration ))

(defmethod schunk_pg70::set_positionRequest
  (:init
   (&key
    ((:goal_position __goal_position) 0.0)
    ((:goal_velocity __goal_velocity) 0.0)
    ((:goal_acceleration __goal_acceleration) 0.0)
    )
   (send-super :init)
   (setq _goal_position (float __goal_position))
   (setq _goal_velocity (float __goal_velocity))
   (setq _goal_acceleration (float __goal_acceleration))
   self)
  (:goal_position
   (&optional __goal_position)
   (if __goal_position (setq _goal_position __goal_position)) _goal_position)
  (:goal_velocity
   (&optional __goal_velocity)
   (if __goal_velocity (setq _goal_velocity __goal_velocity)) _goal_velocity)
  (:goal_acceleration
   (&optional __goal_acceleration)
   (if __goal_acceleration (setq _goal_acceleration __goal_acceleration)) _goal_acceleration)
  (:serialization-length
   ()
   (+
    ;; float32 _goal_position
    4
    ;; float32 _goal_velocity
    4
    ;; float32 _goal_acceleration
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _goal_position
       (sys::poke _goal_position (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _goal_velocity
       (sys::poke _goal_velocity (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _goal_acceleration
       (sys::poke _goal_acceleration (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _goal_position
     (setq _goal_position (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _goal_velocity
     (setq _goal_velocity (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _goal_acceleration
     (setq _goal_acceleration (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(defclass schunk_pg70::set_positionResponse
  :super ros::object
  :slots (_goal_accepted ))

(defmethod schunk_pg70::set_positionResponse
  (:init
   (&key
    ((:goal_accepted __goal_accepted) nil)
    )
   (send-super :init)
   (setq _goal_accepted __goal_accepted)
   self)
  (:goal_accepted
   (&optional __goal_accepted)
   (if __goal_accepted (setq _goal_accepted __goal_accepted)) _goal_accepted)
  (:serialization-length
   ()
   (+
    ;; bool _goal_accepted
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _goal_accepted
       (if _goal_accepted (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _goal_accepted
     (setq _goal_accepted (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass schunk_pg70::set_position
  :super ros::object
  :slots ())

(setf (get schunk_pg70::set_position :md5sum-) "ab6e767af4c4f9e00f0354670a02d7cb")
(setf (get schunk_pg70::set_position :datatype-) "schunk_pg70/set_position")
(setf (get schunk_pg70::set_position :request) schunk_pg70::set_positionRequest)
(setf (get schunk_pg70::set_position :response) schunk_pg70::set_positionResponse)

(defmethod schunk_pg70::set_positionRequest
  (:response () (instance schunk_pg70::set_positionResponse :init)))

(setf (get schunk_pg70::set_positionRequest :md5sum-) "ab6e767af4c4f9e00f0354670a02d7cb")
(setf (get schunk_pg70::set_positionRequest :datatype-) "schunk_pg70/set_positionRequest")
(setf (get schunk_pg70::set_positionRequest :definition-)
      "float32 goal_position
float32 goal_velocity
float32 goal_acceleration
---
bool goal_accepted
")

(setf (get schunk_pg70::set_positionResponse :md5sum-) "ab6e767af4c4f9e00f0354670a02d7cb")
(setf (get schunk_pg70::set_positionResponse :datatype-) "schunk_pg70/set_positionResponse")
(setf (get schunk_pg70::set_positionResponse :definition-)
      "float32 goal_position
float32 goal_velocity
float32 goal_acceleration
---
bool goal_accepted
")



(provide :schunk_pg70/set_position "ab6e767af4c4f9e00f0354670a02d7cb")


