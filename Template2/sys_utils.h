#ifndef __SYS_UTILS_H__
#define __SYS_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define BUTTON_PIN 491
#define msleep(x) usleep((x)*1000)
#define array_num(a) ((sizeof(a)) / (sizeof(a[0])))

int check_shell_status(int rv);
int utils_system_ex(const char *cmd, char *recv, unsigned int max_size);
int gpio_read(int gpio);
void gpio_open(int gpio);
void gpio_close(int gpio);

#ifdef __cplusplus
}
#endif

#endif