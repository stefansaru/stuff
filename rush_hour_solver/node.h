#ifndef NODE
#define NODE

#include "pqueue.h"
#include "util.h"

struct node 
{
  /* Double Linked-List structure */
  struct node *   m_next;
  struct node *   m_previous;
  
  /* Graph structure */
  int             m_depth;
  struct node *   m_parent;
  struct pqueue * m_children;
  
  /* Payload */
  int *           m_vec_car_heads;
  
  /* A* heuristic score */
  int             m_score;
};

typedef struct node node_t;

node_t * node_alloc();
void     node_free(node_t * n);
BOOL     node_same_vec_car_heads(node_t * a,node_t * b);
void     node_copy_vec_car_heads(node_t * src, node_t * dst);

#endif/*NODE*/

