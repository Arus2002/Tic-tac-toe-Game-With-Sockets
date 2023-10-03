#include "Board.hpp"

Board::Board() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = ' ';
        }
    }
}

bool Board::checkPosition(std::size_t i, std::size_t j) {
    if (board[i][j] == ' ') {
        return true;
    }
    return false;
}

char Board::getValueAtPosition(int row, int col) {
    if ((row < 3 && row >= 0) || (col < 3 && col >= 0)) {
        return board[row][col];
    }
    return ' ';
}

void Board::setValueAtPosition(int row, int col, char elem) {
    if (checkPosition(row, col)) {
        board[row][col] = elem;
    }
}

void Board::draw() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << "-----";
            std::cout << "   ";
        }
        std::cout << std::endl;
        for (int j = 0; j < 3; ++j) {
            if (j == 0) {
                std::cout << "| " << board[i][j] << " | ";
            }
            else {
                std::cout << "  | " << board[i][j] << " | ";
            }
        }
        std::cout << std::endl;
    }
    for (int j = 0; j < 3; ++j) {
        std::cout << "-----";
        std::cout << "   ";
    }
    std::cout << std::endl;
}

