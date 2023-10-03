#include "Player.hpp"

Players::Players(char myElem)
    : mMyElem{myElem} {}

Player1::Player1(char myElem, std::string playerName)
    : Players(myElem)
    , mPlayer1{playerName} {}

std::string Player1::getPlayer() const {
    return mPlayer1;
}

void Player1::setPlayer(std::string name) {
    mPlayer1 = "player1";
}

Player2::Player2(char myElem, std::string playerName)
    : Players(myElem)
    , mPlayer2{playerName} {}

std::string Player2::getPlayer() const {
    return mPlayer2;
}

void Player2::setPlayer(std::string name) {
    mPlayer2 = "player2";
}



