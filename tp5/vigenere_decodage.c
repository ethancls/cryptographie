#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

/* COMMANDES COMPILATIONS :

    gcc vigenere_decodage.c -o vigenere_decodage -lm
    ./vigenere_decodage mon_fichier_code.txt mon_fichier_decode.txt

*/

/* CONSTANTES */

#define MAX_LENGTH 30 // En francais la longueur moyenne d'un mot est de 4,8 caractères, 30 est assez grand

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

// Fonction pour deviner la clé à partir des fréquences avec E
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
    int cle = (indiceMaxFrequences + 22) % 26;
    return cle;
}

double calculerIndiceCoincidence(int frequences[26], int N) // Formule IC https://fr.wikipedia.org/wiki/Indice_de_coïncidence
{
    double IC = 0.0;

    for (int i = 0; i < 26; i++)
    {
        IC += (frequences[i] * (frequences[i] - 1));
    }

    IC /= (N * (N - 1));

    return IC;
}

int longueurCle(FILE *in)
{
    double indiceFrancais = 0.0778; // Indice de coïncidence pour la langue française https://fr.wikipedia.org/wiki/Indice_de_coïncidence | section autres langues
    int longueurCle = 0;
    double meilleureIC = 1.0;

    for (int s = 1; s <= MAX_LENGTH; s++)
    {
        int taillemdp = s;
        double sommeIC = 0.0; // Somme des indices de coïncidence pour cette longueur de clé

        for (int i = 0; i < taillemdp; i++)
        {
            char c;
            int j = 0;
            fseek(in, 0, SEEK_SET);
            int frequences[26] = {0};
            int N = 0;

            while (fscanf(in, "%c", &c) != EOF)
            {
                if (estMajuscule(c))
                {
                    if (j == i)
                    {
                        frequences[c - 'A']++;
                        N++;
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
                        N++;
                    }
                    j++;
                    if (j >= taillemdp)
                    {
                        j = 0;
                    }
                }
            }
            double IC = calculerIndiceCoincidence(frequences, N);
            sommeIC += IC;
        }

        double moyenneIC = sommeIC / taillemdp;

        // Comparaison avec l'indice de coïncidence de la langue française
        if (fabs(moyenneIC - indiceFrancais) < fabs(meilleureIC - indiceFrancais))
        {
            meilleureIC = moyenneIC;
            longueurCle = taillemdp;
        }
    }
    printf("\nLa longueur de clé la plus probable est : %d\n", longueurCle);
    return longueurCle;
}

int main(int argc, char *argv[])
{
    // system("clear");
    (void)argc;
    FILE *entree = fopen(argv[1], "rb"); // Ouvrir le fichier en lecture
    FILE *sortie = fopen(argv[2], "w"); // Ouvrir le fichier de sortie en écriture
    int taillemdp = longueurCle(entree);
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
    }
    printf("La clé est probablement (Analyse Fréquentielle) : ");
    for (int h = 0; h < taillemdp; h++)
    { // Écrire la clé
        printf("%c", decalage[h] + 'a');
    }
    printf("\n");
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