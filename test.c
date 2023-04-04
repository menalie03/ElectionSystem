#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include "exo6.h"
#include "exo7.h"
#include "exo8.h"
#include <dirent.h>
#include "exo9.h"
#include <math.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
# include <dirent.h>

int main() {
  srand(time(NULL));
    /*Block* b=read_block("bloc2.txt");
    Block *bb=read_block("bloc10.txt");
    Block *bbb=read_block("bloc10.txt");

    CellTree *T=create_node(b), *t=create_node(bb), *tt=create_node(bbb), *ttt=create_node(bbb);
    print_tree(T);
    add_child(T,t);
      add_child(t,tt);
      add_child(tt,ttt);

    print_tree(T);
    printf("last\n");
    print_tree(last_node(T));//VERIFIER CREATE BLOCK MTN QUE LAST8NODE MARCHE
    create_block(T,str_to_key("(33c,de2)"),2);
    add_block(2,"BTES.txt");
    T=read_tree();
        printf("last\n");
    print_tree(T);
    CellProtected* p=fusion_btree(T);
    print_list_protected(p);*/

    //Generation d’un problème de vote avec 1000 citoyens et 5 candidats 
  printf("Bonjour!\nBienvenue à la simulation d'un processus de votes de 1000 citoyens et 5 candidats:\n");
  generate_random_data(1000, 5);

  //Lecture des déclarations de vote, des candidats et des citoyens
  
  CellKey* voteurs=read_public_keys("keys.txt");
  printf("La liste des clés publiques de nos 1000 citoyens:\n");
  //print_list_keys(voteurs);

  CellKey* candidats=read_public_keys("candidates.txt");
  printf("La liste des clés publiques de nos 5 candidats:\n");
  //print_list_keys(candidats);

  printf("Nous allons maintenant proceder au vote!!\n\nVous trouverez ci-dessous la liste des déclarations de votes:\n");
  CellProtected* declarations=read_protected("declarations.txt");
  //print_list_protected(declarations);

  //Soumission de tous les votes, avec la création d’un bloc valide tous les 10 votes soumis (fonction create_block), suivi directement par l’ajout du bloc dans la blockchain (fonction add_block). 
  printf("\nNous allons maintenant comptabiliser les votes °°\n\n");
   CellTree* tr;
    Block *BI=read_block("BlocInit.txt");
  CellTree* t=create_node(BI);
  //CellTree* tr=NULL;
  int tour=0, i=0;
  int nb_bloc=0;
  char nom_file_bloc_valide[300];
  while(declarations!=NULL){
    if (tour==10){
      //determination d'un auteur créateur du nouveau bloc 
      Key* autP=malloc (sizeof(Key)) ;
      Key* autS=malloc(sizeof(Key)) ;
      initpairkeys (autP,autS,3,7) ;
      //création du bloc
      create_block(t, autP, 2); //ok
      printf("Bloc n°%d créé\n",nb_bloc);
      // tr=read_tree();//mise à jour de l'arbre des blocs du repertoire 
      //print_tree(tr);
      sprintf(nom_file_bloc_valide,"Bloc%d.txt",nb_bloc);
      add_block(3,nom_file_bloc_valide);//ok 
      tour=0;
      nb_bloc++;
      free(autP);
      free(autS);
    } 
    submit_vote(declarations->data); //ok
    declarations=declarations->next;
    tour++;
    printf("Vote n°%d prise en charge\n",i);
    i++;
    if (i==1000){break;}

  }
  printf("\nTous les votes ont enfin été verifié ! \n\n");
  tr=read_tree();
  //print_tree(tr);
  //CellProtected* p_res= fusion_btree(tr);
  //printf("%s\n",key_to_str(compute_winner(p_res,candidats,voteurs,5,1000)));
  //Lecture et affichage de l’arbre final.
  printf("Voici en attendant un affichage de la racine de l'arbre des blocs comptabilisé:\n");
  print_tree(tr);

  //Calcul et affichage du gagnant (fonction compute winner BT).
  printf("\n.. ET FINALEMENT, NOTRE GRAND VAINQUEUR DE L'ELECTION EST :\n");
  Key* winner=compute_winner_BT(tr,candidats,voteurs,5,1000);
  //printf("%s !!!\n",key_to_str(winner));
  printf("Merci d'avoir visualiser notre simulation d'un processus de vote.\nA très bientôt !\n");


  delete_cell_key(voteurs);
  delete_cell_key(candidats);
  delete_cell_protected(declarations);
  delete_tree(tr);
  free(winner);
  return 0;
}