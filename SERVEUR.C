#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

int main() {
    int fd_serveur, nouveau_socket;
    struct sockaddr_in adresse;
    int taille_adresse = sizeof(adresse);
    int compteur = COMPTEUR_INITIAL;

    char buffer[TAILLE_BUFFER] = {0};

    // Création du descripteur de socket
    if ((fd_serveur = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Échec de la création de la socket");
        exit(EXIT_FAILURE);
    }

    // Association de la socket au réseau
    adresse.sin_family = AF_INET;
    adresse.sin_addr.s_addr = INADDR_ANY;
    adresse.sin_port = htons(PORT);

    if (bind(fd_serveur, (struct sockaddr *)&adresse, sizeof(adresse)) < 0) {
        perror("Échec de l'association");
        exit(EXIT_FAILURE);
    }

    // Écoute des connexions entrantes
    if (listen(fd_serveur, 3) < 0) {
        perror("Échec de l'écoute");
        exit(EXIT_FAILURE);
    }

    printf("Le serveur écoute sur le port %d\n", PORT);

    if ((nouveau_socket = accept(fd_serveur, (struct sockaddr *)&adresse, (socklen_t*)&taille_adresse)) < 0) {
        perror("Échec de l'acceptation");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Envoi du compteur au client
        snprintf(buffer, TAILLE_BUFFER, "%d", compteur);
        send(nouveau_socket, buffer, strlen(buffer), 0);
        printf("Compteur envoyé : %d\n", compteur);

        // Attente du compteur mis à jour du client
        memset(buffer, 0, TAILLE_BUFFER);
        read(nouveau_socket, buffer, TAILLE_BUFFER);
        compteur = atoi(buffer) + 1;
        printf("Compteur reçu : %d\n", compteur);

        sleep(1);
    }

    close(nouveau_socket);
    close(fd_serveur);
    return 0;
}
