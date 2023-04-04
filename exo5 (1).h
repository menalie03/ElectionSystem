#ifndef EXO5_H
#define EXO5_H
#include "exo4.h"

typedef struct cellKey {
  Key *data;
  struct cellKey *next;
} CellKey;

typedef struct cellProtected {
  Protected *data;
  struct cellProtected *next;
} CellProtected;

CellKey *create_cell_key(Key *key);
void inserer(CellKey **LCK, Key *key);
CellKey *read_public_keys(char *nomFichier);
void print_list_keys(CellKey *LCK);
void delete_cell_key(CellKey *c);
void delete_list_keys(CellKey **c);

CellProtected *create_cell_protected(Protected *pr);
void inserer_CellProtected(CellProtected **cpr, Protected *data);
CellProtected *read_protected(char *nomFichier);
void print_list_protected(CellProtected *c);
void delete_cell_protected(CellProtected *c);
void delete_list_protected(CellProtected **c);

#endif