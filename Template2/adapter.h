#ifndef __ADAPTER_H__
#define __ADAPTER_H__

#include <gio/gio.h>

#ifdef __cplusplus
 extern "C" {
#endif
void adapter_discoverable_enable(GDBusConnection *conn);
void adapter_discoverable_disable(GDBusConnection *conn);
void adapter_power_on(GDBusConnection *conn);
void adapter_power_off(GDBusConnection *conn);

int adapter_power_state(GDBusConnection *conn);
int adapter_discoverable_state(GDBusConnection *conn);

#ifdef __cplusplus
}
#endif

#endif



