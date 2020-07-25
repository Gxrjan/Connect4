#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"


typedef int *  Array;
typedef int ** Matrix;

typedef struct coordinate {
    int i, j;
} Coordinate;

typedef Coordinate Cursor;
typedef Coordinate Vector;

typedef struct game {
    Matrix board;
    int rows;
    int columns;
    int len;
    int player;
    int winner;
    Coordinate *win_coords;
    int c;
} Game;

void print_board(Game *g);
void init(Game *g, int rows, int columns, int len);
void init_board(Matrix *board, int rows, int columns);
void print_board(Game *g);
int move_cursor(Game *g, char d);
int make_move(Game *g);
int all(Game *g, int i, int j, int di, int dj);
