
(cl:in-package :asdf)

(defsystem "read_sensor-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "proximity_sensor_data" :depends-on ("_package_proximity_sensor_data"))
    (:file "_package_proximity_sensor_data" :depends-on ("_package"))
    (:file "tactile_sensor_data" :depends-on ("_package_tactile_sensor_data"))
    (:file "_package_tactile_sensor_data" :depends-on ("_package"))
  ))