#include<stdio.h>
#include"game.h"


void print_board(Game *g)
{
    for (int j=0;j<g->columns;j++)
        printf("%c%s", (j==g->c) ? 'v' : ' ', (j==g->columns-1) ? "\n" : " "); 
    for (int i=0;i<g->rows;i++)
        for (int j=0;j<g->columns;j++)
            switch (g->board[i][j]) {
                case 0:
                    printf("%c%s", ' ', (j==g->columns-1) ? "|\n" : " ");
                    break;
                case 1:
                    printf(ANSI_COLOR_RED "%c" ANSI_COLOR_RESET, 'o');
                    printf("%s", (j==g->columns-1) ? "|\n" : " "); 
                    break;
                case 2:
                    printf(ANSI_COLOR_YELLOW "%c" ANSI_COLOR_RESET, 'o');
                    printf("%s", (j==g->columns-1) ? "|\n" : " "); 
                    break;
            }
    for (int j=0;j<g->columns;j++)
        printf("--");
    printf("\n");
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
