#include "tile.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>

game_board::game_board(int num_bombs) {
    int x, y;
    int n = 0;

    srand(time(NULL));

    while (n < num_bombs) {
        x = rand() % BOARD_X;
        y = rand() % BOARD_Y;
        if (!board[y][x]) {
            board[y][x] = new tile(true);
            //printf("%d: (%d, %d)\n", n, x, y);
            n++;
        }
    }

    for (y = 0; y < BOARD_Y; ++y) {
        for (x = 0; x < BOARD_X; ++x) {
            if (!board[y][x]) {
                board[y][x] = new tile(false);
            }
        }
    }
}

game_board::~game_board() {
    int x, y;

    for (y = 0; y < BOARD_Y; ++y) {
        for (x = 0; x < BOARD_X; ++x) {
            delete board[y][x];
        }
    }
}