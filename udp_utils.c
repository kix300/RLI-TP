#include "udp_utils.h"

int create_udp_socket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Erreur de création de la socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void setup_address(struct sockaddr_in *address, const char *ip, int port) {
    memset(address, 0, sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &(address->sin_addr)) <= 0) {
        perror("Invalid IP address");
        exit(EXIT_FAILURE);
    }
}

int send_udp_message(int socket_fd, const char *message, struct sockaddr_in *dest_addr) {
    int sent_bytes = sendto(socket_fd, message, strlen(message), 0,
                            (struct sockaddr *)dest_addr, sizeof(struct sockaddr_in));
    if (sent_bytes < 0) {
        perror("Erreur d'envoi du message");
        return -1;
    }
    return sent_bytes;
}

int receive_udp_message(int socket_fd, char *buffer, struct sockaddr_in *src_addr) {
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int received_bytes = recvfrom(socket_fd, buffer, MAX_MSG_LENGTH, 0,
                                  (struct sockaddr *)src_addr, &addr_len);
    if (received_bytes < 0) {
        perror("Erreur de réception du message");
        return -1;
    }
    buffer[received_bytes] = '\0'; // Terminer la chaîne de caractères
    return received_bytes;
}

unsigned long ip_to_long(const char *ip_str) {
    struct sockaddr_in addr;
    inet_pton(AF_INET, ip_str, &(addr.sin_addr));
    return ntohl(addr.sin_addr.s_addr);
}

const char* long_to_ip_str(unsigned long ip_long, char *buffer, size_t buffer_size) {
    struct in_addr ip_addr;
    ip_addr.s_addr = htonl(ip_long);
    return inet_ntop(AF_INET, &ip_addr, buffer, buffer_size);
}
