#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <string>

class Board {
public:
    Board();
    bool checkPosition(std::size_t i, std::size_t j);
    char getValueAtPosition(int row, int col);
    void setValueAtPosition(int row, int col, char elem);
    void draw();

public:
    std::string serialize();
    void deserialize(std::string boardStr);

private:
    char board[3][3];
};

#endif
