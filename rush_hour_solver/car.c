#include <stdlib.h>
#include <string.h>
#include "car.h"
#include "grid.h"
#include "big_grid.h"
#include "map.h"

extern int      g_map_num_lines;
extern int      g_map_num_columns;
extern grid_t * g_map_terrain;
extern car_t ** g_map_vec_cars;

int g_car_move_bitboard[CAR_MOVE_COUNT][2] =
{
/*    LINE  COLUMN */
  {      0,      0}, /* CAR_MOVE_NONE */
  {     -1,      0}, /* CAR_MOVE_N    */
  {     +1,      0}, /* CAR_MOVE_S    */
  {      0,     +1}, /* CAR_MOVE_E    */
  {      0,     -1}, /* CAR_MOVE_W    */
  {     -1,     -1}, /* CAR_MOVE_NW   */
  {     -1,     +1}, /* CAR_MOVE_NE   */
  {     +1,     -1}, /* CAR_MOVE_SW   */
  {     +1,     +1}, /* CAR_MOVE_SE   */
};

car_t * car_alloc()
{
  car_t * c;
  c = (car_t*)malloc(sizeof(car_t));
  c->m_positions = big_grid_alloc();
  c->m_vec_moves = NULL;
  return c;
}

void car_free(car_t * c)
{
  big_grid_free(c->m_positions);
  if(c->m_vec_moves != NULL)
  {
    free(c->m_vec_moves);
  }
  free(c);
}

void car_init(int index,int num_points,int ** vec_points,int num_moves,int * vec_moves)
{
  int point,line,column;
  grid_t * current_grid;
  int current_line,current_column;
  int grid_line,grid_column;
  int move;
  car_t * c;
  
  c = g_map_vec_cars[index];
  c->m_index = index;
  c->m_num_points = num_points;
  c->m_head_line = vec_points[0][0];
  c->m_head_column = vec_points[0][1];
  
  big_grid_fill_with_char(c->m_positions,MAP_CHAR_EMPTY);
  
  for(line=0; line<g_map_num_lines; line++)
  {
    for(column=0; column<g_map_num_columns; column++)
    {
      current_grid = big_grid_get_grid(c->m_positions,line,column);
      
      for(point=0; point<num_points; point++)
      {
        current_line = vec_points[point][0];
        current_column = vec_points[point][1];
        grid_line = current_line - c->m_head_line + line;
        grid_column = current_column - c->m_head_column + column;
        
        if( grid_line < 0                    || 
            grid_line >= g_map_num_lines     ||
            grid_column < 0                  ||
            grid_column >= g_map_num_columns ||
            grid_get(g_map_terrain,grid_line,grid_column) == MAP_CHAR_OBSTACLE)
        {
          grid_fill_with_char(current_grid,MAP_CHAR_NOT_AVAILABLE);
          break;
        }
        grid_set(current_grid,grid_line,grid_column,MAP_CHAR_OBSTACLE);
      }
    }
  }
  
  c->m_vec_moves = (int*)malloc(num_moves*sizeof(int));
  for(move=0; move<num_moves; move++)
  {
    c->m_vec_moves[move] = vec_moves[move];
  }
}

int car_get_direction_as_int(char * direction)
{
  if(strcmp(direction,"N") == 0 || strcmp(direction,"n") == 0)
  {
    return CAR_MOVE_N;
  }
  else if(strcmp(direction,"S") == 0 || strcmp(direction,"s") == 0)
  {
    return CAR_MOVE_S;
  }
  else if(strcmp(direction,"E") == 0 || strcmp(direction,"e") == 0)
  {
    return CAR_MOVE_E;
  }
  else if(strcmp(direction,"W") == 0 || strcmp(direction,"w") == 0)
  {
    return CAR_MOVE_W;
  }
  else if(strcmp(direction,"NW") == 0 || strcmp(direction,"nw") == 0)
  {
    return CAR_MOVE_NW;
  }
  else if(strcmp(direction,"NE") == 0 || strcmp(direction,"ne") == 0)
  {
    return CAR_MOVE_NE;
  }
  else if(strcmp(direction,"SW") == 0 || strcmp(direction,"sw") == 0)
  {
    return CAR_MOVE_SW;
  }
  else if(strcmp(direction,"SE") == 0 || strcmp(direction,"se") == 0)
  {
    return CAR_MOVE_SE;
  }
  return CAR_MOVE_NONE;
}

char * car_get_direction_as_string(int direction)
{
  char * str;
  
  str = (char*)calloc(5,sizeof(char));
  switch(direction)
  {
    case CAR_MOVE_N:
    {
      strcpy(str,"N");
    }; break;
    case CAR_MOVE_S:
    {
      strcpy(str,"S");
    }; break;
    case CAR_MOVE_E:
    {
      strcpy(str,"E");
    }; break;
    case CAR_MOVE_W:
    {
      strcpy(str,"W");
    }; break;
    case CAR_MOVE_NE:
    {
      strcpy(str,"NE");
    }; break;
    case CAR_MOVE_NW:
    {
      strcpy(str,"NW");
    }; break;
    case CAR_MOVE_SE:
    {
      strcpy(str,"SE");
    }; break;
    case CAR_MOVE_SW:
    {
      strcpy(str,"SW");
    }; break;
    default:
    {
      strcpy(str,"NONE");
    }; break;
  }
  
  return str;
}

