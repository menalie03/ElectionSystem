#ifndef EXO6_H
#define EXO6_H
#include "exo3.h"
#include "exo5.h"

typedef struct hashcell {
  Key* key;
  int val;
} HashCell;

typedef struct hashtable {
  HashCell** tab;
  int size;
} HashTable;


void delete_unvalid_sgn(CellProtected **p);
HashCell* create_hashcell(Key* key);
int hash_function(Key* key, int size);
int find_position(HashTable* t, Key* key); 
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
void insert_key(HashTable *t, Key *key);//fonction ajouté
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

#endif