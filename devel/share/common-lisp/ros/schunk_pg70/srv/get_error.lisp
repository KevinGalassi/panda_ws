; Auto-generated. Do not edit!


(cl:in-package schunk_pg70-srv)


;//! \htmlinclude get_error-request.msg.html

(cl:defclass <get_error-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass get_error-request (<get_error-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <get_error-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'get_error-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<get_error-request> is deprecated: use schunk_pg70-srv:get_error-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <get_error-request>) ostream)
  "Serializes a message object of type '<get_error-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <get_error-request>) istream)
  "Deserializes a message object of type '<get_error-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<get_error-request>)))
  "Returns string type for a service object of type '<get_error-request>"
  "schunk_pg70/get_errorRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'get_error-request)))
  "Returns string type for a service object of type 'get_error-request"
  "schunk_pg70/get_errorRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<get_error-request>)))
  "Returns md5sum for a message object of type '<get_error-request>"
  "ea324a22c787839f822b9a025bc2c6fe")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'get_error-request)))
  "Returns md5sum for a message object of type 'get_error-request"
  "ea324a22c787839f822b9a025bc2c6fe")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<get_error-request>)))
  "Returns full string definition for message of type '<get_error-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'get_error-request)))
  "Returns full string definition for message of type 'get_error-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <get_error-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <get_error-request>))
  "Converts a ROS message object to a list"
  (cl:list 'get_error-request
))
;//! \htmlinclude get_error-response.msg.html

(cl:defclass <get_error-response> (roslisp-msg-protocol:ros-message)
  ((error_code
    :reader error_code
    :initarg :error_code
    :type cl:integer
    :initform 0))
)

(cl:defclass get_error-response (<get_error-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <get_error-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'get_error-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<get_error-response> is deprecated: use schunk_pg70-srv:get_error-response instead.")))

(cl:ensure-generic-function 'error_code-val :lambda-list '(m))
(cl:defmethod error_code-val ((m <get_error-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader schunk_pg70-srv:error_code-val is deprecated.  Use schunk_pg70-srv:error_code instead.")
  (error_code m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <get_error-response>) ostream)
  "Serializes a message object of type '<get_error-response>"
  (cl:let* ((signed (cl:slot-value msg 'error_code)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <get_error-response>) istream)
  "Deserializes a message object of type '<get_error-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'error_code) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<get_error-response>)))
  "Returns string type for a service object of type '<get_error-response>"
  "schunk_pg70/get_errorResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'get_error-response)))
  "Returns string type for a service object of type 'get_error-response"
  "schunk_pg70/get_errorResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<get_error-response>)))
  "Returns md5sum for a message object of type '<get_error-response>"
  "ea324a22c787839f822b9a025bc2c6fe")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'get_error-response)))
  "Returns md5sum for a message object of type 'get_error-response"
  "ea324a22c787839f822b9a025bc2c6fe")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<get_error-response>)))
  "Returns full string definition for message of type '<get_error-response>"
  (cl:format cl:nil "int32 error_code~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'get_error-response)))
  "Returns full string definition for message of type 'get_error-response"
  (cl:format cl:nil "int32 error_code~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <get_error-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <get_error-response>))
  "Converts a ROS message object to a list"
  (cl:list 'get_error-response
    (cl:cons ':error_code (error_code msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'get_error)))
  'get_error-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'get_error)))
  'get_error-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'get_error)))
  "Returns string type for a service object of type '<get_error>"
  "schunk_pg70/get_error")