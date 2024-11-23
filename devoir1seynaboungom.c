#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Définition des fonctions
int f(int n) {
    return pow(2, n);
}

int g(int n) {
    return 2 * n;
}

int main(int argc, char *argv[]) {
    int f_value = -1, g_value = -1; // Pour stocker les arguments
    int i;

    // Parcours des arguments de la ligne de commande
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            f_value = atoi(argv[++i]); // Récupérer la valeur après -f
        } else if (strcmp(argv[i], "-g") == 0 && i + 1 < argc) {
            g_value = atoi(argv[++i]); // Récupérer la valeur après -g
        }
    }

    // Cas où les deux options sont présentes
    if (f_value != -1 && g_value != -1) {
        printf("fog(%d) = f(g(%d)) = %d\n", g_value, g_value, f(g(g_value)));
        printf("gof(%d) = g(f(%d)) = %d\n", f_value, f_value, g(f(f_value)));
    } 
    // Cas où seule -f est utilisée
    else if (f_value != -1) {
        printf("f(%d) = %d\n", f_value, f(f_value));
    } 
    // Cas où seule -g est utilisée
    else if (g_value != -1) {
        printf("g(%d) = %d\n", g_value, g(g_value));
    } 
    // Aucun argument valide
    else {
        printf("Veuillez spécifier au moins une des options -f ou -g avec un entier.\n");
    }

    return 0;
}