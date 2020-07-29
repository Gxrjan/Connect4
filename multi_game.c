#include"game.h"
#include"conio.h"
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"headers.c"

void init(Game *g, int rows, int columns, int len)
{
    init_board(&(g->board), rows, columns);
    g->rows = rows;
    g->columns = columns;
    g->len = len;
    g->player = 1;
    g->c=0;
    g->win_coords = malloc(g->len*sizeof(Coordinate));
}

void init_board(Matrix *b, int rows, int columns)
{
    *b = malloc(rows*sizeof(Array));
    for (int i=0;i<rows;i++)
        (*b)[i] = malloc(columns*sizeof(int));
    for (int i=0;i<rows;i++)
        for (int j=0;j<columns;j++)
            (*b)[i][j] = 0;


}


int make_move(Game *g)
{
    if (g->board[0][g->c] != 0)
        return 0;
    int pos = 0;
    while (1) {
        if (pos==g->rows-1 || g->board[pos+1][g->c] != 0) {
            g->board[pos][g->c] = g->player;
            //g->player = (g->player==1) ? 2 : 1;
            return 1;
        }
        pos++;
    }

    return 0;
}

int valid(Game *g, int i, int j)
{
    return i>=0 && i <g->rows && j>=0 && j<g->columns;
}


int game_won(Game *g)
{
    for (int i=0;i<g->rows;i++)
        for (int j=0;j<g->columns;j++)
            if (all(g, i, j, -1, 1) \
                || all(g, i, j, 0, 1)\
                || all(g, i, j, 1, 1)\
                || all(g, i, j, 1, 0))
                return 1;

    return 0;
}

int all(Game *g, int i, int j, int di, int dj)
{
    int n = 0;
    for (int k=0;k<g->len;k++) {
        if (valid(g, i+k*di, j+k*dj)) {
            if (g->board[i+k*di][j+k*dj] != g->player)
                return 0;
        } else {
            return 0;
        }
        n++;
    }

    if (n==g->len) {
        g->winner = g->player;
        for (int k=0;k<g->len;k++) {
            //g->win_coords[k] = malloc(sizeof(Coordinate));
            g->win_coords[k].i = i+k*di; 
            g->win_coords[k].i = j+k*dj; 
        }
        return 1;
    }
    return 0;
}



void start_server(Game *g, int fd)
{
    int out_cmd;
    size_t out_cmd_len;
    int bytes_sent;
    int inc_cmd;
    int inc_cmd_len;
    system("clear");
    print_board(g);
    while (1) {
        out_cmd = getch();
        switch (out_cmd) {
            case 'a':
                move_cursor(g, 'l');
                goto clr;
                break;
            case 'd':
                move_cursor(g, 'r');
                goto clr;
                break;
            case 's':
                make_move(g);
                out_cmd = g->c;
                bytes_sent = send(fd, &out_cmd, sizeof(int), 0);
                if (game_won(g)) {
                    system("clear");
                    print_board(g);
                    printf("Player %d won!\n", g->player);
                    return;
                }
                g->player = (g->player==1) ? 2 : 1;
                break;
            default:
                system("clear");
                print_board(g);
                printf("'a' to move left, 'd' to move right, 's' to make a move\n");
                continue;
        }
        system("clear");
        print_board(g);
        inc_cmd_len = recv(fd, &inc_cmd, sizeof(int), 0);
        if (inc_cmd_len < 0) {
            fprintf(stderr, "recv error: %d\n", errno);
            return;
        }
        if (inc_cmd_len == 0) {
            fprintf(stderr, "Client closed connection\n");
            return;
        }
        g->c = inc_cmd;
        make_move(g);
        if (game_won(g)) {
            system("clear");
            print_board(g);
            printf("Player %d won!\n", g->player);
            return;
        }
        g->player = (g->player==1) ? 2 : 1;
    clr:
        system("clear");
        print_board(g);
    }
    
}

void start_client(Game *g, int fd)
{
    //g->player = 2;
    int out_cmd;
    size_t out_cmd_len;
    int bytes_sent;
    int inc_cmd;
    int inc_cmd_len;
    system("clear");
    print_board(g);
    inc_cmd_len = recv(fd, &inc_cmd, sizeof(int), 0);
    if (inc_cmd_len < 0) {
        fprintf(stderr, "recv error: %d\n", errno);
        return;
    }
    if (inc_cmd_len == 0) {
        fprintf(stderr, "Client closed connection\n");
        return;
    }
    g->c = inc_cmd;
    make_move(g);
    if (game_won(g)) {
        system("clear");
        print_board(g);
        printf("Player %d won!\n", g->player);
        return;
    }
    g->player = (g->player==1) ? 2 : 1;
    system("clear");
    print_board(g);
    while (1) {
        out_cmd = getch();
        switch (out_cmd) {
            case 'a':
                move_cursor(g, 'l');
                goto clr;
                break;
            case 'd':
                move_cursor(g, 'r');
                goto clr;
                break;
            case 's':
                make_move(g);
                out_cmd = g->c;
                bytes_sent = send(fd, &out_cmd, sizeof(int), 0);
                if (game_won(g)) {
                    system("clear");
                    print_board(g);
                    printf("Player %d won!\n", g->player);
                    return;
                }
                g->player = (g->player==1) ? 2 : 1;
                break;
            default:
                system("clear");
                print_board(g);
                printf("'a' to move left, 'd' to move left, 's' to make a move\n");
                continue;
        }
        system("clear");
        print_board(g);
        inc_cmd_len = recv(fd, &inc_cmd, sizeof(int), 0);
        if (inc_cmd_len < 0) {
            fprintf(stderr, "recv error: %d\n", errno);
            return;
        }
        if (inc_cmd_len == 0) {
            fprintf(stderr, "Client closed connection\n");
            return;
        }
        g->c = inc_cmd;
        make_move(g);
        if (game_won(g)) {
            system("clear");
            print_board(g);
            printf("Player %d won!\n", g->player);
            return;
        }
        g->player = (g->player==1) ? 2 : 1;
    clr:
        system("clear");
        print_board(g);
    }
    
}

