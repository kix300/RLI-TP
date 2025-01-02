#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 65000
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, proxy_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(proxy_addr);

    // Création de la socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Erreur de création de la socket");
        exit(EXIT_FAILURE);
    }

    // Initialisation de l'adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Liaison de la socket au port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur de liaison (bind)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur UDP en écoute sur le port %d\n", SERVER_PORT);

    // Boucle de réception
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                (struct sockaddr*)&proxy_addr, &addr_len);
        if (recv_len < 0) {
            perror("Erreur de réception");
            continue;
        }

        printf("Message reçu : %s\n", buffer);

        // Conversion de l'adresse IP en format lisible
        char proxy_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(proxy_addr.sin_addr), proxy_ip, INET_ADDRSTRLEN);
        // Affichage du message et de l'adresse IP
        printf("Message reçu de %s : %s\n", proxy_ip, buffer);

        // Envoi de réponse au client
        char *response = "Message reçu par le serveur";
        sendto(sockfd, response, strlen(response), 0,
               (struct sockaddr*)&proxy_addr, addr_len); // renvoyer une response au proxy
    }

    close(sockfd);
    return 0;
}
