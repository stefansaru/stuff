/*------------------------------------------------------------------------------
 node.h
------------------------------------------------------------------------------*/
#ifndef _NODE_H_
#define _NODE_H_


#include "car.h"

//Node este utilizat in coada cu prioritati Queue (queue.h) folosind 
//referintele next si previous, sortate in coada dupa heuristic. 
//Node mai apare in graful cautarii A* din game.h, folosind parent,
//children,...
//Node contine informatia utila car_positions - pozitiile masinilor 
//sunt relative la constantele lungime,directie,linie(directie=oriz)
//sau coloana(directie=vert) - vezi car.h - iar pentru a folosi cat
//mai putina memorie, numarul de masini este dat ca parametru
struct Node
{
	//ca lista dublu inlantuita
	Node*	next;
	Node*	previous;

	//ca graf
	Node*	parent;
	Node**	children;
	int		num_children;
	int		depth;
	int		heuristic;

	//payload
	int*	car_positions;

	//constructor: aloca spatiu pentru payload
	Node(int num_cars)
	{
		this->car_positions = (int*)MyAlloc(num_cars,sizeof(int));
		this->num_children = 0;
	}

	//destructor: dealoca spatiul folosit de payload
	~Node()
	{
		if(this->car_positions != NULL)
			free((void*)this->car_positions);
	}

	//copiaza  payload-ul de la un alt nod
	void CopyPositions(Node *another, int num_cars)
	{
		for(int i = 0; i < num_cars; i++)
			this->car_positions[i] = another->car_positions[i];
	}

	//compara payload-ul cu cel al altui nod
	bool ComparePositions(Node *another, int num_cars)
	{
		for(int i = 0; i < num_cars; i++)
			if(this->car_positions[i] != another->car_positions[i])
				return false;
		return true;
	}
};

#endif//_NODE_H_
