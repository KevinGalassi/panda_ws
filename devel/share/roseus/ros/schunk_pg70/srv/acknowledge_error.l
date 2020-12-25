;; Auto-generated. Do not edit!


(when (boundp 'schunk_pg70::acknowledge_error)
  (if (not (find-package "SCHUNK_PG70"))
    (make-package "SCHUNK_PG70"))
  (shadow 'acknowledge_error (find-package "SCHUNK_PG70")))
(unless (find-package "SCHUNK_PG70::ACKNOWLEDGE_ERROR")
  (make-package "SCHUNK_PG70::ACKNOWLEDGE_ERROR"))
(unless (find-package "SCHUNK_PG70::ACKNOWLEDGE_ERRORREQUEST")
  (make-package "SCHUNK_PG70::ACKNOWLEDGE_ERRORREQUEST"))
(unless (find-package "SCHUNK_PG70::ACKNOWLEDGE_ERRORRESPONSE")
  (make-package "SCHUNK_PG70::ACKNOWLEDGE_ERRORRESPONSE"))

(in-package "ROS")





(defclass schunk_pg70::acknowledge_errorRequest
  :super ros::object
  :slots ())

(defmethod schunk_pg70::acknowledge_errorRequest
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

(defclass schunk_pg70::acknowledge_errorResponse
  :super ros::object
  :slots (_acknowledge_response ))

(defmethod schunk_pg70::acknowledge_errorResponse
  (:init
   (&key
    ((:acknowledge_response __acknowledge_response) nil)
    )
   (send-super :init)
   (setq _acknowledge_response __acknowledge_response)
   self)
  (:acknowledge_response
   (&optional __acknowledge_response)
   (if __acknowledge_response (setq _acknowledge_response __acknowledge_response)) _acknowledge_response)
  (:serialization-length
   ()
   (+
    ;; bool _acknowledge_response
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _acknowledge_response
       (if _acknowledge_response (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _acknowledge_response
     (setq _acknowledge_response (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass schunk_pg70::acknowledge_error
  :super ros::object
  :slots ())

(setf (get schunk_pg70::acknowledge_error :md5sum-) "216312424a7b13297dba83de8033144e")
(setf (get schunk_pg70::acknowledge_error :datatype-) "schunk_pg70/acknowledge_error")
(setf (get schunk_pg70::acknowledge_error :request) schunk_pg70::acknowledge_errorRequest)
(setf (get schunk_pg70::acknowledge_error :response) schunk_pg70::acknowledge_errorResponse)

(defmethod schunk_pg70::acknowledge_errorRequest
  (:response () (instance schunk_pg70::acknowledge_errorResponse :init)))

(setf (get schunk_pg70::acknowledge_errorRequest :md5sum-) "216312424a7b13297dba83de8033144e")
(setf (get schunk_pg70::acknowledge_errorRequest :datatype-) "schunk_pg70/acknowledge_errorRequest")
(setf (get schunk_pg70::acknowledge_errorRequest :definition-)
      "---
bool acknowledge_response

")

(setf (get schunk_pg70::acknowledge_errorResponse :md5sum-) "216312424a7b13297dba83de8033144e")
(setf (get schunk_pg70::acknowledge_errorResponse :datatype-) "schunk_pg70/acknowledge_errorResponse")
(setf (get schunk_pg70::acknowledge_errorResponse :definition-)
      "---
bool acknowledge_response

")



(provide :schunk_pg70/acknowledge_error "216312424a7b13297dba83de8033144e")


