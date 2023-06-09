;; Auto-generated. Do not edit!


(when (boundp 'car_control::corner)
  (if (not (find-package "CAR_CONTROL"))
    (make-package "CAR_CONTROL"))
  (shadow 'corner (find-package "CAR_CONTROL")))
(unless (find-package "CAR_CONTROL::CORNER")
  (make-package "CAR_CONTROL::CORNER"))

(in-package "ROS")
;;//! \htmlinclude corner.msg.html


(defclass car_control::corner
  :super ros::object
  :slots (_wheel_pos_actual _wheel_pos_desired _wheel_vel_actual _wheel_vel_desired _steer_pos_actual _steer_pos_desired _steer_vel_actual _steer_vel_desired ))

(defmethod car_control::corner
  (:init
   (&key
    ((:wheel_pos_actual __wheel_pos_actual) 0.0)
    ((:wheel_pos_desired __wheel_pos_desired) 0.0)
    ((:wheel_vel_actual __wheel_vel_actual) 0.0)
    ((:wheel_vel_desired __wheel_vel_desired) 0.0)
    ((:steer_pos_actual __steer_pos_actual) 0.0)
    ((:steer_pos_desired __steer_pos_desired) 0.0)
    ((:steer_vel_actual __steer_vel_actual) 0.0)
    ((:steer_vel_desired __steer_vel_desired) 0.0)
    )
   (send-super :init)
   (setq _wheel_pos_actual (float __wheel_pos_actual))
   (setq _wheel_pos_desired (float __wheel_pos_desired))
   (setq _wheel_vel_actual (float __wheel_vel_actual))
   (setq _wheel_vel_desired (float __wheel_vel_desired))
   (setq _steer_pos_actual (float __steer_pos_actual))
   (setq _steer_pos_desired (float __steer_pos_desired))
   (setq _steer_vel_actual (float __steer_vel_actual))
   (setq _steer_vel_desired (float __steer_vel_desired))
   self)
  (:wheel_pos_actual
   (&optional __wheel_pos_actual)
   (if __wheel_pos_actual (setq _wheel_pos_actual __wheel_pos_actual)) _wheel_pos_actual)
  (:wheel_pos_desired
   (&optional __wheel_pos_desired)
   (if __wheel_pos_desired (setq _wheel_pos_desired __wheel_pos_desired)) _wheel_pos_desired)
  (:wheel_vel_actual
   (&optional __wheel_vel_actual)
   (if __wheel_vel_actual (setq _wheel_vel_actual __wheel_vel_actual)) _wheel_vel_actual)
  (:wheel_vel_desired
   (&optional __wheel_vel_desired)
   (if __wheel_vel_desired (setq _wheel_vel_desired __wheel_vel_desired)) _wheel_vel_desired)
  (:steer_pos_actual
   (&optional __steer_pos_actual)
   (if __steer_pos_actual (setq _steer_pos_actual __steer_pos_actual)) _steer_pos_actual)
  (:steer_pos_desired
   (&optional __steer_pos_desired)
   (if __steer_pos_desired (setq _steer_pos_desired __steer_pos_desired)) _steer_pos_desired)
  (:steer_vel_actual
   (&optional __steer_vel_actual)
   (if __steer_vel_actual (setq _steer_vel_actual __steer_vel_actual)) _steer_vel_actual)
  (:steer_vel_desired
   (&optional __steer_vel_desired)
   (if __steer_vel_desired (setq _steer_vel_desired __steer_vel_desired)) _steer_vel_desired)
  (:serialization-length
   ()
   (+
    ;; float64 _wheel_pos_actual
    8
    ;; float64 _wheel_pos_desired
    8
    ;; float64 _wheel_vel_actual
    8
    ;; float64 _wheel_vel_desired
    8
    ;; float64 _steer_pos_actual
    8
    ;; float64 _steer_pos_desired
    8
    ;; float64 _steer_vel_actual
    8
    ;; float64 _steer_vel_desired
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _wheel_pos_actual
       (sys::poke _wheel_pos_actual (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _wheel_pos_desired
       (sys::poke _wheel_pos_desired (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _wheel_vel_actual
       (sys::poke _wheel_vel_actual (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _wheel_vel_desired
       (sys::poke _wheel_vel_desired (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _steer_pos_actual
       (sys::poke _steer_pos_actual (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _steer_pos_desired
       (sys::poke _steer_pos_desired (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _steer_vel_actual
       (sys::poke _steer_vel_actual (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _steer_vel_desired
       (sys::poke _steer_vel_desired (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _wheel_pos_actual
     (setq _wheel_pos_actual (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _wheel_pos_desired
     (setq _wheel_pos_desired (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _wheel_vel_actual
     (setq _wheel_vel_actual (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _wheel_vel_desired
     (setq _wheel_vel_desired (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _steer_pos_actual
     (setq _steer_pos_actual (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _steer_pos_desired
     (setq _steer_pos_desired (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _steer_vel_actual
     (setq _steer_vel_actual (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _steer_vel_desired
     (setq _steer_vel_desired (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get car_control::corner :md5sum-) "f28300dc96da5592dc5c99f06bac2854")
(setf (get car_control::corner :datatype-) "car_control/corner")
(setf (get car_control::corner :definition-)
      "float64 wheel_pos_actual
float64 wheel_pos_desired
float64 wheel_vel_actual
float64 wheel_vel_desired
float64 steer_pos_actual
float64 steer_pos_desired
float64 steer_vel_actual
float64 steer_vel_desired
")



(provide :car_control/corner "f28300dc96da5592dc5c99f06bac2854")


