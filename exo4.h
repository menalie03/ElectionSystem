#ifndef EXO4_H
#define EXO4_H
#include "exo3.h"

void generate_random_data(int nv, int nc);
int creer_declarations_votes(char *nomFichier,int nv, int nc, char *nomfich);
int creer_liste_keys(int nv, char *nomFichier);
int candidate_already_present(Key * pkey,char * nomFichier);
int creer_liste_candidats(int nv, int nc);

#endif