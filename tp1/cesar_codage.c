#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* COMMANDES COMPILATIONS :

    gcc cesar_codage.c -o cesar_codage
    ./cesar_codage mon_fichier.txt sortie.txt

*/

/* CONSTANTES */

/* DECLARATIONS DES FONCTIONS */

/*Test d'un caractère minuscule*/
bool estMinuscule(char *c);

/*Test d'un caractère majuscule*/
bool estMajuscule(char *c);

/* DEFINITIONS DES FONCTIONS */

bool estMinuscule(char *c)
{

    if (*c >= 'a' && *c <= 'z')
    {

        return true;
    }
    return false;
}

bool estMajuscule(char *c)
{

    if (*c >= 'A' && *c <= 'Z')
    {

        return true;
    }
    return false;
}

/*MAIN*/
int main(int argc, char *argv[])
{
    system("clear");
    FILE *f;
    FILE *g;
    char r;
    int decalage;

    f = fopen(argv[1], "rb");
    g = fopen(argv[3], "w");

    decalage = strtol(argv[2], NULL, 10);
    decalage = decalage % 26;

    if (decalage < 0)
    {
        decalage += 26;
    }

    if (f == NULL)
    {
        fprintf(stderr, "Erreur : fichier introuvable dans le repertoire courant\n");
        return 0;
    }

    while (fscanf(f, "%c", &r) != EOF)
    {
        if (estMinuscule(&r))
        {
            r = (r - 'a' + decalage)%26 + 'a';
        }
        if (estMajuscule(&r))
        {
            r = (r - 'A' + decalage)%26 + 'A';
        }
        fputc(r, g);
    }

    printf("Décalage final (modulo 26) %d\n",decalage);
    return 0;
}
