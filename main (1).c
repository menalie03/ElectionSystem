#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include "exo6.h"
#include "exo7.h"
#include "exo8.h"
#include "exo9.h"
#include <dirent.h>
#include <math.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
  srand(time(NULL));

  // generation de cle:
  /*long p=random_prime_number(3,7, 5000);
  long q=random_prime_number(3,7, 5000);
  while(p==q){
    q=random_prime_number(3,7, 5000);
  }
  long n,s,u;
  generate_key_values(p,q,&n,&s,&u);
  //pour avoir des clés positives:
  if (u<0){
    long t=(p-1)*(q-1);
    u+=t; //on aura toujours s*u mod t=1
  }

  //affichage des cles en hexadecimal
  printf (" cle publique= (%lx , %lx ) \n",s,n);
  printf (" cle privee= (%lx , %lx ) \n",u,n);

  //chiffrement
  char mess1[10] ="Hello";
  int len=strlen(mess1);
  long *crypted = encrypt(mess1, s, n);

  printf("Initial message: %s \n",mess1);
  printf("Encoded representation : \n");
  print_long_vector(crypted,len);

  //dechiffrement
  char* decoded=decrypt(crypted,len,u,n);
  printf("Decocded:%s\n",decoded);
  free(crypted);
  free(decoded);*/

  // Test EXO3

  // Testing Init Keys
  Key *pKey = malloc(sizeof(Key));
  Key *sKey = malloc(sizeof(Key));
  initpairkeys(pKey, sKey, 3, 7);
  printf("pKey: %lx , %lx \n", pKey->val, pKey->n);
  printf("sKey: %lx , %lx \n", sKey->val, sKey->n);

  // Testing Key Serialization
  char *chaine = key_to_str(pKey);
  printf("key_to_str: %s \n", chaine);
  Key *k = str_to_key(chaine);
  printf("str_to_key: %lx , %lx \n", k->val, k->n);

  // Testing signature
  // Candidate keys:
  Key *pKeyC = malloc(sizeof(Key));
  Key *sKeyC = malloc(sizeof(Key));
  initpairkeys(pKeyC, sKeyC, 3, 7);

  // Declaration:
  char *mess = key_to_str(pKeyC);
  printf("mess par pKeyC: %s \n", mess);
  printf("%s vote pour %s\n", chaine, mess);
  Signature *sgn = sign(mess, sKey);
  printf("signature: ");
  print_long_vector(sgn->content, sgn->size);

  char *chaineS = signature_to_str(sgn);
  printf("signature_to_str: %s \n", chaineS);
  Signature *sgnS = str_to_signature(chaineS);
  printf("str_to_signature: ");
  print_long_vector(sgnS->content, sgnS->size);

  // Testing protected:
  Protected *pr = init_protected(pKey, mess, sgn);
  // Verification:
  if (verify(pr)) {
    printf("Signature valide\n");
  } else {
    printf("Signature non valide");
  }
  char *chaineP = protected_to_str(pr);
  printf("protected_to_str: %s\n", chaineP);
  Protected *prP = str_to_protected(chaineP);
  char *ckP = key_to_str(prP->pKey);
  char *csP = signature_to_str(prP->sgn);
  printf("str_to_protected: %s %s %s\n", ckP, prP->mess, csP);

  // TEST EXO4
  // int nc;
  // int nv;
  // scanf(" %d %d \n", &nv, &nc);
  // printf("nbr de voteur=%d, nbr de candidat=%d\n", nv, nc);
  generate_random_data(10, 5);

  // TEST EXO5

  // Testing une liste de Key

  /*printf("\n");
  CellKey* LK=create_cell_key(pKey);
  print_list_keys(LK);

  inserer(&LK, sKey);//inser un element dans une liste
  inserer(&LK, pKeyC);
  inserer(&LK, sKeyC);
  print_list_keys(LK);
  printf("%p\n",LK);
  delete_list_keys(&LK);
  print_list_keys(LK);



  //CellKey*  txtkey read_public_keys(char* nomFichier);



  //Testing une cellule de Protected
  CellProtected* lp= create_cell_protected(pr);
  print_list_protected(lp);

  Protected *pr1 =init_protected(sKey,mess,sgn);
  Protected *pr2 =init_protected(pKeyC,"erfyu",sgn);

  inserer_CellProtected(&lp, pr1);//ajouter en tetre un protected
  inserer_CellProtected(&lp, pr2);
  print_list_protected(lp);


  CellProtected* read_protected("declarations.txt");




  //TEST EXO6

  //Test pour supprimer les protected incorrcted

  //delete_unvalid_sgn(&lp); //por trouver une bonne liste de déclaration signée
  printf("une bonne liste chaînée de déclarations signées: \n");
  print_list_protected(lp);

HashCell* hc=create_hashcell(pKey);
printf("hashcell est: key(%lx, %lx) val:%d\n",hc->key->val,hc->key->n,hc->val);

int size=100;
int i=hash_function(pKey,size);
printf("la position de cette cle (%lx,%lx): %d\n", pKey->val, pKey->n,i);

HashTable* lh=create_hashtable(LK,size);
printf("%d\n",lh->tab==NULL);
int j=find_position(lh,pKeyC);

printf("%d\n",j);//marche jusque ici*/

  /*const char *s2 = "Rosetta code";
  unsigned char *d = SHA256((const unsigned char *)s2, strlen(s2), 0);
  int r;
  for (r = 0; r < SHA256_DIGEST_LENGTH; r++)
    printf("%02x", d[r]);
  putchar('\n');
  // affichage: 140173444147aef8cea4d67e803d7956edbc4119663d79278f35fec4d879351c

  unsigned char *dd = hash_chaine(s2);
  int rr ;
  for ( rr= 0; rr < strlen((const char*)dd); rr++)
    printf ( "%c" , dd[rr]) ;
  putchar ( '\n' ) ;*/
  // printf("%s\n", dd);*/
  CellProtected *PP = read_protected("declarations.txt");

  Block *b = malloc(sizeof(Block));
  b->author = str_to_key("(f7,1b5)");
  b->nonce = 0;
  b->votes = PP;
  b->previous_hash = (unsigned char *)"bjrHKKLKHLGKHKH";
  b->hash = NULL;
  b->hash = hash_chaine(bloc_to_str(b));
  // char *hu=bloc_to_str(b);
  printf("%s\n", bloc_to_str(b));
  // printf("%s\n",hu);

  // b->hash = hash_chaine(hu);
  printf("HASH DE B: %s\n", b->hash); // marche
  //("ZEFNEFJFNF\n");
  // printf("%s\n", bloc_to_str(b));
  // b->hash = hash_chaine((const char *)bloc_to_str(b));
  // for ( rr= 0; rr < strlen((const char*)b->hash); rr++)
  //	printf ( "%02x" , b->hash[rr]) ;
  // putchar ( '\n' ) ;
  // printf("%s\n", b->hash);
  // unsigned char *dd = b->hash;
  printf("RENDONS LE HASH VALIDE  avec d=4:\n");
  /*for ( int rr= 0; rr < strlen((const char*)dd); rr++)
    printf ( "%c" , dd[rr]) ;
  putchar ( '\n' ) ;*/
  /*for ( int rr= 0; rr < strlen((const char*)dd); rr++)
    printf ( "%02x" , dd[rr]) ;
  putchar ( '\n' ) ;*/
  ecrire_block("bloc.txt", b); // marche
  Block *BB = read_block("bloc.txt");

  printf("Nonce avant: %d\n", b->nonce);

  compute_proof_of_work(b, 4);
  printf("nonce après: %d\n", b->nonce);
  printf("verify block %d\n Hash après avec 4 zeros:\n", verify_block(b, 4));
  for (int rr = 0; rr < strlen((const char *)b->hash); rr++)
    printf("%c", b->hash[rr]);
  putchar('\n');
  // printf("%s\n",b->hash);
  ecrire_block("bloc.txt", b);
  // delete_block(b);
  printf("FIN\n");
  // printf("%d\n", b->nonce);
  // int v = verify_block(b, 2);
  // printf("le block est valide: %d\n", v);

  // Calcule le temp par rapport  d
  /*clock_t temps_initial1;
  clock_t temps_final1;
  double temps_cpt = 0;
  for (int d = 0; d < 6; d++) {
    printf("d= %d\n", d);
    temps_initial1 = clock();
    compute_proof_of_work(BB, d);
      printf("sdfgh\n");

    temps_final1 = clock();
    temps_cpt = ((double)(temps_final1 - temps_initial1))/ CLOCKS_PER_SEC;
    printf("le temps par rapport à %d: %f\n", d, temps_cpt);
  }*/

  // EXO 8

  CellTree *bt = create_node(b);
  print_tree(bt);
  // delete_node(bt);

  CellTree *btt = create_node(BB);
  printf("change %d\n", update_height(bt, btt));

  add_child(bt, btt);
  print_tree(bt);

  Block *BBB = malloc(sizeof(Block));
  BBB->author = str_to_key("(b4,9f5)");
  BBB->nonce = 0;
  BBB->votes = PP;
  BBB->previous_hash = (unsigned char *)"";
  ;
  BBB->hash = NULL;
  printf("aa\n");
  // printf("%s\n", bloc_to_str(BBB));

  BBB->hash = hash_chaine((const char *)bloc_to_str(BBB));
  printf("%s\n", BBB->hash);
  CellTree *bttt = create_node(BBB);
  add_child(bt, bttt);
  print_tree(bt);

  add_child(btt, bttt);
  printf("change:%d\n", update_height(bt, btt));
  print_tree(bt);
  // delete_tree(bt); //free(): invalid pointer Aborted (core dumped) ou
  // Segmentation fault (core dumped) tj a une bp printf("ertyuik\n");

  /*printf("aaaa\n");
  CellTree* BH=highest_child(bt);
  printf("un fils dont la plus grande hauteur\n");
  print_tree(BH);
  CellTree* BL=last_node(bt);
  printf("la feuille de ce fils\n");
  print_tree(BL);

  //CellProtected* fp= fusion(PP, PP);
  //print_list_protected(fp);

  CellProtected* fbp=fusion_btree(bt);
  printf("la fusion de la plus longue arbre\n");
  print_list_protected(fbp);*/

  // EXO 9

  submit_vote(pr);
  create_block(bt, pKey, 4);
  // Block *bts=read_block("Pending_block.txt");
  // printf("%s\n",bts->hash);
  // int ls=verify_block(bts,4);
  // printf("le block valide: %d\n",ls);
  CellTree *cr = read_tree();
  print_tree(cr);

  add_block(4, "block_valide.txt");
  CellKey *KC = read_public_keys("candidates.txt");
  print_list_keys(KC);
  CellKey *KV = read_public_keys("keys.txt");
  print_list_keys(KV);

  Key *winnerBT = compute_winner_BT(bt, KC, KV, 7, 19);
  printf("%s\n", key_to_str(winnerBT));

  /*free(k);
  free(chaine);
  free(mess);
  free(sgn->content);
  free(sgn);
  free(chaineS);
  free(sgnS->content);
  free(sgnS);
  //free(pr->mess);
  //free(pr);
  //free(pKey);
  //free(sKey);
  //free(pKeyC);
  free(sKeyC),
  free(chaineP);
  free(prP->mess);
  free(prP->pKey);
  free(prP->sgn->content);
  free(prP->sgn);
  free(prP);
  free(ckP);
  free(csP);
  delete_list_protected(&lp);*/
  // delete_hashtable(lh);
  // delete_block(b);

  return 0;
}