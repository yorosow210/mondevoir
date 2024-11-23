#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_POINTS 1000

int counter = 0;
int n = 0;
int results[MAX_POINTS];
int index_result = 0;

// Sémaphores
sem_t sem1, sem2;

// Fonction pour le thread 1 (+1 jusqu'à n)
void *thread1_function(void *arg) {
    for (int i = 0; i <= n; i++) {
        sem_wait(&sem1); // Attente du feu vert
        counter = i;
        results[index_result++] = counter; // Enregistrer le résultat
        printf("Thread 1: %d\n", counter);
        sem_post(&sem2); // Signal pour le thread 2
    }
    return NULL;
}

// Fonction pour le thread 2 (-1 jusqu'à -n)
void *thread2_function(void *arg) {
    for (int i = 0; i <= n; i++) {
        sem_wait(&sem2); 
        counter = -i;
        results[index_result++] = counter; // Enregistrer le résultat
        printf("Thread 2: %d\n", counter);
        sem_post(&sem1); // Signal pour le thread 1
    }
    return NULL;
}

int main() {
    printf("Entrez la valeur de n (entre 1 et %d) : ", MAX_POINTS / 2);
    
    // Contrôle de saisie
    if (scanf("%d", &n) != 1) { // Vérifie si un entier a été saisi
        fprintf(stderr, "Erreur : entrée invalide. Veuillez entrer un entier.\n");
        return 1;
    }

    if (n <= 0 || n >= MAX_POINTS / 2) { // Vérifie les bornes de n
        printf("Erreur : veuillez entrer une valeur de n comprise entre 1 et %d.\n", MAX_POINTS / 2);
        return 1;
    }

    pthread_t thread1, thread2;

    // Initialisation des sémaphores
    sem_init(&sem1, 0, 1); // Le thread 1 démarre
    sem_init(&sem2, 0, 0);

    // Création des threads
    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);

    // Attente de la fin des threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Enregistrement des résultats dans un fichier
    FILE *file = fopen("results.csv", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    fprintf(file, "# Index,Value\n"); // En-tête pour le fichier CSV
    for (int i = 0; i < index_result; i++) {
        fprintf(file, "%d,%d\n", i, results[i]);
    }

    fclose(file);
    printf("Résultats enregistrés dans 'results.csv'.\n");

    // Appel de Gnuplot pour tracer la courbe
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        perror("Erreur lors de l'appel à Gnuplot");
        return 1;
    }

    // Script pour tracer la courbe
    fprintf(gnuplot, "set title 'Variation des Valeurs entre -n et +n'\n");
    fprintf(gnuplot, "set xlabel 'Index'\n");
    fprintf(gnuplot, "set ylabel 'Value'\n");
    fprintf(gnuplot, "set grid\n");
    fprintf(gnuplot, "set style data linespoints\n");
    fprintf(gnuplot, "plot 'results.csv' using 1:2 with linespoints title 'Valeurs générées'\n");

    pclose(gnuplot);

    // Nettoyage des sémaphores
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}
