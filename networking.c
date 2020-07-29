#include "headers.c"
#include<errno.h>
#include<stdlib.h>
#define PORT "5000"
#define BACKLOG 10

int setup_client(char *hostname) {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    int fd;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, "5000", &hints, &servinfo);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo  error: %s\n", gai_strerror(status));
        return -1;
    }

    fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (fd < 0) {
        fprintf(stderr, "Error creating a socket\n");
        return -1;
    }

    status = connect(fd, servinfo->ai_addr, servinfo->ai_addrlen);
    if (status < 0) {
        fprintf(stderr, "Error trying to connect\n");
        return -1;
    }
    freeaddrinfo(servinfo);
    return fd; 
}


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
        return -1;
    }
    
    sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);
    if (sockfd < 0) {
        fprintf(stderr, "Error creating a socket\n");
        return -1;
    }

    status = bind(sockfd, serv_info->ai_addr, serv_info->ai_addrlen);
    if (status < 0) {
        fprintf(stderr, "Error while creating a bind\n");
        return -1;
    }

    status = listen(sockfd, BACKLOG);
    if (status < 0) {
        fprintf(stderr, "Error while trying to listen\n");
        return -1;
    }
    
    client_size = sizeof(client);
    commfd = accept(sockfd, (struct sockaddr *)&client, &client_size);
    if (commfd < 0) {
        fprintf(stderr, "Error accepting\n");
        return -1;
    }

    freeaddrinfo(serv_info);
    return commfd;
    
}

