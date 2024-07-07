#ifndef __CHAR_RESPOND_H__
#define __CHAR_RESPOND_H__

#include <stdint.h>
#include <gio/gio.h>
#include <string>
#include <vector>
#include <set>

int parse_line(std::string line,std::string &value);
int parse_conf(std::string file_name, std::string keyword, std::string &value);
int get_mac_address(std::string net_name, std::string &mac_addr);
int get_wifi_ssid_data(char *res, unsigned int max_size);
void parse_wifi_ssid_data(char *data, std::vector<std::string> &ssid_vector);

struct char_t* get_char_by_index(int idx);
void gatt_notify_callback(int char_idx,uint8_t *buf, int len);
void gatt_tx_callback(GDBusMethodInvocation *invoc, GVariant *params, int char_idx, char *buf, int len);
void gatt_rx_callback(GVariant *params, struct char_t *p_char);

void OnboardingKey_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx);
void PublicKey_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx);
void WiFi_MAC_Address_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx);
void WifiServices_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx);
void Diagnostics_Read(GDBusMethodInvocation *invoc, GVariant *params, int char_idx);

void WifiServices_Write(GVariant *params, int char_idx);
void AssertLocation_Write(GVariant *params, int char_idx);
void AddGateway_Write(GVariant *params, int char_idx);
void WiFiConnect_Write(GVariant *params, int char_idx);
void WiFiRemove_Write(GVariant *params, int char_idx);


#endif