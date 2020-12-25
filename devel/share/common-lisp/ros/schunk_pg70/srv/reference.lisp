; Auto-generated. Do not edit!


(cl:in-package schunk_pg70-srv)


;//! \htmlinclude reference-request.msg.html

(cl:defclass <reference-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass reference-request (<reference-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <reference-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'reference-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<reference-request> is deprecated: use schunk_pg70-srv:reference-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <reference-request>) ostream)
  "Serializes a message object of type '<reference-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <reference-request>) istream)
  "Deserializes a message object of type '<reference-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<reference-request>)))
  "Returns string type for a service object of type '<reference-request>"
  "schunk_pg70/referenceRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'reference-request)))
  "Returns string type for a service object of type 'reference-request"
  "schunk_pg70/referenceRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<reference-request>)))
  "Returns md5sum for a message object of type '<reference-request>"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'reference-request)))
  "Returns md5sum for a message object of type 'reference-request"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<reference-request>)))
  "Returns full string definition for message of type '<reference-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'reference-request)))
  "Returns full string definition for message of type 'reference-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <reference-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <reference-request>))
  "Converts a ROS message object to a list"
  (cl:list 'reference-request
))
;//! \htmlinclude reference-response.msg.html

(cl:defclass <reference-response> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass reference-response (<reference-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <reference-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'reference-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name schunk_pg70-srv:<reference-response> is deprecated: use schunk_pg70-srv:reference-response instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <reference-response>) ostream)
  "Serializes a message object of type '<reference-response>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <reference-response>) istream)
  "Deserializes a message object of type '<reference-response>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<reference-response>)))
  "Returns string type for a service object of type '<reference-response>"
  "schunk_pg70/referenceResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'reference-response)))
  "Returns string type for a service object of type 'reference-response"
  "schunk_pg70/referenceResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<reference-response>)))
  "Returns md5sum for a message object of type '<reference-response>"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'reference-response)))
  "Returns md5sum for a message object of type 'reference-response"
  "d41d8cd98f00b204e9800998ecf8427e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<reference-response>)))
  "Returns full string definition for message of type '<reference-response>"
  (cl:format cl:nil "~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'reference-response)))
  "Returns full string definition for message of type 'reference-response"
  (cl:format cl:nil "~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <reference-response>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <reference-response>))
  "Converts a ROS message object to a list"
  (cl:list 'reference-response
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'reference)))
  'reference-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'reference)))
  'reference-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'reference)))
  "Returns string type for a service object of type '<reference>"
  "schunk_pg70/reference")