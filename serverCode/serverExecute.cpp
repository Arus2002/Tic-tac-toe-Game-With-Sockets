#include "tic-tac-toe.hpp"

void Game::execute() {
        Player1 player1('X', "player1");
        mPlayer = &player1;
        std::size_t pos1 = 0;
        std::size_t pos2 = 0;
        std::cout << std::endl << mPlayer -> getPlayer() << ": Enter positions!" << std::endl;
        std::cin >> pos1 >> pos2;
        if (mBoard.checkPosition(pos1, pos2)) {
            mBoard.setValueAtPosition(pos1, pos2, 'X');
	}
	mBoard.deserialize(mBoard.serialize());
}
