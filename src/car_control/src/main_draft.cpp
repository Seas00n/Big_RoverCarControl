#include <iostream>
#include <math.h>
#include "spine/socketcan.h"
#include "spine/dynamixel_motor.h"
#include "spine/minicheetah_motor.h"
#include "msg_.hpp"
#include "car_control/rover.h"
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

std::vector<SteeringWheelTypeDef> actuators;
RoverTypeDef rover_temp;

bool is_thread_ok = true;


enum CAR_STATE{GO_AHEAD=0,CHANGE_TO_TURN=1,TURN=2,CHANGE_TO_GO=3};



#define AKMOTOR_ON false



void canRxThread(SocketCan *sc){
  sc->clearRxCallback();
  struct can_frame frame{};
  while(is_thread_ok){
    *sc >> frame;
    if(depackMsg(frame,actuators)){
    }else{
      is_thread_ok = false;
      return;
    }
    thread_sleep(5);
  }
}

void cmdTxThread(AK10_9Motor* wheel_Motor, DynamixelMotor* steering_Motor){
  while(is_thread_ok){
      for(int i=0;i<4;i++){
        steering_Motor[i].setQDes(actuators[i].steer_p_desired);
        steering_Motor[i].control();
        thread_sleep(5);
        wheel_Motor[i].setQdDes(actuators[i].wheel_v_desired);
        wheel_Motor[i].setKpKd(0,4);
        wheel_Motor[i].control();
        thread_sleep(5);
      }
    }
}


void cmdTxThread_OnlySteering(DynamixelMotor* steering_Motor){
    while(is_thread_ok){
      for(int i=0;i<4;i++){
        steering_Motor[i].setQDes(actuators[i].steer_p_desired);
        steering_Motor[i].control();
        std::cout<<"steering Motor ["<<i<<"]="<<actuators[i].steer_p_desired<<std::endl;
        std::cout<<"wheel_Motor ["<<i<<"]="<<actuators[i].wheel_v_desired<<std::endl;
        thread_sleep(5);
      }
    }
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
        actuators[i].wheel_v_desired = rover_temp.rover_v;
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
    actuators[0].steer_p_desired = 45.0;
    actuators[1].steer_p_desired = 135.0;
    actuators[2].steer_p_desired = -45.0;
    actuators[3].steer_p_desired = -135.0;
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
    actuators[0].steer_p_desired = 45.0;
    actuators[1].steer_p_desired = 135.0;
    actuators[2].steer_p_desired = -45.0;
    actuators[3].steer_p_desired = -135.0;
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
  // DynamixelMotor steering_Motor[4];
  AK10_9Motor wheel_Motor[4];
  SocketCan can;
  std::cout<<"Steering Motor Ready"<<std::endl;
  if(AKMOTOR_ON){
    can.open("can0",1e6);
    for(int i=0;i<4;i++){
      wheel_Motor[i].attach(i+1,&can);
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  }
  else{
    std::cout<<"Don't use wheel"<<std::endl;
  }





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



  //InitialDebugTurtle(nh);

  if(AKMOTOR_ON){
    std::thread cmdTx(cmdTxThread, wheel_Motor, steering_Motor);
    cmdTx.detach();
    std::thread canRx(canRxThread, &can);
    canRx.detach();
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