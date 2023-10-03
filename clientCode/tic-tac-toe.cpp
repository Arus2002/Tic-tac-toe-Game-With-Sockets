#include "tic-tac-toe.hpp"

Game::Game(Board board)
    : mBoard{board} {}

void Game::checkForWinner(std::string name) {
    if (mPlayer -> getPlayer() == "player1") {
        std::cout << "Win Player1!" << std::endl;
	return;
    }
    else {
        std::cout << "Win Player2!" << std::endl;
	return;
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
		std::string winMessage = "PLayer2 win!";
		std::cout << winMessage << std::endl;
		strcpy(buffer, winMessage.c_str());
		rcv = write(clientSocket, buffer, strlen(buffer));
		if (rcv < 0) {
			std::cerr << "Error while writing win from player2!" << std::endl;
		}
		close(clientSocket);
		return;
	}

	rcv = read(clientSocket, buffer, strlen(buffer));
	if (rcv < 0) {
                std::cerr << "Error while reading" << std::endl;
        }

	std::string mess(buffer);
	if (mess == "PLayer1 win!") {
		std::cout << "Player1 win!" << std::endl;
		close(clientSocket);
		return;
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
    while(true) {
    	int rcvMess = read(clientSocket, buffer, 255);
    	if (rcvMess < 0) {
	    	close(serverSocket);
		return;
    	}

	std::string winMessage(buffer);
	if (winMessage == "PLayer2 win!") {
		std::cout << winMessage << std::endl;
		close(clientSocket);
		close(serverSocket);
		return;
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
		std::string winMessage = "PLayer1 win!";
		std::cout << winMessage << std::endl;
		strcpy(buffer, winMessage.c_str());
		rcvMess = write(clientSocket, buffer, strlen(buffer));
		if (rcvMess < 0) {
			close(serverSocket);
			return;
		}
		close(serverSocket);
		return;
	}

	mBoard.draw();
    }

    // Close sockets and join the receive thread
    close(clientSocket);  // Close clientSocket
    close(serverSocket);  // Close serverSocket after the game is over
}
