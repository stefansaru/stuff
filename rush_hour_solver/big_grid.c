#include <stdlib.h>
#include "big_grid.h"
#include "grid.h"

extern int g_map_num_lines;
extern int g_map_num_columns;

big_grid_t * big_grid_alloc()
{
  int i,j;
  big_grid_t * bg;
  
  bg = (big_grid_t*)malloc(sizeof(big_grid_t));
  bg->m_mat_squares = (grid_t***)malloc(g_map_num_lines*sizeof(grid_t**));
  for(i=0; i<g_map_num_lines; i++)
  {
    bg->m_mat_squares[i] = (grid_t**)malloc(g_map_num_columns*sizeof(grid_t*));
    for(j=0; j<g_map_num_columns; j++)
    {
      bg->m_mat_squares[i][j] = grid_alloc();
    }
  }
  
  return bg;
}

void big_grid_free(big_grid_t * bg)
{
  int i,j;
  
  for(i=0; i<g_map_num_lines; i++)
  {
    for(j=0; j<g_map_num_columns; j++)
    {
      if(bg->m_mat_squares[i][j] != NULL)
      {
        grid_free(bg->m_mat_squares[i][j]);
      }
    }
    free(bg->m_mat_squares[i]);
  }
  free(bg->m_mat_squares);
}

void big_grid_fill_with_char(big_grid_t * bg,char c)
{
  int i,j;
  grid_t *g;
  
  for(i=0; i<g_map_num_lines; i++)
  {
    for(j=0; j<g_map_num_columns; j++)
    {
      g = bg->m_mat_squares[i][j];
      grid_fill_with_char(g,c);
    }
  }
}

grid_t * big_grid_get_grid(big_grid_t * bg,int line,int column)
{
  return bg->m_mat_squares[line][column];
}

