#include <stdlib.h>
#include <math.h>
#include "car.h"
#include "core.h"
#include "node.h"
#include "util.h"

extern int      g_map_num_cars;
extern car_t ** g_map_vec_cars;
extern int      g_map_num_solution_cars;
extern int **   g_map_vec_solution;
extern int      g_car_move_bitboard[CAR_MOVE_COUNT][2];

void core_initialize()
{
  core_build_root_node();
}

void core_terminate()
{
  core_delete_root_node();
}

void core_build_root_node()
{
  int i,line,column;

  core_node_root = node_alloc();
  core_node_root->m_next = NULL;
  core_node_root->m_previous = NULL;
  core_node_root->m_depth = 0;
  core_node_root->m_parent = NULL;
  
  for(i=0; i<g_map_num_cars; i++)
  {
    line = g_map_vec_cars[i]->m_head_line;
    column = g_map_vec_cars[i]->m_head_column;
    core_node_root->m_vec_car_heads[2*i] = line;
    core_node_root->m_vec_car_heads[2*i+1] = column;
  }
  
  core_compute_node_score(core_node_root);
}

void core_delete_root_node()
{
  node_free(core_node_root);
}

void core_compute_node_score(node_t * n)
{
  int i,score,dist,num_pts;
  int sol_lin;
  int sol_col;
  int crt_lin;
  int crt_col;
  
  score = 0;
  
  for(i=0; i<g_map_num_cars; i++)
  {
    crt_lin = n->m_vec_car_heads[i*2];
    crt_col = n->m_vec_car_heads[i*2+1];
    
    if(core_is_car_involved_in_solution(i,&sol_lin,&sol_col) == TRUE)
    {
      dist = core_compute_distance(crt_lin,crt_col,sol_lin,sol_col);
      num_pts = g_map_vec_cars[i]->m_num_points;
      score += dist*num_pts;
    }
  }
  
  n->m_score = score;
}

void core_compute_node_children(node_t * n)
{
  int i,j,m;
  int crt_lin,crt_col;
  int next_lin,next_col;
  int bit_lin,bit_col;
  car_t * crt_car;
  
  for(i=0; i<g_map_num_cars; i++)
  {
    crt_car = g_map_vec_cars[i];
    crt_lin = n->m_vec_car_heads[2*i];
    crt_col = n->m_vec_car_heads[2*i+1];
    
    for(m = 0; m < car->m_num_moves; m++)
    {
      bit_lin = g_car_move_bitboard[m][0];
      bit_col = g_car_move_bitboard[m][1];
      
      next_lin = crt_lin;
      next_col = crt_col;
      
      /* move in that direction for as long as possible */
      while(1337)
      {
        /* compute destination */
        next_lin += bit_lin * crt_lin;
        next_col += bit_col * crt_col;
      
        /* check destination */
        if(core_is_empty_space(n,next_lin,next_col) == FALSE)
        {
          break;
        }
        /* set destination */
        n->m_vec_car_heads[2*i] = next_lin;
        n->m_vec_car_heads[2*i+1] = next_col;
        
        /* test this configuration */
      }
    }
  }
}

BOOL core_is_car_involved_in_solution(int car_index,int * head_line,int * head_column)
{
  int i;
  
  for(i=0; i<g_map_num_solution_cars; i++)
  {
    if(car_index == g_map_vec_solution[i][0])
    {
      *head_line = g_map_vec_solution[i][1];
      *head_column = g_map_vec_solution[i][2];

      return TRUE;
    }
  }
  
  return FALSE;
}

BOOL core_is_empty_space(node_t * n,int line,int column)
{
  int i;
  int crt_line;
  int crt_column;
  car_t * crt_car;
  grid_t * crt_grid;

  if(grid_is_free_at(g_map_terrain,line,column) == FALSE)
  {
    return FALSE;
  }

  for(i=0; i<g_map_num_cars; i++)
  {
    crt_car = g_map_vec_cars[i];
    crt_line = n->m_vec_car_heads[2*i];
    crt_column = n->m_vec_car_heads[2*i+1];
    crt_grid = big_grid_get_grid(crt_car->m_positions,crt_line,crt_column);
    
    if(grid_is_free_at(crt_grid,line,column) == FALSE)
    {
      return FALSE;
    }
  }
  
  return TRUE;
}

int core_compute_distance(int p1_lin,int p1_col,int p2_lin,int p2_col)
{
  return abs(p1_lin-p2_lin) + abs(p1_col-p2_col);
}

