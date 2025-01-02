#ifndef SERVEUR_H
#define SERVEUR_H

#include <stdint.h>
#include <netinet/in.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Types de protocoles pour les connexions
typedef enum {
    PROTOCOL_TCP,
    PROTOCOL_UDP
} ProtocolType;

// Structure pour gérer une adresse IP
typedef struct {
    struct sockaddr_in address;
} IPAddress;

// Structure pour un client
typedef struct {
    int socket_fd;         // Socket du client
    IPAddress ip;          // Adresse IP du client
} Client;

// Structure pour un serveur
typedef struct {
    int socket_fd;         // Socket du serveur
    ProtocolType protocol; // Protocole utilisé (TCP ou UDP)
    IPAddress ip;          // Adresse IP du serveur
} Server;

// Fonctions pour le serveur
int init_server(Server *server, const char *ip, uint16_t port, ProtocolType protocol);
int accept_client(Server *server, Client *client);
int send_message(Client *client, const char *message, size_t length);
int receive_message(Client *client, char *buffer, size_t length);
void close_connection(Client *client);
void shutdown_server(Server *server);

#endif // SERVEUR_H
