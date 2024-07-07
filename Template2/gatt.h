#ifndef __GATT_H__
#define __GATT_H__


#include <gio/gio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <stdint.h>


#ifdef __cplusplus
 extern "C" {
#endif

#define CHAR_FLAGS_SIZE 17
#define UART_OBJECT_PATH "/org/uart/server"
typedef void (*uart_receive_t)(uint8_t *buf, int len);

struct char_t {
	char *UUID;
	char *Service;
	uint8_t Value[512];
	int len;
	/* @Flags:
	 *	"broadcast"
	 *	"read"
	 *	"write-without-response"
	 *	"write"
	 *	"notify"
	 *	"indicate"
	 *	"authenticated-signed-writes"
	 *	"extended-properties"
	 *	"reliable-write"
	 *	"writable-auxiliaries"
	 *	"encrypt-read"
	 *	"encrypt-write"
	 *	"encrypt-authenticated-read"
	 *	"encrypt-authenticated-write"
	 *	"secure-read" (Server only)
	 *	"secure-write" (Server only)
	 *	"authorize"
	 */
	char *Flags[CHAR_FLAGS_SIZE];
	/*
	 * True, if notifications or indications on this
	 * characteristic are currently enabled.
	 */
	int Notifying;
};



struct service_t {
	char *UUID;
	/*
	 * Indicates whether or not this GATT service is a
	 * primary service. If false, the service is secondary.
	 */
	int Primary;
};


struct server_t {
	struct {
		struct service_t service;

		struct char_t char0;
		struct char_t char1;
		struct char_t char2;
		struct char_t char3;
		struct char_t char4;
		struct char_t char5;
		struct char_t char6;
		struct char_t char7;
		struct char_t char8;
		struct char_t char9;
		struct char_t char10;
		struct char_t char11;
		struct char_t char12;

		//struct char_t rx_char;
		//struct char_t tx_char;
	} gatt;

	GDBusConnection *conn;
	GDBusNodeInfo *object_manager_node_info;
	guint object_manager_reg_id;

	GDBusNodeInfo *service_node_info;
	guint service_reg_id;

	GDBusNodeInfo *char_node_info;
	guint char0_reg_id;
	guint char1_reg_id;
	guint char2_reg_id;
	guint char3_reg_id;
	guint char4_reg_id;
	guint char5_reg_id;
	guint char6_reg_id;
	guint char7_reg_id;
	guint char8_reg_id;
	guint char9_reg_id;
	guint char10_reg_id;
	guint char11_reg_id;
	guint char12_reg_id;
	//guint tx_char_reg_id;
	//guint rx_char_reg_id;
	uart_receive_t receive_cb_func;
};

extern struct server_t server_ctx;

int gatt_uart_server_start(GDBusConnection *conn);
void gatt_uart_register_receive_cb(uart_receive_t receive_cb);
//void gatt_uart_send(uint8_t *buf, int len);

#ifdef __cplusplus
}
#endif

#endif




