#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>
#include <sys/mman.h>
#include <limits.h>
#include <math.h>

#include <signal.h>
// #include <realtime_tools/realtime_publisher.h>

#include <ecm_bridge/driverCmd.h>
#include <ecm_bridge/driverState.h>

ros::Publisher pub;

int sockfd;
struct sockaddr_in saddr;

#include "shadesmar.h"
static std::mutex robot_cmd_mutex;
#define EC_PACKED(Bytes) __attribute__((aligned(Bytes), packed))
#define dNum 3
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

driver_cmd driver_cmd_msg[dNum];
driver_state driver_state_msg[dNum];

struct timespec time_start = {0, 0}, time_end = {0, 0};
long long int duration, sum = 0;
long long int cnt = 0, min = 500000, max = 0, avg = 0;

int tx = 0;

driver_cmd driverCMD[3];
shm::pubsub::Publisher p("driver_cmd");

void joyCallback(const sensor_msgs::Joy::ConstPtr &joy)
{

    // std::lock_guard<std::mutex> lock(robot_cmd_mutex);
    // for (auto &cmd : driverCMD)
    // {
    //     cmd.enable = 0 ;
    //     if(joy->buttons[0] == 1){
    //         cmd.enable = joy->buttons[0];
    //         cmd.opMode = 9;
    //         cmd.qd = joy->axes[1] * 30;
    //     }

    //     if(joy->buttons[1] == 1){
    //         cmd.enable = joy->buttons[1];
    //         cmd.opMode = 11;
    //         cmd.kp = 0;
    //         cmd.kd = 10;
    //         cmd.qd = joy->axes[1] * 20;
    //     }

    // }
    // lock.unlock();
    // printf("get joy\n");
}

void cmdCallback(const ecm_bridge::driverCmd::ConstPtr &msg)
{

    std::lock_guard<std::mutex> lock(robot_cmd_mutex);
    for (int i = 0; i < dNum; i++)
    {
        driverCMD[i].enable = msg->enable[i];
        driverCMD[i].opMode = msg->opMode[i];
        driverCMD[i].kp = msg->kp[i];
        driverCMD[i].kd = msg->kd[i];
        driverCMD[i].q = msg->q[i];
        driverCMD[i].qd = msg->qd[i];
        driverCMD[i].cur = msg->cur[i];
    }
   printf("get cmd\n");
}


float data[3];
void ipcSubCB(shm::memory::Memblock *msg)
{

    memcpy(driver_state_msg, msg->ptr, sizeof(driver_state_msg));

    data[0] = (float)driver_state_msg[0].qd;
    clock_gettime(CLOCK_MONOTONIC, &time_end);
    duration = (time_end.tv_sec - time_start.tv_sec) * (int)1e9 + (time_end.tv_nsec - time_start.tv_nsec);

    cnt++;
    if (duration < min)
    {
        min = duration;
    }
    if (duration > max)
    {
        max = duration;
    }
    sum = sum + duration;
    avg = sum / cnt;
    time_start = time_end;
    data[1] = (float)duration;

    std::unique_lock<std::mutex> lock(robot_cmd_mutex);
    p.publish((void *)driverCMD, sizeof(driverCMD));
    lock.unlock();

    ecm_bridge::driverState txmsg;
    for (const auto &state : driver_state_msg)
    {
        txmsg.sw.push_back(state.sw);
        txmsg.error.push_back(state.error);
        txmsg.opMode.push_back(state.opMode);
        txmsg.complement.push_back(state.complement);
        txmsg.q.push_back(state.q);
        txmsg.qd.push_back(state.qd);
        txmsg.cur.push_back(state.cur);
        txmsg.dc_v.push_back(state.dc_v);
    }
    pub.publish(txmsg);
    uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};
    memcpy(&data[2], tail,4);
    //????????????????????????????
    sendto(sockfd, data, sizeof(data), 0, (struct sockaddr *)&saddr, sizeof(saddr));
    

// printf("get shm\n");
    tx = 1;
}

void *ipcSubTask(void *)
{

    shm::pubsub::Subscriber sub("driver_state", ipcSubCB);

    while (ros::ok())
    {
        sub.spin_once();

        usleep(1);
    }
}

void rosHandle()
{
    ros::NodeHandle nh;

    // ros::Subscriber joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 10, joyCallback);
    ros::Subscriber cmd_sub = nh.subscribe<ecm_bridge::driverCmd>("driver_cmd", 10, cmdCallback);
    pub = nh.advertise<ecm_bridge::driverState>("driver_state", 4);
    while (ros::ok())
    {
        usleep(1);
        ros::spinOnce();
    }
}

void mySigintHandler(int sig)
{
  // Do some custom action.
  // For example, publish a stop message to some other nodes.
  
  // All the default sigint handler does is call shutdown()
  printf("quit my_xbox_controller_node !!!\n");
  ros::shutdown();
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "my_xbox_controller_node");

    std::thread th1(rosHandle);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sockfd != -1);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    saddr.sin_addr.s_addr = inet_addr("10.16.98.27");

    struct sched_param param;
    pthread_attr_t attr;
    pthread_t thread;
    int ret;
 signal(SIGINT, mySigintHandler);
    /* Lock memory */
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1)
    {
        printf("mlockall failed: %m\n");
        exit(-2);
    }

    /* Initialize pthread attributes (default values) */
    ret = pthread_attr_init(&attr);
    if (ret)
    {
        printf("init pthread attributes failed\n");
        goto out;
    }

    /* Set a specific stack size  */
    ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN * 10);
    if (ret)
    {
        printf("pthread setstacksize failed\n");
        goto out;
    }

    /* Set scheduler policy and priority of pthread */
    ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if (ret)
    {
        printf("pthread setschedpolicy failed\n");
        goto out;
    }
    param.sched_priority = 99;
    ret = pthread_attr_setschedparam(&attr, &param);
    if (ret)
    {
        printf("pthread setschedparam failed\n");
        goto out;
    }
    /* Use scheduling parameters of attr */
    ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (ret)
    {
        printf("pthread setinheritsched failed\n");
        goto out;
    }

    /* Create a pthread with specified attributes */
    ret = pthread_create(&thread, &attr, ipcSubTask, NULL);
    if (ret)
    {
        printf("create pthread failed\n");
        goto out;
    }

    /* Join the thread and wait until it is done */
    ret = pthread_join(thread, NULL);
    if (ret)
        printf("join pthread failed: %m\n");


out:
    return ret;
}