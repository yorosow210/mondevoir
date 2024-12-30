#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"

int main() {
    int socket_client = 0;
    struct sockaddr_in adresse_serveur;
    char buffer[TAILLE_BUFFER] = {0};
    int compteur;

    // Création de la socket
    if ((socket_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Erreur de création de la socket\n");
        return -1;
    }

    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_port = htons(PORT);

    // Conversion de l'adresse IPv4 du texte en binaire
    if (inet_pton(AF_INET, "127.0.0.1", &adresse_serveur.sin_addr) <= 0) {
        printf("Adresse invalide / Adresse non supportée\n");
        return -1;
    }

    // Connexion au serveur
    if (connect(socket_client, (struct sockaddr *)&adresse_serveur, sizeof(adresse_serveur)) < 0) {
        printf("Échec de la connexion\n");
        return -1;
    }

    while (1) {
        // Réception du compteur du serveur
        memset(buffer, 0, TAILLE_BUFFER);
        read(socket_client, buffer, TAILLE_BUFFER);
        compteur = atoi(buffer) + 1;
        printf("Compteur reçu : %d\n", compteur);

        // Envoi du compteur mis à jour au serveur
        snprintf(buffer, TAILLE_BUFFER, "%d", compteur);
        send(socket_client, buffer, strlen(buffer), 0);
        printf("Compteur envoyé : %d\n", compteur);

        sleep(1);
    }

    close(socket_client);
    return 0;
}
