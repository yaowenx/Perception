;; Auto-generated. Do not edit!


(when (boundp 'segment_msgs::segment_msgs)
  (if (not (find-package "SEGMENT_MSGS"))
    (make-package "SEGMENT_MSGS"))
  (shadow 'segment_msgs (find-package "SEGMENT_MSGS")))
(unless (find-package "SEGMENT_MSGS::SEGMENT_MSGS")
  (make-package "SEGMENT_MSGS::SEGMENT_MSGS"))

(in-package "ROS")
;;//! \htmlinclude segment_msgs.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass segment_msgs::segment_msgs
  :super ros::object
  :slots (_header _segments _ranges ))

(defmethod segment_msgs::segment_msgs
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:segments __segments) (make-array 0 :initial-element 0 :element-type :integer))
    ((:ranges __ranges) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _header __header)
   (setq _segments __segments)
   (setq _ranges __ranges)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:segments
   (&optional __segments)
   (if __segments (setq _segments __segments)) _segments)
  (:ranges
   (&optional __ranges)
   (if __ranges (setq _ranges __ranges)) _ranges)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; int16[] _segments
    (* 2    (length _segments)) 4
    ;; float32[] _ranges
    (* 4    (length _ranges)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; int16[] _segments
     (write-long (length _segments) s)
     (dotimes (i (length _segments))
       (write-word (elt _segments i) s)
       )
     ;; float32[] _ranges
     (write-long (length _ranges) s)
     (dotimes (i (length _ranges))
       (sys::poke (elt _ranges i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; int16[] _segments
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _segments (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _segments i) (sys::peek buf ptr- :short)) (incf ptr- 2)
     ))
   ;; float32[] _ranges
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _ranges (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _ranges i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get segment_msgs::segment_msgs :md5sum-) "ea86f91234554cf339ed28ced396d040")
(setf (get segment_msgs::segment_msgs :datatype-) "segment_msgs/segment_msgs")
(setf (get segment_msgs::segment_msgs :definition-)
      "Header header
int16[] segments
float32[] ranges

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



(provide :segment_msgs/segment_msgs "ea86f91234554cf339ed28ced396d040")


