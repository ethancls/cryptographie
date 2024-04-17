#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* COMMANDES COMPILATIONS :

    gcc cesar_decodage.c -o cesar_decodage
    ./cesar_decodage mon_fichier_code.txt mon_fichier_decode.txt

*/

/* CONSTANTES */

/* DECLARATIONS DES FONCTIONS */

bool estMinuscule(char c)
{

    if (c >= 'a' && c <= 'z')
    {

        return true;
    }
    return false;
}

bool estMajuscule(char c)
{

    if (c >= 'A' && c <= 'Z')
    {

        return true;
    }
    return false;
}

char prochaineLettre(FILE *in)
{
    char c;
    while ((c = fgetc(in)) != EOF)
    {
        if (isalpha(c))
        {
            return c;
        }
    }
    return '\0';
}

char lettreLaPlusFrequente(FILE *in)
{

    int frequences[26] = {0}; // Tableau pour stocker les fréquences de chaque lettre (de 'a' à 'z')
    char lettreMax = '\0';    // Lettre avec la fréquence la plus élevée
    int maxFrequence = 0;     // Fréquence maximale observée

    char lettre;
    while ((lettre = prochaineLettre(in)) != '\0')
    {
        if (estMinuscule(lettre))
        {
            int res = lettre - 'a';
            frequences[res]++;
            if (frequences[res] > maxFrequence)
            {
                maxFrequence = frequences[res];
                lettreMax = lettre;
            }
        }
        else if (estMajuscule(lettre))
        {
            int res = lettre - 'A';
            frequences[res]++;
            if (frequences[res] > maxFrequence)
            {
                maxFrequence = frequences[res];
                lettreMax = lettre;
            }
        }
    }

    return lettreMax;
}

void decodageCesar(FILE *in, FILE *out)
{
    char lettreMax = lettreLaPlusFrequente(in);
    int indicelettreMax = lettreMax - 'a';
    int cle = (indicelettreMax + 22) % 26; // 22 est la position de 'E' dans l'alphabet

    printf("La clé est probablement (Analyse Fréquentielle) : %d\n", cle);

    char r;
    fseek(in, 0, SEEK_SET);
    while (fscanf(in, "%c", &r) != EOF)
    {
        if (estMinuscule(r))
        {
            r = 'a' + ((r - 'a' - cle + 26) % 26);
        }
        else if (estMajuscule(r))
        {
            r = 'A' + ((r - 'A' - cle + 26) % 26);
        }
        fputc(r, out);
    }
}

int main(int argc, char *argv[])
{
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    if (in == NULL || out == NULL)
    {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return 1;
    }

    decodageCesar(in, out);
    fclose(in);
    fclose(out);
    return 0;
}
