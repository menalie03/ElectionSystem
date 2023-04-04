#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include <string.h>
#include <time.h>



int creer_liste_keys(int nv, char *nomFichier){
  FILE *f=fopen(nomFichier,"w");
  if (f==NULL){
    fprintf(stderr,"erreur ouverture fichier keys.txt\n");
    return 0;
  }
  Key *pKey=malloc(sizeof(Key));
  Key *sKey=malloc(sizeof(Key));
  for (int i=0;i<nv;i++){
    //generation de cle:
    //Key *pKey=malloc(sizeof(Key));
    if (pKey==NULL){
      fprintf(stderr,"erreur allocation pKey\n");
      return 0;
    }
    //Key *sKey=malloc(sizeof(Key));
    if (sKey==NULL){
      fprintf(stderr,"erreur allocation sKey\n");
      return 0;
    }
    initpairkeys(pKey,sKey, 3, 7);
    //ecriture dans fichier texte
    char* kp=key_to_str(pKey);
    char* ks=key_to_str(sKey);
    fprintf(f,"public_key: %s private_key: %s\n",kp,ks);
    free(ks);
    free(kp);
  }

  free(pKey);
  free(sKey);
  fclose(f);
  return 1;
}

int candidate_already_present(Key * pkey,char * nomFichier){
  if (pkey==NULL){
     fprintf(stderr,"pas de candidats\n");
     return 0;
  }
  FILE *f=fopen(nomFichier,"r");
  if (f==NULL){
    fprintf(stderr,"erreur ouverture fichier pour verification candidats pas plusieurs fois\n");
    return 0;
  }
  char *kk=key_to_str(pkey), searchedkey[256], buffer[256];
  int garbadge;
  if(fgets(buffer,256,f)!=0)
    sscanf(buffer,"candidat%d: %s",&garbadge,searchedkey);
  while(strcmp(kk,searchedkey)!=0 && fgets(buffer,256,f)){
    sscanf(buffer,"candidat%d: %s",&garbadge,searchedkey);
  }
  /*if (strcmp(kk,searchedkey)==0){return 1;}
  return 0;*/
  int res=(strcmp(kk,searchedkey)==0);
  free(kk);
  fclose(f);
  return res;
}


int creer_liste_candidats(int nv, int nc){
  FILE *f1=fopen("candidates.txt","w");
  if (f1==NULL){
    fprintf(stderr,"erreur ouverture fichier pour creer liste candidats\n");
    return 0;
  }
  for (int i=1;i<=nc;i++){
    FILE *f=fopen("keys.txt","r");
    if (f==NULL){
    fprintf(stderr,"erreur ouverture fichier pour lecture clés\n");
    return 0;
    }
    int m=(int)(rand()%(nv));
    //printf("%d\n",m);
    char buffer[256];
    int j=0;
    while(j<m){//on parcours le texte jusqu'a atteindre la ligne obtenue aléatoirement
      fgets(buffer,256,f);
      j++;
    }
    char valkey[256];
    sscanf(buffer,"%*s %s %*s %*s",valkey);
    Key* pKey= str_to_key(valkey);//on recupère la clé publique
    //printf("%s\n",key_to_str(pKey));
    if (!(candidate_already_present(pKey,"candidates.txt"))){
      //candidat relié à une clé
      fprintf(f1,"candidat%d: %s\n",i,valkey);
      fclose(f);
    }
    free(pKey);
  }
  fclose(f1);
  return 1;
}

int creer_declarations_votes(char *nomFichier,int nv, int nc, char *nomfich){
  FILE *f=fopen(nomFichier,"r");
  FILE *f2=fopen("declarations.txt","w");
  if (f==NULL && f2==NULL){
    fprintf(stderr,"erreur ouverture fichier pour lire clés ou pour ecrire votes \n");
    return 0;
  }
  for (int i=1;i<=nv;i++){
    char pkeyv[256], skeyv[256],bufferkey[256];
    fgets(bufferkey,256,f);
    sscanf(bufferkey,"public_key: %s private_key: %s",pkeyv,skeyv);
    Key *skey=str_to_key(skeyv);
    FILE *f1=fopen(nomfich,"r");
    if (f==NULL){
      fprintf(stderr,"erreur ouverture fichier pour lire candidats\n");
      return 0;
    }
    int choixcandidat=(int) (rand()%(nc));
    char buffercand[256];
    char found[256];
    for (int j=0; j<choixcandidat;j++){
      fgets(buffercand,256,f1);
    }
    sscanf(buffercand,"%*s %s",found);
    Key *pKeyC=NULL;
    pKeyC=str_to_key(found);
    Signature *sgn = sign(found ,skey);//marche jusque ici
    Protected *pr=NULL;
    Key * pp=NULL;
    pp=str_to_key(pkeyv);
    pr=init_protected(pp,found,sgn);//PB ICI
    char *sr=NULL;
    sr=protected_to_str(pr);
    fprintf(f2,"%s\n", sr);//ici arrêt
    free(skey);
    free(pKeyC);
    free(pp);
    free(sgn->content);
    free(sgn);
    free(pr->mess);
    free(pr);
    free(sr);
    fclose(f1);
  }
  fclose(f);
  fclose(f2);
  return 1;
}

void generate_random_data(int nv, int nc){
  
  int step1=creer_liste_keys(nv,"keys.txt");
  if (step1==0){
    printf("problème lors de la création de la liste des clés des citoyens\n");
    return;
  }
  
  int step2=creer_liste_candidats(nv, nc);
  if (step2==0){
    printf("problème lors de la création de la liste des candidats\n");
    return;
  }
  
  int step3=creer_declarations_votes("keys.txt",nv, nc, "candidates.txt");
  if (step3==0){
    printf("problème lors de la création de la liste des votes\n");
    return;
  }
  
}