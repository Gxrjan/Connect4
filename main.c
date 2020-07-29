#include"headers.c"
#include<errno.h>
#include"game.h"
#include<stdlib.h>


int main(int argc, char *argv[])
{
    Game g;
    if (argc != 4 && argc != 2) {
        fprintf(stderr, "Server Usage: connect4 rows columns len\n\
                        Client Usage: connect4 hostname");
        return 1;
    }
    if (argc == 2) {
        int fd = setup_client(argv[1]);
        if (fd < 0)
            return 1;
        int game_attrs[3];
        int game_attrs_len = recv(fd, &game_attrs, sizeof(game_attrs), 0);
        if (game_attrs_len < 0) {
            fprintf(stderr, "recv error: %d\n", errno);
            return 1;
        }
        if (game_attrs_len == 0) {
            fprintf(stderr, "Server closed connection\n");
            return 1;
        }
        init(&g, game_attrs[0], game_attrs[1], game_attrs[2]);
        start_client(&g, fd);  
    } else {
        int rows = atoi(argv[1]);
        int columns = atoi(argv[2]);
        int len = atoi(argv[3]);
        init(&g, rows, columns, len);

        int commfd = setup_server();
        if (commfd < 0)
            return 1;
        int first_message[3] = { rows, columns, len};
        size_t fst_msg_len = sizeof(first_message);
        int first_bytes_sent;
        first_bytes_sent = send(commfd, first_message, fst_msg_len, 0);
        if (first_bytes_sent < fst_msg_len) {
           fprintf(stderr, "Couldn't send all of attributes of a game\n");
           return 1;
        }
          
        start_server(&g, commfd);
    }
    return 0;
}
