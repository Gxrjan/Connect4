#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdlib.h>
#include<errno.h>
#include"game.h"

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
        exit(1);
    }

    fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (fd < 0) {
        fprintf(stderr, "Error creating a socket\n");
        return 1;
    }

    status = connect(fd, servinfo->ai_addr, servinfo->ai_addrlen);
    if (status < 0) {
        fprintf(stderr, "Error trying to connect\n");
        return 1;
    }
    return fd; 
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: client hostname\n");
        return 1;
    }
    int fd = setup_client(argv[1]);
    int game_attrs[3];
    int game_attrs_len;
    game_attrs_len = recv(fd, &game_attrs, sizeof(game_attrs), 0);
    if (game_attrs_len < 0) {
        fprintf(stderr, "recv error: %d\n", errno);
        return 1;
    }
    if (game_attrs_len == 0) {
        fprintf(stderr, "Server closed connection\n");
        return 1;
    }
    //printf("Rows: %d\n", game_attrs[0]);
    //printf("Columns: %d\n", game_attrs[1]);
    //printf("Len: %d\n", game_attrs[2]);
    Game g;
    init(&g, game_attrs[0], game_attrs[1], game_attrs[2]);

    start_client(&g, fd);


    //freeaddrinfo(servinfo);

}
