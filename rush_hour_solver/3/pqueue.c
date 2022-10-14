#include <stdlib.h>
#include "pqueue.h"
#include "node.h"
#include "util.h"

pqueue_t * pqueue_alloc(void)
{
  pqueue_t * pq;
  
  pq = (pqueue_t*)malloc(sizeof(pqueue_t));
  pq->m_size = 0;
  pq->m_first = NULL;
  
  return pq;
}

void pqueue_free(pqueue_t * pq)
{
  free(pq);
}

BOOL pqueue_is_empty(pqueue_t * pq)
{
  if(pq->m_size == 0 || pq->m_first == NULL)
  {
    return TRUE;
  }
  return FALSE;
}

void pqueue_push(pqueue_t * pq, node_t * n)
{
  node_t * p;

  /* Increment number of nodes in the priority queue */
  pq->m_size ++;
  
  /* When adding a new node, we come across 3 situations */

	/* Situation 1 - No elements currently stored */
	if(pqueue_is_empty(pq) == TRUE)
	{
	  /* Insert first node */
	  pq->m_first = n;
	  n->m_next = NULL;
	  n->m_previous = NULL;
		return;
	}

	/* Situation 2 - Node n has better score than the first node*/
	if(n->m_score < pq->m_first->m_score)
	{
	  /* Insert n before first */
	  pq->m_first->m_previous = n;
	  n->m_next = pq->m_first;
	  n->m_previous = NULL;
	  pq->m_first = n;
		return;
	}

	/* Situation 3 - Node n's score is lower than first's */
	for(p = pq->m_first; /*p != NULL*/; p = p->m_next)
	{
	  /* Situation 3.1 - Node n's score is better than p's */
		if(n->m_score < p->m_score)
		{
			/* Insert n before p */
			p->m_previous->m_next = n;
			n->m_previous = p->m_previous;
			n->m_next = p;
			p->m_previous = n;
			return;
		}
		
		/* Situation 3.2 - n has the worst score (no nodes left) */
		if(p->m_next == NULL)
		{
			/* Insert n as last node */
			p->m_next = n;
			n->m_previous = p;
			n->m_next = NULL;
			return;
		}
	}
}

node_t * pqueue_pop(pqueue_t * pq)
{
  /* The node with the best score is returned */
  node_t * node_with_best_score = pq->m_first;

  /* Decrease number of nodes in the priority queue */
  pq->m_size --;
  
  /* Second in queue is now first, if possible */
  if(pq->m_first != NULL)
  {
    pq->m_first = pq->m_first->m_next;
  }
  
  return node_with_best_score;
}

node_t * pqueue_find_by_vec_car_heads(pqueue_t * pq,node_t * n)
{
  node_t * p;
  
  for(p = pq->m_first; p != NULL; p = p->m_next)
  {
    if(node_same_vec_car_heads(p,n) == TRUE)
    {
      return p;
    }
  }

  /* No node in pq has the same payload as n */
  return NULL;
}

