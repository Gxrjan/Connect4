#include"game.h"
#include"conio.h"
#include<stdio.h>
#include<stdlib.h>

void init(Game *g, int rows, int columns, int len)
{
    init_board(&(g->board), rows, columns);
    g->rows = rows;
    g->columns = columns;
    g->len = len;
    g->player = 1;
    g->c=0;
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
            g->player = (g->player==1) ? 2 : 1;
            return 1;
        }
        pos++;
    }

    return 0;
}


int main(int argc, char *argv[])
{
    if (argc != 4) {
        printf("Incorrect usage\n");
        exit(-1);
    }
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    int len = atoi(argv[3]);
    Game g;
    init(&g, rows, columns, len);
    printf("rows: %d, columns: %d\n", g.rows, g.columns);
    printf("len: %d\n", g.len);
    printf("Player: %d\n", g.player);
    print_board(&g);
    move_cursor(&g, 'r');
    make_move(&g);
    print_board(&g);
    char command;
    print_board(&g);
    while (1) {
        command = getch();
        switch (command) {
            case 'a':
                move_cursor(&g, 'l');
                break;
            case 'd':
                move_cursor(&g, 'r');
                break;
            case 's':
                make_move(&g);
                break;
        }
        system("clear");
        print_board(&g);

    }
    return 0;
}
