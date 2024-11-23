Exo3
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_POINTS 1000

// Variables globales
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
        sem_wait(&sem2); // Attente du feu vert
        counter = -i;
        results[index_result++] = counter; // Enregistrer le résultat
        printf("Thread 2: %d\n", counter);
        sem_post(&sem1); // Signal pour le thread 1
    }
    return NULL;
}

// Fonction pour tracer la courbe avec gnuplot
void plot_graph() {
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        perror("Erreur lors de l'ouverture de gnuplot");
        exit(1);
    }

    // Préparer les données pour gnuplot
    fprintf(gnuplot, "set title 'Évolution des valeurs'\n");
    fprintf(gnuplot, "set xlabel 'Index'\n");
    fprintf(gnuplot, "set ylabel 'Valeur'\n");
    fprintf(gnuplot, "plot '-' with linespoints title 'Valeurs'\n");

    for (int i = 0; i < index_result; i++) {
        fprintf(gnuplot, "%d %d\n", i, results[i]);
    }

    fprintf(gnuplot, "e\n");
    fflush(gnuplot);
    pclose(gnuplot);
}

int main() {
    printf("Entrez la valeur de n : ");
    scanf("%d", &n);

    if (n <= 0 || n >= MAX_POINTS / 2) {
        printf("Veuillez entrer une valeur de n comprise entre 1 et %d.\n", MAX_POINTS / 2);
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

    // Tracer la courbe avec gnuplot
    plot_graph();

    // Nettoyage des sémaphores
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}