; Auto-generated. Do not edit!


(cl:in-package schunk_pg70-srv)


;//! \htmlinclude acknowledge_error-request.msg.html

(cl:defclass <acknowledge_error-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass acknowledge_error-request (<acknowledge_error-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <acknowledge_error-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'acknowledge_error-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<acknowledge_error-request> is deprecated: use schunk_pg70-srv:acknowledge_error-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <acknowledge_error-request>) ostream)
  "Serializes a message object of type '<acknowledge_error-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <acknowledge_error-request>) istream)
  "Deserializes a message object of type '<acknowledge_error-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<acknowledge_error-request>)))
  "Returns string type for a service object of type '<acknowledge_error-request>"
  "schunk_pg70/acknowledge_errorRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'acknowledge_error-request)))
  "Returns string type for a service object of type 'acknowledge_error-request"
  "schunk_pg70/acknowledge_errorRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<acknowledge_error-request>)))
  "Returns md5sum for a message object of type '<acknowledge_error-request>"
  "216312424a7b13297dba83de8033144e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'acknowledge_error-request)))
  "Returns md5sum for a message object of type 'acknowledge_error-request"
  "216312424a7b13297dba83de8033144e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<acknowledge_error-request>)))
  "Returns full string definition for message of type '<acknowledge_error-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'acknowledge_error-request)))
  "Returns full string definition for message of type 'acknowledge_error-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <acknowledge_error-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <acknowledge_error-request>))
  "Converts a ROS message object to a list"
  (cl:list 'acknowledge_error-request
))
;//! \htmlinclude acknowledge_error-response.msg.html

(cl:defclass <acknowledge_error-response> (roslisp-msg-protocol:ros-message)
  ((acknowledge_response
    :reader acknowledge_response
    :initarg :acknowledge_response
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass acknowledge_error-response (<acknowledge_error-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <acknowledge_error-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'acknowledge_error-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<acknowledge_error-response> is deprecated: use schunk_pg70-srv:acknowledge_error-response instead.")))

(cl:ensure-generic-function 'acknowledge_response-val :lambda-list '(m))
(cl:defmethod acknowledge_response-val ((m <acknowledge_error-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader schunk_pg70-srv:acknowledge_response-val is deprecated.  Use schunk_pg70-srv:acknowledge_response instead.")
  (acknowledge_response m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <acknowledge_error-response>) ostream)
  "Serializes a message object of type '<acknowledge_error-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'acknowledge_response) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <acknowledge_error-response>) istream)
  "Deserializes a message object of type '<acknowledge_error-response>"
    (cl:setf (cl:slot-value msg 'acknowledge_response) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<acknowledge_error-response>)))
  "Returns string type for a service object of type '<acknowledge_error-response>"
  "schunk_pg70/acknowledge_errorResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'acknowledge_error-response)))
  "Returns string type for a service object of type 'acknowledge_error-response"
  "schunk_pg70/acknowledge_errorResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<acknowledge_error-response>)))
  "Returns md5sum for a message object of type '<acknowledge_error-response>"
  "216312424a7b13297dba83de8033144e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'acknowledge_error-response)))
  "Returns md5sum for a message object of type 'acknowledge_error-response"
  "216312424a7b13297dba83de8033144e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<acknowledge_error-response>)))
  "Returns full string definition for message of type '<acknowledge_error-response>"
  (cl:format cl:nil "bool acknowledge_response~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'acknowledge_error-response)))
  "Returns full string definition for message of type 'acknowledge_error-response"
  (cl:format cl:nil "bool acknowledge_response~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <acknowledge_error-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <acknowledge_error-response>))
  "Converts a ROS message object to a list"
  (cl:list 'acknowledge_error-response
    (cl:cons ':acknowledge_response (acknowledge_response msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'acknowledge_error)))
  'acknowledge_error-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'acknowledge_error)))
  'acknowledge_error-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'acknowledge_error)))
  "Returns string type for a service object of type '<acknowledge_error>"
  "schunk_pg70/acknowledge_error")