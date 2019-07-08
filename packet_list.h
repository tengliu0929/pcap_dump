#ifndef __PAKCET_LIST_H__
#define __PAKCET_LIST_H__


#ifdef   __cplusplus
extern "C"
{
#endif
// list node definition
struct _node_s
{
  struct _node_s *prev;
  struct _node_s *next;
  void *data;
};

struct list_s
{
  struct _node_s *head;
  struct _node_s *tail;
  int size;
};

//int parse_packet1( const char *pcap_name, int udpfd, struct sockaddr_in *dst);



struct list_s *create_list();
int destroy_list(struct list_s *list);
int list_head_insert(struct list_s *list,void *data);
int list_tail_insert(struct list_s *list,void *data);

 
#ifdef   __cplusplus
}
#endif

#endif //__PAKCET_LIST_H__

