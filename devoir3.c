#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// Fonction pour découper un fichier
void decouperFichier(const char *nomFichier, size_t tailleMorceau) {
    int fichierSource = open(nomFichier, O_RDONLY);
    if (fichierSource < 0) {
        perror("Erreur lors de l'ouverture du fichier source");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    ssize_t bytesLus, indexFichier = 0;
    while ((bytesLus = read(fichierSource, buffer, tailleMorceau < sizeof(buffer) ? tailleMorceau : sizeof(buffer))) > 0) {
        char nomFichierPartie[256];
        snprintf(nomFichierPartie, sizeof(nomFichierPartie), "%s.part%zd", nomFichier, indexFichier++);

        int fichierPartie = open(nomFichierPartie, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fichierPartie < 0) {
            perror("Erreur lors de la création d'une partie");
            close(fichierSource);
            exit(EXIT_FAILURE);
        }

        if (write(fichierPartie, buffer, bytesLus) < 0) {
            perror("Erreur lors de l'écriture dans la partie");
            close(fichierSource);
            close(fichierPartie);
            exit(EXIT_FAILURE);
        }

        close(fichierPartie);
    }

    if (bytesLus < 0) {
        perror("Erreur lors de la lecture du fichier source");
    }

    close(fichierSource);
    printf("Fichier découpé en %zd parties avec succès.\n", indexFichier);
}

// Fonction pour rassembler plusieurs fichiers en un seul
void assemblerFichiers(const char *nomFichierSortie, int nbParties, char *fichiersParties[]) {
    int fichierSortie = open(nomFichierSortie, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fichierSortie < 0) {
        perror("Erreur lors de la création du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    for (int i = 0; i < nbParties; i++) {
        int fichierPartie = open(fichiersParties[i], O_RDONLY);
        if (fichierPartie < 0) {
            perror("Erreur lors de l'ouverture d'une partie");
            close(fichierSortie);
            exit(EXIT_FAILURE);
        }

        ssize_t bytesLus;
        while ((bytesLus = read(fichierPartie, buffer, sizeof(buffer))) > 0) {
            if (write(fichierSortie, buffer, bytesLus) < 0) {
                perror("Erreur lors de l'écriture dans le fichier de sortie");
                close(fichierSortie);
                close(fichierPartie);
                exit(EXIT_FAILURE);
            }
        }

        if (bytesLus < 0) {
            perror("Erreur lors de la lecture d'une partie");
        }

        close(fichierPartie);
    }

    close(fichierSortie);
    printf("Fichiers rassemblés dans '%s' avec succès.\n", nomFichierSortie);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage pour découper : ./split split <nomFichier> <taille>\n");
        fprintf(stderr, "Usage pour assembler : ./split join <nomFichierSortie> <part1> <part2> ...\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "split") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Usage : ./split split <nomFichier> <taille>\n");
            exit(EXIT_FAILURE);
        }
        size_t tailleMorceau = atoi(argv[3]);
        if (tailleMorceau <= 0) {
            fprintf(stderr, "La taille doit être un entier positif.\n");
            exit(EXIT_FAILURE);
        }
        decouperFichier(argv[2], tailleMorceau);
    } else if (strcmp(argv[1], "join") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Usage : ./split join <nomFichierSortie> <part1> <part2> ...\n");
            exit(EXIT_FAILURE);
        }
        assemblerFichiers(argv[2], argc - 3, &argv[3]);
    } else {
        fprintf(stderr, "Commande invalide. Utilisez 'split' ou 'join'.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
