#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "util.h"

extern int g_map_num_lines;
extern int g_map_num_columns;

grid_t * grid_alloc()
{
  int i;
  grid_t * g;
  
  g = (grid_t*)malloc(sizeof(grid_t));
  g->m_mat_squares = (char**)malloc(g_map_num_lines*sizeof(char*));
  for(i=0; i<g_map_num_lines; i++)
  {
    g->m_mat_squares[i] = (char*)malloc(g_map_num_columns*sizeof(char));
  }
  
  return g;
}

void grid_free(grid_t * g)
{
  int i;
  
  for(i=0; i<g_map_num_lines; i++)
  {
    free(g->m_mat_squares[i]);
  }
  free(g->m_mat_squares);
}

void grid_fill_with_char(grid_t * g,char c)
{
  int i,j;
  
  for(i=0; i<g_map_num_lines; i++)
  {
    for(j=0; j<g_map_num_lines; j++)
    {
      g->m_mat_squares[i][j] = c;
    }
  }
}

char grid_get(grid_t * g,int line,int column)
{
  return g->m_mat_squares[line][column];
}

void grid_set(grid_t * g,int line,int column,char value)
{
  g->m_mat_squares[line][column] = value;
}

void grid_print(grid_t * g)
{
  int i,j;
  
  for(i=0; i<g_map_num_lines; i++)
  {
    for(j=0; j<g_map_num_columns; j++)
    {
      printf("%c",g->m_mat_squares[i][j]);
    }
    printf("\n");
  }
}

BOOL grid_is_free_at(grid_t * g,int line,int column)
{
  if(g->m_mat_squares[line][column] != MAP_CHAR_FREE)
  {
    return FALSE;
  }
  
  return TRUE;
}


