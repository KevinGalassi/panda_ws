; Auto-generated. Do not edit!


(cl:in-package planner-msg)


;//! \htmlinclude ComputeTrajectoryFeedback.msg.html

(cl:defclass <ComputeTrajectoryFeedback> (roslisp-msg-protocol:ros-message)
  ((output
    :reader output
    :initarg :output
    :type cl:string
    :initform ""))
)

(cl:defclass ComputeTrajectoryFeedback (<ComputeTrajectoryFeedback>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <ComputeTrajectoryFeedback>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'ComputeTrajectoryFeedback)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name planner-msg:<ComputeTrajectoryFeedback> is deprecated: use planner-msg:ComputeTrajectoryFeedback instead.")))

(cl:ensure-generic-function 'output-val :lambda-list '(m))
(cl:defmethod output-val ((m <ComputeTrajectoryFeedback>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader planner-msg:output-val is deprecated.  Use planner-msg:output instead.")
  (output m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <ComputeTrajectoryFeedback>) ostream)
  "Serializes a message object of type '<ComputeTrajectoryFeedback>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'output))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'output))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <ComputeTrajectoryFeedback>) istream)
  "Deserializes a message object of type '<ComputeTrajectoryFeedback>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'output) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'output) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<ComputeTrajectoryFeedback>)))
  "Returns string type for a message object of type '<ComputeTrajectoryFeedback>"
  "planner/ComputeTrajectoryFeedback")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'ComputeTrajectoryFeedback)))
  "Returns string type for a message object of type 'ComputeTrajectoryFeedback"
  "planner/ComputeTrajectoryFeedback")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<ComputeTrajectoryFeedback>)))
  "Returns md5sum for a message object of type '<ComputeTrajectoryFeedback>"
  "0825d95fdfa2c8f4bbb4e9c74bccd3fd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'ComputeTrajectoryFeedback)))
  "Returns md5sum for a message object of type 'ComputeTrajectoryFeedback"
  "0825d95fdfa2c8f4bbb4e9c74bccd3fd")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<ComputeTrajectoryFeedback>)))
  "Returns full string definition for message of type '<ComputeTrajectoryFeedback>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#feedback~%string output~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'ComputeTrajectoryFeedback)))
  "Returns full string definition for message of type 'ComputeTrajectoryFeedback"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#feedback~%string output~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <ComputeTrajectoryFeedback>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'output))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <ComputeTrajectoryFeedback>))
  "Converts a ROS message object to a list"
  (cl:list 'ComputeTrajectoryFeedback
    (cl:cons ':output (output msg))
))
