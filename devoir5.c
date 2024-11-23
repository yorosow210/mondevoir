#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> 

// Variables globales
int nombre_threads;     // Nombre total de threads
sem_t* semaphores;      // Tableau de sémaphores


void* fonction_thread(void* arg) {
    int id = (int)arg; // Identifiant du thread (reçu comme un entier simple)

    while (1) { // Exécution infinie
        sem_wait(&semaphores[id]); // Attend son tour
        printf("Thread %d vient de s'exécuter.\n", id + 1);
        fflush(stdout); // Force l'affichage
        sleep(1); 
        sem_post(&semaphores[(id + 1) % nombre_threads]); // Passe au suivant
    }

    return NULL; 
}

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        printf("Usage: %s <nombre de threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    
    nombre_threads = atoi(argv[1]);
    if (nombre_threads <= 0) {
        printf("Erreur : Le nombre de threads doit être un entier positif !\n");
        return EXIT_FAILURE;
    }

    // Allocation et initialisation des sémaphores
    semaphores = malloc(nombre_threads * sizeof(sem_t));
    if (semaphores == NULL) {
        perror("Erreur d'allocation mémoire");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < nombre_threads; i++) {
        sem_init(&semaphores[i], 0, 0); 
    }
    sem_post(&semaphores[0]); // Démarre avec le premier thread

    // Création des threads
    pthread_t threads[nombre_threads];
    for (int i = 0; i < nombre_threads; i++) {
        if (pthread_create(&threads[i], NULL, fonction_thread, (void*)i) != 0) {
            perror("Erreur de création de thread");
            free(semaphores);
            return EXIT_FAILURE;
        }
    }

    // Attente infinie pour les threads (optionnel ici)
    for (int i = 0; i < nombre_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // suppression des sémaphores
    for (int i = 0; i < nombre_threads; i++) {
        sem_destroy(&semaphores[i]);
    }
    free(semaphores);

    return EXIT_SUCCESS;
}
