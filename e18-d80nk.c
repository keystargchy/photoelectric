#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>

#include "windef.h"
#include "gpio.h"

extern const int control_pin_num[CONTROL_PIN_NUM];


pthread_t ntid;
volatile BOOL m_BrakeFlag = FALSE;


void * handle_e18_d80nk(void *arg)
{
	int readFront = 1, readBehind = 1;
	while(1)
	{
		readFront = gpio_read(control_pin_num[TYPE_PE_FRONT]);
		readBehind = gpio_read(control_pin_num[TYPE_PE_BEHIND]);
		if(readFront == 0 || readBehind == 0){
			m_BrakeFlag = TRUE;
		}else{
			m_BrakeFlag = FALSE;
		}

		usleep(10*1000);
	}
}

void do_sig(int arg)
{
	printf("\nexit\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	int err = 0;

	if(signal(SIGINT, do_sig) == SIG_ERR)
	{
		perror("signal");
		return -1;
	}

	err = kr_gpio_init();
	if(err != 0)
		Err_log("GPIO Init fail...\n");
	
	err = pthread_create(&ntid, NULL, handle_e18_d80nk, NULL);
	if(err != 0)
	{
		Err_log("create thread fail...\n");
		exit(1);
	}

	printf("detect obstacles...\n");
	
	while(1)
	{
		if(m_BrakeFlag)
		{
			printf(FG_RED()"Have obstacles...\n"WD_NONE());
			continue;
		}

		printf(FG_GREEN()"Not have obstacles...\n"WD_NONE());
			
		while(1)
		{
			if(m_BrakeFlag) 
			{
				/* Have obstacles */
				printf(FG_RED()"Have obstacles, please stop...\n");
				printf("Have obstacles, please stop...\n");
				printf("Have obstacles, please stop...\n"WD_NONE());
				break;
			}
			usleep(20*1000);
		}
	}

	return 0;
}
