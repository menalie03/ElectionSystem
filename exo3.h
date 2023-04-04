#ifndef EXO3_H
#define EXO3_H
#include "exo2.h"

typedef struct key{
	long val;      //composante de la clé sous la
	long n;            //forme : Key=(val,n)
}Key;

typedef struct{
  int size;   //taille du message chiffré
  long* content;  //message chiffré
} Signature;

typedef struct{
  Key* pKey;   //clé publique de l'auteur dans le système de vote
  char* mess;   //son message
  Signature* sgn;  //et sa signature associé
}Protected;


void initkey(Key* key, long val, long n);
void initpairkeys(Key* pKey, Key* sKey, long lowsize, long upsize);
char* key_to_str(Key* key);  
Key* str_to_key(char* str);
Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str(Signature *sgn);
Signature* str_to_signature (char* str);
Protected* init_protected(Key* pKey, char* mess, Signature*
sgn);
int verify(Protected* pr);
char* protected_to_str(Protected *pr);
Protected* str_to_protected(char* str);
#endif