; Auto-generated. Do not edit!


(cl:in-package schunk_pg70-srv)


;//! \htmlinclude stop-request.msg.html

(cl:defclass <stop-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass stop-request (<stop-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <stop-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'stop-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<stop-request> is deprecated: use schunk_pg70-srv:stop-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <stop-request>) ostream)
  "Serializes a message object of type '<stop-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <stop-request>) istream)
  "Deserializes a message object of type '<stop-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<stop-request>)))
  "Returns string type for a service object of type '<stop-request>"
  "schunk_pg70/stopRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'stop-request)))
  "Returns string type for a service object of type 'stop-request"
  "schunk_pg70/stopRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<stop-request>)))
  "Returns md5sum for a message object of type '<stop-request>"
  "1dc3c304d8e1ef4b9d7103ce1e3c990a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'stop-request)))
  "Returns md5sum for a message object of type 'stop-request"
  "1dc3c304d8e1ef4b9d7103ce1e3c990a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<stop-request>)))
  "Returns full string definition for message of type '<stop-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'stop-request)))
  "Returns full string definition for message of type 'stop-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <stop-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <stop-request>))
  "Converts a ROS message object to a list"
  (cl:list 'stop-request
))
;//! \htmlinclude stop-response.msg.html

(cl:defclass <stop-response> (roslisp-msg-protocol:ros-message)
  ((stop_result
    :reader stop_result
    :initarg :stop_result
    :type cl:float
    :initform 0.0))
)

(cl:defclass stop-response (<stop-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <stop-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'stop-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<stop-response> is deprecated: use schunk_pg70-srv:stop-response instead.")))

(cl:ensure-generic-function 'stop_result-val :lambda-list '(m))
(cl:defmethod stop_result-val ((m <stop-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader schunk_pg70-srv:stop_result-val is deprecated.  Use schunk_pg70-srv:stop_result instead.")
  (stop_result m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <stop-response>) ostream)
  "Serializes a message object of type '<stop-response>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'stop_result))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <stop-response>) istream)
  "Deserializes a message object of type '<stop-response>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'stop_result) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<stop-response>)))
  "Returns string type for a service object of type '<stop-response>"
  "schunk_pg70/stopResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'stop-response)))
  "Returns string type for a service object of type 'stop-response"
  "schunk_pg70/stopResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<stop-response>)))
  "Returns md5sum for a message object of type '<stop-response>"
  "1dc3c304d8e1ef4b9d7103ce1e3c990a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'stop-response)))
  "Returns md5sum for a message object of type 'stop-response"
  "1dc3c304d8e1ef4b9d7103ce1e3c990a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<stop-response>)))
  "Returns full string definition for message of type '<stop-response>"
  (cl:format cl:nil "float32 stop_result~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'stop-response)))
  "Returns full string definition for message of type 'stop-response"
  (cl:format cl:nil "float32 stop_result~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <stop-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <stop-response>))
  "Converts a ROS message object to a list"
  (cl:list 'stop-response
    (cl:cons ':stop_result (stop_result msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'stop)))
  'stop-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'stop)))
  'stop-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'stop)))
  "Returns string type for a service object of type '<stop>"
  "schunk_pg70/stop")