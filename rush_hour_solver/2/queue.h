/*------------------------------------------------------------------------------
 queue.h
------------------------------------------------------------------------------*/
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "node.h"

//coada cu prioritate: nodul cu euristica minima este pastrat la inceput
struct Queue
{
	int		count;	
	Node*	first;

	Queue	(void);

	bool	IsEmpty(void);
	void	Add(Node *n);
	Node*	ExtractMin(void);
	Node*	GetNodeLike(Node *reference, int num_cars);
};

Queue::Queue()
{
	count = 0;
	first = NULL;
}

bool Queue::IsEmpty()
{
	return this->first == NULL;
}

//adauga nodul ordonat in lista
void Queue::Add(Node *n)
{
	//creste nr de elemente
	this->count++;

	//nici un element
	if(IsEmpty())
	{
		this->first = n;
		this->first->previous = NULL;
		this->first->next = NULL;
		return;
	}

	//insereaza la inceput
	if(this->first->heuristic > n->heuristic)
	{
		this->first->previous = n;
		n->next = this->first;
		n->previous = NULL;
		this->first = n;
		return;
	}

	//insereaza la pozitia corespunzatoare
	for(Node *p = this->first; p != NULL; p = p->next)
	{
		if(n->heuristic < p->heuristic)
		{
			//insereaza nodul n inainte de pozitia p
			p->previous->next = n;
			n->previous = p->previous;
			n->next = p;
			p->previous = n;
			return;
		}
		if(p->next == NULL)
		{
			//insereaza la sfarsit
			p->next = n;
			n->previous = p;
			n->next = NULL;
			return;
		}
	}
}

//extrage primul element din lista, minimul
Node* Queue::ExtractMin()
{
	count--; // scade nr de elemente
	Node *min = this->first; //elem minim e mereu primul

	if(this->first != NULL)
		this->first = this->first->next;//urmatorul la rand e minim acum

	return min;
}

//cauta in lista un nod cu car_positions identic cu cel al nodului referinta
Node* Queue::GetNodeLike(Node *reference, int num_cars)
{
	//parcurge lista
	Node *current = this->first;
	while(current != NULL)
	{
		//daca nodul curent are payload-ul identic cu al referintei
		if(current->ComparePositions(reference,num_cars))
			return current;//este intors

		//altfel trec la nodul urmator
		current = current->next;
	}

	//nu a fost gasit => reference nu este in Queue
	return NULL;
}

#endif//_QUEUE_H_
