; Auto-generated. Do not edit!


(cl:in-package schunk_pg70-srv)


;//! \htmlinclude get_position-request.msg.html

(cl:defclass <get_position-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass get_position-request (<get_position-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <get_position-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'get_position-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<get_position-request> is deprecated: use schunk_pg70-srv:get_position-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <get_position-request>) ostream)
  "Serializes a message object of type '<get_position-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <get_position-request>) istream)
  "Deserializes a message object of type '<get_position-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<get_position-request>)))
  "Returns string type for a service object of type '<get_position-request>"
  "schunk_pg70/get_positionRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'get_position-request)))
  "Returns string type for a service object of type 'get_position-request"
  "schunk_pg70/get_positionRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<get_position-request>)))
  "Returns md5sum for a message object of type '<get_position-request>"
  "40db4b6d09ee0b910be5d9e61aacbb7d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'get_position-request)))
  "Returns md5sum for a message object of type 'get_position-request"
  "40db4b6d09ee0b910be5d9e61aacbb7d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<get_position-request>)))
  "Returns full string definition for message of type '<get_position-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'get_position-request)))
  "Returns full string definition for message of type 'get_position-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <get_position-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <get_position-request>))
  "Converts a ROS message object to a list"
  (cl:list 'get_position-request
))
;//! \htmlinclude get_position-response.msg.html

(cl:defclass <get_position-response> (roslisp-msg-protocol:ros-message)
  ((actual_position
    :reader actual_position
    :initarg :actual_position
    :type cl:float
    :initform 0.0))
)

(cl:defclass get_position-response (<get_position-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <get_position-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'get_position-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<get_position-response> is deprecated: use schunk_pg70-srv:get_position-response instead.")))

(cl:ensure-generic-function 'actual_position-val :lambda-list '(m))
(cl:defmethod actual_position-val ((m <get_position-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader schunk_pg70-srv:actual_position-val is deprecated.  Use schunk_pg70-srv:actual_position instead.")
  (actual_position m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <get_position-response>) ostream)
  "Serializes a message object of type '<get_position-response>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'actual_position))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <get_position-response>) istream)
  "Deserializes a message object of type '<get_position-response>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'actual_position) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<get_position-response>)))
  "Returns string type for a service object of type '<get_position-response>"
  "schunk_pg70/get_positionResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'get_position-response)))
  "Returns string type for a service object of type 'get_position-response"
  "schunk_pg70/get_positionResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<get_position-response>)))
  "Returns md5sum for a message object of type '<get_position-response>"
  "40db4b6d09ee0b910be5d9e61aacbb7d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'get_position-response)))
  "Returns md5sum for a message object of type 'get_position-response"
  "40db4b6d09ee0b910be5d9e61aacbb7d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<get_position-response>)))
  "Returns full string definition for message of type '<get_position-response>"
  (cl:format cl:nil "float32 actual_position~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'get_position-response)))
  "Returns full string definition for message of type 'get_position-response"
  (cl:format cl:nil "float32 actual_position~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <get_position-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <get_position-response>))
  "Converts a ROS message object to a list"
  (cl:list 'get_position-response
    (cl:cons ':actual_position (actual_position msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'get_position)))
  'get_position-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'get_position)))
  'get_position-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'get_position)))
  "Returns string type for a service object of type '<get_position>"
  "schunk_pg70/get_position")