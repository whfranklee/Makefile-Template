#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "log.h"
#include "uart_server.h"
#include "sys_utils.h"

/*
 * 将接收到的数据通过串口回显
 */
void uart_receive_func(uint8_t *buf, int len)
{
	u_tm_log_hex("uart rx: ", buf, len);
	//uart_server_send(buf, len);
}

void ble_task()
{
	time_t lastTm = 0;
	int count = 0;
	int val = 0;

	gpio_open(BUTTON_PIN);
	while(1) 
	{
		time_t nowTm = time(NULL);
		if (nowTm - lastTm >= 1)
		{
			lastTm = nowTm;
			val = gpio_read(BUTTON_PIN);

			if(val == 0)
			{
				count++;
			}
			else
			{
				count = 0;
			}
		}

		if(count >= 5)
		{
			uart_server_init(uart_receive_func);
		}

		msleep(500);
	}
}

int main(void)
{
	pid_t pid;

	pid = fork();
	if(pid < 0)
	{
		exit(1);
	}
	else if(pid == 0)
	{
		system("btattach -B /dev/ttyS1 -S 115200 -N");
	}
	else
	{
		ble_task();
	}

	return 0;
}
