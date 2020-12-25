; Auto-generated. Do not edit!


(cl:in-package schunk_pg70-srv)


;//! \htmlinclude set_position-request.msg.html

(cl:defclass <set_position-request> (roslisp-msg-protocol:ros-message)
  ((goal_position
    :reader goal_position
    :initarg :goal_position
    :type cl:float
    :initform 0.0)
   (goal_velocity
    :reader goal_velocity
    :initarg :goal_velocity
    :type cl:float
    :initform 0.0)
   (goal_acceleration
    :reader goal_acceleration
    :initarg :goal_acceleration
    :type cl:float
    :initform 0.0))
)

(cl:defclass set_position-request (<set_position-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <set_position-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'set_position-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<set_position-request> is deprecated: use schunk_pg70-srv:set_position-request instead.")))

(cl:ensure-generic-function 'goal_position-val :lambda-list '(m))
(cl:defmethod goal_position-val ((m <set_position-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader schunk_pg70-srv:goal_position-val is deprecated.  Use schunk_pg70-srv:goal_position instead.")
  (goal_position m))

(cl:ensure-generic-function 'goal_velocity-val :lambda-list '(m))
(cl:defmethod goal_velocity-val ((m <set_position-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader schunk_pg70-srv:goal_velocity-val is deprecated.  Use schunk_pg70-srv:goal_velocity instead.")
  (goal_velocity m))

(cl:ensure-generic-function 'goal_acceleration-val :lambda-list '(m))
(cl:defmethod goal_acceleration-val ((m <set_position-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader schunk_pg70-srv:goal_acceleration-val is deprecated.  Use schunk_pg70-srv:goal_acceleration instead.")
  (goal_acceleration m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <set_position-request>) ostream)
  "Serializes a message object of type '<set_position-request>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'goal_position))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'goal_velocity))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'goal_acceleration))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <set_position-request>) istream)
  "Deserializes a message object of type '<set_position-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'goal_position) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'goal_velocity) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'goal_acceleration) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<set_position-request>)))
  "Returns string type for a service object of type '<set_position-request>"
  "schunk_pg70/set_positionRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'set_position-request)))
  "Returns string type for a service object of type 'set_position-request"
  "schunk_pg70/set_positionRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<set_position-request>)))
  "Returns md5sum for a message object of type '<set_position-request>"
  "ab6e767af4c4f9e00f0354670a02d7cb")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'set_position-request)))
  "Returns md5sum for a message object of type 'set_position-request"
  "ab6e767af4c4f9e00f0354670a02d7cb")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<set_position-request>)))
  "Returns full string definition for message of type '<set_position-request>"
  (cl:format cl:nil "float32 goal_position~%float32 goal_velocity~%float32 goal_acceleration~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'set_position-request)))
  "Returns full string definition for message of type 'set_position-request"
  (cl:format cl:nil "float32 goal_position~%float32 goal_velocity~%float32 goal_acceleration~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <set_position-request>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <set_position-request>))
  "Converts a ROS message object to a list"
  (cl:list 'set_position-request
    (cl:cons ':goal_position (goal_position msg))
    (cl:cons ':goal_velocity (goal_velocity msg))
    (cl:cons ':goal_acceleration (goal_acceleration msg))
))
;//! \htmlinclude set_position-response.msg.html

(cl:defclass <set_position-response> (roslisp-msg-protocol:ros-message)
  ((goal_accepted
    :reader goal_accepted
    :initarg :goal_accepted
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass set_position-response (<set_position-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <set_position-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'set_position-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<set_position-response> is deprecated: use schunk_pg70-srv:set_position-response instead.")))

(cl:ensure-generic-function 'goal_accepted-val :lambda-list '(m))
(cl:defmethod goal_accepted-val ((m <set_position-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader schunk_pg70-srv:goal_accepted-val is deprecated.  Use schunk_pg70-srv:goal_accepted instead.")
  (goal_accepted m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <set_position-response>) ostream)
  "Serializes a message object of type '<set_position-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'goal_accepted) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <set_position-response>) istream)
  "Deserializes a message object of type '<set_position-response>"
    (cl:setf (cl:slot-value msg 'goal_accepted) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<set_position-response>)))
  "Returns string type for a service object of type '<set_position-response>"
  "schunk_pg70/set_positionResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'set_position-response)))
  "Returns string type for a service object of type 'set_position-response"
  "schunk_pg70/set_positionResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<set_position-response>)))
  "Returns md5sum for a message object of type '<set_position-response>"
  "ab6e767af4c4f9e00f0354670a02d7cb")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'set_position-response)))
  "Returns md5sum for a message object of type 'set_position-response"
  "ab6e767af4c4f9e00f0354670a02d7cb")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<set_position-response>)))
  "Returns full string definition for message of type '<set_position-response>"
  (cl:format cl:nil "bool goal_accepted~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'set_position-response)))
  "Returns full string definition for message of type 'set_position-response"
  (cl:format cl:nil "bool goal_accepted~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <set_position-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <set_position-response>))
  "Converts a ROS message object to a list"
  (cl:list 'set_position-response
    (cl:cons ':goal_accepted (goal_accepted msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'set_position)))
  'set_position-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'set_position)))
  'set_position-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'set_position)))
  "Returns string type for a service object of type '<set_position>"
  "schunk_pg70/set_position")