#ifndef CAR
#define CAR

#include "big_grid.h"

#define CAR_MOVE_COUNT 8
#define CAR_MOVE_NONE  0
#define CAR_MOVE_N     1
#define CAR_MOVE_S     2
#define CAR_MOVE_E     3
#define CAR_MOVE_W     4
#define CAR_MOVE_NW    5
#define CAR_MOVE_NE    6
#define CAR_MOVE_SW    7
#define CAR_MOVE_SE    8

struct car
{
  big_grid_t * m_positions;
  int          m_num_points;
  int          m_num_moves;
  int *        m_vec_moves;
  int          m_head_line;
  int          m_head_column;
  int          m_index;
};

typedef struct car car_t;

car_t * car_alloc                   (void);
void    car_free                    (car_t * c);
void    car_init                    (int index,int num_points,int ** vec_points,int num_moves,int * vec_moves);
int     car_get_direction_as_int    (char * direction);
char *  car_get_direction_as_string (int direction);

#endif/*CAR*/

