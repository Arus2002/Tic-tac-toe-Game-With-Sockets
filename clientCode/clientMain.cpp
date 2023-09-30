#include "tic-tac-toe.hpp"


int main() {
    Board board;
    Game tic_tac_toe(board);
    tic_tac_toe.clientSocket();
}
