#include "tic-tac-toe.hpp"

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>

#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <limits>

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

Game::Game(Board board)
    : mBoard{board} {}

void Game::checkForWinner(std::string name) {
    if (mPlayer -> getPlayer() == "player1") {
        std::cout << "Win Player1!" << std::endl;
    }
    else {
        std::cout << "Win Player2!" << std::endl;
    }
}

bool Game::winLogic() {
    for (int i = 0; i < 3; ++i) {
        if (mBoard.getValueAtPosition(i, 0) == mBoard.getValueAtPosition(i, 1) && 
            mBoard.getValueAtPosition(i, 1) == mBoard.getValueAtPosition(i, 2) && mBoard.getValueAtPosition(i, 0) != ' ') {

            checkForWinner(mPlayer -> getPlayer());
            return true;
        }
    }
    for (int j = 0; j < 3; ++j) {
        if (mBoard.getValueAtPosition(0, j) == mBoard.getValueAtPosition(1, j) && 
            mBoard.getValueAtPosition(1, j) == mBoard.getValueAtPosition(2, j) && mBoard.getValueAtPosition(0, j) != ' ') {

            checkForWinner(mPlayer -> getPlayer());
            return true;
        }
    }

    if (mBoard.getValueAtPosition(0, 0) == mBoard.getValueAtPosition(1, 1) &&
        mBoard.getValueAtPosition(1, 1) == mBoard.getValueAtPosition(2, 2) && mBoard.getValueAtPosition(0, 0) != ' ') {

        checkForWinner(mPlayer -> getPlayer());
        return true;
    }

    if (mBoard.getValueAtPosition(0, 2) == mBoard.getValueAtPosition(1, 1) &&
        mBoard.getValueAtPosition(1, 1) == mBoard.getValueAtPosition(2, 0) && mBoard.getValueAtPosition(0, 2) != ' ') {

        checkForWinner(mPlayer -> getPlayer());
        return true;
    }
    
    return false;
}

//veradardznum enq boardy stringi tesqov
std::string Board::serialize() {
    std::string res;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            res += getValueAtPosition(i, j);
        }
    }
    return res;
}


//havaqum enq stringic board
void Board::deserialize(std::string boardStr) {
    std::string res = boardStr;
    std::size_t resIndex = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (resIndex < res.size()) {
                setValueAtPosition(i, j, res[resIndex]);
                ++resIndex;
            }
        }
    }
}

void Game::clientSocket() {
    // Create a socket for the client (Player 2)
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // Connect to the server (Player 1)
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Use the same port as the server
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.53");  // Connect to localhost (adjust the IP address as needed)
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to (server)player 1" << std::endl;
        close(clientSocket);
        return;
    }

    // Communication with Player 2
    char buffer[256];
    int rcv;
    mBoard.draw();
    while(true) {
	//clienty katarum e ir qayly
	execute();

	//poxenq boardy vory kgrvi server-i hamar
	std::string message = mBoard.serialize();
        mBoard.deserialize(message);

	//copy anenq bufferi mej
	strcpy(buffer, message.c_str());

	//nkarenq clienti qaylic heto boardy
	mBoard.draw();

    	rcv = write(clientSocket, buffer, strlen(buffer));
	if (rcv < 0) {
		std::cerr << "Error while writing" << std::endl;
	}

	if (winLogic()) {
		break;
	}

	rcv = read(clientSocket, buffer, strlen(buffer));
	if (rcv < 0) {
                std::cerr << "Error while reading" << std::endl;
        }

	if (winLogic()) {
		break;
	}

	std::string receivedData(buffer);
        mBoard.deserialize(receivedData);
        mBoard.draw();
    }

    // Close sockets and join the receive thread
    close(clientSocket);
}

void Game::serverSocket() {
    // Create a socket for the server (Player 1)
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // Bind the socket to a specific IP address and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Use any available port
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
	close(serverSocket);
        return;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(serverSocket);
        return;
    }

    // Accept a connection from Player 2
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        std::cerr << "Error accepting connection from (client)Player 2" << std::endl;
        close(serverSocket);
        return;
    } 

    // Communication with Player 1
    // read from Player2 (client)
    char buffer[256];
    while(!winLogic()) {
    	int rcvMess = read(clientSocket, buffer, 255);
    	if (rcvMess < 0) {
	    	close(serverSocket);
	    	return;
    	}
	if (winLogic()) {
		break;
	}

	//board-y sarqenq yst serveri kardacatsi tesqi
        std::string tmpBuffer(buffer);
        mBoard.deserialize(tmpBuffer);
	
	//katarenq server-i qayly
        mBoard.draw();
        execute();
	
	//trvats stringin avelacnenq executeic heto stacvats board-i stringy vory servery write kani
	std::string message = mBoard.serialize();
	mBoard.deserialize(message); //stacanq verjin stacvats boardi patkery

    	rcvMess = write(clientSocket, message.c_str(), message.size());
	if (rcvMess < 0) {
                close(serverSocket);
                return;
        }
	if (winLogic()) {
		break;
	}
	
	mBoard.draw();
    }

    // Close sockets and join the receive thread
    close(clientSocket);  // Close clientSocket
    close(serverSocket);  // Close serverSocket after the game is over
}
