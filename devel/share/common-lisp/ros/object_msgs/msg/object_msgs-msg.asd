
(cl:in-package :asdf)

(defsystem "object_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "object_msgs" :depends-on ("_package_object_msgs"))
    (:file "_package_object_msgs" :depends-on ("_package"))
  ))