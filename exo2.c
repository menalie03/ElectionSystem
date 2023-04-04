#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exo2.h"
#include <string.h>
#include <assert.h>

long extended_gcd(long s,long t,long *u,long *v){
  if (t==0) {
    *u=1;
    *v=0;
    return s ;
  }
  long uPrim, vPrim ;
  long gcd=extended_gcd(t,(long)s%t,&uPrim,&vPrim); 
  *u=vPrim;
  *v=uPrim -(s/t)*vPrim ;
  
  assert((*u)*s + (*v)*t ==gcd);
  return gcd;
}

/*void generate_key_values(long p,long q, long *n, long *s,long *u){
  *n=p*q;
  long t=(p-1)*(q-1);
  long ss;
  ss=rand_long(0,t);
  while () 
  t=extended_gcd(s,t,u,long *v)
}*/

void generate_key_values(long p,long q, long *n, long *s,long *u){
  *n=p*q;
  long t=(p-1)*(q-1);
  *s=rand_long(0,t);
  long v=0;
  while(extended_gcd(*s,t,u,&v)!=1 && ((*s)*(*u) %t)!=1){
    *s=rand_long(0,t);
  }
//  assert(((*s)*(*u) %t)==1);
}
    
long* encrypt(char* chaine, long s, long n){
    long *tab=malloc(sizeof(long)*strlen(chaine));
    if (!tab){
      fprintf(stderr,"erreur allocation\n");
      return NULL;
    }
    int i=0;
    while (*chaine!='\0'){

      //printf("%ld \t",(long)*chaine); transforme les caracteres en entier
      tab[i]=modpow((long)*chaine,s,n);
      chaine++;
      i++;
    }
    return tab;
} 

char* decrypt(long* crypted,int size, long u, long n){
  char *res=(char*) malloc(sizeof(char)*(size+1));
  for (int i=0;i<size;i++){
  //printf("%d \n", modpow(crypted[i],u,n)); dechiffrer les codes en bon entier 
    res[i]=(char)modpow(crypted[i],u,n);
  }
  res[size]='\0';
  return res;
}

void print_long_vector (long *result , int size ) { 
  printf ("Vector: [ "); 
  for (int i =0; i < size ; i ++){
    printf("%lx \t", result [i]);
  } 
  printf ("] \n");
} 

