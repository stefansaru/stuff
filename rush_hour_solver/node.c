#include <stdlib.h>
#include "pqueue.h"
#include "node.h"

extern int g_map_num_cars;

node_t * node_alloc()
{
  node_t * n;
  
  n = (node_t*)malloc(sizeof(node_t));
  n->m_children = pqueue_alloc();
  n->m_vec_car_heads = (int*)malloc(2*g_map_num_cars*sizeof(int));
  
  return n;
}

void node_free(node_t * n)
{
  pqueue_free(n->m_children);
  free(n->m_vec_car_heads);
  free(n);
}

BOOL node_same_vec_car_heads(node_t * a,node_t * b)
{
  int i;
  
  for(i = 0; i < (2 * g_map_num_cars); i++)
  {
    if(a->m_vec_car_heads[i] != b->m_vec_car_heads[i])
    {
      return FALSE;
    }
  }
  
  return TRUE;
}

void node_copy_vec_car_heads(node_t * src, node_t * dst)
{
  int i;
  int * ptr_src;
  int * ptr_dst;
  
  ptr_src = src->m_vec_car_heads;
  ptr_dst = dst->m_vec_car_heads;
  
  for(i = 0; i < (2 * g_map_num_cars); i++)
  {
    *ptr_dst = *ptr_src;
    ptr_dst++;
    ptr_src++;
  }
}

