#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <unistd.h>

#include "config.h"
#include "packet_parse.h"
#include "packet_list.h"




char *ip = "192.168.233.139";
int port = 9054;
char *path = NULL;


pcap_config_t gconfig;




int param_check(int argc, char **argv)
{
	int c;
  	
	while ( ( c = getopt(argc,argv,"R:")) != -1)
    switch( c )
	{
          
		case 'R':
			path  = optarg;
			break;
	    default:
	      return -1;
    }
	argc -= optind; argv += optind;

	return 0;
}


int create_sock()
{
   int fd = -1;

   fd = socket(AF_INET,SOCK_DGRAM,0);
   if(fd < 0 )
   	return -1;

   return fd;
}


int main(int argc, char **argv)
{
    param_check(argc,argv);

	if( path == NULL || access(path,F_OK) != 0)
		return -1;
	


	parse_config(path,&gconfig);
	int sock = 0;
	struct sockaddr_in dst;
	struct list_s *list = (struct list_s *)(gconfig.list);
	if( list == NULL )
	{
	  printf("list is NULL\n");
	  return -1;
	}
	

	sock = create_sock();
	if( sock <= 0 ) return -1;

	memset(&dst,0,sizeof(struct sockaddr_in));
	dst.sin_family = AF_INET;
	dst.sin_addr.s_addr = inet_addr(gconfig.ip);
	dst.sin_port = htons(gconfig.port);

	//parse_packet1(path,sock,&dst);

	struct _node_s  *pnode = list->head;

	while(pnode != NULL )
	{
	  parse_packet1((const char *)pnode->data, sock, &dst);
	  pnode = pnode->next;
	}

	
  
	return 0;
}


int main1()
{
    parse_config("config.xml", &gconfig);
	int a = 0;
	int b = 0;
    return 0;
}
