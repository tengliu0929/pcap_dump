#ifndef  PACKET_PARSE_H__
#define PACKET_PARSE_H__


#ifdef   __cplusplus
extern "C"
{
#endif 
int parse_packet1( const char *pcap_name, int udpfd, struct sockaddr_in *dst);
#ifdef   __cplusplus
}
#endif

#endif
