/*c stardard section*/
#include <stdio.h>
#include <stdlib.h>




/* local headers section*/
#include "packet_list.h"





static int free_node_data( struct _node_s *node )
{
   if( node == NULL )
   	return -1;

   free(node->data);
   return 0;
}


struct list_s *create_list()
{
    struct list_s *list = (struct list_s *)malloc(sizeof(struct list_s));
	if( list == NULL )
		return NULL;

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return list;
	
}
int destroy_list(struct list_s *list)
{
  	if( NULL == list )
		return -1;

	if( list->size == 0 )
		return 0;

	struct _node_s *n = list->head;
	struct _node_s *tmp = NULL;

	while( n != NULL )
	{
		tmp = n;
		n = n->next;
	    free_node_data(tmp);
		free(tmp);
	}

	free(list);
  
}



int list_head_insert(struct list_s *list,void *data)
{

   if( data == NULL || NULL == list)
   	return -1;

	struct _node_s *dnode = (struct _node_s *)malloc(sizeof(struct _node_s));

	if( NULL == dnode )
		return -1;

	dnode->prev = dnode->next = NULL;
	dnode->data = (void *)data;

	if( list->size == 0 )
	{
		list->head = list->tail = dnode;
		list->size += 1;
		
	}
	else
	{
	   dnode->next = list->head;
	   list->head->prev = dnode;
	   list->head  = dnode;
	   list->size += 1;
	}
   
  return 0;
}



int list_tail_insert(struct list_s *list,void *data)
{ 

	if( NULL == list || NULL == data )
		return -1;

	struct _node_s *dnode;
	dnode = (struct _node_s *)malloc(sizeof(struct _node_s));
	if( NULL == dnode ) return -1;

	dnode->prev = dnode->next = NULL;
	dnode->data = data;

    if( list->size == 0 )
    {
        list->head = list->tail = dnode;
        list->size += 1;
    }
	else
	{
	  dnode->prev = list->tail;
	  list->tail->next = dnode;
	  list->tail = dnode;
	  
	  list->size += 1;
	}
	
}




