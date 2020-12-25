;; Auto-generated. Do not edit!


(when (boundp 'schunk_pg70::get_position)
  (if (not (find-package "SCHUNK_PG70"))
    (make-package "SCHUNK_PG70"))
  (shadow 'get_position (find-package "SCHUNK_PG70")))
(unless (find-package "SCHUNK_PG70::GET_POSITION")
  (make-package "SCHUNK_PG70::GET_POSITION"))
(unless (find-package "SCHUNK_PG70::GET_POSITIONREQUEST")
  (make-package "SCHUNK_PG70::GET_POSITIONREQUEST"))
(unless (find-package "SCHUNK_PG70::GET_POSITIONRESPONSE")
  (make-package "SCHUNK_PG70::GET_POSITIONRESPONSE"))

(in-package "ROS")





(defclass schunk_pg70::get_positionRequest
  :super ros::object
  :slots ())

(defmethod schunk_pg70::get_positionRequest
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(defclass schunk_pg70::get_positionResponse
  :super ros::object
  :slots (_actual_position ))

(defmethod schunk_pg70::get_positionResponse
  (:init
   (&key
    ((:actual_position __actual_position) 0.0)
    )
   (send-super :init)
   (setq _actual_position (float __actual_position))
   self)
  (:actual_position
   (&optional __actual_position)
   (if __actual_position (setq _actual_position __actual_position)) _actual_position)
  (:serialization-length
   ()
   (+
    ;; float32 _actual_position
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _actual_position
       (sys::poke _actual_position (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _actual_position
     (setq _actual_position (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(defclass schunk_pg70::get_position
  :super ros::object
  :slots ())

(setf (get schunk_pg70::get_position :md5sum-) "40db4b6d09ee0b910be5d9e61aacbb7d")
(setf (get schunk_pg70::get_position :datatype-) "schunk_pg70/get_position")
(setf (get schunk_pg70::get_position :request) schunk_pg70::get_positionRequest)
(setf (get schunk_pg70::get_position :response) schunk_pg70::get_positionResponse)

(defmethod schunk_pg70::get_positionRequest
  (:response () (instance schunk_pg70::get_positionResponse :init)))

(setf (get schunk_pg70::get_positionRequest :md5sum-) "40db4b6d09ee0b910be5d9e61aacbb7d")
(setf (get schunk_pg70::get_positionRequest :datatype-) "schunk_pg70/get_positionRequest")
(setf (get schunk_pg70::get_positionRequest :definition-)
      "---
float32 actual_position

")

(setf (get schunk_pg70::get_positionResponse :md5sum-) "40db4b6d09ee0b910be5d9e61aacbb7d")
(setf (get schunk_pg70::get_positionResponse :datatype-) "schunk_pg70/get_positionResponse")
(setf (get schunk_pg70::get_positionResponse :definition-)
      "---
float32 actual_position

")



(provide :schunk_pg70/get_position "40db4b6d09ee0b910be5d9e61aacbb7d")


