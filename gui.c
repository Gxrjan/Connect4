#include<stdio.h>
#include"game.h"


void print_board(Game *g)
{
    for (int j=0;j<g->columns;j++)
        printf("%c%s", (j==g->c) ? 'v' : ' ', (j==g->columns-1) ? "\n" : " "); 
    for (int i=0;i<g->rows;i++)
        for (int j=0;j<g->columns;j++)
            printf("%d%s", g->board[i][j], (j==g->columns-1) ? "\n" : " ");
}


int move_cursor(Game *g, char d)
{
    switch(d) {
        case 'r':
            if (g->c+1 <= g->columns-1) {
                g->c++;
                return 1;
            }
            return 0;
            break;
        case 'l':
            if (g->c-1 >= 0) {
                g->c--;
                return 1;
            }
            return 0;
            break;

    }

    return 0;
}
