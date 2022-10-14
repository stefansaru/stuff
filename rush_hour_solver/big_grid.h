#ifndef BIG_GRID
#define BIG_GRID

#include "grid.h"

typedef struct
{
  grid_t *** m_mat_squares;
} big_grid_t;

big_grid_t * big_grid_alloc          ();
void         big_grid_free           (big_grid_t * bg);
void         big_grid_fill_with_char (big_grid_t * bg,char c);
grid_t *     big_grid_get_grid       (big_grid_t * bg,int line,int column);

#endif/*BIG_GRID*/

