/**********************************************************************************************
 * Interface du modèle simplifé d'un fichier TOF / SFSD 2021 / Djermoune Amayes 	*
 * les types des blocs de données et de l'entête 			*
 * les fonctions du modèle : open, close, DirRead, DirWrite ...	*
 * les fonctions principales du modèle : Recherche, Insertion, Chargement initial ... *
 *********************************************************************************************/


#ifndef TOF__H
#define TOF__H

#define MAX 4   //Nombre d'étudiants maximum/bloc 
#define NAME_SIZE 20   //Taille de nom maximale
#define MAX_LINE_SIZE 255  //taille de ligne maximale 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

    
    // STRUCTURES

    typedef struct student
    {
        char fname[NAME_SIZE];   //Nom
        char name[NAME_SIZE];     //Prénom
        int key;     //Immatricule/clé
        int erased;   //champ utile pour la suppression logique 
    }student;

    typedef struct Bloc    //Buffer
    {
        student arr[MAX];
        int N;
    }Bloc;

    typedef struct Position   //Structure utile pour la recherche & l'insertion
    {
        int bloc;
        int movement;
        int found;
    }Position;

    typedef struct Header   // Entete 
    {
        int nbBlocs;       
        int nbIns;       
    }Header;

    typedef struct TOF   //Structure du fichier TOF 
    {
        FILE *file;
        Header hdr;
    }TOF;


    
/*--------------------------------------------------Primitives--------------------------------------------------*/

void DirRead(TOF *file, int i, Bloc *buf);
//procédure de lecture directe dans le buff 

void DirWrite(TOF *file, int i, Bloc buf);
//procédure d'écriture directe dans le buff 

int getHeaderValue(TOF *f, int pos);
//fonction pour récupérer l'entete 

void setHeaderValue(TOF *file, int pos, int value);
//fonction pour modifier l'entete 

TOF *open(char filename[], char mode);
//fonction d'ouverture d'un TOF 

int close(TOF *f);
//fonction de fermeture d'un TOF 

Position search(int key, TOF *F, int max);
//fonction d'insertion qui renvoie la position de la clé (bloc, mouvements)

void insert(TOF *F, student s, int max);
//fonction d'insertion spéciale pour TOF 

int logical_Delete(TOF *F, int key);
//fonction de suppression logique 

int physical_Delete(TOF *F, int key);
//fonction de suppression physique 

void displayFile(TOF *F);
//fonction d'affichage du fichier de façon séquencielle 

student parseLine(char str[]);
//fonction qui génère un étudiant à travers une chaine de caractère utile pour le chargmement initial

int textInit(char binFile[], char txtFile[], float coeff);
//fonction du chargement initial d'un fichier texte et le mettre dans un fichier bin(TOF)

/* Copyright protected : Djermoune Amayes*/ 

#endif

