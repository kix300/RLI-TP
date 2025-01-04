#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define PROXY_IP "0.0.0.0" //ip du proxy
//#define PROXY_IP "192.168.1.166" //ip pc laregion
//#define PROXY_IP "192.168.1.48" // ip steam deck
#define PROXY_PORT 65001 //proxy
#define CLIENT_PORT 0
#define BUFFER_SIZE 1024

int main() {
	int sockfd;
	struct sockaddr_in client_addr, proxy_addr;
	char buffer[BUFFER_SIZE];

	// Création de la socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Erreur de création de la socket");
		exit(errno);
	}

	// Initialisation de l'adresse du client
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_port = htons(CLIENT_PORT);

	// Liaison de la socket au port
	if (bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
		perror("Erreur de liaison (bind)");
		close(sockfd);
		exit(errno);
	}

	// Initialisation de l'adresse du serveur
	memset(&proxy_addr, 0, sizeof(proxy_addr));
	proxy_addr.sin_family = AF_INET;
	proxy_addr.sin_addr.s_addr = inet_addr(PROXY_IP);
	proxy_addr.sin_port = htons(PROXY_PORT);

	printf("Client prêt à envoyer des messages. Tapez 'exit' pour quitter.\n");

	while (1) {
		// Lire un message depuis l'utilisateur
		printf("Entrez un message : ");
		fgets(buffer, BUFFER_SIZE, stdin);

		// Supprimer le caractère de saut de ligne
		buffer[strcspn(buffer, "\n")] = '\0';

		// Vérifier si l'utilisateur veut quitter
		if (strcmp(buffer, "exit") == 0) {
			printf("Fermeture du client...\n");
			break;
		}

		// Envoyer le message au serveur
		if (sendto(sockfd, buffer, strlen(buffer), 0,
			 (struct sockaddr*)&proxy_addr, sizeof(proxy_addr)) < 0) {
			perror("Erreur d'envoi");
			close(sockfd);
			exit(EXIT_FAILURE);
		}

		printf("Message envoyé au serveur : %s\n", buffer);

		// Attend un message de reception du server
		/*
		// Réception de la réponse du serveur
		socklen_t addr_len = sizeof(proxy_addr);
		int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
								(struct sockaddr*)&proxy_addr, &addr_len); // boucle infini si il recvfrom ne recois rien 
		if (recv_len < 0) {
			perror("Erreur de réception");
			close(sockfd);
			exit(EXIT_FAILURE);
		}

		buffer[recv_len] = '\0';
		printf("Réponse du serveur : %s\n", buffer);
		*/
	}

	close(sockfd);
	return 0;
}
