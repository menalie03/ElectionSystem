#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include "exo6.h"
#include <string.h>
#include "exo7.h"
#include <openssl/sha.h>


void ecrire_block(char* nomFichier,Block *b){
  //ouverture du fichier
  FILE *f=fopen(nomFichier,"w");
  if(f==NULL){
    printf("Erreur d'ouverture\n");
  }
  //ajout de chaque information
  char* k=key_to_str(b->author);
  fprintf(f,"auteur: %s \n",k);
  fprintf(f,"Valeur_hachee_de_ce_bloc: %s\nValeur_hachee_du_bloc_precedent: %s\n",b->hash, b->previous_hash);
  fprintf(f,"la_preuve_de_travail: %d\n",b->nonce);
  fprintf(f,"les_declarations_de_votes:\n");
  CellProtected *tmp=b->votes;
  while(tmp){
    char* ctmp= protected_to_str(tmp->data);
    fprintf(f,"%s\n",ctmp);
    tmp=tmp->next;
    free(ctmp);
  }
  free(k);
  fclose(f);
}

Block* read_block(char *nomFichier){
  //ouverture du fichier
  FILE *f=fopen(nomFichier,"r");
  if(f==NULL){
    printf("Erreur d'ouverture du fichier\n");
    return NULL;
  }

  //création des espaces de stockage des attributs du bloc
  char buffer[256];
  char key[256];
  char hache[256] ;
  char hache_prec[256];
  int nonce;

  //lecture et attribution des valeurs
  /* Attention: 
  prise de la 2e chaine uniquement, la première étant pour la présentation esthétique du fichier texte*/
  fgets(buffer,256,f);//lecture de l'auteur (1ere ligne)
  sscanf(buffer,"%*s %s",key);  
  fgets(buffer,256,f);//lecture de la valeur hachée  (2e ligne)
  sscanf(buffer,"%*s %s",hache);
  fgets(buffer,256,f);//lecture de la valeur hachée précédente 
  sscanf(buffer,"%*s %s",hache_prec);
  fgets(buffer,256,f);
  sscanf(buffer,"%*s %d",&nonce); //lecture de nonce
  fgets(buffer,256,f);//lecture des votes
  CellProtected *P=NULL; 
  while(fgets(buffer,256,f)){
    //printf(" %s",buffer);
    Protected* tmp=str_to_protected(buffer);
    inserer_CellProtected(&P,tmp);  
  }

  //création du bloc
  Block *B=malloc(sizeof(Block));
  B->author=str_to_key(key);
  B->votes=P;
  B->hash=(unsigned char*) strdup((char*)hache);
  B->previous_hash=(unsigned char*) strdup(( char*)hache_prec);
  B->nonce=nonce;
  fclose(f);

  return B;  
}

  
char * bloc_to_str(Block* block){
  if (block==NULL){

    return NULL;
  }
  //prise des éléments du bloc pour la chaine str du bloc
  //printf("dfghut\n");
  char* key=key_to_str(block->author);
  //unsigned char* hash_prec=(unsigned char*)strdup((char*)block->previous_hash);
  //unsigned char* hash_prec=strdup((char*)block->previous_hash);
  unsigned char* hash_prec=NULL;
  if((char*)block->previous_hash!=NULL){
    hash_prec=(unsigned char*)strdup((char*)block->previous_hash);
  }
   
    //calcul de la taille mémoire du bloc
  int taille = 0;
  if((char*)hash_prec!=NULL){
    taille=strlen(key)+strlen((char*)hash_prec)+256;
  }else{
    taille=strlen(key)+256;
    }//dernière valeur pour les espaces et les sauts de lignes
  int taillevote=256;//valeur pour les sauts de lignes
  CellProtected * cpt = block->votes;
  while (cpt) {
    char *c=protected_to_str(cpt->data);
    taillevote += strlen(c) + 1;
    cpt = cpt->next;
    free(c);
  }  
  //free(cpt);
  taille+=taillevote;

  //prise des votes dans le buffer
  char buffer[taillevote];
  CellProtected *tmp=block->votes;
  char* ct=protected_to_str(tmp->data);
  strcpy(buffer,ct);//prise du premier vote réalisé en dehors de la boucle pour éviter un problème de concaténation avec un buffer vide
  strcat(buffer,"\n");
  tmp = tmp->next;
  while(tmp){
    char* ctmp=protected_to_str(tmp->data);
    strcat(buffer,ctmp);
    strcat(buffer,"\n");
    free(ctmp);
    tmp=tmp->next;
  }
  free(ct);
  //composition de la chaine de retour  
  char *res=malloc(sizeof(char)*taille);
  sprintf(res, "%s %s %d\n%s", key, hash_prec, block->nonce, buffer);
  
  free(key);
  free(hash_prec);
  
  return res;
}

unsigned char * hash_chaine(const char* str){
  //hachage
  unsigned char *d=SHA256((const unsigned char*)str,strlen(str),0) ;
  
  //conversion en hexadecimal
  unsigned char* chaine=malloc(sizeof(unsigned char)*256);
  chaine[0]='\0';
  char c[25];//biais pour la conversion de chaque caractère
  for(int i=0;i<SHA256_DIGEST_LENGTH;i++){
    sprintf(c,"%02x",d[i]);//pour la copie en hexadécimal de chaque caractère
    strcat((char*)chaine,c);
  }

  chaine[strlen((char*)chaine)]='\0';//terminaison de la chaine de retour
  return chaine;
}

int enough_zero(unsigned char* str, int d){//verifie l'existence de d zéros en début de chaine
  for(int i=0;i<d;i++){
    if(str[i]!='0')
      return 0;
  }
  return 1;
}

void compute_proof_of_work(Block *b, int d){
  //initiation du processus de validité
  char* bs=bloc_to_str(b);
  b->hash=hash_chaine(bs);
  //b->hash=hash_chaine(bloc_to_str(b));
  b->nonce=0;
  int verify=enough_zero(b->hash,d);//première vérification

  //hachage de la chaine avec incrémentation de la valeur nonce jusqu'à avoir d zéros en début de chaine
  while(!verify){
    b->nonce ++;
    char* bst=bloc_to_str(b);
    b->hash=hash_chaine(bst);
    //b->hash=hash_chaine(bloc_to_str(b));
    verify=enough_zero(b->hash,d);
    free(bst);
  } 
  free(bs);
}	

int verify_block(Block* b, int d){  //même principe que la fonction en complémentaire de 'compute_proof_of_work' mais avec le block directement
  unsigned char *str= b->hash;
  for(int i=0;i<d;i++){
    if(str[i]!='0')
      return 0;
  }
  return 1;
}

void delete_block(Block *b){
  free(b->author);
  free(b->hash); 
  //l'attribut previous_hash n'est pas libéré comme il s'agit d'un unsigned
  if(b->previous_hash!=NULL){
    free(b->previous_hash);
  }
  delete_list_protected(&(b->votes));
  free(b);
}