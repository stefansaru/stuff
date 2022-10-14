#ifndef PQUEUE
#define PQUEUE

#include "node.h"
#include "util.h"

struct pqueue
{
  int           m_size;
  struct node * m_first;
};

typedef struct pqueue pqueue_t;

pqueue_t *      pqueue_alloc                 (void);
void            pqueue_free                  (pqueue_t * pq);
BOOL            pqueue_is_empty              (pqueue_t * pq);
void            pqueue_push                  (pqueue_t * pq,struct node * n);
struct node *   pqueue_pop                   (pqueue_t * pq);
struct node *   pqueue_find_by_vec_car_heads (pqueue_t * pq,struct node * n);

#endif/*QUEUE*/

