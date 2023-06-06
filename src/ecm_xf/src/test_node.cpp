#include <ros/ros.h>
#include <signal.h>
#include "ecm_bridge/driverCmd.h"
#include "ecm_bridge/driverState.h"
ros::Publisher pub;
static std::mutex robot_cmd_mutex;
#include <thread>
#define EC_PACKED(Bytes) __attribute__((aligned(Bytes), packed))
#define dNum 4
#define ENCODER_17BIT 20860.76

typedef struct
{
    uint8_t enable;
    uint8_t opMode;
    double kp;
    double kd;
    double q;
    double qd;
    double cur;
} EC_PACKED(1) driver_cmd;

typedef struct
{
    uint16_t sw;
    uint16_t error;
    uint8_t opMode;
    uint8_t complement;
    double q;
    double qd;
    double cur;
    double dc_v;
} EC_PACKED(1) driver_state;

driver_cmd driverCMD[dNum];
driver_state driver_state_msg[dNum];

void stateCallback(const ecm_bridge::driverState::ConstPtr &msg)
{

    std::lock_guard<std::mutex> lock(robot_cmd_mutex);
    for (int i = 0; i < dNum; i++)
    {
        driver_state_msg[i].sw = msg->sw[i];
        driver_state_msg[i].error = msg->error[i];
        driver_state_msg[i].opMode = msg->opMode[i];
        driver_state_msg[i].q = msg->q[i];
        driver_state_msg[i].qd = msg->qd[i];
        driver_state_msg[i].cur = msg->cur[i];
        driver_state_msg[i].dc_v = msg->dc_v[i];
    }
    // printf("get cmd\n");
}




static void sig_handler(int signo)
{
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
        pub.publish(txmsg);
	
	usleep(10000);
	ros::shutdown();
	exit(0);
}

int main(int argc, char *argv[])
{
    cpu_set_t mask;
    CPU_ZERO(&mask);//初始化CPU集合，将cpuset置空
    CPU_SET(0, &mask);//将本进程绑定到CPU0上

    ros::init(argc, argv, "ecm_xf_test_node");

    ros::NodeHandle nh;
    //订阅ecm_bridge::driverState
    ros::Subscriber cmd_sub = nh.subscribe<ecm_bridge::driverState>("driver_state", 10, stateCallback);
    //发布ecm_bridge::driverCmd
    pub = nh.advertise<ecm_bridge::driverCmd>("driver_cmd", 1);

    //当按下ctrl+c中断程序，sigint被捕获调用sig_handler程序终止
    signal(SIGINT, sig_handler); 
    ros::Rate loop_rate(100);
    float time = 0;
    while (ros::ok())
    {
        std::lock_guard<std::mutex> lock(robot_cmd_mutex);
        //消息数据为数组类型，需要每次重新设置长度
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
            txmsg.enable[i]=1;
            txmsg.qd[i]=5.0*sin(time);     //  set velocity here,  unit:  rad/s
        }
        time+=0.01;
        pub.publish(txmsg);
        ros::spinOnce();
         loop_rate.sleep();
    }
}



