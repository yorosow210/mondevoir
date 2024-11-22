#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void split(const char *file_name, long part_size) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    size_t bytes_read;
    long total_bytes = 0;
    int part_num = 1;
    char part_name[256];

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        snprintf(part_name, sizeof(part_name), "%s.part%d", file_name, part_num);

        FILE *part_file = fopen(part_name, "wb");
        if (!part_file) {
            perror("Erreur lors de la création d'une partie");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        fwrite(buffer, 1, bytes_read, part_file);
        fclose(part_file);

        total_bytes += bytes_read;
        if (total_bytes >= part_size) {
            total_bytes = 0;
            part_num++;
        }
    }

    fclose(file);
    printf("Fichier divisé en %d parties.\n", part_num);
}

void join(const char *file_name, int part_count) {
    FILE *output_file = fopen(file_name, "wb");
    if (!output_file) {
        perror("Erreur lors de la création du fichier fusionné");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    size_t bytes_read;
    char part_name[256];

    for (int i = 1; i <= part_count; i++) {
        snprintf(part_name, sizeof(part_name), "%s.part%d", file_name, i);

        FILE *part_file = fopen(part_name, "rb");
        if (!part_file) {
            perror("Erreur lors de l'ouverture d'une partie");
            fclose(output_file);
            exit(EXIT_FAILURE);
        }

        while ((bytes_read = fread(buffer, 1, sizeof(buffer), part_file)) > 0) {
            fwrite(buffer, 1, bytes_read, output_file);
        }

        fclose(part_file);
    }

    fclose(output_file);
    printf("Fichier fusionné avec succès : %s\n", file_name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Utilisation :\n");
        fprintf(stderr, "  Pour diviser : %s split nomfichier taille\n", argv[0]);
        fprintf(stderr, "  Pour fusionner : %s join nomfichier nb_parties\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "split") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Erreur : Arguments insuffisants pour 'split'.\n");
            exit(EXIT_FAILURE);
        }
        long part_size = atol(argv[3]);
        split(argv[2], part_size);
    } else if (strcmp(argv[1], "join") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Erreur : Arguments insuffisants pour 'join'.\n");
            exit(EXIT_FAILURE);
        }
        int part_count = atoi(argv[3]);
        join(argv[2], part_count);
    } else {
        fprintf(stderr, "Erreur : Commande non reconnue.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}