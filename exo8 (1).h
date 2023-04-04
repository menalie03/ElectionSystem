#ifndef EXO8_H
#define EXO8_H
#include "exo7.h" 

typedef struct block_tree_cell {
  Block * block ;
  struct block_tree_cell * father ;
  struct block_tree_cell * firstChild ;
  struct block_tree_cell * nextBro ;
  int height ;
} CellTree ;

CellTree* create_node(Block* b);
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
void print_tree(CellTree *bt);
void delete_node(CellTree* node);
void delete_tree(CellTree* node);
CellTree* highest_child(CellTree* cell);
CellTree* last_node(CellTree* tree);
CellProtected* fusion(CellProtected *p1, CellProtected *p2);
CellProtected* fusion_btree(CellTree* cell);

#endif