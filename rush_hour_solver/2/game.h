/*------------------------------------------------------------------------------
 game.h
------------------------------------------------------------------------------*/
#ifndef _GAME_H_
#define _GAME_H_

#include "util.h"
#include "queue.h"
#include <time.h>

class Game
{
	Node*	root;			//nodul radacina

	Car**	cars;			//vecrtorul de masini
	int		num_cars;		//nr de masini
	int		main_car_index;	//indexul masinii principale(in vectorii folositi ulterior)
	int		main_car_solved;//pozitia masinii principale(in solutie)

	//harta
	int		width;			//nr coloane
	int		height;			//nr linii
	int		exit_x;			//linia pt iesire
	int		exit_y;			//coloana pt iesire
	int		exit_direction;	//directia de iesire (HORIZONTAL/VERTICAL)
	int		exit_border;	//latura de iesire (UP/DOWN/LEFT/RIGHT)

public:

	Game(const char *input_file_name);
	~Game();

	void	AStarSolve(const char * output_file);

private:

	Node*	ChildFromMove(Node *parent, int car_index, int offset);
	void	ComputeChildren(Node *parent);
	int		ComputeHeuristic(Node *n);
	int		ComputeSolvedPosition(Node *root, Car **cars);
	Car**	CreateCars(char **map, int height, int width, int num_cars);
	char**	CreateMap(const char *input_file_name, int *height, int *width);
	Node*	CreateRootNode(Car **cars, int num_cars);
	void	DeleteCars(Car **cars);
	void	DeleteMap(char **map, int height);
	void	DeleteNodesFrom(Node *n);
	int		GetBlockingExitCarCount(Node *n);
	int		GetCarCount(char **map, int height, int width);
	int		GetDistanceToExit(Node *n);
	bool	IsBlockingExit(int car_index, Node *n);
	bool	IsFree(int x, int y, Node *n);
	bool	IsSolution(Node *n);
	char**	Node2Map(Node *node);
	void	PrintMap(char **map, int height, int width, FILE *stream);
	void	ViewSolution(Node **sol_vect, int count);
	void	WriteSolution(const char *output_file, Node *sol);

};
/*------------------------------------------------------------------------------
 Game() constructor
		- initializeaza variabilele membre
------------------------------------------------------------------------------*/
Game::Game(const char * input_file_name)
{
	char **map				= CreateMap(input_file_name, &this->height, &this->width);//incarca harta
	this->num_cars			= GetCarCount(map,this->height,this->width);//numara masinile din harta
	this->cars				= CreateCars(map,this->height,this->width,this->num_cars);//masinile

	DeleteMap(map,this->height); //nu mai este nevoie de map

	this->root				= CreateRootNode(this->cars,this->num_cars);//nodul de start
	this->main_car_solved	= ComputeSolvedPosition(this->root,this->cars);//pozitia solutie pentru masina principala
}
/*------------------------------------------------------------------------------
 Game() destructor
		- dealoca memoria
------------------------------------------------------------------------------*/
Game::~Game()
{
	if(this->cars != NULL)			//masini
		free((void*)this->cars);

	DeleteNodesFrom(this->root);	//arborele pornind din root
}

/*------------------------------------------------------------------------------
 AStarSolve
		- cauta solutia pornind din Game::root
		- daca nu gaseste solutie -> exit(FAILURE)
		- altfel cheama WriteSolution()
		- contorizeaza timpul si adancimea
------------------------------------------------------------------------------*/
void Game::AStarSolve(const char * output_file)
{
	printf("\n caut solutie");

	unsigned start_time = (unsigned)time(NULL);

	Queue *open = new Queue();
	Queue *closed = new Queue();

	open->Add(this->root);
	Node *current_node = NULL;
	Node *current_child = NULL;
	Node *old_child = NULL;
	bool better_choice = false;
	bool found_solution = false;

	int node_count = 0;

	while(open->IsEmpty() == false)
	{
		node_count++;
		if(node_count % 100 == 0) printf(".");

		current_node = open->ExtractMin();
		if(this->IsSolution(current_node)) 
		{
			found_solution = true;
			break;
		}
		
		ComputeChildren(current_node);

		closed->Add(current_node);

		for(int child_index = 0; child_index < current_node->num_children; child_index++)
		{
			current_child = current_node->children[child_index];

			if(closed->GetNodeLike(current_child,this->num_cars) != NULL) 
				continue; // deja in closed

			old_child = open->GetNodeLike(current_child,this->num_cars);

			if(old_child == NULL) // copilul curent nu se afla in open
			{
				open->Add(current_child);
				better_choice = true;
			}
			else// copilul curent asemenea cu cel din open (old_child)
			{
				better_choice = (old_child->depth + old_child->heuristic) > (current_child->depth + current_child->heuristic);
			}

			if(better_choice == true)
			{
				current_child->parent = current_node;
				current_child->depth = current_node->depth + 1;
			}
		}//end for
	}//end while

	if(found_solution == false) MyExit(FAILURE);

	unsigned total_time = (unsigned)time(NULL) - start_time;

	printf("\n gasit dupa %u secunde, solutie in %i mutari",total_time,current_node->depth);
	WriteSolution(output_file,current_node);
}
/*------------------------------------------------------------------------------
 ChildFromMove
		- creeaza o copie a nodului parinte in care s-a mutat masina specificata
		  la indexul specificat
------------------------------------------------------------------------------*/
Node* Game::ChildFromMove(Node *parent, int car_index, int offset)
{
	Node *child = new Node(this->num_cars);
	child->CopyPositions(parent,this->num_cars);
	child->car_positions[car_index] += offset;
	return child;
}
/*------------------------------------------------------------------------------
 DeleteNodesFrom
		- stergere recursiva a subarborelui care porneste din nodul n
------------------------------------------------------------------------------*/
void Game::DeleteNodesFrom(Node *n)
{
	if(!n) return;
	
	for(int i = 0; i < n->num_children; i++)
		DeleteNodesFrom(n->children[i]);
	
	delete n;
}
/*------------------------------------------------------------------------------
 CreateMap
		- aloca memorie pt harta Game::map si citeste harta din fisier
------------------------------------------------------------------------------*/
char** Game::CreateMap(const char *input_file_name, int *height, int *width)
{
	printf("\n deschid [%s]",input_file_name);
	FILE *in=fopen(input_file_name,"rt");

	if(in == NULL) MyExit(BAD_INPUT);

	fscanf(in,"%i%i",height,width);
	char **map = (char**)MyAlloc(*height,sizeof(char**));

	char c;
	for(int i = 0; i < *height; i++)
	{
		map[i] = (char*)MyAlloc(*width,sizeof(char*));
		for(int j = 0; j < *width; j++)
		{
			do fscanf(in,"%c",&c);
			while (c == ' ' || c == '\t' || c == '\n');

			map[i][j] = c;
		}
	}
	fclose(in);
	return map;
}
/*------------------------------------------------------------------------------
 PrintMap
		- tipareste harta map la un stream
------------------------------------------------------------------------------*/
void Game::PrintMap(char **map, int height, int width, FILE *stream)
{
	fprintf(stream,"\n");
	for(int i = 0; i < height; i++)
	{
		fprintf(stream,"\n ");
		for(int j = 0; j < width; j++)
			fprintf(stream,"%c",map[i][j]);
	}
}
/*------------------------------------------------------------------------------
 DeleteMap
		- elibereaza memoria ocupata de harta map
------------------------------------------------------------------------------*/
void Game::DeleteMap(char **map, int height)
{
	if(!map) return;

	for(int i = 0; i < height; i++)
		if(map[i] != NULL)
			free((void*)map[i]);

	if(map != NULL) free((void*)map);
}
/*------------------------------------------------------------------------------
 GetCarCount
		- numara masinile din harta map, numarand "capetele" de masini, adica
			- SQ_HORIZ_HEAD
			- SQ_VERTI_HEAD
			- SQ_MAIN_CAR
------------------------------------------------------------------------------*/
int Game::GetCarCount(char **map, int height, int width)
{
	int count = 0;
	bool found_main_car = false;

	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			switch(map[i][j])
			{
			case SQ_HORIZ_HEAD:
			case SQ_VERTI_HEAD: count++;
			case SQ_MAIN_CAR:
				{
					if(found_main_car) break;
					found_main_car = true;
					count++;
				};break;
			default: break;
			}
	return count;
}
/*------------------------------------------------------------------------------
 CreateCars
		- genereaza un vector pt masinile din harta map
		- calculeaza lungimea si orientarea masinilor
------------------------------------------------------------------------------*/
Car **Game::CreateCars(char **map, int height, int width, int num_cars)
{
	bool found_main_car = false;

	//aloca memorie pt masini
	Car **cars = (Car**)calloc(num_cars,sizeof(Car*));

	//calculeaza lungimea si orientarea masinilor
	Car *current;
	int index = 0;
	found_main_car = false;
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			switch(map[i][j])
			{
			case SQ_HORIZ_HEAD:
				{
					current = new Car();
					current->direction = HORIZONTAL;
					current->SetLocation(i,j);
					for(int k = j ; k < width; k++)
					{
						current->length++;
						if(map[i][k] == SQ_HORIZ_BODY) continue;
						if(map[i][k] == SQ_HORIZ_TAIL) break;
					}
					cars[index] = current;
					index++;
				};break;
			case SQ_VERTI_HEAD:
				{
					current = new Car();
					current->direction = VERTICAL;
					current->SetLocation(i,j);
					for(int k = i ; k < height; k++)
					{
						current->length++;
						if(map[k][j] == SQ_VERTI_BODY) continue;
						if(map[k][j] == SQ_VERTI_TAIL) break;
					}
					cars[index] = current;
					index++;
				};break;
			case SQ_MAIN_CAR:
				{
					if(found_main_car) break;
					found_main_car = true;

					current = new Car();
					current->SetLocation(i,j);

					if(map[i][j+1] == SQ_MAIN_CAR)
					{
						this->exit_direction = HORIZONTAL;
						current->direction = HORIZONTAL;
						for(int k = j ; k < width; k++)
						{
							if(map[i][k] != SQ_MAIN_CAR) break;
							current->length++;
						}
					}
					else if(map[i+1][j] == SQ_MAIN_CAR)
					{
						this->exit_direction = VERTICAL;
						current->direction = VERTICAL;
						for(int k = i ; k < width; k++)
						{
							if(map[k][j] != SQ_MAIN_CAR) break;
							current->length++;
						}
					}

					this->main_car_index = index;
					cars[index] = current;
					index++;
				};break;
			case SQ_EXIT:
				{
					//coordonatele de iesire
					this->exit_x = i;
					this->exit_y = j;

					//latura de iesire
					if(i == 0)				this->exit_border = UP;
					else if(i == height-1)	this->exit_border = DOWN;
					else if(j == 0)			this->exit_border = LEFT;
					else/*if(j == width-1)*/this->exit_border = RIGHT;
				};
			default: break;
			}//end switch
		return cars;
}
/*------------------------------------------------------------------------------
 WriteSolution
		- scrie in fisier + afiseaza pe ecran solutia
------------------------------------------------------------------------------*/
void Game::WriteSolution(const char *output_file, Node *sol)
{
	int depth = sol->depth;

	//pornind de la nodul parinte spre radacina
	//scrie in sol_vector nodurile parcurse
	Node **sol_vector = (Node**)calloc(depth + 1,sizeof(Node*));
	for(int i = depth; i >= 0; i--)
	{
		sol_vector[i] = sol;
		sol = sol->parent;
	}

	printf("\n scriu solutie in [%s]",output_file);

	FILE *out = fopen(output_file,"wt");
	
	//parcurge vectorul construit anterior in sens opus
	char **current_map;
	for(int i = 0; i <= depth; i++)
	{
		current_map = Node2Map(sol_vector[i]);	//fac harta din nod
		PrintMap(current_map,this->height,this->width,out);	//tiparesc harta in fisier
		DeleteMap(current_map,this->height);	//sterg harta
	}

	//afisarea pe ecran
	ViewSolution(sol_vector, depth + 1);

	free((void*)sol_vector);
	fclose(out);
}
/*------------------------------------------------------------------------------
 CreateRootNode
		- stabileste o radacina dintr-un vector de masini
		- determina pozitiile initiale
			- pt o masina care se misca pe orizontala (linie) va conta coloana
			- iar pt masina care se misca pe verticala(coloana), va conta linia
------------------------------------------------------------------------------*/
Node* Game::CreateRootNode(Car **cars, int num_cars)
{
	Node *node = new Node(num_cars);
	for(int i = 0; i < num_cars; i++)
	{
		if(cars[i]->direction == HORIZONTAL)
		{
			//pozitia initiala este coloana pt ca se misca pe linie
			node->car_positions[i] = cars[i]->y;
		}
		else//if(this->cars[i]->direction == VERTICAL)
		{
			//pozitia initiala este linia pt ca se misca pe coloana
			node->car_positions[i] = cars[i]->x;
		}
	}
	node->depth = 0;
	return node;
}
/*------------------------------------------------------------------------------
 ComputeSolvedPosition
		- calculeaza pozitia relativa pentru masina principala la solutie
------------------------------------------------------------------------------*/
int Game::ComputeSolvedPosition(Node *root, Car **cars)
{
	int main_car_head	= root->car_positions[main_car_index];
	int main_car_length	= cars[main_car_index]->length;
	int main_car_tail	= main_car_head + main_car_length;
	int main_car_line	= cars[main_car_index]->x;
	int main_car_column	= cars[main_car_index]->y;

	if(this->exit_direction == HORIZONTAL)
	{
		if(this->exit_border == RIGHT)
		{
			return main_car_head + (this->exit_y - main_car_tail) + 1;
		}
		else //if(this->exit_border == LEFT)
		{
			return 0;//margine
		}
	}
	else //if(this->exit_direction == VERTICAL)
	{
		if(this->exit_border == DOWN)
		{
			return main_car_head + (this->exit_x - main_car_tail) + 1;
		}
		else //if(this->exit_border == UP)
		{
			return 0;//margine
		}
	}

	return -1;// nu se va ajunge aici
}
/*------------------------------------------------------------------------------
 ComputeChildren
		- adauga copii la un nod parent
		- orice mutare a oricarei masini este considerata un nod
		- copiii sunt pastrati intr-o coada, apoi adaugati la vectorul de copii
		  al parintelui
------------------------------------------------------------------------------*/
void Game::ComputeChildren(Node *parent)
{
	Queue *babies = new Queue();

	//verifica mutari pt toate masinile
	for(int car_index = 0; car_index < this->num_cars; car_index++)
	{
		int head = parent->car_positions[car_index];
		int length = this->cars[car_index]->length;
		int direction = this->cars[car_index]->direction;
		
		if(direction == VERTICAL)
		{
			int column = this->cars[car_index]->y;

			//mutari in sus
			for(int offset = -1; IsFree(head+offset,column,parent); offset--)
			{
				Node *baby = ChildFromMove(parent,car_index,offset);
				ComputeHeuristic(baby);
				babies->Add(baby);
			}

			//mutari in jos
			for(int offset = 0; IsFree(head+length+offset,column,parent); offset++)
			{
				Node *baby = ChildFromMove(parent,car_index,offset+1);
				ComputeHeuristic(baby);
				babies->Add(baby);
			}
		}
		else //if(direction == HORIZONTAL)
		{
			int line = this->cars[car_index]->x;

			//mutari la stanga
			for(int offset = -1; IsFree(line,head+offset,parent); offset--)
			{
				Node *baby = ChildFromMove(parent,car_index,offset);
				ComputeHeuristic(baby);
				babies->Add(baby);
			}

			//mutar la dreapta
			for(int offset = 0; IsFree(line,head+length+offset,parent); offset++)
			{
				Node *baby = ChildFromMove(parent,car_index,offset+1);
				ComputeHeuristic(baby);
				babies->Add(baby);
			}
		}
	}//end for

	//aloc mem
	parent->num_children = babies->count;
	parent->children = (Node**)calloc(parent->num_children,sizeof(Node*));

	//adaugare
	for(int i = 0; babies->IsEmpty() == false; i++)
	{
		parent->children[i] = babies->ExtractMin();
		parent->children[i]->depth = parent->depth+1;
	}

	delete babies;
}
/*------------------------------------------------------------------------------
 Node2Map
		- creeaza o harta dintr-un nod, conform payload-ului car_positions,
		  relativ la vectorul de masini Game::cars
------------------------------------------------------------------------------*/
char** Game::Node2Map(Node *node)
{
	//aloca si creeaza harta goala
	char ** map = (char**)calloc(this->height,sizeof(char*));
	for(int i = 0; i < this->height; i++)
	{
		map[i] = (char*)calloc(this->width,sizeof(char));
		for(int j = 0; j < this->width; j++)
			map[i][j] = (i == 0 || j == 0 || i == this->height-1 || j == this->width-1) ? '0' : '-';
	}
	map[this->exit_x][this->exit_y] = '*';

	//adauga masini
	for(int car_index = 0; car_index < this->num_cars; car_index++)
	{
		int head = node->car_positions[car_index];
		int length = this->cars[car_index]->length;
		int tail = head + length - 1;
		int direction = this->cars[car_index]->direction;

		if(direction == VERTICAL)
		{
			int column = this->cars[car_index]->y;

			//head
			map[head][column] = (car_index == this->main_car_index) ? SQ_MAIN_CAR : SQ_VERTI_HEAD;

			//body
			for(int i = head + 1; i < tail; i++)
				map[i][column] = (car_index == this->main_car_index) ? SQ_MAIN_CAR : SQ_VERTI_BODY;

			//tail
			map[tail][column] = (car_index == this->main_car_index) ? SQ_MAIN_CAR : SQ_VERTI_TAIL;
		}
		else//if(direction == HORIZONTAL)
		{
			int line = this->cars[car_index]->x;
			
			//head
			map[line][head] = (car_index == this->main_car_index) ? SQ_MAIN_CAR : SQ_HORIZ_HEAD;

			//body
			for(int i = head + 1; i < tail; i++)
				map[line][i] = (car_index == this->main_car_index) ? SQ_MAIN_CAR : SQ_HORIZ_BODY;

			//tail
			map[line][tail] = (car_index == this->main_car_index) ? SQ_MAIN_CAR : SQ_HORIZ_TAIL;
		}
	}

	return map;
}
/*------------------------------------------------------------------------------
 ViewSolution
		- este apelata doar din WriteSolution
		- afiseaza so_vect obtinut la WriteSolution
------------------------------------------------------------------------------*/
void Game::ViewSolution(Node **sol_vect, int count)
{
	printf("\n vezi solutie: \'S\' = stanga & \'D\' = dreapta)");
	printf("\n apasa o tasta");
	_getch();
	int i = 0;

	char **current_map;
	while(i < count)
	{
		system("cls");
		printf("mutare %i/%i\n",i,count-1);
		if(i == 0)
			printf(" (start)");
		if(i == count - 1)
			printf(" (solutie)");

		current_map = Node2Map(sol_vect[i]);
		PrintMap(current_map, this->height, this->width, stdout);
		DeleteMap(current_map, this->height);
		char c = _getch();
		if(c == 's' || c == 'S')
			if(i != 0) 
				i--;
		if(c == 'd' || c == 'D')
			i++;
	}
}
/*------------------------------------------------------------------------------
 IsBlockingExit
		- verifica daca masina de la indexul specificat blocheaza iesirea
		  pentru masina principala
------------------------------------------------------------------------------*/
bool Game::IsBlockingExit(int car_index, Node *n)
{
	//daca e pe aceeasi directie cu iesirea nu poate bloca
	//nici nu poate sta in fata masinii pt ca atunci nu ar mai avea sens
	if(this->cars[car_index]->direction == this->exit_direction)
		return false;

	int head			= n->car_positions[car_index];
	int length			= this->cars[car_index]->length;
	int tail			= head + length;
	int this_car_column	= this->cars[car_index]->y;
	int main_car_column	= this->cars[this->main_car_index]->y;
	int exit_column		= this->exit_y;
	int this_car_line	= this->cars[car_index]->x;
	int main_car_line	= this->cars[this->main_car_index]->x;
	int exit_line		= this->exit_x;

	if(this->exit_direction == HORIZONTAL)
	{
		for(int i = head; i < tail; i++)
			if(i == exit_line)
			{
				if(this->exit_border == RIGHT)
					return (this_car_column > main_car_column);
				else //if(this->exit_border == LEFT)
					return (this_car_column < main_car_column);
			}
	}
	else //if(this->exit_direction == VERTICAL)
	{
		for(int i = head; i < tail; i++)
			if(i == exit_column)
			{
				if(this->exit_border == DOWN)
					return (this_car_line > main_car_line);
				else //if(this->exit_border == UP)
					return (this_car_line < main_car_line);
			}
	}

	return false;
}
/*------------------------------------------------------------------------------
 GetBlockingExitCarCount
		- numara masinile care blocheaza iesirea
------------------------------------------------------------------------------*/
int Game::GetBlockingExitCarCount(Node *n)
{
	int count = 0;
	for(int i = 0; i < this->num_cars; i++)
		if(IsBlockingExit(i,n))
			count++;
	return count;
}
/*------------------------------------------------------------------------------
 GetDistanceToExit
		- inotarce distanta dintre masina si iesire
		- in functie de orientare, distanta se masoara fie de la cap(head)
		  fie de la coada(tail)
------------------------------------------------------------------------------*/
int Game::GetDistanceToExit(Node *n)
{
	int head		= n->car_positions[this->main_car_index];
	int length		= this->cars[this->main_car_index]->length;
	int tail		= head + length;
	int distance	= 0;

	if(this->exit_direction == VERTICAL)
	{
		if(this->exit_border == UP)
		{
			distance = head;
		}
		else//(this->exit_border == DOWN)
		{
			distance = tail;
		}
	}
	else////if(this->exit_direction == HORIZONTAL)
	{
		if(this->exit_border == LEFT)
		{
			distance = head;
		}
		else//(this->exit_border == RIGHT)
		{
			distance = tail;
		}
	}

	return distance;
}
/*------------------------------------------------------------------------------
 ComputeHeuristic
		- atribuie unui nod valoarea euristica
		- intoarce aceasta valoare
------------------------------------------------------------------------------*/
int Game::ComputeHeuristic(Node *n)
{
	int h = 0;

	h += GetBlockingExitCarCount(n);
	h += GetDistanceToExit(n);

	n->heuristic = h;

	return h;
}
/*------------------------------------------------------------------------------
 IsFree
		- verifica daca punctul de coordonate(x,y) apartinand nodului n este
		  liber
------------------------------------------------------------------------------*/
bool Game::IsFree(int x, int y, Node *n)
{
	//iesire?
	if(x == this->exit_x && y == this->exit_y)
		return true; //=> mereu liber

	//margine?
	if(x <= 0 || y <= 0 || x >= this->height - 1 || y >= this->width - 1)
		return false;//=> mereu ocupat

	//verific daca punctul apartine unei masini
	int head;
	int length;
	int tail;
	int direction;

	for(int i = 0; i < this->num_cars; i++)
	{
		head		= n->car_positions[i];
		length		= this->cars[i]->length;
		tail		= head + length;
		direction	= this->cars[i]->direction;

		if(direction == VERTICAL)
		{
			int column = this->cars[i]->y;

			//verific de la cap la coada pe coloana
			if(column == y)
				for(int j = head; j < tail; j++)
					if(j == x)
						return false;
		}
		else//if(direction == HORIZONTAL)
		{
			int line = this->cars[i]->x;
			
			//verific pe linie
			if(line == x)			
				for(int j = head; j < tail; j++)
					if(j == y)
						return false;
		}
	}
	return true;
}
/*------------------------------------------------------------------------------
 IsSolution
		- verifica daca pozitia masinii principale din n == pozitia prestabilita
		  cu ComputeSolvedPosition
------------------------------------------------------------------------------*/
bool Game::IsSolution(Node *n)
{
	return (n->car_positions[this->main_car_index] == this->main_car_solved);
}


#endif//_GAME_H_
