#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DEBUT_PORT 0
#define FIN_PORT 3000

int main() {
    int socket_fd;
    struct sockaddr_in adresse_serveur;

    // Configuration de l'adresse du serveur
    memset(&adresse_serveur, 0, sizeof(adresse_serveur));
    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Scan des ports TCP ouverts sur 127.0.0.1 (ports %d-%d):\n", DEBUT_PORT, FIN_PORT);
     int port;
    for (port = DEBUT_PORT; port <= FIN_PORT; port++) {
        // Création de la socket
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0) {
            perror("Erreur lors de la création de la socket");
            exit(EXIT_FAILURE);
        }

        // Définir le port à tester
        adresse_serveur.sin_port = htons(port);

        // Tentative de connexion
        if (connect(socket_fd, (struct sockaddr*)&adresse_serveur, sizeof(adresse_serveur)) == 0) {
            // Si la connexion réussit, afficher le port
            printf("Port ouvert : %d\n", port);
        }

        // Fermeture de la socket
        close(socket_fd);
    }

    printf("Scan terminé.\n");
    return 0;
}
