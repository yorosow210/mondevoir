#include "common.h"

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];
    int compteur = 0;

    // Création du socket serveur
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialisation de l'adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // Lier le socket à l'adresse
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Écouter les connexions entrantes
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion...\n");

    // Accepter une connexion du client
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Envoi du compteur initial à 0
    snprintf(buffer, sizeof(buffer), "%d", compteur);
    send(client_fd, buffer, strlen(buffer), 0);
    printf("Compteur initial envoyé au client: %d\n", compteur);

    // Boucle pour le compteur distribué
    while (1) {
        // Réception du compteur du client
        recv(client_fd, buffer, sizeof(buffer), 0);
        compteur = atoi(buffer);
        printf("Compteur reçu du client: %d\n", compteur);

        // Incrémentation du compteur
        compteur++;
        printf("Compteur incrémenté par le serveur: %d\n", compteur);

        // Envoi du compteur incrémenté au client
        snprintf(buffer, sizeof(buffer), "%d", compteur);
        send(client_fd, buffer, strlen(buffer), 0);

        // Attente d'une seconde avant la prochaine itération
        sleep(1);
    }

    // Fermeture des connexions
    close(client_fd);
    close(server_fd);
    return 0;
}
