#ifndef EXO7_H
#define EXO7_H
#include "exo6.h"

typedef struct block {
  Key* author ;
  unsigned char* hash ;
  unsigned char* previous_hash ;
  int nonce ;
  CellProtected* votes ;
} Block ;

void ecrire_block(char* nomFichier,Block *b);
Block* read_block(char *nomFichier);
char* block_to_str(Block* block);
unsigned char *hash_chaine(const char* str);
int enough_zero(unsigned char* str, int d);//en complémentaire de la fonction en dessous
void compute_proof_of_work(Block *B, int d);
int verify_block(Block*, int d);
void delete_block(Block* b);

#endif