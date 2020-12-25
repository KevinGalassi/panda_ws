
(cl:in-package :asdf)

(defsystem "schunk_pg70-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "acknowledge_error" :depends-on ("_package_acknowledge_error"))
    (:file "_package_acknowledge_error" :depends-on ("_package"))
    (:file "get_error" :depends-on ("_package_get_error"))
    (:file "_package_get_error" :depends-on ("_package"))
    (:file "get_position" :depends-on ("_package_get_position"))
    (:file "_package_get_position" :depends-on ("_package"))
    (:file "reference" :depends-on ("_package_reference"))
    (:file "_package_reference" :depends-on ("_package"))
    (:file "set_position" :depends-on ("_package_set_position"))
    (:file "_package_set_position" :depends-on ("_package"))
    (:file "stop" :depends-on ("_package_stop"))
    (:file "_package_stop" :depends-on ("_package"))
  ))