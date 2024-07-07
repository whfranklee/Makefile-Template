#include <gio/gio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <stdint.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "gatt.h"
#include "log.h"

#include "sys_utils.h"
#include "proto/WiFiServices.pb.h"
#include "proto/Diagnostics.pb.h"
#include "proto/AssertLocation.pb.h"
#include "proto/AddGateway.pb.h"
#include "proto/WiFiConnect.pb.h"
#include "proto/WiFiRemove.pb.h"
#include "char_respond.h"

using namespace std;

#define __CHAR_DEBUG__

char wifi_ssid_buffer[512] = {0};

int parse_line(string line,string &value)
{
	size_t pos = line.find(":");
	if(pos == string::npos)
	{
		return -1;
	}
	pos++;

	string m_str = line.substr(pos);
	size_t start = m_str.find("\"");
	if(start == string::npos)
	{
		return -1;
	}
	start++;
	
	size_t end = m_str.rfind("\"");
	if(end == string::npos)
	{
		return -1;
	}
	
	if(end <= start)
	{
		return -1;
	}

	value = m_str.substr(start,end-start);

	return 0;
}

int parse_conf(string file_name, string keyword, string &value)
{
	int ret = -1;
	ifstream is(file_name.c_str());

	if(!is)
	{
		cout << "Error open" << endl;
		return -1;
	}

	string buf;
	while(getline(is,buf))
	{
		//cout << "Read: " << buf << endl;
		size_t pos = buf.find(keyword);
		
		if(pos != string::npos)
		{	
			string m_str = buf.substr(pos);
			if(parse_line(m_str, value) == 0)
			{
				ret = 0;			
			}
		}
	}

	return ret;
}

int get_mac_address(string net_name, string &mac_addr)
{
    char path[128];

    sprintf(path, "/sys/class/net/%s/address",net_name.c_str());

    ifstream is(path);
    if(!is)
    {
        cout << "Error open" << endl;
        return -1;
    }

    string line_buf;
    while(getline(is,line_buf))
    {
        mac_addr = line_buf.substr(0,line_buf.size());

        for(int i = 0; i < 5; i++)
        {
            size_t pos = mac_addr.find(":");
            if(pos == string::npos)
            {
                continue;
            }
	    
            mac_addr = mac_addr.replace(pos,1,"");
        }
    }

    return 0;
}

int get_wifi_ssid_data(char *res, unsigned int max_size)
{
	char cmd[512] = {0};
	int ret = -1;

	sprintf(cmd, "iw dev wlan0 scan | grep SSID | cut -d ':' -f 2");
	ret = utils_system_ex(cmd, res, max_size);

	if(ret <= 0)
	{
		ret = -1;
	}
	
	return ret;
}

void parse_wifi_ssid_data(char *data, std::vector<std::string> &ssid_vector)
{
	string mixture = data;
	size_t size = mixture.size();
	set<string> ssid_set;

	for(size_t i = 0; i < size; i++)
	{
		if(mixture[i] == ' ')
		{
			continue;		
		}

		size_t pos = mixture.find("\n",i);
		string ssid = mixture.substr(i, pos-i);

		if(ssid.size() > 0)
		{
			//ssid.erase(0,ssid.find_first_not_of(" "));
			set<string>::iterator iter = ssid_set.find(ssid);
			if(iter == ssid_set.end())
			{
				ssid_set.insert(ssid);
				ssid_vector.push_back(ssid);
			}
		}

		i= pos;
	}
}

struct char_t* get_char_by_index(int idx)
{
    struct char_t *p = NULL;

    switch(idx)
	{
		case 0:
			p = &server_ctx.gatt.char0;
			break;
		case 1:
			p = &server_ctx.gatt.char1;
			break;
		case 2:
			p = &server_ctx.gatt.char2;
			break;
		case 3:
			p = &server_ctx.gatt.char3;
			break;
		case 4:
			p = &server_ctx.gatt.char4;
			break;
		case 5:
			p = &server_ctx.gatt.char5;
			break;
		case 6:
			p = &server_ctx.gatt.char6;
			break;
		case 7:
			p = &server_ctx.gatt.char7;
			break;
		case 8:
			p = &server_ctx.gatt.char8;
			break;
		case 9:
			p = &server_ctx.gatt.char9;
			break;
		case 10:
			p = &server_ctx.gatt.char10;
			break;
		case 11:
			p = &server_ctx.gatt.char11;
			break;
		case 12:
			p = &server_ctx.gatt.char12;
			break;
        default:
            p = NULL;
            break;
	}

    return p;
};

void gatt_notify_callback(int char_idx,uint8_t *buf, int len)
{
	struct char_t *p_char = get_char_by_index(char_idx);
	if(!server_ctx.conn || !(p_char->Notifying) || len > 512) 
	{
		return ;
	}

	ostringstream os;
	os << UART_OBJECT_PATH << "/service00/char" << setw(4) << setfill('0') << to_string(char_idx);
	string object_path = os.str();
	/*
	 * 通知是通过PropertiesChanged信号实现的。
	 * 当bluez收到Value属性PropertiesChanged的信号,
	 * bluez就会向客户端发送notification  或者 indication。
	 *
	 * 参考:doc/gatt-api.txt
	 * The cached value of the characteristic. This property
	 * gets updated only after a successful read request and
	 * when a notification or indication is receiv   int len = m_ded, upon
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
	for(i = 0; i < len; i++) {
		g_variant_builder_add(builder, "y", buf[i]);
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
                               object_path.c_str(),
                               "org.freedesktop.DBus.Properties",
                               "PropertiesChanged" ,
                               g_variant_new_tuple(parameters, 3), /* (sa{sv}as) */
                               &error);
	
	if(error) {
		printf("[%s:%d] error: %s\n", __FUNCTION__, __LINE__, error->message);
		g_error_free (error);
	}						   
}

void gatt_tx_callback(GDBusMethodInvocation *invoc, GVariant *params, int char_idx, char *buf, int len)
{
#ifdef __CHAR_DEBUG__
	printf("dbus_tx_callback params type: \"%s\"\n", g_variant_get_type_string(params));
#endif
	/*
	 * params type: "(a{sv})"
	 */

	GVariant *flags;
	g_variant_get(params, "(@a{sv})", &flags);
	printf("flags type: \"%s\"\n", g_variant_get_type_string(flags));

	struct char_t *p_char = get_char_by_index(char_idx);

	if(p_char != NULL)
	{
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
		g_dbus_method_invocation_return_value(invoc, g_variant_new_tuple(tuples, 1));
	}
}

void gatt_rx_callback(GVariant *params, struct char_t *p_char)
{
	/*
	 * params type: "(aya{sv})"
	 */
	GVariant *value, *flags;
	g_variant_get(params, "(@ay@a{sv})", &value, &flags);
	printf("value type: \"%s\"\n", g_variant_get_type_string(value));
	printf("flags type: \"%s\"\n", g_variant_get_type_string(flags));
	
	GVariantIter *iter = g_variant_iter_new(value);

	/*
	* 提取数据
	*/
	p_char->len = 0;
	uint8_t *pdata = p_char->Value;

	while(g_variant_iter_next(iter, "y", (pdata + p_char->len)))
	{
		p_char->len++;
	}
	
	g_variant_iter_free(iter);
}

void OnboardingKey_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx)
{
	string data;
	char buf[128] = {0};
	int len = 1;
	
	if(parse_conf("/root/.conf/miner.conf","onboarding",data) >= 0)
	{
		strcpy(buf,data.c_str());
		len = strlen(buf);
	}

	gatt_tx_callback(invoc,params,char_idx,buf,len);
}

void PublicKey_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx)
{
	string data;
	char buf[128] = {0};
	int len = 1;

	if(parse_conf("/root/.conf/miner.conf","key",data) >= 0)
	{
		strcpy(buf,data.c_str());
		len = strlen(buf);
	}

	gatt_tx_callback(invoc,params,char_idx,buf,len);
}

void WiFi_MAC_Address_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx)
{
    string mac_addr;
    char buf[128] = {0};
    int len = 1;

	if(get_mac_address("wlan0",mac_addr) >= 0)
	{
        strcpy(buf,mac_addr.c_str());
        len = strlen(buf);
	}
	
	gatt_tx_callback(invoc,params,char_idx,buf,len);
}

void WifiServices_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx)
{
    wifi_services_v1 m_wifi_srv;
	std::vector<std::string> ssid_array;

	char *res = wifi_ssid_buffer;

	parse_wifi_ssid_data(res,ssid_array);
	for(size_t i=0; i < ssid_array.size(); i++)
	{
		m_wifi_srv.add_services(ssid_array[i]);
		cout << "ssid:"<< ssid_array[i] << endl;
	}

    int len = m_wifi_srv.ByteSizeLong();
    char *pbuf = new char[len];
    m_wifi_srv.SerializeToArray(pbuf,len);

	gatt_tx_callback(invoc,params,char_idx,pbuf,len);
    delete [] pbuf;
}

void Diagnostics_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx)
{
    diagnostics_v1 m_diagnostics;

	(*m_diagnostics.mutable_diagnostics())["connected"] = "Loading...";
	(*m_diagnostics.mutable_diagnostics())["dialable"] = "Loading...";
	(*m_diagnostics.mutable_diagnostics())["height"] = "Loading...";
	(*m_diagnostics.mutable_diagnostics())["nat_type"] = "Loading...";

	(*m_diagnostics.mutable_diagnostics())["eth"] = "00:01:02:03:04:05";
	(*m_diagnostics.mutable_diagnostics())["wifi"] = "00:01:02:03:04:05";
	(*m_diagnostics.mutable_diagnostics())["fw"] = "v1.0";
	(*m_diagnostics.mutable_diagnostics())["miner"] = "v2.0";
	(*m_diagnostics.mutable_diagnostics())["ip"] = "";

    int len = m_diagnostics.ByteSizeLong();
    char *pbuf = new char[len];
    m_diagnostics.SerializeToArray(pbuf,len);

	gatt_tx_callback(invoc,params,char_idx,pbuf,len);
    delete [] pbuf;
}

void WifiServices_Write(GVariant *params, int char_idx)
{
#ifdef __CHAR_DEBUG__
	printf("WifiServices_Write params type: \"%s\"\n", g_variant_get_type_string(params));
#endif
	struct char_t *p_char = get_char_by_index(char_idx);
	
	if(p_char != NULL)
	{
		gatt_rx_callback(params,p_char);

	#ifdef __CHAR_DEBUG__
		printf("WifiServices_Write len: %d\n", p_char->len);
		u_tm_log_hex("WifiServices_Write value: ", p_char->Value, p_char->len);
	#endif

		char *res = wifi_ssid_buffer;
		uint8_t sig[16] = {0};
		string str = "ok";

		memset(res,0,sizeof(wifi_ssid_buffer));

		if(get_wifi_ssid_data(res,sizeof(wifi_ssid_buffer)) > 0)
		{
			str = "ok";
		}
		else
		{
			str = "false";
		}

		memcpy(sig,str.c_str(),str.size());
		gatt_notify_callback(char_idx,sig,str.size());
	}
}

void AssertLocation_Write(GVariant *params, int char_idx)
{
#ifdef __CHAR_DEBUG__
	printf("AssertLocation_Write params type: \"%s\"\n", g_variant_get_type_string(params));
#endif
	struct char_t *p_char = get_char_by_index(char_idx);

	if(p_char != NULL)
	{
		gatt_rx_callback(params,p_char);

	#ifdef __CHAR_DEBUG__
		printf("AssertLocation_Write len: %d\n", p_char->len);
		u_tm_log_hex("AssertLocation_Write value: ", p_char->Value, p_char->len);
	#endif

		assert_loc_v1 m_assert_loc;
		m_assert_loc.ParseFromArray(p_char->Value,p_char->len);

		printf("lat: %f\n", m_assert_loc.lat());
		printf("lon: %f\n", m_assert_loc.lon());
		printf("owner: \"%s\"\n", m_assert_loc.owner().c_str());
		printf("nonce: %d\n", m_assert_loc.nonce());
		printf("fee: %d\n", m_assert_loc.fee());
		printf("amount: %d\n", m_assert_loc.amount());
		printf("payer: \"%s\"\n", m_assert_loc.payer().c_str());
	}
}

void AddGateway_Write(GVariant *params, int char_idx)
{
#ifdef __CHAR_DEBUG__
	printf("AddGateway_Write params type: \"%s\"\n", g_variant_get_type_string(params));
#endif
	struct char_t *p_char = get_char_by_index(char_idx);

	if(p_char != NULL)
	{
		gatt_rx_callback(params,p_char);
	
	#ifdef __CHAR_DEBUG__
		printf("AddGateway_Write len: %d\n", p_char->len);
		u_tm_log_hex("AddGateway_Write value: ", p_char->Value, p_char->len);
	#endif

		add_gateway_v1 m_add_gateway;
		m_add_gateway.ParseFromArray(p_char->Value,p_char->len);

		printf("owner: \"%s\"\n", m_add_gateway.owner().c_str());
		printf("amount: %d\n", m_add_gateway.amount());
		printf("fee: %d\n", m_add_gateway.fee());
		printf("payer: \"%s\"\n", m_add_gateway.payer().c_str());
	}
}

void WiFiConnect_Write(GVariant *params, int char_idx)
{
#ifdef __CHAR_DEBUG__
	printf("WiFiConnect_Write params type: \"%s\"\n", g_variant_get_type_string(params));
#endif
	struct char_t *p_char = get_char_by_index(char_idx);

	if(p_char != NULL)
	{
		gatt_rx_callback(params,p_char);
	
	#ifdef __CHAR_DEBUG__
		printf("WiFiConnect_Write len: %d\n", p_char->len);
		u_tm_log_hex("WiFiConnect_Write value: ", p_char->Value, p_char->len);
	#endif

		wifi_connect_v1 m_wifi_details;
		m_wifi_details.ParseFromArray(p_char->Value,p_char->len);

		printf("service: \"%s\"\n", m_wifi_details.service().c_str());
		printf("password: \"%s\"\n", m_wifi_details.password().c_str());
	}
}

void WiFiRemove_Write(GVariant *params, int char_idx)
{
#ifdef __CHAR_DEBUG__
	printf("WiFiRemove_Write params type: \"%s\"\n", g_variant_get_type_string(params));
#endif
	struct char_t *p_char = get_char_by_index(char_idx);

	if(p_char != NULL)
	{
		gatt_rx_callback(params,p_char);
	
	#ifdef __CHAR_DEBUG__
		printf("WiFiRemove_Write len: %d\n", p_char->len);
		u_tm_log_hex("WiFiRemove_Write value: ", p_char->Value, p_char->len);
	#endif

		wifi_remove_v1 wifi_remove_ssid;
		wifi_remove_ssid.ParseFromArray(p_char->Value,p_char->len);

		printf("service: \"%s\"\n", wifi_remove_ssid.service().c_str());
	}
}