/**********************************************************************************************
 * Interface du modèle simplifé d'un fichier TOF / SFSD 2021 / Djermoune Amayes 	*
 * le programme principal de notre TOF à manipuler like a boss 
 * à compiler avec gcc main.c tof.c tof.h -o main 
 * execution avec ./main ou ./main nom_de_mon_fichier.txt  
 *********************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tof.h"


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
    printf("This program will treats the files TOF ") ; 
    printf("What are you waiting for , let's begin ") ; 
    
    while (!end)
    {
        printf("Here's the menu \n\n");
        printf("please feel free to choose whatever you want \n\n");
        printf("1.Import from a file\n2.Add a student\n3.Search a student\n4.Display a file\n5. Delete a student \n6.Exit\n");
        scanf("%d", &choice);
     
        if (choice == 1)
        {
            system("clear");
            char* txtfile = (char*)malloc(sizeof(char));
            printf("Enter the file which contains text : ");
            scanf("%s", txtfile);
            printf("Enter the percentage : ");
            float p;
            scanf("%f", &p);
            textInit(filename, txtfile, p);
        }
        else if (choice == 2)
        {
            system("clear");
            TOF *f = open(filename, 'a');
            printf("Enter the student's data with a separator (key,name,firstname) : \n");
            scanf("%s", tmp);
            insert(f, parseLine(tmp), MAX);
            close(f);
        }
        else if (choice == 3)
        {
            system("clear");
            int key;
            TOF *f = open(filename, 'a');
            printf("Enter the key : ");
            scanf("%d", &key);
            Position p = search(key, f, MAX);
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
        else if (choice == 4)
        {
            system("clear");
            TOF *f = open(filename, 'a');
            displayFile(f);
        }
        else if (choice == 5)
        {
            system("clear");
            int key;
            TOF *f = open(filename, 'a');
            printf("Enter the key of the student you wanna delete : ");
            scanf("%d", &key);
            if(logical_Delete(f, key))
                printf("Student deleted !\n");
            else
                printf("Couldn't delete the student\n");
        }
        else if (choice == 6)
        {
            system("clear");
            end = 1;
        }
    }
}
 
//copyright : Djermoune Amayes 
