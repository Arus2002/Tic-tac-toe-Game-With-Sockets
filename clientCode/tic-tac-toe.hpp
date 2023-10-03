#ifndef TIC_TAC_TOE_HPP
#define TIC_TAC_TOE_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

#include "Board.hpp"
#include "Player.hpp"

class Game {
public:
    Game(Board board);
    void checkForWinner(std::string name);
    bool winLogic(); 
    void execute();

public:
    void serverSocket();
    void clientSocket();

private:
    Players* mPlayer;
    Board mBoard;
};

#endif
