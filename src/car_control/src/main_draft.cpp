#include <iostream>
#include <math.h>
#include "spine/socketcan.h"
#include "spine/dynamixel_motor.h"
#include "spine/minicheetah_motor.h"
#include "msg_.hpp"
#include "car_control/rover.h"
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ecm_bridge/driverCmd.h"
#include "ecm_bridge/driverState.h"
#include "ecm_.hpp"
#include "signal.h"


std::vector<SteeringWheelTypeDef> actuators;
RoverTypeDef rover_temp;

bool is_thread_ok = true;


enum CAR_STATE{GO_AHEAD=0,CHANGE_TO_TURN=1,TURN=2,CHANGE_TO_GO=3};


ros::Publisher ecm_pub;
driver_cmd driverCMD[4];
driver_state driver_state_msg[4];
static std::mutex robot_cmd_mutex;




#define AKMOTOR_ON true





void cmdTxThread(DynamixelMotor* steering_Motor){
  while(is_thread_ok){
    ecm_bridge::driverCmd txmsg;
    txmsg.enable.resize(dNum, 0);
    txmsg.opMode.resize(dNum, 0);
    txmsg.kp.resize(dNum, 0);
    txmsg.kd.resize(dNum, 0);
    txmsg.q.resize(dNum, 0);
    txmsg.qd.resize(dNum, 0);
    txmsg.cur.resize(dNum, 0);
    for(int i=0;i<4;i++){
      steering_Motor[i].setQDes(actuators[i].steer_p_desired);
      steering_Motor[i].control();
      txmsg.enable[i] = 1;
      txmsg.qd[i] = actuators[i].wheel_v_desired;  
      thread_sleep(3);
    }
    if(AKMOTOR_ON)
      ecm_pub.publish(txmsg);
  }
}


void cmdTxThread_OnlySteering(DynamixelMotor* steering_Motor){
    while(is_thread_ok){
      for(int i=0;i<4;i++){
        // steering_Motor[i].setQDes(actuators[i].steer_p_desired);
        // steering_Motor[i].control();
        std::cout<<"steering Motor ["<<i<<"]="<<actuators[i].steer_p_desired<<std::endl;
        std::cout<<"wheel_Motor ["<<i<<"]="<<actuators[i].wheel_v_desired<<std::endl;
        thread_sleep(5);
      }
    }
}


void ecm_driverStateCallback(const ecm_bridge::driverState::ConstPtr &msg){
  std::lock_guard<std::mutex> lock(robot_cmd_mutex);
  for(int i=0;i<4;i++){
    driver_state_msg[i].sw = msg->sw[i];
    driver_state_msg[i].error = msg->error[i];
    driver_state_msg[i].opMode = msg->opMode[i];
    driver_state_msg[i].q = msg->q[i];
    driver_state_msg[i].qd = msg->qd[i];
    driver_state_msg[i].cur = msg->cur[i];
    driver_state_msg[i].dc_v = msg->dc_v[i];
    actuators[i].wheel_p_actual = driver_state_msg[i].q;
    actuators[i].wheel_v_actual = driver_state_msg[i].qd;

  }
}


static void sig_handler(int signo){
  printf("capture sign no:%d\n",signo);
        ecm_bridge::driverCmd txmsg;
        txmsg.enable.resize(dNum, 0);
        txmsg.opMode.resize(dNum, 0);
        txmsg.kp.resize(dNum, 0);
        txmsg.kd.resize(dNum, 0);
        txmsg.q.resize(dNum, 0);
        txmsg.qd.resize(dNum, 0);
        txmsg.cur.resize(dNum, 0);
        for (int i = 0; i < dNum; i++)
        {
            txmsg.enable[i]=0;
            txmsg.qd[i]=0;
        }
        if(AKMOTOR_ON)
          ecm_pub.publish(txmsg);
	
	usleep(10000);
  is_thread_ok = false;
	ros::shutdown();
	exit(0);
}



void SingleBikeControl(const car_control::rover::ConstPtr& p, RoverTypeDef& rover_temp,
                std::vector<SteeringWheelTypeDef>& actuators){
    volatile int current_state = rover_temp.rover_motion_state;
    if(current_state==0){
      //此处的p->
      double vf = p->rover_vx;
      double delta_f = p->rover_w;
      if(delta_f>45)
        delta_f = 45;
      else if (delta_f<-45)
        delta_f = -45;
      
      //设置后轮
      actuators[2].steer_p_desired = 0;
      actuators[3].steer_p_desired = 0;
      actuators[2].wheel_v_desired = vf;
      actuators[3].wheel_v_desired = vf;
      //设置前轮
      actuators[0].steer_p_desired = delta_f;
      actuators[1].steer_p_desired = delta_f;
      actuators[0].wheel_v_desired = vf;
      actuators[1].wheel_v_desired = vf;
      
      rover_temp.rover_v = vf;
      rover_temp.rover_w = vf*tan(delta_f*PI/180)/(2*wheel_to_center_x);

    }
}

void FSMControl(const car_control::rover::ConstPtr& p, RoverTypeDef& rover_temp,
                std::vector<SteeringWheelTypeDef>& actuators){
  volatile int current_state = rover_temp.rover_motion_state;
  if(current_state==0){
    std::cout<<"State GO_AHEAD"<<p->rover_vx<<std::endl;
    rover_temp.rover_v= p->rover_vx;
    rover_temp.rover_w = 0;
    for(int i=0;i<4;i++){
      actuators[i].steer_p_desired = 0;
      if(AKMOTOR_ON){
        if(i==0||i==2)
          actuators[i].wheel_v_desired = rover_temp.rover_v;
        else if(i==1||i==3){
          actuators[i].wheel_v_desired = -rover_temp.rover_v;
        }
      }
    }
    if((abs(p->rover_w)<0.01)&&abs(p->rover_vx<0.01)&&(p->rover_state==CHANGE_TO_TURN)){
      rover_temp.rover_motion_state = CHANGE_TO_TURN;
    }
    return;
  }
  if (current_state ==1){
    std::cout<<"State Change to Turn"<<std::endl;
    rover_temp.rover_v = 0;
    rover_temp.rover_w = 0;
    actuators[0].steer_p_desired = 90-diag_angle;
    actuators[1].steer_p_desired = 90+diag_angle;
    actuators[2].steer_p_desired = -90+diag_angle;
    actuators[3].steer_p_desired = -90-diag_angle;
    if(p->rover_state==TURN){
      rover_temp.rover_motion_state = TURN;
    }
    return;
  }
  if(current_state == 2){
    std::cout<<"State Turn"<<p->rover_w<<std::endl;
    rover_temp.rover_v = 0;
    rover_temp.rover_w = p->rover_w;
    if(AKMOTOR_ON){
      for(int i=0;i<4;i++){
        actuators[i].wheel_v_desired = rover_temp.rover_w*wheel_to_center;
      }
    }
    actuators[0].steer_p_desired = 90-diag_angle;
    actuators[1].steer_p_desired = 90+diag_angle;
    actuators[2].steer_p_desired = -90+diag_angle;
    actuators[3].steer_p_desired = -90-diag_angle;
    if(abs(p->rover_w)<0.01&&abs(p->rover_vx)<0.01&&p->rover_state==CHANGE_TO_GO){
      rover_temp.rover_motion_state = CHANGE_TO_GO;
    }
    return;
  }
  if(current_state == 3){
    std::cout<<"State Change to GO"<<std::endl;
    rover_temp.rover_v = 0;
    rover_temp.rover_w = 0;
    for(int i=0;i<4;i++){
      actuators[i].steer_p_desired = 0;
    }
    if(p->rover_state==GO_AHEAD){
      rover_temp.rover_motion_state = GO_AHEAD;
    }
    return;
  }
}




void SoftwareInitialization(std::vector<SteeringWheelTypeDef>& steeringwheels,
                            ros::NodeHandle& nh){
  for(int i=0;i<4;i++){
    std::string topic_name = "steerwheel"+i;
    steeringwheels.push_back(SteeringWheelTypeDef());
    steeringwheels[i].setPublisher(nh,topic_name);
  }
}


int main(int argc, char *argv[]){
  DynamixelMotor steering_Motor[4] = {
    DynamixelMotor(1,"/dev/ttyUSB0"),DynamixelMotor(2,"/dev/ttyUSB0"),
    DynamixelMotor(3,"/dev/ttyUSB0"),DynamixelMotor(4,"/dev/ttyUSB0")
  };
  //DynamixelMotor steering_Motor[4] = {};

  // DynamixelMotor steering_Motor[4];
  std::cout<<"Steering Motor Ready"<<std::endl;


  setlocale(LC_ALL, "");
  ros::init(argc, argv, "control");
  ros::NodeHandle nh;  
  ros::Rate loop_rate(200);
  ROS_INFO_STREAM("节点初始化完成");

  SoftwareInitialization(actuators,nh);

  //订阅手柄发布的无人车状态消息
  ros::Subscriber sub = nh.subscribe<car_control::rover>("rover_state",
                                                          1000,boost::bind(&FSMControl,_1,
                                                          boost::ref(rover_temp),boost::ref(actuators)));
  ros::Subscriber cmd_sub = nh.subscribe<ecm_bridge::driverState>("driver_state",10,ecm_driverStateCallback);
  ecm_pub = nh.advertise<ecm_bridge::driverCmd>("driver_cmd",1);

  signal(SIGINT, sig_handler);


  //InitialDebugTurtle(nh);

  if(AKMOTOR_ON){
    std::thread cmdTx(cmdTxThread,steering_Motor);
    cmdTx.detach(); 
  }else{
    std::thread cmdTx(cmdTxThread_OnlySteering, steering_Motor);
    cmdTx.detach();
  }


  while(ros::ok()&&is_thread_ok){
    ros::spinOnce();
    //DebugTurtleDynamics(rover_temp, actuators);
    for(int i=0;i<4;i++){
      actuators[i].MsgPublish();
    }
    loop_rate.sleep();
  }
  ros::shutdown();
  is_thread_ok = false;
  return 0;
}