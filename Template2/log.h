
#ifndef __LOG_H__
#define __LOG_H__
#include <string>

#ifdef __cplusplus
 extern "C" {
#endif

void u_tm_log(char *arg, ...);
void u_tm_log_hex(std::string info, unsigned char *buf, int count);

#ifdef __cplusplus
}
#endif

#endif

