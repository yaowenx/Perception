;; Auto-generated. Do not edit!


(when (boundp 'object_msgs::object_msgs)
  (if (not (find-package "OBJECT_MSGS"))
    (make-package "OBJECT_MSGS"))
  (shadow 'object_msgs (find-package "OBJECT_MSGS")))
(unless (find-package "OBJECT_MSGS::OBJECT_MSGS")
  (make-package "OBJECT_MSGS::OBJECT_MSGS"))

(in-package "ROS")
;;//! \htmlinclude object_msgs.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass object_msgs::object_msgs
  :super ros::object
  :slots (_header _types _center_x _center_y _edge1_x _edge1_y _edge2_x _edge2_y ))

(defmethod object_msgs::object_msgs
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:types __types) (make-array 0 :initial-element 0 :element-type :integer))
    ((:center_x __center_x) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:center_y __center_y) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:edge1_x __edge1_x) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:edge1_y __edge1_y) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:edge2_x __edge2_x) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:edge2_y __edge2_y) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _header __header)
   (setq _types __types)
   (setq _center_x __center_x)
   (setq _center_y __center_y)
   (setq _edge1_x __edge1_x)
   (setq _edge1_y __edge1_y)
   (setq _edge2_x __edge2_x)
   (setq _edge2_y __edge2_y)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:types
   (&optional __types)
   (if __types (setq _types __types)) _types)
  (:center_x
   (&optional __center_x)
   (if __center_x (setq _center_x __center_x)) _center_x)
  (:center_y
   (&optional __center_y)
   (if __center_y (setq _center_y __center_y)) _center_y)
  (:edge1_x
   (&optional __edge1_x)
   (if __edge1_x (setq _edge1_x __edge1_x)) _edge1_x)
  (:edge1_y
   (&optional __edge1_y)
   (if __edge1_y (setq _edge1_y __edge1_y)) _edge1_y)
  (:edge2_x
   (&optional __edge2_x)
   (if __edge2_x (setq _edge2_x __edge2_x)) _edge2_x)
  (:edge2_y
   (&optional __edge2_y)
   (if __edge2_y (setq _edge2_y __edge2_y)) _edge2_y)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; int16[] _types
    (* 2    (length _types)) 4
    ;; float32[] _center_x
    (* 4    (length _center_x)) 4
    ;; float32[] _center_y
    (* 4    (length _center_y)) 4
    ;; float32[] _edge1_x
    (* 4    (length _edge1_x)) 4
    ;; float32[] _edge1_y
    (* 4    (length _edge1_y)) 4
    ;; float32[] _edge2_x
    (* 4    (length _edge2_x)) 4
    ;; float32[] _edge2_y
    (* 4    (length _edge2_y)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; int16[] _types
     (write-long (length _types) s)
     (dotimes (i (length _types))
       (write-word (elt _types i) s)
       )
     ;; float32[] _center_x
     (write-long (length _center_x) s)
     (dotimes (i (length _center_x))
       (sys::poke (elt _center_x i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _center_y
     (write-long (length _center_y) s)
     (dotimes (i (length _center_y))
       (sys::poke (elt _center_y i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _edge1_x
     (write-long (length _edge1_x) s)
     (dotimes (i (length _edge1_x))
       (sys::poke (elt _edge1_x i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _edge1_y
     (write-long (length _edge1_y) s)
     (dotimes (i (length _edge1_y))
       (sys::poke (elt _edge1_y i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _edge2_x
     (write-long (length _edge2_x) s)
     (dotimes (i (length _edge2_x))
       (sys::poke (elt _edge2_x i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[] _edge2_y
     (write-long (length _edge2_y) s)
     (dotimes (i (length _edge2_y))
       (sys::poke (elt _edge2_y i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; int16[] _types
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _types (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _types i) (sys::peek buf ptr- :short)) (incf ptr- 2)
     ))
   ;; float32[] _center_x
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _center_x (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _center_x i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _center_y
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _center_y (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _center_y i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _edge1_x
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _edge1_x (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _edge1_x i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _edge1_y
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _edge1_y (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _edge1_y i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _edge2_x
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _edge2_x (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _edge2_x i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32[] _edge2_y
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _edge2_y (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _edge2_y i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get object_msgs::object_msgs :md5sum-) "4168c556534593784663dd7042fab0b2")
(setf (get object_msgs::object_msgs :datatype-) "object_msgs/object_msgs")
(setf (get object_msgs::object_msgs :definition-)
      "Header header
int16[] types
float32[] center_x
float32[] center_y
float32[] edge1_x
float32[] edge1_y
float32[] edge2_x
float32[] edge2_y

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

")



(provide :object_msgs/object_msgs "4168c556534593784663dd7042fab0b2")


