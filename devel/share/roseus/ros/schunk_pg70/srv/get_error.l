;; Auto-generated. Do not edit!


(when (boundp 'schunk_pg70::get_error)
  (if (not (find-package "SCHUNK_PG70"))
    (make-package "SCHUNK_PG70"))
  (shadow 'get_error (find-package "SCHUNK_PG70")))
(unless (find-package "SCHUNK_PG70::GET_ERROR")
  (make-package "SCHUNK_PG70::GET_ERROR"))
(unless (find-package "SCHUNK_PG70::GET_ERRORREQUEST")
  (make-package "SCHUNK_PG70::GET_ERRORREQUEST"))
(unless (find-package "SCHUNK_PG70::GET_ERRORRESPONSE")
  (make-package "SCHUNK_PG70::GET_ERRORRESPONSE"))

(in-package "ROS")





(defclass schunk_pg70::get_errorRequest
  :super ros::object
  :slots ())

(defmethod schunk_pg70::get_errorRequest
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

(defclass schunk_pg70::get_errorResponse
  :super ros::object
  :slots (_error_code ))

(defmethod schunk_pg70::get_errorResponse
  (:init
   (&key
    ((:error_code __error_code) 0)
    )
   (send-super :init)
   (setq _error_code (round __error_code))
   self)
  (:error_code
   (&optional __error_code)
   (if __error_code (setq _error_code __error_code)) _error_code)
  (:serialization-length
   ()
   (+
    ;; int32 _error_code
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _error_code
       (write-long _error_code s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _error_code
     (setq _error_code (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass schunk_pg70::get_error
  :super ros::object
  :slots ())

(setf (get schunk_pg70::get_error :md5sum-) "ea324a22c787839f822b9a025bc2c6fe")
(setf (get schunk_pg70::get_error :datatype-) "schunk_pg70/get_error")
(setf (get schunk_pg70::get_error :request) schunk_pg70::get_errorRequest)
(setf (get schunk_pg70::get_error :response) schunk_pg70::get_errorResponse)

(defmethod schunk_pg70::get_errorRequest
  (:response () (instance schunk_pg70::get_errorResponse :init)))

(setf (get schunk_pg70::get_errorRequest :md5sum-) "ea324a22c787839f822b9a025bc2c6fe")
(setf (get schunk_pg70::get_errorRequest :datatype-) "schunk_pg70/get_errorRequest")
(setf (get schunk_pg70::get_errorRequest :definition-)
      "---
int32 error_code

")

(setf (get schunk_pg70::get_errorResponse :md5sum-) "ea324a22c787839f822b9a025bc2c6fe")
(setf (get schunk_pg70::get_errorResponse :datatype-) "schunk_pg70/get_errorResponse")
(setf (get schunk_pg70::get_errorResponse :definition-)
      "---
int32 error_code

")



(provide :schunk_pg70/get_error "ea324a22c787839f822b9a025bc2c6fe")


