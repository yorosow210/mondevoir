#include "common.h"

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    int compteur;

    // Création du socket client
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialisation de l'adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // Connexion au serveur
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Réception du compteur initial du serveur
    recv(client_fd, buffer, sizeof(buffer), 0);
    compteur = atoi(buffer);
    printf("Compteur reçu du serveur: %d\n", compteur);

    // Boucle pour l'incrémentation
    while (1) {
        // Incrémentation du compteur
        compteur++;
        printf("Compteur incrémenté par le client: %d\n", compteur);

        // Envoi du compteur incrémenté au serveur
        snprintf(buffer, sizeof(buffer), "%d", compteur);
        send(client_fd, buffer, strlen(buffer), 0);

        // Attente d'une seconde avant la prochaine itération
        sleep(1);

        // Réception du compteur incrémenté du serveur
        recv(client_fd, buffer, sizeof(buffer), 0);
        compteur = atoi(buffer);
        printf("Compteur reçu du serveur: %d\n", compteur);
    }

    // Fermeture de la connexion
    close(client_fd);
    return 0;
}
