#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exo1.h"

int is_prime_naive(long p){
  if (p%2==0){
    fprintf(stderr,"l'entier n'est pas impair\n");
    return -1;
  }else{
    //p est impair
    for(int i=2;i<=(p-1);i++){
      if(p%i==0){
        return 0;
        }
    }
    return 1;
  }
}

long modpow_naive(long a, long m, long n){
  long prod=1;
  long res=0;
  for(int i=1;i<=m;i++){
    prod=prod*a;
    res=prod%n;
    //printf("a=%ld, res=%ld\n",prod,res);
  }
  return res;
}


int modpow(long a, long m, long n){
  if(m==0){
    return 1;}
  else if(m==1){//je sis pas sure que ce if soit utile, il peut aller avec le dernier cas nan?
    return a%n;}
  else if(m%2==0){
    long temp = modpow(a,m/2,n);
    return (temp*temp)%n;}//mais tu multiplie jamais a là? et pareil en bas nn
  else{
    long temp = modpow(a,(m-1)/2,n);
    return (a*temp*temp)%n;}
}

/*long modpow(long a,long m,long n)
{
	if(m==1)
	{
		return a%n;
	}

	if( m % 2 == 0)
	{
		long b= modpow(a, m/2, n);
		return b* b % n;
	}
	else
	{
		long b=modpow(a,floor(m/2),n);
		return a * b * b % n;
	}
}*/

int witness(long a,long b,long d,long p){
  long x=modpow(a,d,p);
  if(x==1){
    return 0;
  }
  for(long i=0;i<b;i++){
    if(x==p-1){
      return 0;
    }
    if(p!=0)
    x=modpow(x,2,p);
  }
  return 1;
}

long rand_long(long low,long up){
  return rand()%(up-low+1)+low;
}

int is_prime_miller(long p,int k){
  if(p==2){
    return 1;
  }
  if(!(p & 1)||(p <= 1)) { //on verifie que p est impair et different de 1
    return 0;
  }
  //on determine b et d :
  long b=0;
  long d=p-1;
  while(!(d & 1)) { //tant que d n’est pas impair
    d=d/2;
    b=b+1;
  }
  // On genere k valeurs pour a, et on teste si c’est un temoin :
  long a;
  int i;
  for(i=0;i<k;i++){
    a=rand_long(2,p-1);
    if(witness(a,b,d,p)){
      return 0;
    }
  }
  return 1;
}

long random_prime_number(int low_size, int up_size, int k){  
  long p=rand_long(pow_fct(2,low_size),(pow_fct(2,up_size+1)-1));
  while(is_prime_miller(p,k)==0){
        p=rand_long(pow_fct(2,low_size),(pow_fct(2,up_size+1)-1));
  }
  return p;
}

long pow_fct(int a, int b){
  //renvoie a^b
  if (b==0){
    return 1;
  }if (b==1){
    return a;
  }
  for (int i=2;i<=b;i++){
    a*=a;
  }
  return a;
}

