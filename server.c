#include "headers.c"
#include<errno.h>
#define PORT "5000"
#define BACKLOG 10
#include "game.h"
#include<stdlib.h>

int setup_server()
{
    struct sockaddr_storage client;
    socklen_t client_size;
    struct addrinfo *serv_info, hints;
    int status;
    int sockfd, commfd;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, PORT, &hints, &serv_info);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }
    
    sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);
    if (sockfd < 0) {
        fprintf(stderr, "Error creating a socket\n");
        return 1;
    }

    status = bind(sockfd, serv_info->ai_addr, serv_info->ai_addrlen);
    if (status < 0) {
        fprintf(stderr, "Error while creating a bind\n");
        return 1;
    }

    status = listen(sockfd, BACKLOG);
    if (status < 0) {
        fprintf(stderr, "Error while trying to listen\n");
        return 1;
    }
    
    client_size = sizeof(client);
    commfd = accept(sockfd, (struct sockaddr *)&client, &client_size);
    if (commfd < 0) {
        fprintf(stderr, "Error accepting\n");
        return 1;
    }
    return commfd;
    
}


int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: server rows columns len\n");
        return 1;
    }
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    int len = atoi(argv[3]);
    Game g;
    init(&g, rows, columns, len);

    int commfd = setup_server();
    int first_message[3] = { rows, columns, len};
    size_t fst_msg_len = sizeof(first_message);
    int first_bytes_sent;
    first_bytes_sent = send(commfd, first_message, fst_msg_len, 0);
    if (first_bytes_sent < fst_msg_len) {
       fprintf(stderr, "Couldn't send all of attributes of a game\n");
       return 1;
    }
      
    start_server(&g, commfd);
    return 0;
}
