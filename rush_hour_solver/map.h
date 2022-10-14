#ifndef MAP
#define MAP

#include <stdio.h>
#include "node.h"

#define MAP_CHAR_EMPTY         '-'
#define MAP_CHAR_OBSTACLE      'X'
#define MAP_CHAR_NOT_AVAILABLE '0'
#define MAP_CHAR_COMMENT       '#'
#define MAP_CHAR_NEW_LINE      '\n'
#define MAP_CHAR_SPACE         ' '
#define MAP_CHAR_TAB           '\t'

/* Map loading functions */
void map_load(char * filename);
void map_load_skip_comments(FILE * f);
void map_load_dimensions(FILE * f);
void map_load_terrain(FILE * f);
void map_load_cars(FILE * f);
void map_load_solution(FILE * f);

/* Map unloading functions */
void map_unload(void);
void map_unload_dimensions(void);
void map_unload_terrain(void);
void map_unload_cars(void);
void map_unload_solution(void);

#endif/*MAP*/

