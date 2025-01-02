#ifndef UDP_UTILS_H
#define UDP_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define SERVER_PORT 65000
#define CLIENT_PORT 49450
#define MAX_MSG_LENGTH 100

// Fonction pour créer une socket UDP et gérer les erreurs
int create_udp_socket();

// Fonction pour remplir une structure sockaddr_in avec une adresse IP et un port
void setup_address(struct sockaddr_in *address, const char *ip, int port);

// Fonction pour envoyer un message UDP
int send_udp_message(int socket_fd, const char *message, struct sockaddr_in *dest_addr);

// Fonction pour recevoir un message UDP
int receive_udp_message(int socket_fd, char *buffer, struct sockaddr_in *src_addr);

// Fonction pour convertir une adresse IP en entier long non signé
unsigned long ip_to_long(const char *ip_str);

// Fonction pour convertir une adresse IP en format texte
const char* long_to_ip_str(unsigned long ip_long, char *buffer, size_t buffer_size);

#endif // UDP_UTILS_H
