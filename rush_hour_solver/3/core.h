#ifndef CORE
#define CORE

#include "node.h"
#include "util.h"

node_t * core_node_root;

void core_initialize();
void core_terminate();
void core_build_root_node();
void core_delete_root_node();
void core_compute_node_score(node_t * n);
void core_compute_node_children(node_t * n);
BOOL core_is_car_involved_in_solution(int car_index,int * head_line,int * head_column);
BOOL core_is_empty_space(node_t * n,int line,int column);
int  core_compute_distance(int p1_lin,int p1_col,int p2_lin,int p2_col);

#endif/*CORE*/

