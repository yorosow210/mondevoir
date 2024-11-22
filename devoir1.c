#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Déclaration des fonctions
int f(int n);
int g(int n);
int fog(int n);
int gof(int n);

int main(int argc, char *argv[]) {
    int n_f = -1, n_g = -1; // Valeurs pour `f` et `g`
    int calc_fog = 0, calc_gof = 0; // Indicateurs pour fog ou gof
    int i; // Indice pour parcourir les arguments

    // Parcourir les arguments
    for (i = 1; i < argc; i++) {
        switch (argv[i][1]) { // Vérifie la première lettre après le "-"
            case 'f':
                if (i + 1 < argc) {
                    n_f = atoi(argv[++i]); // Lire l'entier suivant pour `f`
                } else {
                    fprintf(stderr, "Erreur : -f nécessite un entier.\n");
                    return 1;
                }
                break;
            case 'g':
                if (i + 1 < argc) {
                    n_g = atoi(argv[++i]); // Lire l'entier suivant pour `g`
                } else {
                    fprintf(stderr, "Erreur : -g nécessite un entier.\n");
                    return 1;
                }
                break;
            default:
                fprintf(stderr, "Erreur : option inconnue '%s'.\n", argv[i]);
                return 1;
        }
    }

    // Déterminer si fog ou gof doit être calculé
    for (i = 1; i < argc - 1; i++) {
        if (argv[i][1] == 'f' && argv[i + 1][1] == 'g') {
            calc_fog = 1; // fog
            break;
        } else if (argv[i][1] == 'g' && argv[i + 1][1] == 'f') {
            calc_gof = 1; // gof
            break;
        }
    }

    // Calculs et affichage des résultats
    if (n_f != -1 && n_g == -1) {
        printf("Résultat de f(%d) = %d\n", n_f, f(n_f));
    } else if (n_g != -1 && n_f == -1) {
        printf("Résultat de g(%d) = %d\n", n_g, g(n_g));
    } else if (calc_fog) {
        printf("Résultat de f(g(%d)) = %d\n", n_g, fog(n_g));
    } else if (calc_gof) {
        printf("Résultat de g(f(%d)) = %d\n", n_f, gof(n_f));
    } else {
        fprintf(stderr, "Erreur : combinaison d'options invalide.\n");
        return 1;
    }

    return 0;
}

// Définition des fonctions
int f(int n) {
    return (int)pow(2, n);  // f(n) = 2^n
}

int g(int n) {
    return 2 * n;  // g(n) = 2 * n
}

int fog(int n) {
    return f(g(n));  // fog(n) = f(g(n)) = 2^(2 * n)
}

int gof(int n) {
    return g(f(n));  // gof(n) = g(f(n)) = 2 * (2^n)
}
