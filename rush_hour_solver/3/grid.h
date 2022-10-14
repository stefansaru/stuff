#ifndef GRID
#define GRID

#include "util.h"

typedef struct
{
  char ** m_mat_squares;
} grid_t;

grid_t * grid_alloc();
void     grid_free(grid_t * g);
void     grid_fill_with_char(grid_t * g,char c);
char     grid_get(grid_t * g,int line,int column);
void     grid_set(grid_t * g,int line,int column,char value);
void     grid_print(grid_t * g);
BOOL     grid_is_free_at(grid_t * g,int line,int column);

#endif/*GRID*/
