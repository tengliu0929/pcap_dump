
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "packet_parse.h"
#include "config.h"




extern pcap_config_t gconfig;


int count = 0;


int proc_packet( pcap_t *pktHhandle, struct pcap_pkthdr *pktHeader, u_char *pktData, int udpfd, struct sockaddr_in *dst)
{
	int pktDataLen = pktHeader->len;

	struct tm *ltime;
	char timestr[16];
	time_t local_tv_sec;
	
	u_char* eth_data = NULL;
	struct iphdr*  iph = NULL;
	struct tcphdr* tcph = NULL;
	struct udphdr* udph = NULL;

	u_short hdr_len = 0;
	u_short eth_data_len = 14;
	u_short iph_len = 0;
	u_short tcph_len = 0;
	u_short udph_len = 0;
	u_short sport,dport;

	u_char* usr_data = NULL;//point to the header of data.
	u_short usr_data_len = 0;

	//struct packet_info pkt_info;
	
	eth_data = (u_char *)pktData;
	u_short pkt_type = *(u_short *)(eth_data + 12);
	pkt_type = ntohs(pkt_type);
	if(0x8100 == pkt_type) //VLAN
	{
		pkt_type = ntohs(*(u_short *)(eth_data + 16));
		eth_data_len = 18;
		if (pkt_type != 0x0800)
		{
			printf("Unknow VLAN flag!\n");
			return -1;
		}
	}
	if(0x8847 == pkt_type) //mpls
	{
		eth_data_len = 18;
	}
	else if (pkt_type != 0x0800) 
	{
		printf("Not is IP protocol!\n");
		return -1;
	}

	iph = (struct iphdr*)(eth_data + eth_data_len);
	iph_len = iph->ihl << 2;

	if(pktDataLen <= eth_data_len + iph_len)
	{
		printf("Paket Length error!\n");
		return -1;
	}
	if(ntohs(iph->tot_len) > 1600)
	{
		printf("IP paket Length error!\n");
		return -1;
	}
	if(IPPROTO_GRE == iph->protocol)
	{
		if((*((u_char*)iph + iph_len + 2) == 0x08) && (*((u_char*)iph + iph_len + 3) == 0x0))
		{
			iph = (struct iphdr*)((u_char*)iph + iph_len + 8);
		}
		else
		{
			printf("IPPROTO GRE error!\n");
			return -1;
		}
	}

	if (IPPROTO_TCP == iph->protocol)
	{
		tcph = (struct tcphdr*)(((u_char*)iph) + iph_len);
		hdr_len = tcph_len = tcph->doff << 2;
		if(pktDataLen < eth_data_len + iph_len + hdr_len)
		{
			printf("TCP paket Length error!\n");
			return -1;
		}
		usr_data = (u_char*)tcph + hdr_len;
		usr_data_len = ntohs(iph->tot_len) - iph_len - hdr_len;
		sport=tcph->source;
		dport=tcph->dest;
         
		if(gconfig.protocal_type ==  TCP)
		{
			sendto(udpfd,usr_data+gconfig.user_data_offset,usr_data_len-gconfig.user_data_offset,0,(const struct sockaddr *)dst,sizeof(struct sockaddr_in));
			usleep(gconfig.sleep_time);
		}
		//set packet info
		//pkt_info.protocol_type = iph->protocol;
		//strcpy(pkt_info.protocol_desc, "TCP");
	}
	else if (IPPROTO_UDP == iph->protocol)
	{
		udph = (struct udphdr*)(((u_char*)iph) + iph_len);
		hdr_len = udph_len = sizeof(struct udphdr);
		if(pktDataLen <= eth_data_len + iph_len + hdr_len)
		{
			printf("UDP paket Length error!\n");
			return -1;
		}
		usr_data = (u_char*)udph + hdr_len;
		usr_data_len = ntohs(iph->tot_len) - iph_len - hdr_len;
		sport=udph->source;
		dport=udph->dest;
		//if(count == 100 )
		{
		  //count = 0;
		  //usleep(100);
		}

	    //if(NULL != strstr(usr_data,"dash?") )

		 if(gconfig.protocal_type == UDP)
		 {
		 	sendto(udpfd,usr_data+gconfig.user_data_offset,usr_data_len-gconfig.user_data_offset,0,(const struct sockaddr *)dst,sizeof(struct sockaddr_in));
		 	usleep(gconfig.sleep_time);
		 }
	   
		//count++;
			
	}
	else
	{
		printf("Not is tcp or udp packet!\n");
		return -1;
	}

	
	return 0;
}


int parse_packet1( const char *pcap_name, int udpfd, struct sockaddr_in *dst)
{

   if( NULL == pcap_name || udpfd <= 0 || access(pcap_name,F_OK) != 0 ) 
   	{
   	   printf("%s %d, pcap or sock or dst is NULL, or perhaps related pcap doesn't exist,please check !\n",__FUNCTION__,__LINE__);
   	   return -1;
   	}

   char ebuf[PCAP_ERRBUF_SIZE] = {0};
   pcap_t *p_handle = pcap_open_offline(pcap_name,ebuf);
   struct pcap_pkthdr pkthdr;

    while(1)
    {
      const u_char *pktstr = pcap_next(p_handle,&pkthdr);
	  if( pktstr == NULL )
	  	return -1;

	  proc_packet(p_handle,&pkthdr,pktstr,udpfd,dst);
    }
	
  	pcap_close(p_handle);
}
