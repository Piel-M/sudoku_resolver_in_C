// Mallet Piel, Ait Ouazzou Yan
// ELIMINATION_Version2.c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define N 4
#define TAILLE (N * N)

typedef int tGrille[TAILLE][TAILLE];

int chargerGrille(tGrille g) {
    int count = 0;
    // Code pour charger la grille
    char nomFichier[30];
    FILE *f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL) {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
        return 0;
    } else {
        // fread de la structure de la case (valeur + tableau de candidats)
        for (int i = 0; i < TAILLE; i++) {
            for (int j = 0; j < TAILLE; j++) {
                fread(&g[i][j], sizeof(int), 1, f);
                if (g[i][j] == 0) {
                    count++;
                }
            }
        }
        fclose(f);
        return count;
    }
}

void afficherGrille(tGrille grille) {
    // Afficher les numéros de colonnes
    printf("    ");
    for (int col = 1; col <= TAILLE; col++) {
        printf(" %3d ", col);
        if (col % (int)sqrt(TAILLE) == 0 && col < TAILLE) {
            printf(" ");
        }
    }
    printf("\n");

    printf("   +");
    for (int col = 0; col < TAILLE; col++) {
        printf("-----");
        if (col % (int)sqrt(TAILLE) == (int)sqrt(TAILLE) - 1 && col < TAILLE - 1) {
            printf("+");
        }
    }
    printf("+\n");

    // Afficher la grille
    for (int row = 0; row < TAILLE; row++) {
        printf("%2d |", row + 1);
        for (int col = 0; col < TAILLE; col++) {
            if (col % (int)sqrt(TAILLE) == 0 && col > 0) {
                printf("|");
            }
            if (grille[row][col] == 0) {
                printf("   . ");
            } else {
                printf(" %3d ", grille[row][col]);
            }
        }
        printf("|\n");

        if (row % (int)sqrt(TAILLE) == sqrt(TAILLE) - 1 && row < TAILLE - 1) {
            printf("   +");
            for (int col = 0; col < TAILLE; col++) {
                printf("-----");
                if (col % (int)sqrt(TAILLE) == (int)sqrt(TAILLE) - 1 && col < TAILLE - 1) {
                    printf("+");
                }
            }
            printf("+\n");
        }
    }
    printf("   +");
    for (int col = 0; col < TAILLE; col++) {
        printf("-----");
        if (col % (int)sqrt(TAILLE) == (int)sqrt(TAILLE) - 1 && col < TAILLE - 1) {
            printf("+");
        }
    }
    printf("+\n");
    printf("\n");
}

bool possible(tGrille grille, int ligne, int colonne, int valeur) {
    bool possible = true;
    int i, l;
    // vérification de la ligne et de la colonne
    i=0;
    while (i < TAILLE && possible) {
        if (grille[ligne][i] == valeur || grille[i][colonne] == valeur) {
            possible = false;
        }
        i++;
    }
    // vérification du carré
    int ligneCarre = ligne / N;
    int colonneCarre = colonne / N;
    i=0;
    while (i < N && possible) {
        l = 0;
        while (l < N && possible) {
            if (grille[ligneCarre * N + i][colonneCarre * N + l] == valeur) {
                possible = false;
            }
            l++;
        }
        i++;
    }
    return possible;
}

bool backtracking(tGrille grille, int numeroCase) {
    int ligne, colonne;
    bool resultat = false;
    if (numeroCase == TAILLE * TAILLE) {
        // on a traité toutes les cases, la grille est résolue
        resultat = true;
    } else {
        // On récupère les "coordonnées" de la case
        ligne = numeroCase / TAILLE;
        colonne = numeroCase % TAILLE;

        if (grille[ligne][colonne] != 0) {
            // Si la case n’est pas vide, on passe à la suivante (appel récursif)
            resultat =  backtracking(grille, numeroCase + 1);
        } else {
            for (int valeur = 1; valeur <= TAILLE; valeur++) {
                if (possible(grille, ligne, colonne, valeur)) {
                    // Si la valeur est autorisée on l'inscrit dans la case...
                    grille[ligne][colonne] = valeur;
                    // ... et on passe à la suivante : appel récursif
                    // pour voir si ce choix est bon par la suite
                    if (backtracking(grille, numeroCase + 1)) {
                        resultat = true;
                    } else {
                        grille[ligne][colonne] = 0;
                    }
                }
            }
        }
    }
    return resultat;
}

int main() {
    tGrille g;
    int nbCasesVides = chargerGrille(g);
    int nbCasesVidesDebut = nbCasesVides;
    int nbCasesPleinesDebut = TAILLE * TAILLE - nbCasesVides;
    afficherGrille(g);

    clock_t begin = clock();
    backtracking(g, 0);
    clock_t end = clock();

    double tempsCPU = (end - begin) * 1.0 / CLOCKS_PER_SEC;

    afficherGrille(g);
    printf("Temps CPU = %f secondes\n", tempsCPU);

    return 0;
}