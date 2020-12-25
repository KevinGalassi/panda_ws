
(cl:in-package :asdf)

(defsystem "planner-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :actionlib_msgs-msg
               :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "ComputeTrajectoryAction" :depends-on ("_package_ComputeTrajectoryAction"))
    (:file "_package_ComputeTrajectoryAction" :depends-on ("_package"))
    (:file "ComputeTrajectoryActionFeedback" :depends-on ("_package_ComputeTrajectoryActionFeedback"))
    (:file "_package_ComputeTrajectoryActionFeedback" :depends-on ("_package"))
    (:file "ComputeTrajectoryActionGoal" :depends-on ("_package_ComputeTrajectoryActionGoal"))
    (:file "_package_ComputeTrajectoryActionGoal" :depends-on ("_package"))
    (:file "ComputeTrajectoryActionResult" :depends-on ("_package_ComputeTrajectoryActionResult"))
    (:file "_package_ComputeTrajectoryActionResult" :depends-on ("_package"))
    (:file "ComputeTrajectoryFeedback" :depends-on ("_package_ComputeTrajectoryFeedback"))
    (:file "_package_ComputeTrajectoryFeedback" :depends-on ("_package"))
    (:file "ComputeTrajectoryGoal" :depends-on ("_package_ComputeTrajectoryGoal"))
    (:file "_package_ComputeTrajectoryGoal" :depends-on ("_package"))
    (:file "ComputeTrajectoryResult" :depends-on ("_package_ComputeTrajectoryResult"))
    (:file "_package_ComputeTrajectoryResult" :depends-on ("_package"))
  ))