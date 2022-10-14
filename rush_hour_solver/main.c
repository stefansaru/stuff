#include <stdio.h>
#include "map.h"
#include "core.h"

int main(int argc,char ** argv)
{
  printf("map load\n");
  map_load(argv[1]);
  printf("core initialize\n");
  core_initialize();

  printf("core terminate\n");
  core_terminate();
  printf("map unload\n");
  map_unload();

  return 0;
}

