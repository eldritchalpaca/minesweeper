#ifndef TILE_H
#define TILE_H

#define BOARD_X 20
#define BOARD_Y 12

class tile {
private:
    bool is_bomb;

public:
    tile(bool is_bomb) : is_bomb(is_bomb) {}
    bool check_if_bomb() { return is_bomb; }
    ~tile() {}
};

class game_board {
private:
    int num_bombs;

public:
    tile *board[BOARD_Y][BOARD_X];
    game_board(int num_bombs);
    ~game_board();
};

#endif