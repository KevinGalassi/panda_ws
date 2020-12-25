;; Auto-generated. Do not edit!


(when (boundp 'schunk_pg70::stop)
  (if (not (find-package "SCHUNK_PG70"))
    (make-package "SCHUNK_PG70"))
  (shadow 'stop (find-package "SCHUNK_PG70")))
(unless (find-package "SCHUNK_PG70::STOP")
  (make-package "SCHUNK_PG70::STOP"))
(unless (find-package "SCHUNK_PG70::STOPREQUEST")
  (make-package "SCHUNK_PG70::STOPREQUEST"))
(unless (find-package "SCHUNK_PG70::STOPRESPONSE")
  (make-package "SCHUNK_PG70::STOPRESPONSE"))

(in-package "ROS")





(defclass schunk_pg70::stopRequest
  :super ros::object
  :slots ())

(defmethod schunk_pg70::stopRequest
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

(defclass schunk_pg70::stopResponse
  :super ros::object
  :slots (_stop_result ))

(defmethod schunk_pg70::stopResponse
  (:init
   (&key
    ((:stop_result __stop_result) 0.0)
    )
   (send-super :init)
   (setq _stop_result (float __stop_result))
   self)
  (:stop_result
   (&optional __stop_result)
   (if __stop_result (setq _stop_result __stop_result)) _stop_result)
  (:serialization-length
   ()
   (+
    ;; float32 _stop_result
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _stop_result
       (sys::poke _stop_result (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _stop_result
     (setq _stop_result (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(defclass schunk_pg70::stop
  :super ros::object
  :slots ())

(setf (get schunk_pg70::stop :md5sum-) "1dc3c304d8e1ef4b9d7103ce1e3c990a")
(setf (get schunk_pg70::stop :datatype-) "schunk_pg70/stop")
(setf (get schunk_pg70::stop :request) schunk_pg70::stopRequest)
(setf (get schunk_pg70::stop :response) schunk_pg70::stopResponse)

(defmethod schunk_pg70::stopRequest
  (:response () (instance schunk_pg70::stopResponse :init)))

(setf (get schunk_pg70::stopRequest :md5sum-) "1dc3c304d8e1ef4b9d7103ce1e3c990a")
(setf (get schunk_pg70::stopRequest :datatype-) "schunk_pg70/stopRequest")
(setf (get schunk_pg70::stopRequest :definition-)
      "---
float32 stop_result

")

(setf (get schunk_pg70::stopResponse :md5sum-) "1dc3c304d8e1ef4b9d7103ce1e3c990a")
(setf (get schunk_pg70::stopResponse :datatype-) "schunk_pg70/stopResponse")
(setf (get schunk_pg70::stopResponse :definition-)
      "---
float32 stop_result

")



(provide :schunk_pg70/stop "1dc3c304d8e1ef4b9d7103ce1e3c990a")


