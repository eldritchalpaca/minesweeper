#include "tile.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>


int game_board::check_tile_neighbors(int x, int y) {
    int num_bombs = 0;
    if (in_bounds(x - 1, y - 1) && board[y - 1][x - 1] && board[y - 1][x - 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x, y - 1)     && board[y - 1][x    ] && board[y - 1][x    ]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x + 1, y - 1) && board[y - 1][x + 1] && board[y - 1][x + 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x - 1, y)     && board[y    ][x - 1] && board[y    ][x - 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x + 1, y)     && board[y    ][x + 1] && board[y    ][x + 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x - 1, y + 1) && board[y + 1][x - 1] && board[y + 1][x - 1]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x, y + 1)     && board[y + 1][x    ] && board[y + 1][x    ]->check_if_bomb()) {
        num_bombs++;
    }
    if (in_bounds(x + 1, y + 1) && board[y + 1][x + 1] && board[y + 1][x + 1]->check_if_bomb()) {
        num_bombs++;
    }
    return num_bombs;
}

std::vector<tile *> game_board::get_tile_neighbors(int x, int y) {
    std::vector<tile *> v;
    if (in_bounds(x - 1, y - 1) && board[y - 1][x - 1]) {
        v.push_back(board[y - 1][x - 1]);
    }
    if (in_bounds(x, y - 1)     && board[y - 1][x    ]) {
        v.push_back(board[y - 1][x    ]);
    }
    if (in_bounds(x + 1, y - 1) && board[y - 1][x + 1]) {
        v.push_back(board[y - 1][x + 1]);
    }
    if (in_bounds(x - 1, y)     && board[y    ][x - 1]) {
        v.push_back(board[y    ][x - 1]);
    }
    if (in_bounds(x + 1, y)     && board[y    ][x + 1]) {
        v.push_back(board[y    ][x + 1]);
    }
    if (in_bounds(x - 1, y + 1) && board[y + 1][x - 1]) {
        v.push_back(board[y + 1][x - 1]);
    }
    if (in_bounds(x, y + 1)     && board[y + 1][x    ]) {
        v.push_back(board[y + 1][x    ]);
    }
    if (in_bounds(x + 1, y + 1) && board[y + 1][x + 1]) {
        v.push_back(board[y + 1][x + 1]);
    }
    return v;
}

game_board::game_board(int num_bombs) {
    int x, y;
    int n = 0;

    for (y = 0; y < BOARD_Y; ++y) {
        for (x = 0; x < BOARD_X; ++x) {
            board[y][x] = NULL;
        }
    }
    

    srand(time(NULL));

    while (n < num_bombs) {
        x = rand() % BOARD_X;
        y = rand() % BOARD_Y;
        if (!board[y][x]) {
            board[y][x] = new tile(true, -1, x, y);
            //printf("%d: (%d, %d)\n", n, x, y);
            n++;
        }
    }

    for (y = 0; y < BOARD_Y; ++y) {
        for (x = 0; x < BOARD_X; ++x) {
            if (!board[y][x]) {
                board[y][x] = new tile(false, check_tile_neighbors(x, y), x, y);
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