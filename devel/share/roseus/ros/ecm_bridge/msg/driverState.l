;; Auto-generated. Do not edit!


(when (boundp 'ecm_bridge::driverState)
  (if (not (find-package "ECM_BRIDGE"))
    (make-package "ECM_BRIDGE"))
  (shadow 'driverState (find-package "ECM_BRIDGE")))
(unless (find-package "ECM_BRIDGE::DRIVERSTATE")
  (make-package "ECM_BRIDGE::DRIVERSTATE"))

(in-package "ROS")
;;//! \htmlinclude driverState.msg.html


(defclass ecm_bridge::driverState
  :super ros::object
  :slots (_sw _error _opMode _complement _q _qd _cur _dc_v ))

(defmethod ecm_bridge::driverState
  (:init
   (&key
    ((:sw __sw) (make-array 0 :initial-element 0 :element-type :integer))
    ((:error __error) (make-array 0 :initial-element 0 :element-type :integer))
    ((:opMode __opMode) (make-array 0 :initial-element 0 :element-type :char))
    ((:complement __complement) (make-array 0 :initial-element 0 :element-type :char))
    ((:q __q) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:qd __qd) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:cur __cur) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:dc_v __dc_v) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _sw __sw)
   (setq _error __error)
   (setq _opMode __opMode)
   (setq _complement __complement)
   (setq _q __q)
   (setq _qd __qd)
   (setq _cur __cur)
   (setq _dc_v __dc_v)
   self)
  (:sw
   (&optional __sw)
   (if __sw (setq _sw __sw)) _sw)
  (:error
   (&optional __error)
   (if __error (setq _error __error)) _error)
  (:opMode
   (&optional __opMode)
   (if __opMode (setq _opMode __opMode)) _opMode)
  (:complement
   (&optional __complement)
   (if __complement (setq _complement __complement)) _complement)
  (:q
   (&optional __q)
   (if __q (setq _q __q)) _q)
  (:qd
   (&optional __qd)
   (if __qd (setq _qd __qd)) _qd)
  (:cur
   (&optional __cur)
   (if __cur (setq _cur __cur)) _cur)
  (:dc_v
   (&optional __dc_v)
   (if __dc_v (setq _dc_v __dc_v)) _dc_v)
  (:serialization-length
   ()
   (+
    ;; uint16[] _sw
    (* 2    (length _sw)) 4
    ;; uint16[] _error
    (* 2    (length _error)) 4
    ;; uint8[] _opMode
    (* 1    (length _opMode)) 4
    ;; uint8[] _complement
    (* 1    (length _complement)) 4
    ;; float64[] _q
    (* 8    (length _q)) 4
    ;; float64[] _qd
    (* 8    (length _qd)) 4
    ;; float64[] _cur
    (* 8    (length _cur)) 4
    ;; float64[] _dc_v
    (* 8    (length _dc_v)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint16[] _sw
     (write-long (length _sw) s)
     (dotimes (i (length _sw))
       (write-word (elt _sw i) s)
       )
     ;; uint16[] _error
     (write-long (length _error) s)
     (dotimes (i (length _error))
       (write-word (elt _error i) s)
       )
     ;; uint8[] _opMode
     (write-long (length _opMode) s)
     (princ _opMode s)
     ;; uint8[] _complement
     (write-long (length _complement) s)
     (princ _complement s)
     ;; float64[] _q
     (write-long (length _q) s)
     (dotimes (i (length _q))
       (sys::poke (elt _q i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[] _qd
     (write-long (length _qd) s)
     (dotimes (i (length _qd))
       (sys::poke (elt _qd i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[] _cur
     (write-long (length _cur) s)
     (dotimes (i (length _cur))
       (sys::poke (elt _cur i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[] _dc_v
     (write-long (length _dc_v) s)
     (dotimes (i (length _dc_v))
       (sys::poke (elt _dc_v i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint16[] _sw
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _sw (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _sw i) (sys::peek buf ptr- :short)) (incf ptr- 2)
     ))
   ;; uint16[] _error
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _error (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _error i) (sys::peek buf ptr- :short)) (incf ptr- 2)
     ))
   ;; uint8[] _opMode
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _opMode (make-array n :element-type :char))
     (replace _opMode buf :start2 ptr-) (incf ptr- n))
   ;; uint8[] _complement
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _complement (make-array n :element-type :char))
     (replace _complement buf :start2 ptr-) (incf ptr- n))
   ;; float64[] _q
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _q (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _q i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64[] _qd
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _qd (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _qd i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64[] _cur
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _cur (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _cur i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64[] _dc_v
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _dc_v (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _dc_v i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;;
   self)
  )

(setf (get ecm_bridge::driverState :md5sum-) "2deea21f9bf25c298b3269d351909d41")
(setf (get ecm_bridge::driverState :datatype-) "ecm_bridge/driverState")
(setf (get ecm_bridge::driverState :definition-)
      "uint16[] sw
uint16[] error
uint8[] opMode
uint8[] complement
float64[] q
float64[] qd
float64[] cur
float64[] dc_v
")



(provide :ecm_bridge/driverState "2deea21f9bf25c298b3269d351909d41")


