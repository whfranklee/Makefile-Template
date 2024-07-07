/*
 * Copyright (C) 2021, 2021  huohongpeng
 * Author: huohongpeng <1045338804@qq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Change logs:
 * Date        Author       Notes
 * 2021-04-12  huohongpeng  初次创建
 *							1.蓝牙版本bluez5.54
 *							2.实现了uart的gatt service
 *							3.为了便于使用Nordic的APP(nRF Connect)调试,程序中的UUID使用的都是Nodic的uart的UUID
 */


#include "gatt.h"
#include "log.h"
#include "char_respond.h"

#define __DEBUG__


static const gchar object_manager_xml[] =
"<node>"
"  <interface name='org.freedesktop.DBus.ObjectManager'>"
"    <method name='GetManagedObjects'>"
"      <arg name='objects' type='a{oa{sa{sv}}}' direction='out'/>"
"    </method>"
"    <signal name='InterfacesRemoved'>"
"      <arg name='interfaces' type='as'/>"
"    </signal>"
"  </interface>"
"</node>";


/*
 * 服务对象的xml
 */
static const gchar service_xml[] =
"<node>"
"  <interface name='org.bluez.GattService1'>"
"    <property name='UUID' type='s' access='read'/>"
"    <property name='Primary' type='b' access='read'/>"
"  </interface>"
"</node>";


/*
 * 特性对象的xml
 */
static const gchar char_xml[] = 
"<node>"
"  <interface name='org.bluez.GattCharacteristic1'>"
"    <property name='UUID' type='s' access='read'/>"
"    <property name='Service' type='o' access='read'/>"
"    <property name='Value' type='ay' access='read'/>"
"    <property name='Notifying' type='b' access='read'/>"
"    <property name='Flags' type='as' access='read'/>"
"    <method name='ReadValue'>"
"      <arg name='options' type='a{sv}' direction='in'/>"
"      <arg name='value' type='ay' direction='out'/>"
"    </method>"
"    <method name='WriteValue'>"
"      <arg name='value' type='ay' direction='in'/>"
"      <arg name='options' type='a{sv}' direction='in'/>"
"    </method>"
"    <method name='StartNotify'>"
"    </method>"
"    <method name='StopNotify'/>"
"  </interface>"
"</node>";




/*
-> /org/uart/server
  |   - org.freedesktop.DBus.ObjectManager
  |
  -> /org/uart/server/service00
  | |   - org.freedesktop.DBus.Properties
  | |   - org.bluez.GattService1
  | |
  | -> /org/uart/server/service00/char0000
  | |     - org.freedesktop.DBus.Properties
  | |     - org.bluez.GattCharacteristic1
  | |
  | -> /org/uart/server/service00/char0001
  |   |   - org.freedesktop.DBus.Properties
  |   |   - org.bluez.GattCharacteristic1
  |   |
  |   -> /org/uart/server/service00/char0001/desc000 (cccd被bluez自动创建)
  |       - org.freedesktop.DBus.Properties
  |       - org.bluez.GattDescriptor1
  |
  -> /org/uart/server/serviceXX
    |   - org.freedesktop.DBus.Properties
    |   - org.bluez.GattService1
    |
    -> /org/uart/server/serviceXX/char0000
        - org.freedesktop.DBus.Properties
        - org.bluez.GattCharacteristic1
*/

char service_uuid[128] = "0fda92b2-44a2-4af2-84f5-fa682baa2b8d";

char char_uuid[13][128] = {
	"d083b2bd-be16-4600-b397-61512ca2f5ad",
	"0a852c59-50d3-4492-bfd3-22fe58a24f01",
	"d7515033-7e7b-45be-803f-c8737b171a29",
	"b833d34f-d871-422c-bf9e-8e6ec117d57e",
	"9c4314f2-8a0c-45fd-a58d-d4a7e64c3a57",
	"180efdef-7579-4b4a-b2df-72733b7fa2fe",
	"7731de63-bc6a-4100-8ab1-89b2356b038b",
	"d435f5de-01a4-4e7d-84ba-dfd347f60275",
	"df3b16ca-c985-4da2-a6d2-9b9b9abdb858",
	"398168aa-0111-4ec0-b1fa-171671270608",
	"8cc6e0b3-98c5-40cc-b1d8-692940e6994b",
	"e5866bd6-0288-4476-98ca-ef7da6b4d289",
	"e125bda4-6fb8-11ea-bc55-0242ac130003"
};

char service_name[128] = "/org/uart/server/service00";

char flags_name[3][32] = {
	"read",
	"write-without-response",
	"notify"
};

struct server_t server_ctx = {
	.gatt = {
		.service = {
			.UUID = service_uuid,
			.Primary = 1,
		},
		/*	OnboardingKey
		 * "/service00/char0000"
		 */
		.char0 = {
			.UUID = char_uuid[0],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[0],
			},
		},
		/*	PublicKey
		 * "/service00/char0001"
		 */
		.char1 = {
			.UUID = char_uuid[1],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[0],
			},
		},
		/*	WiFiServices
		 * "/service00/char0002"
		 */
		.char2 = {
			.UUID = char_uuid[2],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[0],
				[1] = flags_name[1],
				[2] = flags_name[2],
			},
		},
		/*	Diagnostics
		 * "/service00/char0003"
		 */
		.char3 = {
			.UUID = char_uuid[3],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[0],
				[1] = flags_name[1],
			},
		},
		/*	WiFi MAC Address
		 * "/service00/char0004"
		 */
		.char4 = {
			.UUID = char_uuid[4],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[0],
			},
		},
		/*	Lights 
		 * "/service00/char0005"
		 */
		.char5 = {
			.UUID = char_uuid[5],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[0],
			},
		},
		/*	WiFiSSID
		 * "/service00/char0006"
		 */
		.char6 = {
			.UUID = char_uuid[6],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[1],
			},
		},
		/*	AssertLocation
		 * "/service00/char0007"
		 */
		.char7 = {
			.UUID = char_uuid[7],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[1],
			},
		},
		/*	AddGateway
		 * "/service00/char0008"
		 */
		.char8 = {
			.UUID = char_uuid[8],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[1],
			},
		},
		/*	WiFiConnect
		 * "/service00/char0009"
		 */
		.char9 = {
			.UUID = char_uuid[9],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[1],
			},
		},
		/*	WiFiRemove
		 * "/service00/char0010"
		 */
		.char10 = {
			.UUID = char_uuid[10],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[1],
			},
		},
		/*	EthernetOnline 
		 * "/service00/char0011"
		 */
		.char11 = {
			.UUID = char_uuid[11],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[0],
			},
		},
		/*	WiFiConfiguredServices
		 * "/service00/char0012"
		 */
		.char12 = {
			.UUID = char_uuid[12],
			.Service = service_name,
			.Flags = {
				[0] = flags_name[0],
			},
		},
	},
};


void gatt_uart_register_receive_cb(uart_receive_t receive_cb)
{
	server_ctx.receive_cb_func = receive_cb;
}

#if 0
/*
 * 最大发送512个字节
 */
void gatt_uart_send(uint8_t *buf, int len)
{
	//if(!server_ctx.conn || !server_ctx.gatt.tx_char.Notifying || len > 512)
	if(!server_ctx.conn || !server_ctx.gatt.tx_char.Notifying || len > 512) 
	{
		return ;
	}

	memcpy(server_ctx.gatt.tx_char.Value, buf, len);
	server_ctx.gatt.tx_char.len = len;

	/*
	 * 通知是通过PropertiesChanged信号实现的。
	 * 当bluez收到Value属性PropertiesChanged的信号,
	 * bluez就会向客户端发送notification  或者 indication。
	 *
	 * 参考:doc/gatt-api.txt
	 * The cached value of the characteristic. This property
	 * gets updated only after a successful read request and
	 * when a notification or indication is received, upon
	 * which a PropertiesChanged signal will be emitted.
	 */
	GVariant *parameters[3];
	
	/*
	 * interface_name
	 */
	parameters[0] = g_variant_new_string("org.bluez.GattCharacteristic1");

	/*
	 * changed_properties
	 */
	GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
	int i;
	for(i = 0; i < server_ctx.gatt.tx_char.len; i++) {
		g_variant_builder_add(builder, "y", server_ctx.gatt.tx_char.Value[i]);
	}
	
	GVariant *value = g_variant_builder_end(builder);
	g_variant_builder_unref(builder);

	GVariantBuilder *prop_builder = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));
	g_variant_builder_add(prop_builder, "{sv}", "Value", value);
	parameters[1] = g_variant_builder_end(prop_builder);
	g_variant_builder_unref(prop_builder);

	/*
	 * invalidated_properties
	 */
	GVariantBuilder *inv_prop_builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
	parameters[2] = g_variant_builder_end(inv_prop_builder);
	g_variant_builder_unref(inv_prop_builder);

	GError *error = NULL;
	g_dbus_connection_emit_signal(server_ctx.conn,
                               "org.bluez",
                               UART_OBJECT_PATH"/service00/char0001",
                               "org.freedesktop.DBus.Properties",
                               "PropertiesChanged" ,
                               g_variant_new_tuple(parameters, 3), /* (sa{sv}as) */
                               &error);

	printf("gatt_uart_send done\n");
	
	if(error) {
		printf("[%s:%d] error: %s\n", __FUNCTION__, __LINE__, error->message);
		g_error_free (error);
	}						   
}
#endif

static int gatt_create_node_info(void)
{
	GError *error = NULL;

	server_ctx.object_manager_node_info = g_dbus_node_info_new_for_xml(object_manager_xml, &error);

	if(error) {
		printf("[%s:%d] error: %s\n", __FUNCTION__, __LINE__, error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.service_node_info = g_dbus_node_info_new_for_xml(service_xml, &error);

	if(error) {
		printf("[%s:%d] error: %s\n", __FUNCTION__, __LINE__, error->message);
		g_error_free (error);
		goto ERROR_1;
	}

	server_ctx.char_node_info = g_dbus_node_info_new_for_xml(char_xml, &error);

	if(error) {
		printf("[%s:%d] error: %s\n", __FUNCTION__, __LINE__, error->message);
		g_error_free (error);
		goto ERROR_2;
	}
	
	printf("[%s:%d] %s\n", __FUNCTION__, __LINE__, "gatt_create node info ok");
	return 0;

ERROR_2:
	g_dbus_node_info_unref(server_ctx.service_node_info);

ERROR_1:
	g_dbus_node_info_unref(server_ctx.object_manager_node_info);
	return -1;
}



static GVariant *
get_property_variant(const gchar *object_path, const gchar *interface_name, const gchar *property_name)
{
	GVariant *v = NULL;

	if(!strcmp(object_path, UART_OBJECT_PATH"/service00")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.service.UUID);
		} else if(!strcmp(property_name, "Primary")) {
			v = g_variant_new("b", server_ctx.gatt.service.Primary);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0000")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char0.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char0.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char0.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char0.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char0.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char0.Service);
		}  else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char0.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0001")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char1.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char1.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char1.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char1.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char1.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char1.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char1.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0002")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char2.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char2.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char2.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char2.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char2.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char2.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char2.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0003")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char3.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char3.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char3.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char3.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char3.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char3.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char3.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0004")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char4.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char4.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char4.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char4.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char4.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char4.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char4.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0005")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char5.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char5.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char5.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char5.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char5.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char5.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char5.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0006")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char6.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char6.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char6.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char6.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char6.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char6.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char6.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0007")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char7.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char7.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char7.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char7.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char7.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char7.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char7.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0008")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char8.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char8.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char8.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char8.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char8.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char8.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char8.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0009")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char9.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char9.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char9.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char9.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char9.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char9.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char9.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0010")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char10.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char10.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char10.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char10.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char10.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char10.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char10.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0011")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char11.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char11.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char11.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char11.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char11.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char11.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char11.Notifying);
		}
	} else if(!strcmp(object_path, UART_OBJECT_PATH"/service00/char0012")) {
		if(!strcmp(property_name, "UUID")) {
			v = g_variant_new("s", server_ctx.gatt.char12.UUID);
		} else if(!strcmp(property_name, "Flags")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
			int i;
			for(i = 0; i < CHAR_FLAGS_SIZE; i++) {
				if(server_ctx.gatt.char12.Flags[i])
					g_variant_builder_add(builder, "s", server_ctx.gatt.char12.Flags[i]);
			}
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Value")) {
			GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
			int i;
			for(i = 0; i < server_ctx.gatt.char12.len; i++) {
				g_variant_builder_add(builder, "y", server_ctx.gatt.char12.Value[i]);
			}
			
			v= g_variant_builder_end(builder);
			g_variant_builder_unref(builder);
		} else if(!strcmp(property_name, "Service")) {
			v = g_variant_new("o", server_ctx.gatt.char12.Service);
		} else if(!strcmp(property_name, "Notifying")) {
			v = g_variant_new("b", server_ctx.gatt.char12.Notifying);
		}
	}

	return v;
}

/*
 * type='a{oa{sa{sv}}}'
 */
static GVariant *gatt_create_managed_objects(void)
{
	GVariant *v_property, *v;
	GVariantBuilder *builder_if;
	GVariantBuilder *builder_obj;
	GVariantBuilder *builder_mobjs;
	int i;

	builder_mobjs = g_variant_builder_new(G_VARIANT_TYPE("a{oa{sa{sv}}}"));

	/*
	 * 构建/org/uart/server/service00的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));
	
	v_property = get_property_variant(UART_OBJECT_PATH"/service00", "org.bluez.GattService1", "UUID");
	g_variant_builder_add(builder_if, "{&sv}", "UUID", v_property);

	v_property = get_property_variant(UART_OBJECT_PATH"/service00", "org.bluez.GattService1", "Primary");
	g_variant_builder_add(builder_if, "{&sv}", "Primary", v_property);
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);

	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattService1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);

	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00", v);

	/*
	 * 构建/org/uart/server/service00/char0000的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0000_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0000_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0000", "org.bluez.GattCharacteristic1", char0000_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0000_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0000", v);

	/*
	 * 构建/org/uart/server/service00/char0001的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0001_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0001_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0001", "org.bluez.GattCharacteristic1", char0001_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0001_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0001", v);
	
	/*
	 * 构建/org/uart/server/service00/char0002的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0002_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0002_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0002", "org.bluez.GattCharacteristic1", char0002_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0002_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0002", v);
	
	/*
	 * 构建/org/uart/server/service00/char0003的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0003_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0003_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0003", "org.bluez.GattCharacteristic1", char0003_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0003_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0003", v);

	/*
	 * 构建/org/uart/server/service00/char0004的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0004_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0004_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0004", "org.bluez.GattCharacteristic1", char0004_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0004_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0004", v);

	/*
	 * 构建/org/uart/server/service00/char0005的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0005_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0005_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0005", "org.bluez.GattCharacteristic1", char0005_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0005_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0005", v);

	/*
	 * 构建/org/uart/server/service00/char0006的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0006_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0006_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0006", "org.bluez.GattCharacteristic1", char0006_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0006_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0006", v);

	/*
	 * 构建/org/uart/server/service00/char0007的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0007_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0007_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0007", "org.bluez.GattCharacteristic1", char0007_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0007_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0007", v);

	/*
	 * 构建/org/uart/server/service00/char0008的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0008_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0008_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0008", "org.bluez.GattCharacteristic1", char0008_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0008_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0008", v);

	/*
	 * 构建/org/uart/server/service00/char0009的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0009_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0009_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0009", "org.bluez.GattCharacteristic1", char0009_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0009_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0009", v);

	/*
	 * 构建/org/uart/server/service00/char0010的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0010_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0010_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0010", "org.bluez.GattCharacteristic1", char0010_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0010_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0010", v);

	/*
	 * 构建/org/uart/server/service00/char0011的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0011_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0011_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0011", "org.bluez.GattCharacteristic1", char0011_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0011_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0011", v);

	/*
	 * 构建/org/uart/server/service00/char0012的接口和属性
	 */
	builder_if = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));

	const char *char0012_list[] = {"UUID", "Service", "Value", "Notifying", "Flags"};

	for(i = 0; i < sizeof(char0012_list)/sizeof(const char *); i++) {
		v_property = get_property_variant(UART_OBJECT_PATH"/service00/char0012", "org.bluez.GattCharacteristic1", char0012_list[i]);
		g_variant_builder_add(builder_if, "{&sv}", char0012_list[i], v_property);
	}
	
	v = g_variant_builder_end(builder_if);
	g_variant_builder_unref(builder_if);
	
	builder_obj = g_variant_builder_new(G_VARIANT_TYPE("a{sa{sv}}"));
	
	g_variant_builder_add(builder_obj, "{&s@a{sv}}", "org.bluez.GattCharacteristic1", v);
	
	v = g_variant_builder_end(builder_obj);
	g_variant_builder_unref(builder_obj);
	
	g_variant_builder_add(builder_mobjs, "{&o@a{sa{sv}}}", UART_OBJECT_PATH"/service00/char0012", v);

	/*
	 * 构建GetManagedObjects返回值类型
	 */
	v = g_variant_builder_end(builder_mobjs);
	g_variant_builder_unref(builder_mobjs);

	GVariant *tuples[] = {v};
	
	return g_variant_new_tuple(tuples, 1);
}

static void uart_rx_callback(GVariant *params, int char_idx)
{
#ifdef __DEBUG__
	printf("uart_rx_callback params type: \"%s\"\n", g_variant_get_type_string(params));
#endif
	/*
	 * params type: "(aya{sv})"
	 */
	GVariant *value, *flags;
	g_variant_get(params, "(@ay@a{sv})", &value, &flags);
	printf("value type: \"%s\"\n", g_variant_get_type_string(value));
	printf("flags type: \"%s\"\n", g_variant_get_type_string(flags));
	
	GVariantIter *iter = g_variant_iter_new(value);
	struct char_t *p_char = NULL;

	switch(char_idx)
	{
		case 0:
			p_char = &server_ctx.gatt.char0;
			break;
		case 1:
			p_char = &server_ctx.gatt.char1;
			break;
		case 2:
			p_char = &server_ctx.gatt.char2;
			break;
		case 3:
			p_char = &server_ctx.gatt.char3;
			break;
		case 4:
			p_char = &server_ctx.gatt.char4;
			break;
		case 5:
			p_char = &server_ctx.gatt.char5;
			break;
		case 6:
			p_char = &server_ctx.gatt.char6;
			break;
		case 7:
			p_char = &server_ctx.gatt.char7;
			break;
		case 8:
			p_char = &server_ctx.gatt.char8;
			break;
		case 9:
			p_char = &server_ctx.gatt.char9;
			break;
		case 10:
			p_char = &server_ctx.gatt.char10;
			break;
		case 11:
			p_char = &server_ctx.gatt.char11;
			break;
		case 12:
			p_char = &server_ctx.gatt.char12;
			break;
	}

	/*
	 * 提取数据
	 */
	//server_ctx.gatt.rx_char.len = 0;
	//uint8_t *pdata = server_ctx.gatt.rx_char.Value;

	p_char->len = 0;
	uint8_t *pdata = p_char->Value;

	while(g_variant_iter_next(iter, "y", (pdata + p_char->len)))
	{
		p_char->len++;
	}
	g_variant_iter_free(iter);
	
#ifdef __DEBUG__
	printf("uart_rx_callback len: %d\n", p_char->len);
	u_tm_log_hex("uart_rx_callback value: ", p_char->Value, p_char->len);
#endif

#if 0
	/*
	 * 将数据提供给回调函数
	 */
	if(server_ctx.gatt.rx_char.len && server_ctx.receive_cb_func) {
		server_ctx.receive_cb_func(server_ctx.gatt.rx_char.Value, server_ctx.gatt.rx_char.len);
	}
#endif	
}

static void uart_tx_callback(GDBusMethodInvocation *invoc, GVariant *params, int char_idx, uint8_t *buf, int len)
{
#ifdef __DEBUG__
	printf("uart_tx_callback params type: \"%s\"\n", g_variant_get_type_string(params));
#endif
	/*
	 * params type: "(a{sv})"
	 */

	GVariant *flags;
	g_variant_get(params, "(@a{sv})", &flags);
	printf("flags type: \"%s\"\n", g_variant_get_type_string(flags));
	//printf("value type: \"%s\"\n", g_variant_get_type_string(val));

	struct char_t *p_char = NULL;

	switch(char_idx)
	{
		case 0:
			p_char = &server_ctx.gatt.char0;
			break;
		case 1:
			p_char = &server_ctx.gatt.char1;
			break;
		case 2:
			p_char = &server_ctx.gatt.char2;
			break;
		case 3:
			p_char = &server_ctx.gatt.char3;
			break;
		case 4:
			p_char = &server_ctx.gatt.char4;
			break;
		case 5:
			p_char = &server_ctx.gatt.char5;
			break;
		case 6:
			p_char = &server_ctx.gatt.char6;
			break;
		case 7:
			p_char = &server_ctx.gatt.char7;
			break;
		case 8:
			p_char = &server_ctx.gatt.char8;
			break;
		case 9:
			p_char = &server_ctx.gatt.char9;
			break;
		case 10:
			p_char = &server_ctx.gatt.char10;
			break;
		case 11:
			p_char = &server_ctx.gatt.char11;
			break;
		case 12:
			p_char = &server_ctx.gatt.char12;
			break;
	}

	//memcpy(server_ctx.gatt.tx_char.Value, buf, len);
	//server_ctx.gatt.tx_char.len = len;

	memcpy(p_char->Value, buf, len);
	p_char->len = len;

	GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("ay"));
	int i;
	for(i = 0; i < p_char->len; i++) {
		g_variant_builder_add(builder, "y", p_char->Value[i]);
	}
	
	GVariant *value = g_variant_builder_end(builder);
	g_variant_builder_unref(builder);

	GVariant *tuples[1] = { value };

	printf("uart_tx_callback done\n");

	g_dbus_method_invocation_return_value(invoc, g_variant_new_tuple(tuples, 1));
}


static void 
on_method_call(GDBusConnection *con,
                       const gchar *sender,
                       const gchar *obj_path,
                       const gchar *iface_name,
                       const gchar *method_name,
                       GVariant *params,
                       GDBusMethodInvocation *invoc,
                       gpointer udata)
{
#ifdef __DEBUG__
	printf("[%s:%d] object_path :%s\n", __FUNCTION__, __LINE__, obj_path);
	printf("[%s:%d] iface_name :%s\n", __FUNCTION__, __LINE__, iface_name);
	printf("[%s:%d] method_name :%s\n", __FUNCTION__, __LINE__, method_name);
#endif

	if(!strcmp(obj_path, UART_OBJECT_PATH)) {
		if(!strcmp(method_name, "GetManagedObjects")) {
			g_dbus_method_invocation_return_value(invoc, gatt_create_managed_objects());
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0000")) {/* Tx */
		/*
		if(!strcmp(method_name, "StartNotify")) {
			server_ctx.gatt.tx_char.Notifying = 1;
			printf("Start server_ctx.gatt.tx_char.Notifying = %d\n", server_ctx.gatt.tx_char.Notifying);
		} else if(!strcmp(method_name, "StopNotify")) {
			server_ctx.gatt.tx_char.Notifying = 0;
			printf("Stop server_ctx.gatt.tx_char.Notifying = %d\n", server_ctx.gatt.tx_char.Notifying);
		} 
		else 
		*/
		if(!strcmp(method_name, "ReadValue")) {
			OnboardingKey_Read(invoc,params,0);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0001")) {
		if(!strcmp(method_name, "ReadValue")) {
			PublicKey_Read(invoc,params,1);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0002")) {/* Rx */
		if(!strcmp(method_name, "StartNotify")) {
			server_ctx.gatt.char2.Notifying = 1;
			printf("Start server_ctx.gatt.char2.Notifying = %d\n", server_ctx.gatt.char2.Notifying);
		} 
		else if(!strcmp(method_name, "StopNotify")) {
			server_ctx.gatt.char2.Notifying = 0;
			printf("Stop server_ctx.gatt.char2.Notifying = %d\n", server_ctx.gatt.char2.Notifying);
		} 
		else if(!strcmp(method_name, "WriteValue")) {
			WifiServices_Write(params,2);
		}
		else if(!strcmp(method_name, "ReadValue")) {
			WifiServices_Read(invoc,params,2);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0003")) {/* Rx */
		if(!strcmp(method_name, "WriteValue")) {
			uart_rx_callback(params,3);
		}
		else if(!strcmp(method_name, "ReadValue")){
			Diagnostics_Read(invoc,params,3);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0004")) {
		if(!strcmp(method_name, "ReadValue")) {
			WiFi_MAC_Address_Read(invoc,params,4);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0005")) {
		if(!strcmp(method_name, "ReadValue")) {
			uint8_t Lights[16] = "on";
			uart_tx_callback(invoc,params,5,Lights,2);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0006")) {/* Rx */
		if(!strcmp(method_name, "WriteValue")) {
			uart_rx_callback(params,6);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0007")) {/* Rx */
		if(!strcmp(method_name, "WriteValue")) {
			//uart_rx_callback(params,7);
			AssertLocation_Write(params,7);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0008")) {/* Rx */
		if(!strcmp(method_name, "WriteValue")) {
			//uart_rx_callback(params,8);
			AddGateway_Write(params,8);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0009")) {/* Rx */
		if(!strcmp(method_name, "WriteValue")) {
			//uart_rx_callback(params,9);
			WiFiConnect_Write(params,9);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0010")) {
		if(!strcmp(method_name, "WriteValue")) {
			WiFiRemove_Write(params,10);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0011")) {
		if(!strcmp(method_name, "ReadValue")) {
			uint8_t EthernetOnline[16] = "false";
			uart_tx_callback(invoc,params,11,EthernetOnline,5);
		}
	} else if(!strcmp(obj_path, UART_OBJECT_PATH"/service00/char0012")) {
		WifiServices_Read(invoc,params,12);
		//printf("char0012 method_name=%s \n",method_name);
	}
}

/*
 * 如果interface info中有可读的属性存在,那么必须提供一个非空的get_property,
 * 或者在org.freedesktop.DBus.Properties接口上的method_call方法中实现Get和GetAll两个函数.
 * 
 */
static GVariant *
get_property(GDBusConnection *connection,
					const gchar *sender,
					const gchar *object_path,
					const gchar *interface_name,
					const gchar *property_name,
					GError **error,
					gpointer user_data)
{
#ifdef __DEBUG__
	printf("[%s:%d] sender: %s\n", __FUNCTION__, __LINE__, sender);
	printf("[%s:%d] object_path: %s\n", __FUNCTION__, __LINE__, object_path);
	printf("[%s:%d] interface_name: %s\n", __FUNCTION__, __LINE__, interface_name);
	printf("[%s:%d] property_name: %s\n", __FUNCTION__, __LINE__, property_name);
#endif

	GVariant *v = get_property_variant(object_path, 
										interface_name, 
										property_name);

	return v;
}


static int gatt_object_register(GDBusConnection *conn)
{
	GError *error = NULL;
	GDBusInterfaceVTable interface_vtable;

	/*
	 * 这些回调函数的执行是依赖于g_main_loop的
	 */
	interface_vtable.method_call = on_method_call;
	interface_vtable.get_property = NULL; /*ObjectManager 不存在属性*/
	interface_vtable.set_property = NULL;

	server_ctx.object_manager_reg_id = 
		g_dbus_connection_register_object(conn,
                                   		UART_OBJECT_PATH,
                                   		server_ctx.object_manager_node_info->interfaces[0],/*org.freedesktop.DBus.ObjectManager*/
                                   		&interface_vtable,
                                   		NULL,
                                   		NULL,
                                   		&error);
	if(error) {
		printf("<org.freedesktop.DBus.ObjectManager> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	interface_vtable.method_call = on_method_call;
	interface_vtable.get_property = get_property;
	interface_vtable.set_property = NULL;
	
	server_ctx.service_reg_id = 
		g_dbus_connection_register_object(conn,
                                   		UART_OBJECT_PATH"/service00",
                                   		server_ctx.service_node_info->interfaces[0],/*org.bluez.GattService1*/
                                   		&interface_vtable,
                                   		NULL,
                                   		NULL,
                                   		&error);
	if(error) {
		printf("<org.bluez.GattService1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char0_reg_id = 
		g_dbus_connection_register_object(conn,
                                   		UART_OBJECT_PATH"/service00/char0000",
                                   		server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
                                   		&interface_vtable,
                                   		NULL,
                                   		NULL,
                                   		&error);
	if(error) {
		printf("<char0 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char1_reg_id = 
		g_dbus_connection_register_object(conn,
                                   		UART_OBJECT_PATH"/service00/char0001",
                                   		server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
                                   		&interface_vtable,
                                   		NULL,
                                   		NULL,
                                   		&error);
	if(error) {
		printf("<char1 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char2_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0002",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char2 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char3_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0003",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char3 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}
	
	server_ctx.char4_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0004",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char4 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char5_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0005",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char5 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char6_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0006",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char6 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char7_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0007",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char7 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char8_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0008",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char8 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char9_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0009",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char9 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char10_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0010",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char10 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char11_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0011",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char11 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	server_ctx.char12_reg_id = 
	g_dbus_connection_register_object(conn,
									UART_OBJECT_PATH"/service00/char0012",
									server_ctx.char_node_info->interfaces[0],/*org.bluez.GattCharacteristic1*/
									&interface_vtable,
									NULL,
									NULL,
									&error);
	if(error) {
		printf("<char12 org.bluez.GattCharacteristic1> interface info register Error\n");
		printf("%s\n", error->message);
		g_error_free (error);
		return -1;
	}

	printf("[%s:%d] %s\n", __FUNCTION__, __LINE__, "gatt object register ok");
	
	return 0;
}

static void async_ready_callback(GObject *source_object,
                        GAsyncResult *res,
                        gpointer user_data)
{
	
	GDBusConnection *conn = (GDBusConnection *)user_data;
	GError *error = NULL;
	
	g_dbus_connection_call_finish (conn,
                               res,
                               &error);

   if(error) {
	   printf("Error: RegisterApplication %s\n", error->message);
	   g_error_free (error);
   		return;
   }

   printf("async_ready_callback: uart_register_application ok \n");
}


static void uart_register_application_async(GDBusConnection *conn)
{
	GVariant *parameters;
	
	GVariant *vobject_path = g_variant_new("o", UART_OBJECT_PATH);

	GVariantBuilder *dict_builder = g_variant_builder_new(G_VARIANT_TYPE("a{sv}"));
	GVariant *dict_v = g_variant_builder_end(dict_builder);
	g_variant_builder_unref(dict_builder);
	
	GVariant *children[] = {vobject_path, dict_v};
	parameters = g_variant_new_tuple(children, 2);

	g_dbus_connection_call (conn,
							"org.bluez",
							"/org/bluez/hci0",
							"org.bluez.GattManager1",
							"RegisterApplication",
							parameters,
							NULL,
							G_DBUS_CALL_FLAGS_NONE,
							-1,
							NULL,
							async_ready_callback,
							conn);	
}


int gatt_uart_server_start(GDBusConnection *conn)
{
	gatt_create_node_info();
	gatt_object_register(conn);
	uart_register_application_async(conn);
	server_ctx.conn = conn;
	return 0;
}





