;; Auto-generated. Do not edit!


(when (boundp 'ecm_bridge::driverCmd)
  (if (not (find-package "ECM_BRIDGE"))
    (make-package "ECM_BRIDGE"))
  (shadow 'driverCmd (find-package "ECM_BRIDGE")))
(unless (find-package "ECM_BRIDGE::DRIVERCMD")
  (make-package "ECM_BRIDGE::DRIVERCMD"))

(in-package "ROS")
;;//! \htmlinclude driverCmd.msg.html


(defclass ecm_bridge::driverCmd
  :super ros::object
  :slots (_enable _opMode _kp _kd _q _qd _cur ))

(defmethod ecm_bridge::driverCmd
  (:init
   (&key
    ((:enable __enable) (make-array 0 :initial-element 0 :element-type :char))
    ((:opMode __opMode) (make-array 0 :initial-element 0 :element-type :char))
    ((:kp __kp) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:kd __kd) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:q __q) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:qd __qd) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:cur __cur) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _enable __enable)
   (setq _opMode __opMode)
   (setq _kp __kp)
   (setq _kd __kd)
   (setq _q __q)
   (setq _qd __qd)
   (setq _cur __cur)
   self)
  (:enable
   (&optional __enable)
   (if __enable (setq _enable __enable)) _enable)
  (:opMode
   (&optional __opMode)
   (if __opMode (setq _opMode __opMode)) _opMode)
  (:kp
   (&optional __kp)
   (if __kp (setq _kp __kp)) _kp)
  (:kd
   (&optional __kd)
   (if __kd (setq _kd __kd)) _kd)
  (:q
   (&optional __q)
   (if __q (setq _q __q)) _q)
  (:qd
   (&optional __qd)
   (if __qd (setq _qd __qd)) _qd)
  (:cur
   (&optional __cur)
   (if __cur (setq _cur __cur)) _cur)
  (:serialization-length
   ()
   (+
    ;; uint8[] _enable
    (* 1    (length _enable)) 4
    ;; uint8[] _opMode
    (* 1    (length _opMode)) 4
    ;; float64[] _kp
    (* 8    (length _kp)) 4
    ;; float64[] _kd
    (* 8    (length _kd)) 4
    ;; float64[] _q
    (* 8    (length _q)) 4
    ;; float64[] _qd
    (* 8    (length _qd)) 4
    ;; float64[] _cur
    (* 8    (length _cur)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8[] _enable
     (write-long (length _enable) s)
     (princ _enable s)
     ;; uint8[] _opMode
     (write-long (length _opMode) s)
     (princ _opMode s)
     ;; float64[] _kp
     (write-long (length _kp) s)
     (dotimes (i (length _kp))
       (sys::poke (elt _kp i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[] _kd
     (write-long (length _kd) s)
     (dotimes (i (length _kd))
       (sys::poke (elt _kd i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
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
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8[] _enable
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _enable (make-array n :element-type :char))
     (replace _enable buf :start2 ptr-) (incf ptr- n))
   ;; uint8[] _opMode
   (let ((n (sys::peek buf ptr- :integer))) (incf ptr- 4)
     (setq _opMode (make-array n :element-type :char))
     (replace _opMode buf :start2 ptr-) (incf ptr- n))
   ;; float64[] _kp
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _kp (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _kp i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
   ;; float64[] _kd
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _kd (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _kd i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     ))
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
   ;;
   self)
  )

(setf (get ecm_bridge::driverCmd :md5sum-) "4ac7643f368fa7335ff4aa8b3f789c47")
(setf (get ecm_bridge::driverCmd :datatype-) "ecm_bridge/driverCmd")
(setf (get ecm_bridge::driverCmd :definition-)
      "uint8[] enable
uint8[] opMode
float64[] kp
float64[] kd
float64[] q
float64[] qd
float64[] cur


")



(provide :ecm_bridge/driverCmd "4ac7643f368fa7335ff4aa8b3f789c47")


