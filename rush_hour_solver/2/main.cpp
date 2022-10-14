/*------------------------------------------------------------------------------
 main.cpp
------------------------------------------------------------------------------*/
#include "game.h"

int main(int argc, char **argv)
{
	Game *g = new Game("in.txt");
	g->AStarSolve("out.txt");
	delete g;
	MyExit(SUCCESS);
	return 0;
}
