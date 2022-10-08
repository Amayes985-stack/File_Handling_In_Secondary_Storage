
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"TnOF.h"

/*---------------------------------------Implémentation des Fonctions-------------------------------------------*/


//fonction d'ouverture d'un fichier TnOF
TnOF *open(char *nom, char mode) {

    TnOF *File = malloc(sizeof(TnOF));
   if ( mode == 'E' || mode == 'e' )
    {
	File->F = fopen( nom, "rb+" );
	if ( File->F == NULL ) perror( "open (fopen failed)" );
    rewind(File->F) ; 
	fread( &(File->hdr), sizeof(Header), 1, File->F );
    fseek(File->F, sizeof(Header)+(getHeaderValue(File,1)*sizeof(Bloc)),SEEK_SET) ;
    fread(&(File->T), sizeof(Index), 1, File->F ) ;   
    }
   else { // mode == 'N' || mode == 'n'
	File->F = fopen( nom, "wb+" );
	if ( File->F == NULL ) perror( "open (fopen failed)" );
	(File->hdr).nb_blocs = 0;
	fwrite( &(File->hdr), sizeof(Header), 1, File->F );
    setHeaderValue(File,1,0) ;
    setHeaderValue(File,2,0) ;
   }
   return File;
}


// ferme le fichier et sauvegarde de l'entete au début du fichier
void close( TnOF *File)
{
   fseek(File->F, 0L, SEEK_SET);
   fwrite( &(File->hdr), sizeof(Header), 1, File->F ); //bien sur on prends l'index en considération
   fseek(File->F, sizeof(Header)+(getHeaderValue(File,1)*sizeof(Bloc)),SEEK_SET) ;
   fwrite(&(File->T), sizeof(Index), 1, File->F ) ; 
   fclose(File->F);
}


// lecture du bloc de données numéro i dans la variable buf
void DirRead( TnOF *File, int i, Bloc *buf )
{
   fseek(File -> F, sizeof(Header) + (i-1)*sizeof(Header), SEEK_SET);
   fread( buf, sizeof(Header), 1, File->F );
}


// écriture du contenu de buf dans le bloc de données numéro i
void DirWrite( TnOF *File, int i, Bloc *buf )
{
   fseek(File -> F, sizeof(Header) + (i-1)*sizeof(Bloc), SEEK_SET);
   fwrite( buf, sizeof(Bloc), 1, File->F);
}

//fonction pour récupérer l'entete
int getHeaderValue(TnOF *file , int pos) {
  switch(pos) {
      case 1 : 
       return (file->hdr).nb_blocs ; 
       break ; 
     case 2 : 
       return (file->hdr). nb_students ; 
    default : 
      return 0 ;
  } 
} 

//procédure pour modifier la valeur de l'entete
void setHeaderValue(TnOF *file , int pos, int value ) {
    switch(pos) {
      case 1 : 
       (file->hdr).nb_blocs = value ; 
       break ; 
     case 2 : 
       (file->hdr). nb_students = value ;
    }  
    
}   

//la fonction de recherche dans un index dense 
position research(int key, TnOF *F ) {
    int bi = 1 ; 
    int bs = getHeaderValue(F,2) ; 
    int med ; 
    int found = 0 ; 
    position pos;
      while (bi <= bs && found == 0) { //recherche dichotomique dans la table d'index
          med = (bi + bs) / 2 ; 
           if (F->T.arr[med].key==key){
               found=1;
           }else if(F->T.arr[med].key>key){
               bs=med-1;
           }else{
               bi=med+1;
           }
        }
        pos.found=found;
        pos.bloc=F->T.arr[med].bloc;
        pos.movement=F->T.arr[med].movement;
        return pos;  //on renvoie la position 
}

//la fonction d'insertion  
void insert (student S, TnOF *F) {
     Bloc Buff ;
     position pos ;
     position pos2 ;  
     student x ; 
      
     pos = research(x.key, F) ; //je recherche la clé dont je veux insérer

        if (!pos.found)
        {
            if (Buff.Nb > A_MAX)  //je me positionne à la fin du bloc 
            {
                pos.found = 1 ;   
                Buff.arr[0] = x ; 
                Buff.Nb = 1 ;
                DirWrite(F,getHeaderValue(F,1)+1, &Buff) ;  //et j'insère à la fin bien sur 
                  while (pos.bloc<getHeaderValue(F,1)+1) {
                      setHeaderValue(F,getHeaderValue(F,1), 1) ;  //sinon je recherche toujours ma position 
                      setHeaderValue(F, getHeaderValue(F,1), 2) ; 
                      Buff.Nb -- ; 
                  }   
                 
            }

        
        } 

        if (pos.found) {
           
        }        
        
        

           
}

//notre fonction de gérération d'un étudiant à partir d'une chaine de caractères
student parseLine(char str[])
{
    char separators[] = " ,./-_*";  //séparateurs 
    char *p = (char*)malloc(sizeof(char));
    student st;
    p = strtok(str, separators); //utilisation de la fonction strtok

    int word = 0;
    while(p)
    {
        if(word == 0)
            st.key = atoi(p);  
        else if(word == 1) 
            strcpy(st.name, p);
        else if(word == 2) 
            strcpy(st.fname, p);
        p = strtok(NULL, separators);
        word++;
    }
    st.erased = 0;
    return st;
}

//procédure d'affichage d'un fichier Tnof
void display_file(TnOF *File)
{
    int i,j;
    Bloc buff;
    printf("\nLe contenu de notre fichier TnOF:\n");
    for (i=1;i<=EOF;i++)
    {
        DirRead(File,i,&buff);
        printf(" [ ");
        for (j=0;j<5;j++)
        {
            printf(" %d ",buff.arr[j].key);
        }
        printf(" ]");
    }
}

//procédure d'affichage de l'index de notre fichier 
void display_Index(TnOF *File) {
    int i,j ; 
    Bloc buff ;
      printf("\nLe contenu de notre Index dense est : \n") ; 
       for (i=1; i<= A_MAX; i++) {
           printf("Position de i = %d, Position de j =%d , Clé = %d",  File->T.arr[i].bloc, File->T.arr[i].movement, File->T.arr[i].key ) ; 
       } 
}
