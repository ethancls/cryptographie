#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* COMMANDES COMPILATIONS :

    gcc vigenere_decodage.c -o vigenere_decodage
    ./vigenere_decodage mon_fichier_code.txt taille_mdp mon_fichier_decode.txt

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

// Fonction pour deviner la clé à partir des fréquences
int devinerCle(int frequences[26])
{
    int maxFrequences = 0;
    int indiceMaxFrequences = 0;

    // Trouver la lettre la plus fréquemment utilisée
    for (int i = 0; i < 26; ++i)
    {
        if (frequences[i] > maxFrequences)
        {
            maxFrequences = frequences[i];
            indiceMaxFrequences = i;
        }
    }
    // Deviner la clé en supposant que 'E' est la lettre la plus fréquemment utilisée en français
    int cle = (indiceMaxFrequences + 22) % 26; // 22 est la position de 'E' dans l'alphabet
    return cle;
}

int main(int argc, char *argv[])
{
    // system("clear");
    (void)argc;
    FILE *entree = fopen(argv[1], "r"); // Ouvrir le fichier en lecture
    FILE *sortie = fopen(argv[3], "w"); // Ouvrir le fichier de sortie en écriture
    int taillemdp = strtol(argv[2], NULL, 10);
    printf("%d\n", taillemdp);
    int *decalage = malloc(taillemdp * sizeof(int));

    if (entree == NULL || sortie == NULL)
    {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return 1;
    }

    for (int i = 0; i < taillemdp; i++)
    {
        char c;
        int j = 0;
        fseek(entree, 0, SEEK_SET);
        int frequences[26] = {0};
        while (fscanf(entree, "%c", &c) != EOF)
        {
            if (estMajuscule(c))
            {
                if (j == i)
                {
                    frequences[c - 'A']++;
                }
                j++;
                if (j >= taillemdp)
                {
                    j = 0;
                }
            }
            else if (estMinuscule(c))
            {
                if (j == i)
                {
                    frequences[c - 'a']++;
                }
                j++;
                if (j >= taillemdp)
                {
                    j = 0;
                }
            }
        }
        decalage[i] = devinerCle(frequences);
        printf("%c", decalage[i]+'a'); // Écrire la clé
    }

    fseek(entree, 0, SEEK_SET);
    int m = 0;
    char r;
    while (fscanf(entree, "%c", &r) != EOF)
    {
        if (estMinuscule(r))
        {
            r = (r - 'a' - decalage[m] + 26) % 26 + 'a';
            m++;
            if (m >= taillemdp)
            {
                m = 0;
            }
        }
        if (estMajuscule(r))
        {
            r = (r - 'A' - decalage[m] + 26) % 26 + 'A';
            m++;
            if (m >= taillemdp)
            {
                m = 0;
            }
        }
        fputc(r, sortie);
    }

    free(decalage);
    fclose(entree); // Fermer le fichier d'entrée
    fclose(sortie); // Fermer le fichier de sortie

    return 0;
}