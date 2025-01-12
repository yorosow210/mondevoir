// common.h
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Taille maximale du message
#define MAX_BUFFER_SIZE 1024

// Port et adresse pour la communication
#define SERVER_PORT 8080
#define SERVER_ADDR "127.0.0.1"

#endif
