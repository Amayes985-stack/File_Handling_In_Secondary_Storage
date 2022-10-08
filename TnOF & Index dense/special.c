/*******************************************************************************************************************
 * Interface du modèle simplifé d'un fichier TnOF / SFSD 2021 / Djermoune Amayes 	*
 * le programme principal de notre TnOF 
 * à compiler avec gcc special.c TnOF.c TnOF.h -o TnOF, l'exécution est la meme que dans TOF vu que j'ai utilisé presque le meme menu 
 * Vous aurez un menu qui nous permettrait d'effectuer divers opérations sur le TnOF like a Boss 
 * et bien sur c'est gratuit ^_^ et open source  *
 * Vous trouverez ainsi le ficher readme.txt afin de retrouver les instructions sur commment exécuter le programme
 *****************************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TnOF.h"



int main(int argc, char* argv[])
{
    char* filename = (char*)malloc(sizeof(char));
    if (argc < 2 )
    {
        printf("Enter the file name : ");
        scanf("%s", filename);
    }
    else 
        filename = argv[1];

    int end = 0;
    int choice;
    char* tmp = (char*)malloc(sizeof(char));
    printf("Good Evening, longtime no see !\n\n");
    printf("This program will treats the files TnOF (not sure if it's said like this in english but still lol)") ; 
    printf("What are you waiting for , let's begin ") ; 
    while (!end)
    {
        printf("Here's the menu \n\n");
        printf("please feel free to choose whatever you want \n\n");
        printf("1.Add a student\n2.Search a student\n3.Display a file\n4. Display an Index \n5.Exit\n");
        scanf("%d", &choice);
     
        if (choice == 1)
        {
            system("clear");
            TnOF *f = open(filename, 'a');
            printf("Enter the student's data with a separator key,name,firstname : \n");
            scanf("%s", tmp);
            insert(parseLine(tmp), f);
            close(f);
        }
        else if (choice == 2)
        {
            system("clear");
            int key;
            TnOF *f = open(filename, 'a');
            printf("Enter the key : ");
            scanf("%d", &key);
            position p = research(key, f);
            if (p.found == 0)
                printf("Student not found.\n\n");
            else
            {
                Bloc buff;
                DirRead(f, p.bloc, &buff);
                printf ("Student found !\n\n");
                printf("RegNumber : %d\nName : %s\nFirst Name : %s  \n\n", buff.arr[p.movement].key, buff.arr[p.movement].fname, buff.arr[p.movement].name);
            }
            
        }
        else if (choice == 3)
        {
            system("clear");
            TnOF *f = open(filename, 'a');
            display_file(f);
        }
        else if (choice == 4)
        {
            system("clear");
            TnOF *f = open(filename, 'a');
            display_Index(f);
        }
        else if (choice == 5)
        {
            system("clear");
            end = 1;
        }
    }
}

