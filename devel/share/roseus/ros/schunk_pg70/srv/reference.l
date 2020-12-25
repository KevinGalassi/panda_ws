;; Auto-generated. Do not edit!


(when (boundp 'schunk_pg70::reference)
  (if (not (find-package "SCHUNK_PG70"))
    (make-package "SCHUNK_PG70"))
  (shadow 'reference (find-package "SCHUNK_PG70")))
(unless (find-package "SCHUNK_PG70::REFERENCE")
  (make-package "SCHUNK_PG70::REFERENCE"))
(unless (find-package "SCHUNK_PG70::REFERENCEREQUEST")
  (make-package "SCHUNK_PG70::REFERENCEREQUEST"))
(unless (find-package "SCHUNK_PG70::REFERENCERESPONSE")
  (make-package "SCHUNK_PG70::REFERENCERESPONSE"))

(in-package "ROS")





(defclass schunk_pg70::referenceRequest
  :super ros::object
  :slots ())

(defmethod schunk_pg70::referenceRequest
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

(defclass schunk_pg70::referenceResponse
  :super ros::object
  :slots ())

(defmethod schunk_pg70::referenceResponse
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

(defclass schunk_pg70::reference
  :super ros::object
  :slots ())

(setf (get schunk_pg70::reference :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get schunk_pg70::reference :datatype-) "schunk_pg70/reference")
(setf (get schunk_pg70::reference :request) schunk_pg70::referenceRequest)
(setf (get schunk_pg70::reference :response) schunk_pg70::referenceResponse)

(defmethod schunk_pg70::referenceRequest
  (:response () (instance schunk_pg70::referenceResponse :init)))

(setf (get schunk_pg70::referenceRequest :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get schunk_pg70::referenceRequest :datatype-) "schunk_pg70/referenceRequest")
(setf (get schunk_pg70::referenceRequest :definition-)
      "---


")

(setf (get schunk_pg70::referenceResponse :md5sum-) "d41d8cd98f00b204e9800998ecf8427e")
(setf (get schunk_pg70::referenceResponse :datatype-) "schunk_pg70/referenceResponse")
(setf (get schunk_pg70::referenceResponse :definition-)
      "---


")



(provide :schunk_pg70/reference "d41d8cd98f00b204e9800998ecf8427e")


