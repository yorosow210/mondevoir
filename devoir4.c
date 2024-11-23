#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h> 
#include <unistd.h>
#include <ctype.h> 

#define FILENAME "resultat.txt"

// Variables partagées
int valeur = 0;
int n;

// Sémaphores
sem_t sem1, sem2;

// Fonction pour le thread 1 : incrémente
void* incrementer(void* arg) {
    int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Ouvre le fichier pour écrire
    if (fd == -1) {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= n; i++) {
        sem_wait(&sem1); // Attend son tour
        valeur = i;
        dprintf(fd, "%d\n", valeur); // Écrit la valeur dans le fichier
        sem_post(&sem2); // Donne le tour au thread 2
    }

    close(fd); // Ferme le fichier
    pthread_exit(NULL);
}

// Fonction pour le thread 2 : décrémente
void* decrementer(void* arg) {
    int fd = open(FILENAME, O_WRONLY | O_APPEND); // Ouvre le fichier en mode append
    if (fd == -1) {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }

    for (int i = n; i >= -n; i--) {
        sem_wait(&sem2); // Attend son tour
        valeur = i;
        dprintf(fd, "%d\n", valeur); // Écrit la valeur dans le fichier
        sem_post(&sem1); // Donne le tour au thread 1
    }

    close(fd); // Ferme le fichier
    pthread_exit(NULL);
}

// Fonction de vérification si une chaîne est un nombre entier positif
int est_entier_positif(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) { // Vérifie si chaque caractère est un chiffre
            return 0; // Ce n'est pas un entier valide
        }
    }
    return 1; // C'est un entier valide
}

int main(int argc, char* argv[]) {
    // Vérifie si un argument a été fourni
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <entier positif>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Vérifie si l'argument est un entier positif
    if (!est_entier_positif(argv[1])) {
        fprintf(stderr, "Erreur : L'argument doit être un entier positif.\n");
        return EXIT_FAILURE;
    }

    // Convertit l'argument en entier
    n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Erreur : L'entier doit être strictement positif.\n");
        return EXIT_FAILURE;
    }

    // Initialisation des sémaphores
    sem_init(&sem1, 0, 1); // Thread 1 commence
    sem_init(&sem2, 0, 0); // Thread 2 attend

    // Création des threads
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, incrementer, NULL);
    pthread_create(&thread2, NULL, decrementer, NULL);

    // Attente des threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destruction des sémaphores
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    // Tracé avec Gnuplot
    FILE* gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        perror("Erreur lors de l'ouverture de Gnuplot");
        exit(EXIT_FAILURE);
    }

    fprintf(gnuplot, "set title 'Variation de la valeur entre -n et +n'\n");
    fprintf(gnuplot, "set xlabel 'Index'\n");
    fprintf(gnuplot, "set ylabel 'Valeur'\n");
    fprintf(gnuplot, "plot '%s' with lines title 'Valeur'\n", FILENAME);
    pclose(gnuplot);

    printf("Le tracé de la courbe a été généré avec Gnuplot.\n");

    return EXIT_SUCCESS;
}
