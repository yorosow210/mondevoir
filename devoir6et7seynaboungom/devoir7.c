#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TARGET_IP "127.0.0.1" 
#define START_PORT 0          
#define END_PORT 3000         

// Fonction pour scanner un port
int scan_port(const char *ip, int port) {
    int sock;
    struct sockaddr_in server_addr;

    // Création du socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Initialisation de la structure sockaddr_in
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Tentative de connexion au port
    int connection_status = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Fermeture du socket
    close(sock);

    // Retourner 1 si la connexion réussit (port ouvert), sinon 0
    if (connection_status == 0) {
        return 1;
    }
    return 0;
}

int main() {
    printf("Scanning ports from %d to %d...\n", START_PORT, END_PORT);

    // Scanner les ports entre START_PORT et END_PORT
    for (int port = START_PORT; port <= END_PORT; port++) {
        if (scan_port(TARGET_IP, port)) {
            printf("Port %d is open\n", port);
        }
    }

    return 0;
}
