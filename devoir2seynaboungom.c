#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

void generate_matrix(int **matrix, int dimension) {
    srand(time(NULL));
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            matrix[i][j] = rand() % 100; // Valeurs entre 0 et 99
        }
    }
}

void save_matrix_binary(int **matrix, int dimension, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier en mode binaire");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < dimension; i++) {
        fwrite(matrix[i], sizeof(int), dimension, file);
    }
    fclose(file);
}

void save_matrix_text(int **matrix, int dimension, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier en mode texte");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void load_matrix_binary(int **matrix, int dimension, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier en mode binaire");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < dimension; i++) {
        fread(matrix[i], sizeof(int), dimension, file);
    }
    fclose(file);
}

void load_matrix_text(int **matrix, int dimension, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier en mode texte");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    fclose(file);
}

void display_matrix(int **matrix, int dimension) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int c_flag = 0, a_flag = 0, t_flag = 0, dimension = 0;
    char *filename = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "cad:btf:")) != -1) {
        switch (opt) {
            case 'c':
                c_flag = 1;
                break;
            case 'a':
                a_flag = 1;
                break;
            case 'd':
                dimension = atoi(optarg);
                break;
            case 'b':
                t_flag = 0; // Mode binaire
                break;
            case 't':
                t_flag = 1; // Mode texte
                break;
            case 'f':
                filename = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-c | -a] -d dimension [-b | -t] -f filename\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!filename) {
        fprintf(stderr, "Erreur : le fichier doit être spécifié avec -f.\n");
        exit(EXIT_FAILURE);
    }

    if (dimension <= 0) {
        fprintf(stderr, "Erreur : une dimension valide doit être spécifiée avec -d.\n");
        exit(EXIT_FAILURE);
    }

    // Allocation de la matrice
    int **matrix = malloc(dimension * sizeof(int *));
    for (int i = 0; i < dimension; i++) {
        matrix[i] = malloc(dimension * sizeof(int));
    }

    if (c_flag) {
        // Création de la matrice
        generate_matrix(matrix, dimension);

        // Sauvegarde de la matrice
        if (t_flag) {
            save_matrix_text(matrix, dimension, filename);
        } else {
            save_matrix_binary(matrix, dimension, filename);
        }
        printf("Matrice de dimension %dx%d créée et enregistrée dans '%s'.\n", dimension, dimension, filename);

    } else if (a_flag) {
        // Chargement de la matrice
        if (t_flag) {
            load_matrix_text(matrix, dimension, filename);
        } else {
            load_matrix_binary(matrix, dimension, filename);
        }

        // Affichage de la matrice
        printf("Contenu de la matrice :\n");
        display_matrix(matrix, dimension);

    } else {
        fprintf(stderr, "Erreur : Vous devez spécifier une action (-c pour créer ou -a pour afficher).\n");
        exit(EXIT_FAILURE);
    }

    // Libération de la mémoire
    for (int i = 0; i < dimension; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}