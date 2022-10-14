#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "car.h"
#include "grid.h"
#include "node.h"

int      g_map_num_lines;
int      g_map_num_columns;
int      g_map_num_cars;
int      g_map_num_obstacles;
car_t ** g_map_vec_cars;
grid_t * g_map_terrain;
int      g_map_num_solution_cars;
int **   g_map_vec_solution;

void map_load(char * filename)
{
  FILE * f;
  
  f=fopen(filename,"rt");
  
  map_load_dimensions(f);
  map_load_terrain(f);
  map_load_cars(f);
  map_load_solution(f);
  
  fclose(f);
}

void map_load_dimensions(FILE * f)
{
  map_load_skip_comments(f);
  fscanf(f,"%i",&g_map_num_lines);
  map_load_skip_comments(f);
  fscanf(f,"%i",&g_map_num_columns);
}

void map_load_terrain(FILE * f)
{
  int i,line,column;
  
  g_map_terrain = grid_alloc();
  grid_fill_with_char(g_map_terrain,MAP_CHAR_EMPTY);
  map_load_skip_comments(f);
  fscanf(f,"%i",&g_map_num_obstacles);
  for(i=0; i<g_map_num_obstacles; i++)
  {
    map_load_skip_comments(f);
    fscanf(f,"%i",&line);
    map_load_skip_comments(f);
    fscanf(f,"%i",&column);
    grid_set(g_map_terrain,line,column,MAP_CHAR_OBSTACLE);
  }
}

void map_load_cars(FILE * f)
{
  int i,j;
  int num_points,**vec_points;
  int num_directions,*vec_directions;
  char * direction_string;
  
  map_load_skip_comments(f);
  fscanf(f,"%i",&g_map_num_cars);
  g_map_vec_cars = (car_t**)malloc(g_map_num_cars*sizeof(car_t*));
  direction_string = (char*)calloc(3,sizeof(char));
  for(i=0; i<g_map_num_cars; i++)
  {
    map_load_skip_comments(f);
    fscanf(f,"%i",&num_points);
    vec_points = (int**)malloc(num_points*sizeof(int*));
    for(j=0; j<num_points; j++)
    {
      vec_points[j] = (int*)malloc(2*sizeof(int));
      map_load_skip_comments(f);
      fscanf(f,"%i",&vec_points[j][0]);
      map_load_skip_comments(f);
      fscanf(f,"%i",&vec_points[j][1]);
    }
    map_load_skip_comments(f);
    fscanf(f,"%i",&num_directions);
    vec_directions = (int*)malloc(num_directions*sizeof(int));
    for(j=0; j<num_directions; j++)
    {
      map_load_skip_comments(f);
      fscanf(f,"%s",direction_string);
      vec_directions[j] = car_get_direction_as_int(direction_string);
    }
    g_map_vec_cars[i] = car_alloc();
    car_init(i,num_points,vec_points,num_directions,vec_directions);
    for(j=0; j<num_points; j++)
    {
      free(vec_points[j]);
    }
    free(vec_points);
    free(vec_directions);
  }
  free(direction_string);
}

void map_load_solution(FILE * f)
{
  int i;

  map_load_skip_comments(f);
  fscanf(f,"%i",&g_map_num_solution_cars);
  g_map_vec_solution = (int**)malloc(g_map_num_solution_cars*sizeof(int*));
  for(i=0; i<g_map_num_solution_cars; i++)
  {
    g_map_vec_solution[i] = (int*)malloc(3*sizeof(int));
    
    /* Read car index */
    map_load_skip_comments(f);
    fscanf(f,"%i",&g_map_vec_solution[i][0]);
    
    /* Read car head line */
    map_load_skip_comments(f);
    fscanf(f,"%i",&g_map_vec_solution[i][1]);
    
    /* Read car head column */
    map_load_skip_comments(f);
    fscanf(f,"%i",&g_map_vec_solution[i][2]);
  }
}

void map_load_skip_comments(FILE * f)
{
  char c;
  while(1337)
  {
    fscanf(f,"%c",&c);

    switch(c)
    {
    case MAP_CHAR_COMMENT:
      {
        do
        {
          fscanf(f,"%c",&c);
        } while(c != '\n');
        continue;
      }
    case MAP_CHAR_SPACE:
    case MAP_CHAR_TAB:
    case MAP_CHAR_NEW_LINE:
      {
        continue;
      }
    default:
      {
        /* I have reached my destination: rewind 1 char and return */
        fseek(f,-1,SEEK_CUR);
        return;
      }
    }
  }
}

void map_unload()
{
  /* Unload data */
  map_unload_dimensions();
  map_unload_terrain();
  map_unload_cars();
  map_unload_solution();
}

void map_unload_dimensions()
{
  /* Nothing to do */
}

void map_unload_terrain(void)
{
  grid_free(g_map_terrain);
}

void map_unload_cars(void)
{
  int i;

  for(i=0; i<g_map_num_cars; i++)
  {
    car_free(g_map_vec_cars[i]);
  }
  free(g_map_vec_cars);
}

void map_unload_solution(void)
{
  int i;

  for(i=0; i<g_map_num_solution_cars; i++)
  {
    free(g_map_vec_solution[i]);
  }
  free(g_map_vec_solution);
}

