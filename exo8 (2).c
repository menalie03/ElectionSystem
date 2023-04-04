#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include <string.h>
#include "exo8.h"

CellTree* create_node(Block* b){  //création du noeud
  CellTree* CT=malloc(sizeof(CellTree));
  if (!CT){
    printf("erreur d'allocation du noeud\n");
    return NULL;
  }
  CT->block=b;
  CT->father=NULL;
  CT->firstChild=NULL;
  CT->nextBro=NULL;
  CT->height=0;
  return CT;
}

int update_height(CellTree* father, CellTree* child){
//les tailles décrémentent plus on descend dans l'arbre
  if (father->height<=child->height){
    father->height=(child->height)+1;
    return 1;
  }
  return 0;
}

void add_child(CellTree* father, CellTree* child){
    child->father=father;
  if (father->firstChild==NULL){//si le père n'a pas de fils
    father->firstChild=child;
  }
  else{
    CellTree* tmp=father->firstChild;
    while (tmp->nextBro){//tant que tous les fils n'ont pas été parcouru pour ajouter un fils
      tmp=tmp->nextBro;
    }
    tmp->nextBro=child;
  }
  update_height(father,child);//incrémentation de la hauteur du père
  //incrémentation de la hauteur des grands-pères
  CellTree*tmp=father;
  while(tmp->father!=NULL){
    update_height(tmp->father,tmp);
    tmp=tmp->father;
  }
}


void print_tree(CellTree* bt){//tree représentée réecursivement dans l'ordre père > premier fils > frères du fils (parcours prefixe)
  if (bt==NULL){
    return;
  }
  //printf("Hauteur: %d, Valeur_hachee: %s\n",bt->height,hash_chaine(bloc_to_str(bt->block)));//affichage
  printf("Hauteur: %d, Valeur_hachee: %s\n",bt->height,bt->block->hash);//affichage
  print_tree(bt->firstChild);//recursion pour le premier fils
  if(bt->firstChild!=NULL){
    CellTree* tmp=bt->firstChild->nextBro;
    while (tmp){//recursion pour les autres fils
      print_tree(tmp);
      tmp=tmp->nextBro;
    }
  }
}

void delete_node(CellTree* node){
  if(node!=NULL){
    delete_block(node->block);
    free(node);
  }
}

void delete_tree(CellTree* bt){
  if (bt==NULL){return;}
  if (bt->firstChild!=NULL){
    CellTree* tmp=bt->firstChild->nextBro;
    while (tmp){
      delete_tree(tmp);//suppression des frères d'abord
      tmp=tmp->nextBro;
    }
  }
  delete_tree(bt->firstChild);//suppression du premier fils qui sert à pointer aux frères
  delete_node(bt);//suppression de la racine
}


CellTree* highest_child(CellTree* cell){
  CellTree *max=cell->firstChild, *tmp=max->nextBro;
  if(!tmp){//si cell n'a qu'un seul fils
    return max;
  }
  while(tmp){  //cas inverse: on parcours les fils jusqu'à obtenir la plus grande hauteur
    if(max->height<tmp->height){
      max=tmp;
    }
    tmp=tmp->nextBro;
  }
  return  max;
}

/*CellTree* last_node(CellTree* tree, int height_max){//fonction récursive terminale, avec height_max qui décrémente tel que sa valeur nulle donne le noeud voulu (en descendant l'arbre)
  if (height_max==0 || tree==NULL){
    return tree;
  }
  return last_node(highest_child(tree),tree->height);
}*/


CellTree* last_node(CellTree* tree){
  //quand la hauteur est nulle, nous avons atteint la dernière lignée d'enfants, nous nous occupons aussi du cas où l'arbre est vide en même temps
  if(tree->firstChild==NULL){ // Cas d'arrêt (l'arbre est terminé)
      return tree;
  }
  return last_node(highest_child(tree));
}

CellProtected* fusion(CellProtected *p1, CellProtected *p2){//on suppose les p1 et p2 sont valides
  //cas où l'un des deux est vide ou les deux
  if(!p1){
    if(!p2){
      return NULL;}
    else{
      return p2;}
  }
  if(!p2){
    return p1;
  }
  else{
    //processus d'ajout des declarations de p1 dans p2
    while (p2){
      inserer_CellProtected(&p1,p2->data);
      p2=p2->next;
    }
    return p1;
  }
}


CellProtected* fusion_btree(CellTree* cell){
  CellProtected* res=NULL;
  CellTree *node=cell;
  while(node->height){//on descend l'arbre en fusionnant les listes de votes des noeuds
    res=fusion(res,node->block->votes);
    node=highest_child(node);//selection du noeud avec la plus grande hauteur
  }
  res=fusion(res,last_node(node)->block->votes);//on récupère les votes du noeud à la hauteur 0
  return res;
}
