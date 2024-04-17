#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* COMMANDES COMPILATIONS :

    gcc decryptage_substitution.c -lm -o decryptage_substitution
    ./decryptage_substitution secret_fr.txt french_quadgram.txt sortie.txt

*/

#define GEN 50000

/* STRUCTURES */

typedef struct quadgram_t
{
    double ****qt;
} quadgram_tab;

quadgram_tab *init_tabquad(FILE *f)
{
    quadgram_tab *quadgram = (quadgram_tab *)calloc(1, sizeof(quadgram_tab));

    if (quadgram == NULL)
    {
        // Gestion de l'erreur d'allocation
        fprintf(stderr, "Erreur d'allocation de mémoire pour quadgram_tab\n");
        exit(1);
    }

    quadgram->qt = (double ****)calloc(26, sizeof(double ***));
    if (quadgram->qt == NULL)
    {
        // Gestion de l'erreur d'allocation
        fprintf(stderr, "Erreur d'allocation de mémoire pour quadgram->qt\n");
        free(quadgram);
        exit(1);
    }

    for (int i = 0; i < 26; i++)
    {
        quadgram->qt[i] = (double ***)calloc(26, sizeof(double **));
        if (quadgram->qt[i] == NULL)
        {
            // Gestion de l'erreur d'allocation
            fprintf(stderr, "Erreur d'allocation de mémoire pour quadgram->qt[%d]\n", i);
            // Libérer la mémoire allouée précédemment
            for (int j = 0; j < i; j++)
            {
                free(quadgram->qt[j]);
            }
            free(quadgram->qt);
            free(quadgram);
            exit(1);
        }

        for (int j = 0; j < 26; j++)
        {
            quadgram->qt[i][j] = (double **)calloc(26, sizeof(double *));
            if (quadgram->qt[i][j] == NULL)
            {
                // Gestion de l'erreur d'allocation
                fprintf(stderr, "Erreur d'allocation de mémoire pour quadgram->qt[%d][%d]\n", i, j);
                // Libérer la mémoire allouée précédemment
                for (int k = 0; k < j; k++)
                {
                    free(quadgram->qt[i][k]);
                }
                free(quadgram->qt[i]);
                for (int k = 0; k < i; k++)
                {
                    free(quadgram->qt[k]);
                }
                free(quadgram->qt);
                free(quadgram);
                exit(1);
            }

            for (int k = 0; k < 26; k++)
            {
                quadgram->qt[i][j][k] = (double *)calloc(26, sizeof(double));
                if (quadgram->qt[i][j][k] == NULL)
                {
                    // Gestion de l'erreur d'allocation
                    fprintf(stderr, "Erreur d'allocation de mémoire pour quadgram->qt[%d][%d][%d]\n", i, j, k);
                    // Libérer la mémoire allouée précédemment
                    for (int l = 0; l < k; l++)
                    {
                        free(quadgram->qt[i][j][l]);
                    }
                    free(quadgram->qt[i][j]);
                    for (int l = 0; l < j; l++)
                    {
                        free(quadgram->qt[i][l]);
                    }
                    free(quadgram->qt[i]);
                    for (int l = 0; l < i; l++)
                    {
                        free(quadgram->qt[l]);
                    }
                    free(quadgram->qt);
                    free(quadgram);
                    exit(1);
                }
            }
        }
    }

    char c1, c2, c3, c4;
    double valeur;

    while (fscanf(f, "%c%c%c%c %lf\n", &c1, &c2, &c3, &c4, &valeur) == 5)
    {
        quadgram->qt[c1 - 'a'][c2 - 'a'][c3 - 'a'][c4 - 'a'] += valeur;
        // printf("Lettres : %c%c%c%c, Valeur : %lf\n", c1, c2, c3, c4, valeur);
    }

    return quadgram;
}

double valeur_quadgram(char c1, char c2, char c3, char c4, quadgram_tab *quadgram)
{
    double freq = quadgram->qt[c1 - 'a'][c2 - 'a'][c3 - 'a'][c4 - 'a'];
    if (freq == 0)
    {
        return 1;
    }
    return freq;
}

double compute_score(char *input, quadgram_tab *tabquad, int size)
{
    double score = 0.0;
    int i = 0;
    while (i + 3 < size)
    {
        if (tabquad->qt[(int)(input[i] - 'a')][(int)(input[i + 1] - 'a')][(int)(input[i + 2] - 'a')][(int)(input[i + 3] - 'a')] > 0)
        {
            score += log(tabquad->qt[(int)(input[i] - 'a')][(int)(input[i + 1] - 'a')][(int)(input[i + 2] - 'a')][(int)(input[i + 3] - 'a')]);
        }
        i++;
    }
    return score;
}

char *permute(char *mdp)
{
    int index1, index2;
    char temp;
    char new_mdp[27];

    for (int i = 0; i < 26; i++)
    {
        new_mdp[i] = mdp[i];
    }

    new_mdp[26] = '\0';

    index1 = rand() % 26;
    do
    {
        index2 = rand() % 26;
    } while (index2 == index1);

    temp = new_mdp[index1];
    new_mdp[index1] = new_mdp[index2];
    new_mdp[index2] = temp;

    return strdup(new_mdp);
}

char *decrypt(char *input, char *mdp, int size)
{
    int i = 0;
    char *output = (char *)calloc(size, sizeof(char)); // Allouer un nouveau tableau pour le résultat
    if (output == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire pour output\n");
        exit(1);
    }

    while (i < size)
    {
        output[i] = mdp[input[i] - 'a'];
        i++;
    }
    output[size] = '\0';

    return output;
}

void afficherBarreChargement(int progress, int total)
{
    int barWidth = 70;
    float pourcentage = (float)progress / total;
    int pos = (int)(barWidth * pourcentage);

    printf("\033[0;33m["); // Change la couleur du texte en jaune
    for (int i = 0; i < barWidth; i++)
    {
        if (i <= pos)
            printf("#");
        else
            printf(" ");
    }

    printf("\033[0m] \033[0;35m%.0f%%\r", pourcentage * 100); // Change la couleur du texte en violet
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    (void)argc;
    srand(time(NULL));
    FILE *entree = fopen(argv[1], "rb");
    FILE *quadgram_file = fopen(argv[2], "rb");
    FILE *sortie = fopen(argv[3], "w");

    if (entree == NULL || sortie == NULL || quadgram_file == NULL)
    {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return 1;
    }

    fseek(entree, 0, SEEK_END);
    int size = (int)ftell(entree);

    printf("size : %d\n", size);

    char *input = (char *)calloc(size, sizeof(char));

    fseek(entree, 0, SEEK_SET);
    char c;
    int p = 0;
    while (!feof(entree))
    {
        fscanf(entree, "%c", &c);
        input[p] = c;
        p++;
    }

    if (input == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire pour input\n");
        free(input);
        exit(1);
    }

    quadgram_tab *quadgram = init_tabquad(quadgram_file);

    char M[] = "abcdefghijklmnopqrstuvwxyz";
    int gradient = 0;
    while (gradient < GEN)
    {
        char *M1 = permute(M);
        double score = compute_score(decrypt(input, M, size), quadgram, size);
        double score1 = compute_score(decrypt(input, M1, size), quadgram, size);
        //printf(" score : %f score' : %f \n", score, score1);
        if (score1 > score)
        {
            for (int i = 0; i < 26; i++)
            {
                M[i] = M1[i];
            }
            gradient = 0;
        }
        else
        {
            gradient++;
        }
        afficherBarreChargement(gradient,GEN);
    }
    printf("\n");

    char *output = decrypt(input, M, size);
    for (int m = 0; m < size; m++)
    {
        fprintf(sortie, "%c", output[m]);
        fprintf(stdout, "%c", output[m]);
    }
    printf("\n");

    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            for (int k = 0; k < 26; k++)
            {
                free(quadgram->qt[i][j][k]);
            }
            free(quadgram->qt[i][j]);
        }
        free(quadgram->qt[i]);
    }
    free(quadgram->qt);
    free(quadgram);

    fclose(entree);
    fclose(quadgram_file);
    fclose(sortie);
}