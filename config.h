#ifndef __CONFIG_H__
#define __CONFIG_H__


/*
  

*/

enum protocal_type_s
{
  UDP=1,
  TCP
};

typedef struct pcap_config_s
{
  char ip[256];
  int port;
  enum protocal_type_s protocal_type;
  int send_interval_packet;
  int sleep_time;
  int user_data_offset;
  void *list;
  
}pcap_config_t;

int parse_config(const char *path, pcap_config_t *config);

#endif //__CONFIG_H__


