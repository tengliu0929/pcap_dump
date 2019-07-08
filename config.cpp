
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "config.h"
#include "tinyxml2.h"
#include "packet_list.h"


using namespace std;
using namespace tinyxml2;

/*



   {
   "dstip":"192.168.233.139",
   "destport":9054
   "protocal":"udp",
   "user_data_offset":0,
   "sleep":0,
   "send_interval_packet":100,
   "packages":
   [
   "/home1/1.pcap",
   "/home2/2.pcap",
   "/home3/pcap"
   ]
   }

*/


static int process_rn( char *p )
{
	if( NULL == p  )
		return -1;

	char *prn = NULL, *cur = NULL;
	cur = p;
	while( (prn = strstr(cur,"\r\n")) != NULL )
	{
		cur += 2;
		memset(prn,0,2);
	}
	return 0;
}

int parse_config(const char *path, pcap_config_t *config)
{
	if( NULL == path || NULL == config )
		return -1;


	XMLDocument doc;

	if( doc.LoadFile(path) != 0 )
	{
		cout<<"load failed"<<endl;
		return -1;
	}
	XMLElement *root = doc.RootElement();
	if( root == NULL )
		return -1;


	XMLElement *ip = root->FirstChildElement("ip");
	XMLElement *port = root->FirstChildElement("port");
	XMLElement *protocal = root->FirstChildElement("protocal");
	XMLElement *user_data_offset = root->FirstChildElement("user_data_offset");
	XMLElement *sleep = root->FirstChildElement("sleep");
	XMLElement *send_packet_interval = root->FirstChildElement("send_packet_interval");
	XMLElement *packeges = root->FirstChildElement("packeges");

	if( NULL == ip || NULL == port || NULL == protocal  || NULL == user_data_offset || NULL == sleep \
			|| send_packet_interval == NULL || NULL == packeges)
		return -1;

	strcpy(config->ip,ip->GetText());
	config->port = atoi(port->GetText());

	if( strcmp(protocal->GetText(),"udp") == 0 )
		config->protocal_type = (enum protocal_type_s)1;
	else if(strcmp(protocal->GetText(),"tcp") == 0 )
		config->protocal_type=(enum protocal_type_s)2;
	else 
		return -1;


	config->user_data_offset = atoi(user_data_offset->GetText());

	config->sleep_time = atoi(sleep->GetText());
	config->send_interval_packet = atoi(send_packet_interval->GetText());



	cout<<config->ip<<endl;

	XMLElement *node = packeges->FirstChildElement("node");

	config->list = create_list();

	while(node)
	{
		const XMLAttribute *attr = node->FirstAttribute();

		if( NULL == attr)
		{
			cout<<"attr is null"<<endl; 
			node = node->NextSiblingElement();
			continue;
		}


		if( strcmp(attr->Name(),"path") == 0)
			list_head_insert((list_s*)config->list,(void *)attr->Value());

		node = node->NextSiblingElement();

	}


	return 0;

}
