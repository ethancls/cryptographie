#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* COMMANDES COMPILATIONS :

    gcc vigenere_codage.c -o vigenere_codage
    ./vigenere_codage mon_fichier.txt java sortie.txt

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
    int taillemdp = strlen(argv[2]);
    int *decalage = malloc(taillemdp * sizeof(int));
    int i = 0;

    while (argv[2][i] != '\0')
    {
        decalage[i] = (int)(argv[2][i] - 'a');
        i++;
    }

    f = fopen(argv[1], "rb");
    g = fopen(argv[3], "w");

    if (f == NULL)
    {
        fprintf(stderr, "Erreur : fichier introuvable dans le repertoire courant\n");
        return 0;
    }

    int j = 0;
    while (fscanf(f, "%c", &r) != EOF)
    {

        if (estMinuscule(&r))
        {
            r = (r - 'a' + decalage[j]) % 26 + 'a';
            j++;
            if (j >= taillemdp)
            {
                j = 0;
            }
        }
        if (estMajuscule(&r))
        {
            r = (r - 'A' + decalage[j]) % 26 + 'A';
            j++;
            if (j >= taillemdp)
            {
                j = 0;
            }
        }
        fputc(r, g);
    }
    free(decalage);
    return 0;
}
