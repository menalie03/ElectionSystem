#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include "exo6.h"
#include "exo7.h"
#include "exo8.h"
#include <string.h>
#include "exo9.h"
#include "dirent.h"

void submit_vote(Protected* p){
  FILE *f=fopen("Pending_votes.txt","a");
  if (f==NULL){
    printf("erreur lors de l'ouverture du fichier\n");
    return;
  }
  char* cp=protected_to_str(p);
  fprintf(f,"%s\n",cp);//ajout d'un vote
  free(cp);
  fclose(f);
}

void create_block(CellTree* tree, Key* author, int d){ 
  //enregistrement de la liste des votes 
  CellProtected *P=NULL;
  P=read_protected("Pending_votes.txt");
  //enregistrement de la valeur haché du bloc  précédent dépendant du noeud le plus bas dans l'arbre de blocs 'tree'
  unsigned char *prec=NULL;
  if (tree!=NULL){
    prec=last_node(tree)->block->hash;//le bloc du noeud avec le plus d'ascendants est celui qui a une plus grande probabilité d'être non fraudé/valide
    printf("%d\n",last_node(tree)!=NULL);
  }
  //création du bloc
  Block *b=malloc(sizeof(Block));
  if(b==NULL){
      printf("Erreur à l'allocation du bloc\n");
      return;
  }
  b->author=author;
  b->nonce=0;
  b->previous_hash=(unsigned char*)strdup((char*)prec);
  b->votes=P;
  char* kb=bloc_to_str(b);
  unsigned char* hc=hash_chaine(kb);
  b->hash=hc;
  //b->hash=hash_chaine(kb);
  printf("%s\n",kb);
  //bloc devient valide
  compute_proof_of_work(b,d);
  
  //suppression du fichier Pendule_votes.txt
  if (remove("Pending_votes.txt")!=0){
    printf("erreur lors de la suppression du fichier Pendule_votes.txt\n");
    return;
  }

  //ecriture du bloc dans fichier Pending_block
  ecrire_block("Pending_block.txt",b);
  delete_list_protected(&P);
  free(kb);
  free(b->previous_hash);
  free(hc);
  free(b);

}

void add_block(int d, char* name){ 
  Block *b=read_block("Pending_block.txt");
  printf("le block valide: %d\n",verify_block(b,d));
  if (verify_block(b,d)){//si le bloc est valide, ajout de celui-ci dans le fichier name
    char file[256];
    char *nomdir="./Blockchain/";
    DIR *rep =opendir(nomdir);//ouverture du repertoire Blockchain
    if (rep==NULL){//s'il ne s'ouvre pas
      printf("Erreur dans l'ouverture du repertoire.\n");
      return;
    }
    sprintf(file,"%s%s",nomdir,name);
		ecrire_block(file,b);//ecriture dans le dossier Bockchain
    closedir(rep);//fermeture du repertoire
  } 
  else{
    printf("block n'est pas valide\n");
  } 
  delete_block(b);
  remove("Pending_block.txt");
}

CellTree* read_tree(){
  CellTree** T=(CellTree**)malloc(sizeof(CellTree*)*2048);
  int taille=0;
  DIR * rep = opendir ("./Blockchain/") ;
  if ( rep != NULL ) {
    struct dirent * dir ;
    Block* b=NULL;
    //CellTree* noeud=malloc(sizeof(CellTree));
    CellTree* noeud=NULL;
    char file[300];
    while (( dir = readdir ( rep ) ) ) {
      if ( strcmp ( dir -> d_name ,".") !=0 && strcmp ( dir -> d_name ,"..") !=0) {
        printf ("Chemin du fichier:./Blockchain/%s\n",dir -> d_name ) ;
        sprintf(file,"./Blockchain/%s",dir -> d_name);
        printf("%s\n",file);
        b=read_block(file);
        //printf("%s\n",b->hash);
        noeud=create_node(b);
        //printf("%s\n",noeud->block->hash);
        T[taille]=noeud;
        //printf("%s\n",T[taille]->block->hash);
        taille++;       
      }
    }
    
    closedir ( rep ) ;
  } 
  printf("taille=%d\n",taille);
  for(int j=0;j<taille;j++){
    Block *tmp=T[j]->block;
    for(int k=j+1;k<taille;k++){
      Block *tmpp=T[k]->block;
      if(strcmp((char*)tmp->hash,(char*)tmpp->previous_hash)==0){
        add_child(T[j], T[k]);
      }
    }
  }
  for(int j=0;j<taille;j++){
    if(T[j]->father==NULL){
      return T[j];
    }
  }
  return NULL;
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
  
  //extraction de la liste des votes de la plus longue chaine de l'arbre
  CellProtected* p_res= fusion_btree(tree);

  //suppression des signatures non valides
  delete_unvalid_sgn(&p_res);

  //calcul du gagnant de l'éléction !
  Key* key=NULL;
  key=compute_winner(p_res, candidates, voters, sizeC,sizeV);
  //printf("gbh\n");
  return key;
}