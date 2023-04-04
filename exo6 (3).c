#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include <string.h>
#include "exo6.h"

void delete_unvalid_sgn(CellProtected **p){
  CellProtected *cour=*p;
  CellProtected *prec=NULL;
  CellProtected *next=NULL;
  CellProtected *trash=NULL;
  while (cour){
    next=cour->next;
    if (!verify(cour->data)){
      if(prec==NULL){
        *p=next;
        trash=cour;
        cour=cour->next;
        delete_cell_protected(trash);
        //free(trash->data->mess);
        //free(trash->data);
      }else{
        prec->next=next;
        trash=cour;
        cour=cour->next;
        delete_cell_protected(trash);
        //free(trash->data->mess);
        //free(trash->data);
      }
    }
    else{
      prec=cour;
      cour=cour->next;
    }
  }
}

HashCell* create_hashcell(Key* key){
  HashCell* hsc=(HashCell*)malloc(sizeof(HashCell));
  if(hsc==NULL){
    printf("erreur d'allocation\n");
    free(hsc);
    return NULL;
  }
  if(key==NULL){
    printf("erreur d'allocation\n");
    free(hsc);
    return NULL;
  }
  hsc->key=key;
  hsc->val=0;
  return hsc; 
} 

/*int hash_function(Key* key, int size){
  //return ((int)(key->val + key->n))%size;
  int i=((int)(key->val + key->n))%size;
  if(i<0){
    i=size+i;
  }
  return i;
}*/            
int hash_function(Key* key, int size){
  long cle= key->val * key->n;
  float A = (sqrt(5) - 1) / 2.;
  return (int)(size * (cle * A - (int)(cle * A)));
}

int find_position(HashTable* t, Key* key){
  return hash_function(key, t->size);
} 
int find_position(HashTable* t, Key* key){
  int i;
  for(i=0;i<t->size;i++){
    if((t->tab[i]->key->val==key->val) && (t->tab[i]->key->n==key->n)){
    printf("edcfg\n");
        return i;//retourne la case de l'élément key
    }
  }
// si l'élément n'est pas dans le tableau
  return hash_function(key, t->size);
} 

HashTable* create_hashtable(CellKey* keys, int size){
  if(!keys){
    return NULL;
  }
  HashTable* th=(HashTable*)malloc(sizeof(HashTable));
  th->tab=(HashCell**)malloc(size*sizeof(HashCell*));
  /*if(th->tab==NULL){
    printf("erreur d'allocation\n");  
    return NULL;
  }*/
  th->size=size;
  for(int i=0;i<size;i++){//initialisation de toutes les cases à NULL
    th->tab[i]=NULL;   
  }
  int j;
  while(keys!=NULL){//insertion des keys dans le tableau par probing linéaire
    j=find_position(th,keys->data);
    //printf("j==%d\n",j);
    while(th->tab[j]!=NULL){
      if (j==size-1){j=0;}
      else{j++;}
      //printf("%d\n",j);
    }
    th->tab[j]=create_hashcell(keys->data);
    keys=keys->next;
    } 
  return th;
}
  
/*void delete_hashtable(HashTable* t){
  if(!t) return;
  for(int i=0;i<t->size;i++){
      //printf("%d\t",t->tab[i]==NULL);
      if(t->tab[i]){
        //printf("(%lx,lx)\t",t->tab[i]->key->val,t->tab[i]->key->n);
        free(t->tab[i]->key);
        free(t->tab[i]);
        }
  }
  free(t->tab);
  free(t);
}*/

void delete_hashtable(HashTable* t){
    for (int i = 0; i < t->size; i++){
      if (t->tab[i]!= NULL){
        //free(t->tab[i]->key) ;// nécéssité d'enlever cett ligne à cause d'un affichage 'double free()' 
        free(t->tab[i]);
      }
    }
    free(t->tab);
    free(t);
}

void insert_key(HashTable *t, Key *key){
  int ind=find_position(t,key);
  if (t->tab[ind]==NULL){
    t->tab[ind]=create_hashcell(key);
  }
  else{
    (t->tab[ind]->val)+=1;
  }
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){// fonction marche avec un printf dedans, mais sans, affiche seg fault
  
  HashTable* Hc=create_hashtable(candidates,sizeC);
  
  HashTable* Hv=create_hashtable(voters,sizeV);
  int verif_voteur=0, verif_candidat=0;
  delete_unvalid_sgn(&decl);// on supprime les signatures non valides
  while (decl){
    //verifions que la personne qui vote a le droit de voter et qu’elle n’a pas déjà voté,
    for (int i=0;i<Hv->size;i++){
      if((Hv->tab[i]->key->val==decl->data->pKey->val) && (Hv->tab[i]->key->n==decl->data->pKey->n) &&(Hv->tab[i]->val ==0) ){
        verif_voteur=1;
        break; 
      }
    }
    //verifions que la personne sur qui porte le vote est bien un candidat de l’´election
    Key* pKeyC=NULL;
    pKeyC=str_to_key(decl->data->mess);
    for (int i=0;i<Hc->size;i++){
      if ((Hc->tab[i]->key->val==pKeyC->val) && (Hv->tab[i]->key->n==pKeyC->n)){
        verif_candidat=1;
        break;
      }
    }
    printf("verif_V %d  verif_cand %d \t\t\t",verif_voteur, verif_candidat);
    if(verif_voteur && verif_candidat){
      //mise à jour  
      insert_key(Hc,pKeyC);//le candidat en question gagne 1 vote
      insert_key(Hv,decl->data->pKey);//le voteur a un vote comptabilisé
    }
    free(pKeyC);
    decl=decl->next;
  }
  //determination du gagnant !!
  int max=0;
  Key* gagnant=NULL;
  for (int i=0;i<Hc->size;i++){
    if (max<Hc->tab[i]->val){ 
      max=Hc->tab[i]->val;
      gagnant=Hc->tab[i]->key;
    }
  }
  return gagnant;
}


  




  
