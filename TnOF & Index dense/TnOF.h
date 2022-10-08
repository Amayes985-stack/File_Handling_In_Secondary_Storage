/**********************************************************************************************
 * Interface du modèle simplifé d'un fichier TnOF & l'Index dense/ SFSD 2021 / Djermoune Amayes 	*
 * les types des blocs de données et de l'entête 			*
 * les fonctions du modèle : open, close, DirRead, DirWrite ...	*
 * les fonctions principales du modèle : Recherche, Insertion....
 *********************************************************************************************/


#ifndef TnOF_H
#define TnOF_H

#include<stdio.h>
#include<stdlib.h>

#define A_MAX 5
#define NAME_SIZE 20
typedef struct student
    {
        char fname[NAME_SIZE];   //Nom
        char name[NAME_SIZE];     //Prénom
        int key;     //Immatricule/clé
        int erased;   //champ utile pour la suppression logique 
    }student;
// déclaration type d'un bloc de données (y compris buffers aussi)
typedef struct Bloc {
        int Nb; // derniere position occupé (pour le dernier bloc s'il est incomplet)
        student arr[A_MAX];	// tableau d'enregistremnts (des entiers)
        }Bloc;

typedef struct position {   //adresse de notre clé utile pour l'index dense et la fonction de recherche  
    int bloc ; 
    int movement ;
    int found ;  
    int  key ; 
}position;

typedef struct Index {   //structure de notre table d'index 
    position arr[A_MAX]; 
    int size ; 
}Index;

// type du bloc d'entête (les caractéristiques du fichier)
typedef struct Header {
	   int nb_blocs;
       int nb_students ; 
	} Header;

//structure du fichier TnOF 

typedef struct TnOF
{
    FILE *F;
    Header hdr;
    Index T ; 
} TnOF;

/*-----------------------------------------------Primitives-------------------------------------------------------------*/
 

// ouvrir un fichier en mode 'N' ou 'A'
//retourne une structure TnOF remplie
TnOF* open(char *nom, char mode);

// fermer le fichier en sauvegardant l'entête ent dans le 1er bloc (num 0) du fichier
void close( TnOF *p);

// lecture du bloc de données num i dans la variable buf
void DirRead( TnOF *p, int i, Bloc *buf );

// ecriture du contenu de la variable buf dans le bloc de données num i
void DirWrite( TnOF *p, int i, Bloc *buf );

//fonction pour récuréper l'entete 
int getHeaderValue(TnOF *file , int pos);

//procédure pour modifier l'entete 
void setHeaderValue(TnOF *file , int pos, int value );

//fonction de recherche dichotomique utile pour la recherche dans l'index et renvoie son adresse 
position research(int key, TnOF *F ) ; 

//procédure d'insertion pour le fichier TnOF 
void insert (student S, TnOF *F) ; 

//procédure pour afficher le contenu du fichier de façon séquencielle 
void displayFile(TnOF *F);

//fonction pour générer des étudiants à partir d'une chaine de caractères 
student parseLine(char str[]) ; 

//procédure pour  afficher le contenu d'un fichier TnOF
void display_file(TnOF *File) ; 

//procédure pour afficher le contenu d'une table d'Index
void display_Index(TnOF *File) ; 

/*Copyrights : Amayes Djermoune  */
#endif 