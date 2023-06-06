/*
 * TTY testing utility (using tty driver)
 *
 * Copyright (C) 2020 WCH Corporation.
 * Author: TECH39 <zhangj@wch.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I /path/to/cross-kernel/include
 *
 * Update Log:
 * V1.0 - initial version
 * V1.1 - added hardflow control
 *		- added sendbreak
 *		- added uart to file function
 *		- VTIME and VMIN changed
 * V1.2 - added custom baud rates supports
 * V1.3 - fixed get speed parameter in getopt_long
 *		- added file send operation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <getopt.h>
#include <linux/serial.h>
#define termios asmtermios
#include <asm/termios.h>
#undef termios
#include <termios.h>

#include <time.h>
#include <sys/mman.h>
#include <limits.h>

#include <pthread.h>
#include "crc16.h"
#include "udptools.h"
sockfdc fds;
extern "C" int ioctl(int d, int request, ...);

static const char *device = "/dev/ttyACM1";
static int speed = 9600;
static int hardflow = 0;
static int verbose = 0;
static FILE *fp;

static const struct option lopts[] = {
	{"device", required_argument, 0, 'D'},
	{"speed", optional_argument, 0, 'S'},
	{"verbose", optional_argument, 0, 'v'},
	{"hardflow", required_argument, 0, 'f'},
	{NULL, 0, 0, 0},
};

static void print_usage(const char *prog)
{
	printf("Usage: %s [-DSvf]\n", prog);
	puts("  -D --device    tty device to use\n"
		 "  -S --speed     uart speed\n"
		 "  -v --verbose   Verbose (show rx buffer)\n"
		 "  -f --hardflow  open hardware flowcontrol\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	int c;

	while (1)
	{
		c = getopt_long(argc, argv, "D:S:vfh", lopts, NULL);
		if (c == -1)
		{
			break;
		}
		switch (c)
		{
		case 'D':
			if (optarg != NULL)
				device = optarg;
			break;
		case 'S':
			if (optarg != NULL)
				speed = atoi(optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		case 'f':
			hardflow = 1;
			break;
		case 'h':
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

/**
 * libtty_setcustombaudrate - set baud rate of tty device
 * @fd: device handle
 * @speed: baud rate to set
 *
 * The function return 0 if success, or -1 if fail.
 */
static int libtty_setcustombaudrate(int fd, int baudrate)
{
	struct termios2 tio;

	if (ioctl(fd, TCGETS2, &tio))
	{
		perror("TCGETS2");
		return -1;
	}

	tio.c_cflag &= ~CBAUD;
	tio.c_cflag |= BOTHER;
	tio.c_ispeed = baudrate;
	tio.c_ospeed = baudrate;

	if (ioctl(fd, TCSETS2, &tio))
	{
		perror("TCSETS2");
		return -1;
	}

	if (ioctl(fd, TCGETS2, &tio))
	{
		perror("TCGETS2");
		return -1;
	}

	return 0;
}

/**
 * libtty_setopt - config tty device
 * @fd: device handle
 * @speed: baud rate to set
 * @databits: data bits to set
 * @stopbits: stop bits to set
 * @parity: parity to set
 * @hardflow: hardflow to set
 *
 * The function return 0 if success, or -1 if fail.
 */
static int libtty_setopt(int fd, int speed, int databits, int stopbits, char parity, char hardflow)
{
	struct termios newtio;
	struct termios oldtio;
	int i;

	bzero(&newtio, sizeof(newtio));
	bzero(&oldtio, sizeof(oldtio));

	if (tcgetattr(fd, &oldtio) != 0)
	{
		perror("tcgetattr");
		return -1;
	}
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	/* set data bits */
	switch (databits)
	{
	case 5:
		newtio.c_cflag |= CS5;
		break;
	case 6:
		newtio.c_cflag |= CS6;
		break;
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr, "unsupported data size\n");
		return -1;
	}

	/* set parity */
	switch (parity)
	{
	case 'n':
	case 'N':
		newtio.c_cflag &= ~PARENB; /* Clear parity enable */
		newtio.c_iflag &= ~INPCK;  /* Disable input parity check */
		break;
	case 'o':
	case 'O':
		newtio.c_cflag |= (PARODD | PARENB); /* Odd parity instead of even */
		newtio.c_iflag |= INPCK;			 /* Enable input parity check */
		break;
	case 'e':
	case 'E':
		newtio.c_cflag |= PARENB;  /* Enable parity */
		newtio.c_cflag &= ~PARODD; /* Even parity instead of odd */
		newtio.c_iflag |= INPCK;   /* Enable input parity check */
		break;
	default:
		fprintf(stderr, "unsupported parity\n");
		return -1;
	}

	/* set stop bits */
	switch (stopbits)
	{
	case 1:
		newtio.c_cflag &= ~CSTOPB;
		break;
	case 2:
		newtio.c_cflag |= CSTOPB;
		break;
	default:
		perror("unsupported stop bits\n");
		return -1;
	}

	if (hardflow)
		newtio.c_cflag |= CRTSCTS;
	else
		newtio.c_cflag &= ~CRTSCTS;

	newtio.c_cc[VTIME] = 10; /* Time-out value (tenths of a second) [!ICANON]. */
	newtio.c_cc[VMIN] = 0;	 /* Minimum number of bytes read at once [!ICANON]. */

	tcflush(fd, TCIOFLUSH);

	if (tcsetattr(fd, TCSANOW, &newtio) != 0)
	{
		perror("tcsetattr");
		return -1;
	}

	/* set tty speed */
	if (libtty_setcustombaudrate(fd, speed) != 0)
	{
		perror("setbaudrate");
		return -1;
	}

	return 0;
}

/**
 * libtty_open - open tty device
 * @devname: the device name to open
 *
 * In this demo device is opened blocked, you could modify it at will.
 */
static int libtty_open(const char *devname)
{
	int fd = open(devname, O_RDWR | O_NOCTTY | O_NDELAY);
	int flags = 0;

	if (fd < 0)
	{
		perror("open device failed");
		return -1;
	}

	flags = fcntl(fd, F_GETFL, 0);
	flags &= ~O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0)
	{
		printf("fcntl failed.\n");
		return -1;
	}

	if (isatty(fd) == 0)
	{
		printf("not tty device.\n");
		return -1;
	}
	else
		printf("tty device test ok.\n");

	return fd;
}

/**
 * libtty_close - close tty device
 * @fd: the device handle
 *
 * The function return 0 if success, others if fail.
 */
static int libtty_close(int fd)
{
	return close(fd);
}

/**
 * libtty_tiocmset - modem set
 * @fd: file descriptor of tty device
 * @bDTR: 0 on inactive, other on DTR active
 * @bRTS: 0 on inactive, other on RTS active
 *
 * The function return 0 if success, others if fail.
 */
static int libtty_tiocmset(int fd, char bDTR, char bRTS)
{
	unsigned long controlbits = 0;

	if (bDTR)
		controlbits |= TIOCM_DTR;
	if (bRTS)
		controlbits |= TIOCM_RTS;

	return ioctl(fd, TIOCMSET, &controlbits);
}

/**
 * libtty_tiocmget - modem get
 * @fd: file descriptor of tty device
 * @modembits: pointer to modem status
 *
 * The function return 0 if success, others if fail.
 */
static int libtty_tiocmget(int fd, unsigned long *modembits)
{
	int ret;

	ret = ioctl(fd, TIOCMGET, modembits);
	if (ret == 0)
	{
		if (*modembits & TIOCM_DSR)
			printf("DSR Active!\n");
		if (*modembits & TIOCM_CTS)
			printf("CTS Active!\n");
		if (*modembits & TIOCM_CD)
			printf("DCD Active!\n");
		if (*modembits & TIOCM_RI)
			printf("RI Active!\n");
	}

	return ret;
}

/**
 * libtty_tiocmwait - wiat for modem signal to changed
 * @fd: file descriptor of tty device
 *
 * The function return 0 if success, others if fail.
 */
static int libtty_tiocmwait(int fd)
{
	unsigned long modembits = TIOCM_DSR | TIOCM_CTS | TIOCM_CD | TIOCM_RI;

	return ioctl(fd, TIOCMIWAIT, modembits);
}

/**
 * libtty_sendbreak - uart send break
 * @fd: file descriptor of tty device
 *
 * Description:
 *  tcsendbreak() transmits a continuous stream of zero-valued bits for a specific duration, if the  termi�\
 *	nal is using asynchronous serial data transmission.  If duration is zero, it transmits zero-valued bits
 *	for at least 0.25 seconds, and not more that 0.5 seconds.  If duration is not zero, it sends  zero-val�\
 *	ued bits for some implementation-defined length of time.
 *
 *  If  the terminal is not using asynchronous serial data transmission, tcsendbreak() returns without tak�\
 *	ing any action.
 */
static int libtty_sendbreak(int fd)
{
	return tcsendbreak(fd, 0);
}

static uint16_t getCW2(int32_t nSevoOn, uint16_t u16Sate, uint16_t *u16Ctrol)
{

	if (u16Sate & 0x08)
	{
		if (*u16Ctrol == 0x80)
			*u16Ctrol = 0;
		else
			*u16Ctrol = 0x80;
		return 0;
	}
	if (nSevoOn == 1)
	{
		switch (u16Sate & 0x6F)
		{
		case 0x40:
			*u16Ctrol = 0x06;
			break;
		case 0x21:
			*u16Ctrol = 0x07;
			break;
		case 0x23:
			*u16Ctrol = 0x0F;
			break;
		case 0x27:
			*u16Ctrol = 0x0F;
			// nret = 1;
			break;
		default:
			*u16Ctrol = 0x06;
			break;
		}
	}
	else
	{
		*u16Ctrol = 0x00;
	}
}
#include <ros/ros.h>
#include "ecm_bridge/driverCmd.h"
#include "ecm_bridge/driverState.h"
ros::Publisher pub;
static std::mutex robot_cmd_mutex;
#include <thread>
#define EC_PACKED(Bytes) __attribute__((aligned(Bytes), packed))
#define dNum TEST_DRV_CNT
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

int fd;
TXPDO_ST_DEF_T txBuffer; // in pdo   driver state
RXPDO_ST_DEF_T rxBuffer; // out pdo   driver cmd
int off = 0;

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
		// printf("driverCMD[%d]:\n", i);
		// printf("  enable: %d\n", driverCMD[i].enable);
		// printf("  opMode: %d\n", driverCMD[i].opMode);
		// printf("  kp: %f\n", driverCMD[i].kp);
		// printf("  kd: %f\n", driverCMD[i].kd);
		// printf("  q: %f\n", driverCMD[i].q);
		// printf("  qd: %f\n", driverCMD[i].qd);
		// printf("  cur: %f\n", driverCMD[i].cur);
	}
	//   printf("get cmd\n");
}

void *rxFunction(void *)
{

	ros::NodeHandle nh;

	// ros::Subscriber joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 10, joyCallback);
	ros::Subscriber cmd_sub = nh.subscribe<ecm_bridge::driverCmd>("driver_cmd", 10, cmdCallback);
	pub = nh.advertise<ecm_bridge::driverState>("driver_state", 1);

	int nwrite, nread, rnet;
	char buf[4096];
	ros::Rate rate(100);
	while (ros::ok() && off != 1)
	{

		int i;
		// printf("read nread  \n");
		nread = read(fd, buf, sizeof(buf));
		if (nread >= 0)
		{
			// printf("read nread %d bytes. ", nread);
		}
		else
		{
			printf("read error: %d\n", nread);
		}

		rnet = unpack((uint8_t *)buf, (uint8_t *)&txBuffer, sizeof(TXPDO_ST_DEF_T));

		if (rnet != 0)
		{
			printf("******  unpack error   %d     *******\n", rnet);
		}

		if (rnet == 0) // receve and unpack ok, write cmd
		{
			memset(&rxBuffer, 0, sizeof(RXPDO_ST_DEF_T));
			memset(&buf, 0, sizeof(buf));

			std::unique_lock<std::mutex> lock(robot_cmd_mutex);
			for (size_t i = 0; i < TEST_DRV_CNT; i++)
			{
				rxBuffer.Axis[i].opMode = 9;
				getCW2(driverCMD[i].enable, txBuffer.Axis[i].u16StaWord, &rxBuffer.Axis[i].u16CtlWord);
				rxBuffer.Axis[i].tVel = (int32_t)(driverCMD[i].qd * 8.0 * ENCODER_17BIT);
			}
			lock.unlock();

			rnet = pack((uint8_t *)buf, (uint8_t *)&rxBuffer, sizeof(RXPDO_ST_DEF_T));
			nwrite = write(fd, buf, sizeof(RXPDO_ST_DEF_T) + 8);
			if (nwrite < 0)
			{
				printf("******  write error   %d     *******\n", rnet);
			}

			ecm_bridge::driverState txmsg;
			for (int i = 0; i < dNum; i++)
			{
				txmsg.sw.push_back(txBuffer.Axis[i].u16StaWord);
				txmsg.error.push_back(txBuffer.Axis[i].u16StaWord);
				txmsg.opMode.push_back(txBuffer.Axis[i].opMode);
				txmsg.complement.push_back(txBuffer.Axis[i].complement);
				txmsg.q.push_back(txBuffer.Axis[i].Pos / ENCODER_17BIT / 8);
				txmsg.qd.push_back(txBuffer.Axis[i].vel / ENCODER_17BIT / 8);
				txmsg.cur.push_back(txBuffer.Axis[i].Torque / 1000.0 * 12);
				txmsg.dc_v.push_back(txBuffer.Axis[i].Vdc / 1000.0);
			}
			pub.publish(txmsg);
		}

		ros::spinOnce();
		updSenDuration(&fds);
		//  rate.sleep(); 
	}
	for (size_t i = 0; i < TEST_DRV_CNT; i++)
	{
		rxBuffer.Axis[i].opMode = 0;
		rxBuffer.Axis[i].tVel = 0;
		rxBuffer.Axis[i].u16CtlWord = 0;
	}

	rnet = pack((uint8_t *)buf, (uint8_t *)&rxBuffer, sizeof(RXPDO_ST_DEF_T));
	nwrite = write(fd, buf, sizeof(RXPDO_ST_DEF_T) + 8);
}

static void sig_handler(int signo)
{
	printf("capture sign no:%d\n", signo);
	off = 1;

	usleep(10000);
	if (fp != NULL)
	{

		fflush(fp);
		fsync(fileno(fp));
		fclose(fp);
	}
	ros::shutdown();

	exit(0);
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "ecm_xf_node");

	usleep(1000);
	int ret;
	char c;
	unsigned long modemstatus;

	// parse_opts(argc, argv);

	signal(SIGINT, sig_handler);
	updinit(&fds, "10.16.92.142", 8889);
	fd = libtty_open(device);
	if (fd < 0)
	{
		printf("libtty_open: %s error.\n", device);
		exit(0);
	}

	ret = libtty_setopt(fd, speed, 8, 1, 'n', hardflow);
	if (ret != 0)
	{
		printf("libtty_setopt error.\n");
		exit(0);
	}

	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);

	int result = sched_setaffinity(0, sizeof(mask), &mask);

	struct sched_param param;
	pthread_attr_t attr;
	pthread_t thread;
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
	ret = pthread_create(&thread, &attr, rxFunction, NULL);
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

	ret = libtty_close(fd);
	if (ret != 0)
	{
		printf("libtty_close error.\n");
		exit(0);
	}
}

