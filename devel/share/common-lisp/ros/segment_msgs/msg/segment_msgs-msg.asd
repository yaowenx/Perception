
(cl:in-package :asdf)

(defsystem "segment_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "segment_msgs" :depends-on ("_package_segment_msgs"))
    (:file "_package_segment_msgs" :depends-on ("_package"))
  ))