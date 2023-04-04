#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo5.h"
#include <string.h>

CellKey* create_cell_key(Key* key){
  CellKey* ck=malloc(sizeof(CellKey));
  if(ck==NULL){
    printf("Erreur d'allocation\n");
    return NULL;
  }
  if(key==NULL){
    printf("il n'y a pas de clé\n");
    return NULL;
  }
  ck->data=key;
  ck->next=NULL;
  return ck;
}

void inserer(CellKey**LCK, Key* key){
  CellKey* ck=create_cell_key(key);
  if(*LCK==NULL){
    *LCK=ck;
    return;
  }

  ck->next=*LCK;
  *LCK=ck;
}

CellKey* read_public_keys(char* nomFichier){
  FILE *f=fopen(nomFichier,"r");
  if (!f){
    fprintf(stderr,"erreur ouverture %s\n",nomFichier);
    return NULL;
  }
  CellKey *LCK=NULL;
  Key* pKey=NULL;
  char buffer[256];
  char valkey[256];
  while(fgets(buffer,4096,f)!=NULL){
    if (strcmp(nomFichier,"candidates.txt")==0){
      sscanf(buffer,"%*s %s", valkey);
    }else{
      sscanf(buffer,"%*s %s %*s %*s", valkey);
    }
    pKey=str_to_key(valkey);
    inserer(&LCK, pKey);
    //printf("(%lx,%lx)\t",LCK->data->val,LCK->data->n);

  }
  fclose(f);
  return LCK;
}

void print_list_keys(CellKey* LCK){
  if(LCK==NULL){
    printf("La liste de clé est vide\n");
  }else{
    while(LCK!=NULL){
      Key* k=LCK->data;
      char* c=key_to_str(k);
      printf("%s\t",c);
      free(c);
      LCK=LCK->next;
    }
  }
  printf("\n");
}

void delete_cell_key(CellKey* LK){
  free(LK->data);
  if (LK->next==NULL){ free(LK);}
  else{
    CellKey* tmp=LK;
    LK=LK->next;
    free(tmp);
  }

 }

void delete_list_keys(CellKey** c){
  CellKey* tmp=NULL;
  while(*c!=NULL){
    tmp=(*c)->next;
    delete_cell_key(*c);
    *c=tmp;
  }
}

//liste de protected
CellProtected* create_cell_protected(Protected* pr){
  CellProtected *p=malloc(sizeof(CellProtected));
  if (!p){
    printf("erreur allocation\n");
    return NULL;
  }
  p->data=pr;
  p->next=NULL;
  return p;
}

void inserer_CellProtected(CellProtected** cpr, Protected* data ){
  if (*cpr==NULL){
    *cpr=create_cell_protected(data);
    return;
  }
  CellProtected *p;
  p=create_cell_protected(data);
  p->next=*cpr;
  *cpr=p;
}

CellProtected* read_protected(char* nomFichier){
  FILE *f=fopen("declarations.txt","r");
  CellProtected *p=NULL;
  char buffer[256];
  while(fgets(buffer,256,f)){
    inserer_CellProtected(&p,str_to_protected(buffer));
  }
  fclose(f);
  return p;
}

void print_list_protected(CellProtected* c){
  if(c==NULL){
    printf("La liste de  déclarations signées est vide\n");
  }else{
    while(c!=NULL){
      if(c->data!=NULL){
        char* k=protected_to_str(c->data);
        printf("%s\n",k);
        free(k);
      }
      c=c->next;
    }
  }
  printf("\n");
}

void delete_cell_protected(CellProtected* c){
    free(c->data->pKey);
    free(c->data->mess);
    free(c->data->sgn->content);
    free(c->data->sgn);
    free(c->data);
    free(c);

}

void delete_list_protected(CellProtected** c){
  CellProtected *tmp=NULL;
  while(*c!=NULL){
    tmp=(*c)->next;
    delete_cell_protected(*c);
    *c=tmp;
  }
}
