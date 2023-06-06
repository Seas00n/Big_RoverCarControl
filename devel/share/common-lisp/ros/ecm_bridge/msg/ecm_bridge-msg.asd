
(cl:in-package :asdf)

(defsystem "ecm_bridge-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "driverCmd" :depends-on ("_package_driverCmd"))
    (:file "_package_driverCmd" :depends-on ("_package"))
    (:file "driverState" :depends-on ("_package_driverState"))
    (:file "_package_driverState" :depends-on ("_package"))
  ))