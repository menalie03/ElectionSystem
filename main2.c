#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "exo1.h"
#include "exo2.h"

int main(void) {
  //srand(time(NULL));
  //long p=409169967;
  //char buffer[256];
  //fgets(buffer,256,stdin);
  //sscanf(buffer,"%ld",&p);
  
  /*clock_t temps_initial1;
  clock_t temps_final1;
  double temps_cpu1=0;
  while(temps_cpu1<2 && p>0){
    if(p%2!=0){
    temps_initial1=clock();
    printf("%d\n",is_prime_naive(p));
    temps_final1=clock();
    temps_cpu1 = ((double)(temps_final1 - temps_initial1))/ CLOCKS_PER_SEC;
    printf("le temps poup %ld: %f\n",p, temps_cpu1); }
    p++;}*/

  //int i;
  //scanf(" %d\n",&i);
  /*clock_t temps_initial2;
  clock_t temps_final2;
  double temps_cpu2=0;
    temps_initial2=clock();
    printf("a par modprod_naive=%ld\n",modpow_naive(6,0,13));
    temps_final2=clock();
    temps_cpu2 = ((double)(temps_final2 - temps_initial2))/ CLOCKS_PER_SEC;
    printf("le temps poup naive : %f\n", temps_cpu2); 

printf("--------------------------------\n");
  clock_t temps_initial3;
  clock_t temps_final3;
  double temps_cpu3=0;
    temps_initial3=clock();
    printf("a par modprod=%d\n",modpow(6,0,13));
    temps_final3=clock();
    temps_cpu3 = ((double)(temps_final3 - temps_initial3))/ CLOCKS_PER_SEC;
    printf("le temps poup : %f\n", temps_cpu3);*/

  printf("a par modprod_naive=%ld\n",modpow_naive(6,9,13));

  printf("b par modpow=%d\n",modpow(6,9,13));

  long a=3456757;
  long p=41;
  long b=3;
  long d=5;
 printf("a=%ld est un témoin de Miller pour p=%ld? %d\n",a,p,witness(a,b,d,p));
  long valal=rand_long(12321,98675587);
  printf("valeur aléatoire=%ld\n",valal);
  printf("la valeur %ld est nombre premier? %d\n",valal,is_prime_miller(valal, 6));
 printf("la valeur %ld est nombre premier? %d\n",p,is_prime_miller(p, 6));

  printf("nombre premier est %ld\n",random_prime_number(15,32,10));
  return 0;


}


