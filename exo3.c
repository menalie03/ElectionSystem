#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include <string.h>

void init_key(Key* key, long val, long n){
	key->val=val;
	key->n=n;
}

void initpairkeys(Key* pKey, Key* sKey, long lowsize, long upsize){
	long n,s,u;
	long p=random_prime_number(lowsize,upsize, 5000);
  long q=random_prime_number(lowsize,upsize, 5000);
	while(p==q){
	  	  q=random_prime_number(lowsize,upsize, 5000);
	}
	generate_key_values(p,q, &n, &s,&u);
	if (u<0){
    	long t=(p-1)*(q-1);
    	u+=t; //on aura toujours s*u mod t=1
 	}
	//après avoir obtenue les valeurs des pKey et sKey on initialise
	/*Key *pKey=malloc(sizeof(Key));
	Key *sKey=malloc(sizeof(Key));*/
	init_key(pKey,s,n);
	init_key(sKey,u,n);
}

char* key_to_str(Key* key){
  char *s=malloc(sizeof(char)*256);
  if (!s){
    printf("erreur allocation\n");
    return NULL;
  }
  sprintf(s,"(%lx,%lx)",key->val,key->n);
  s[strlen(s)]='\0';
  return s;
}

Key* str_to_key(char* str){
  long val, n;
  Key* key=malloc(sizeof(Key));
  sscanf(str,"(%lx,%lx)",&val,&n);
  init_key(key,val,n);
  return key;
}

Signature* init_signature(long* content, int size){
  Signature* s=(Signature*)malloc(sizeof(Signature));
  if(s==NULL){
    printf("Erreur d'allocation");
    return NULL;
  }
  s->size=size;
  s->content=content;
  return s;
}

//je pas trop sure car pour chiffrer on doit utiliser cle public mais ici c'est sKey le cle private
//pour la signature ça change, il faut sKey (de l'electeur) pour faire la signature  et pKey (du coup du candidat) pour verifier de ce que j'ai compris
Signature* sign(char* mess, Key* sKey){
  long* content=encrypt(mess,sKey->val, sKey->n);
  int size=strlen(mess);
  return init_signature(content,size);
}

char* signature_to_str( Signature* sgn ){
  char* result=malloc(10* sgn->size * sizeof(char));
  result [0]='#' ;
  int pos = 1;
  char buffer[156];
  for(int i=0; i<sgn->size; i++){
    sprintf(buffer, "%lx",sgn->content[i]);
    for(int j=0; j<strlen(buffer); j++) {
      result[pos]=buffer[j];
      pos = pos+1;
    }
    result[pos]='#' ;
    pos = pos+1;
  }
  result[pos]='\0' ;
  result=realloc(result,(pos+1)*sizeof(char));
  return result ;
}

Signature* str_to_signature(char* str){
  int len = strlen(str);
  long* content = (long*)malloc(sizeof(long)*len) ;
  int num=0;
  char buffer[256];
  int pos =0;
  for(int i=0;i<len;i++){
    if(str[i]!='#'){
      buffer[pos]=str[i];
      pos= pos+1;
      }else{
        if(pos!=0){
          buffer[pos]='\0';
          sscanf(buffer,"%lx", &(content[num]));
          num=num+1;
          pos=0;
        }   
      }
  }
 content =realloc(content, num* sizeof(long)) ;
 return init_signature (content, num ) ;
}

Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
  Protected* prot=malloc(sizeof(Protected));
  if(prot==NULL){
    printf("Erreur d'allocation");
    return NULL;
  }
  prot->pKey=pKey;
  prot->mess=strdup(mess);
  prot->sgn=sgn;
  //printf("/n Je suis la clef clrée: %s %s %s", key_to_str(prot->pKey),prot->mess,signature_to_str(prot->sgn));
  return prot;
}

int verify(Protected* pr){
  if(pr->sgn==NULL){
    printf("Erreur dans la signature");
    return 0;
  }
  Signature *s=pr->sgn;
  char *test=decrypt(s->content,s->size,pr->pKey->val,pr->pKey->n);  
  int res=(strcmp(pr->mess,test)==0);
  free(test);
  return res;
}

char* protected_to_str(Protected* pr){
  char* result=malloc(sizeof(char)*(pr->sgn->size*20));
  char* c1=key_to_str(pr->pKey);
  char* c2=signature_to_str (pr->sgn);
  sprintf(result,"clé: %s message: %s signature: %s",c1, pr->mess, c2);
  result[strlen(result)]='\0';
  free(c1);
  free(c2);
  return result;
}

Protected* str_to_protected(char* str){
  char* k=malloc(256*sizeof(char)); 
  char* m=malloc(256*sizeof(char)); 
  char* s=malloc(256*sizeof(char));
  if (!s && !m){
    printf("erreur s ou m ou k\n");
    return NULL;
  }
  sscanf(str, "clé: %s message: %s signature: %s",k,m,s);
  Protected* new=init_protected(str_to_key(k), m, str_to_signature(s));
  free(k);
  free(m);
  free(s);
  return  new;
}