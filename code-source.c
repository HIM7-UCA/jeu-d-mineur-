#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE 10

char grille[TAILLE][TAILLE];
char grilleVisible[TAILLE][TAILLE];
int minesPlacees = 0;

void initialiserGrille() {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            grille[i][j] = ' ';
            grilleVisible[i][j] = '-';
        }
    }
    minesPlacees = 0;
}

void placerMines(int difficulte) {
    int MINES = 10 * difficulte;
    srand(time(NULL));
    while (minesPlacees < MINES) {
        int x = rand() % TAILLE;
        int y = rand() % TAILLE;
        if (grille[x][y] != '*') {
            grille[x][y] = '*';
            minesPlacees++;
        }
    }
}

void afficherGrille() {
    printf("  ");
    for (int j = 0; j < TAILLE; j++) {
        printf("[%d]", j);
    }
    printf("\n");

    for (int i = 0; i < TAILLE; i++) {
        printf("[%d]", i);
        for (int j = 0; j < TAILLE; j++) {
            printf("[%c]", grilleVisible[i][j]);
        }
        printf("\n");
    }
}

void basculerDrapeau(int x, int y) {
    if (grilleVisible[x][y] == '-') {
        grilleVisible[x][y] = 'd';
    } else if (grilleVisible[x][y] == 'd') {
        grilleVisible[x][y] = '-';
    }
}

int compterMinesAdjacentes(int x, int y) {
    int count = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && i < TAILLE && j >= 0 && j < TAILLE && grille[i][j] == '*') {
                count++;
            }
        }
    }
    return count;
}

void revelerCase(int x, int y) {
    if (x < 0 || x >= TAILLE || y < 0 || y >= TAILLE) {
        return;
    }
    if (grilleVisible[x][y] != '-') {
        return;
    }
    if (grille[x][y] == '*') {
        grilleVisible[x][y] = '*';
        return;
    }
    int minesAdjacentes = compterMinesAdjacentes(x, y);
    grilleVisible[x][y] = (minesAdjacentes == 0) ? ' ' : (minesAdjacentes + '0');
    if (minesAdjacentes == 0) {
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                revelerCase(i, j);
            }
        }
    }
}

int verifierVictoire() {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (grille[i][j] != '*' && grilleVisible[i][j] == '-') {
                return 0; // Le jeu continue
            }
        }
    }
    return 1; // Le joueur a gagné
}

int main() {
    int finDuJeu = 0;
    int n;
    int difficulte;

    printf("Choose the difficulty (1-3): ");
    scanf("%d", &difficulte);

    do {
        initialiserGrille();
        placerMines(difficulte);
        finDuJeu = 0;

        while (!finDuJeu) {
            afficherGrille();
            int x, y;
            printf("Enter the coordinates (x y) of the cell to reveal/flag: ");
            if (scanf("%d %d", &x, &y) != 2 || x < 0 || x >= TAILLE || y < 0 || y >= TAILLE) {
                printf("Invalid coordinates.\n");
                continue;
            }

            char action;
            printf("Reveal (r) or place/remove a flag (d)? ");
            scanf(" %c", &action);

            if (action == 'r') {
                if (grille[x][y] == '*') {
                    printf("Game Over\n");
                    finDuJeu = 1;
                } else {
                    revelerCase(x, y);
                    if (verifierVictoire()) {
                        printf("Congratulations, you won!\n");
                        finDuJeu = 1;
                    }
                }
            } else if (action == 'd') {
                basculerDrapeau(x, y);
            } else {
                printf("Unrecognized action.\n");
            }
        }

        printf("\t1- Play Again.\n");
        printf("\t2- Exit.\n");
        printf("Choice: ");
        scanf("%d", &n);
    } while (n == 1);
getch();
    return 0;
}
