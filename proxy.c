#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define PROXY_PORT 65001          // Port du proxy
#define SERVER_IP "192.168.1.48" // IP du serveur cible
#define SERVER_PORT 65000         // Port du serveur cible
#define BUFFER_SIZE 1024          // Taille du tampon

// Fonction pour calculer le checksum
unsigned short checksum(void *b, int len) {
	unsigned short *buf = b;
	unsigned int sum = 0;
	unsigned short result;

	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

int main() {
	int recv_sock;
	struct sockaddr_in proxy_addr, client_addr, server_addr;
	char buffer[BUFFER_SIZE];
	socklen_t addr_len = sizeof(struct sockaddr_in);

	// Étape 1 : Création de la socket UDP pour recevoir les paquets
	if ((recv_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Erreur de création de la socket de réception");
		exit(EXIT_FAILURE);
	}

	// Liaison de la socket au port du proxy
	memset(&proxy_addr, 0, sizeof(proxy_addr));
	proxy_addr.sin_family = AF_INET;
	proxy_addr.sin_addr.s_addr = INADDR_ANY;
	proxy_addr.sin_port = htons(PROXY_PORT);

	if (bind(recv_sock, (struct sockaddr *)&proxy_addr, sizeof(proxy_addr)) < 0) {
		perror("Erreur de liaison (bind) du proxy");
		close(recv_sock);
		exit(EXIT_FAILURE);
	}

	printf("Proxy en attente d'un message sur le port %d\n", PROXY_PORT);

	while (1)
	{

		// Étape 2 : Réception d'un message du client
		memset(buffer, 0, BUFFER_SIZE);
		int recv_len = recvfrom(recv_sock, buffer, BUFFER_SIZE, 0,
						  (struct sockaddr *)&client_addr, &addr_len);
		if (recv_len < 0) {
			perror("Erreur de réception du client");
			close(recv_sock);
			exit(EXIT_FAILURE);
		}

		printf("Message reçu du client : %s\n", buffer);

		// Étape 3 : Analyse de la trame
		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
		printf("Adresse IP du client : %s\n", client_ip);

		// Étape 4 : Construction de la nouvelle trame avec modification de l'IP source
		/*
	char packet[BUFFER_SIZE];
	struct iphdr *ip_header = (struct iphdr *)packet;
	struct udphdr *udp_header = (struct udphdr *)(packet + sizeof(struct iphdr));
	char *data = packet + sizeof(struct iphdr) + sizeof(struct udphdr);

	// Copie des données
	memcpy(data, buffer, recv_len);

	// Configuration de l'en-tête UDP
	udp_header->source = htons(client_addr.sin_port); // Port source
	udp_header->dest = htons(SERVER_PORT);            // Port destination
	udp_header->len = htons(sizeof(struct udphdr) + recv_len);
	udp_header->check = 0; // Pas de checksum pour simplifier

	// Configuration de l'en-tête IP
	ip_header->ihl = 5;
	ip_header->version = 4;
	ip_header->tos = 0;
	ip_header->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + recv_len);
	ip_header->id = htonl(rand() % 65535);
	ip_header->frag_off = 0;
	ip_header->ttl = 64;
	ip_header->protocol = IPPROTO_UDP;
	ip_header->check = 0;
	ip_header->saddr = inet_addr("192.168.100.100"); // Nouvelle IP source fictive
	ip_header->daddr = inet_addr(SERVER_IP);         // IP du serveur

	// Calcul du checksum de l'en-tête IP
	ip_header->check = checksum(ip_header, sizeof(struct iphdr));
	*/
		// Étape 5 : Envoi de la trame modifiée au serveur
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
		server_addr.sin_port = htons(SERVER_PORT);

		if (sendto(recv_sock, buffer, strlen(buffer),0,
			 (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
			perror("Erreur d'envoi au serveur");
			close(recv_sock);
			exit(EXIT_FAILURE);
		}

		printf("Message transmis au serveur avec l'IP source modifiée.\n");

		memset(buffer, 0, BUFFER_SIZE);
		recv_len = recvfrom(recv_sock, buffer, BUFFER_SIZE, 0,
					  (struct sockaddr*)&server_addr, &addr_len);
		if (recv_len < 0)
			perror("Erreur de réception");

		if (sendto(recv_sock, buffer, strlen(buffer),0,
			 (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
			perror("Erreur d'envoi au serveur");
			close(recv_sock);
			exit(EXIT_FAILURE);
		}
	}
	close(recv_sock);
	return 0;
}
