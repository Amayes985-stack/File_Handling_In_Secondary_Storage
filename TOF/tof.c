#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tof.h"

/*--------------------------------------Implémentation des fonctions ---------------------------------------------*/


//fonction de lecture directe 
void DirRead(TOF *file, int i, Bloc *buf)
{
    fseek(file->file, sizeof(Header) + (i - 1) * sizeof(Bloc), SEEK_SET);
    fread(buf, sizeof(Bloc), 1, file->file);
}

//fonction d'écriture directe 
void DirWrite(TOF *file, int i, Bloc buf)
{
    fseek(file->file, sizeof(Header) + (i - 1) * sizeof(Bloc), SEEK_SET);
    fwrite(&buf, sizeof(Bloc), 1, file->file);
}

//récupérer la valeur de l'entete 
int getHeaderValue(TOF *f, int pos) 
{
    switch (pos)
    {
    case 1:
        return((f->hdr).nbBlocs);
    case 2:
        return((f->hdr).nbIns);
    
    default:
        return -1;
    }
}

//modifier la valeur de l'entete 
void setHeaderValue(TOF *file, int pos, int value)
{
    switch(pos)
    {
        case 1:
            file->hdr.nbBlocs = value;
            break;
        case 2:
            file->hdr.nbIns = value;
            break;
    }
}

//Ouverture d'un fichier TOF 
TOF *open(char filename[], char mode)
{
    TOF *f = malloc(sizeof(TOF));  //allocation dynamique 

    if(mode == 'N' || mode == 'n')    //si le mode d'entrée est 'N'
    {
        f->file = fopen(filename, "w+b");  //ouverture du fichier en écriture
        f->hdr.nbBlocs = 0;  //initialisations 
        f->hdr.nbIns = 0;
        fwrite(&(f->hdr), sizeof(Header), 1, f->file);  //écriture dans le fichier 
        rewind(f->file); //reposition de notre pointeur 
    }
    else if(mode == 'A' || mode == 'a')  //sinon si le fichier est 'A'
    {
        f->file = fopen(filename, "r+b");  //ouverture du fichier en lecture
        rewind(f->file); //reposition de notre pointeur 
        fread(&(f->hdr), sizeof(Header), 1, f->file); //lecture dans le fichier 
        rewind(f->file); //reposition de notre pointeur 
    }
    else
    {
        printf("Unrocognized mode."); //sinon afficher l'erreur 
        exit;
    }
    return f;
}

//fonction de fermeture du TOF 
int close(TOF *f)
{
    rewind(f->file); //je repositionne mon pointeur
    fwrite(&(f->hdr), sizeof(Header), 1, f->file); //Écriture directe 
    int cls = fclose(f->file);//je ferme le fichier 
    free(f); //je libère de l'espace 
    return cls;
}

//voici la fonction de recherche 
Position search(int key, TOF *F, int max)
{
    int inf, sup; //valeur inférieur & valeur supérieur 
    int bs = getHeaderValue(F, 1);  //borne supérieure 
    int bi = 1; //borne inférieure 

    Bloc buf;
    Position pos;

    int stop = 0;  //variable booléenne pour marquer l'arrét 
    pos.found = 0; //variable booléenne pour marquer les retrouvailles

    if(bs == 0)  //si ma borne sup = 0  
    {
        pos.bloc = 1;   //nous sommes dans le premier bloc 
        pos.movement = 0;   
        pos.found = 0;
        return pos;  //on renvoie la position
    }

    while(bi <= bs && stop == 0 && pos.found == 0) //début de la recherche dichotomique 
    {
        pos.bloc = (bi + bs) / 2;
        DirRead(F, pos.bloc, &buf); //lecture directe dans le buff
        if(key >= buf.arr[0].key && key <= buf.arr[buf.N - 1].key) 
        {
            inf = 0;        //recherche dichotomique part II
            sup = buf.N;
            while(inf <= sup && pos.found == 0)
            {
                pos.movement = (inf + sup) / 2;
                if(buf.arr[pos.movement].key == key) //si je retrouve la clé 
                    pos.found = 1;  //found = 1 
                else if(key < buf.arr[pos.movement].key) //sinon je continue ma recherche en fixant les bornes  
                    sup = pos.movement - 1;
                else 
                    inf = pos.movement + 1;
            }

            if(pos.found == 0) //si je ne trouves pas la clé 
                pos.movement = inf;  
            break; //je sors 
        }
        if(key < buf.arr[0].key) //si ma clé est < a la clé du premier bloc 
        {
            bs = pos.bloc - 1; //j'ajuste les bornes  
            pos.movement = 0; 
        }
        else if(key > buf.arr[buf.N - 1].key && pos.bloc == getHeaderValue(F, 1)) //sinon
        {
            if(buf.N != max) //si la taille du bloc différente du nombre d'étudiants / bloc 
            {
                pos.bloc = bi; //je récupère les parammètres
                pos.movement = buf.N;  
                pos.found = 0;
                return pos;
            }
            else if(buf.N == max) //sinon je récupère aussi mes paramètres 
            {
                pos.movement = 0;
                pos.bloc = getHeaderValue(F, 1) + 1;
                pos.found = 0;
                return pos;
            }
        }
        else bi = pos.bloc+1;
    }
    //si je ne retrouves pas ma valeur, bah je ne l'ai pas trouvé 
    if(pos.found == 0)
        pos.bloc = bi;
    return pos;
}

//notre fameuse fonction d'insertion (pour anecdote est la fonction qui m'a arrété le cerveau)
void insert(TOF *F, student s, int max)
{
    Bloc buf;
    student x;
    Position pos = search(s.key, F, max);  //je recherche d'abord ou insérer 

    if(!pos.found) //si je ne retrouves pas 
    {
        while(pos.bloc <= getHeaderValue(F, 1))
        {
            DirRead(F, pos.bloc, &buf); //j'écris directement dans le buff 
            x = buf.arr[buf.N - 1];
            for(int k = buf.N - 1 ; k > pos.movement ; k--)
                buf.arr[k] = buf.arr[k - 1]; //et je décale 
            
            buf.arr[pos.movement] = s;
            if(buf.N < max)   //ceci est une suite d'opérations avec des conditions afin de garder l'aspect du fichier TOF
            {
                buf.N++;
                if(x.key != buf.arr[buf.N - 2].key)
                    buf.arr[buf.N - 1] = x;
                DirWrite(F, pos.bloc, buf);
                break;
            }
            else
            {
                DirWrite(F, pos.bloc, buf);
                pos.bloc++;
                pos.movement = 0;
                s = x;
            }
        }
        if(pos.bloc > getHeaderValue(F, 1))
        {
            buf.arr[0] = s;
            buf.N = 1;
            DirWrite(F, pos.bloc, buf);
            setHeaderValue(F, 1, pos.bloc);
        }
        setHeaderValue(F, 2, getHeaderValue(F, 2) + 1);
    }
} 

//fonction de suppression dite logique 
int logical_Delete(TOF *F, int key)
{
    Bloc buf;
    Position pos = search(key, F, MAX);  //d'abord je recherche l'élément dont je veux supprimer 
    
    if(!pos.found) //si je ne retrouves pas bah tant pis xd 
        return 0;
    else //sinon
    {
        DirRead(F, pos.bloc, &buf); //lecture directe dans le bloc  
        if(buf.arr[pos.movement].erased == 0) //maintenant si la case est à 0 
            buf.arr[pos.movement].erased = 1; //je la mets à 1 
        DirWrite(F, pos.bloc, buf); //écriture directe dans le buff 
        close(F);
        return 1;   //et on renvoie true 
    }
}

//fonction de suppression physique 
int physicDelete(TOF *F, int key)
{
    Position pos = search(key, F, MAX); //je recherche la valeur dont je veux supprimer 
    Bloc buf;

    if(!pos.found)
        return 0; //si je ne la retrouves pas bah tant pis 
    else
    {
        DirRead(F, pos.bloc, &buf);  //lecture dans le buff 
        for(int i = pos.movement; i < buf.N - 1 ; i++)
            buf.arr[i] = buf.arr[i + 1]; //j'écrase la valeur tout en effectuant un décalage sans pour autant créer des débordements
        
        buf.N--;
        DirWrite(F, pos.bloc, buf);
        return 1; //et on renvoie true 
    }
}

//fonction d'affichage de notre fichier 
void displayFile(TOF *F)
{
    Bloc tmp;
    if(getHeaderValue(F, 1) == 0) puts("The file is empty."); //si mon entete est = 0, bah mon fichier est vide
    else //si  non 
    {
        for(int i = 1 ; i <= getHeaderValue(F, 1) ; i++)  //j'affiches mon contenu (it's not rocket science )
        {
            DirRead(F, i, &tmp);
            printf("bloc %d:\n", i);
            printf("----------------------\n");
            printf("keyricule \t      Nom \t\t Prénom\n\n");
            for(int j = 0 ; j < tmp.N ; j++)
                if (tmp.arr[j].erased == 0)
                    printf("%d          %s               %s\n", tmp.arr[j].key, tmp.arr[j].name, tmp.arr[j].fname);
            printf("\n");
        }
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

//fonction du chargement initial d'un fichier text jusqu'à un fichier bin (TOF)  à n%
int textInit(char binFile[], char txtFile[], float coeff)
{
    TOF *F = open(binFile, 'n'); //j'ouvre le fichier TOF 
    FILE *tFile = fopen(txtFile, "r");  //j'ouvre le fichier texte en écriture 
    char str[MAX_LINE_SIZE];  
    if(coeff <= 0 || coeff > 1 ) // 0 < pourcentage < 1
        return -1;
    int nb = (int)(coeff * MAX);  //nombre de lignes 
    while(fgets(str, MAX_LINE_SIZE, tFile))  //tant que je ne suis pas arrivé à la fin du fichier et que mon coeff n'a pas dépassé 1 ou s'est rabaissé à moin de zéro 
        insert(F, parseLine(str), nb);  //je lis une ligne du fichier text que je convertie en student et que je l'insère dans le fichier binfile 
    close(F);
    fclose(tFile);
    return 1;
}

//Copyright* : Djermoune Amayes   