#ifndef TIC_TAC_TOE_HPP
#define TIC_TAC_TOE_HPP

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>

class Players {
public:
    Players(char myElem);
    virtual std::string getPlayer() const = 0;
    virtual void setPlayer(std::string name) = 0;
protected:
    char mMyElem;
};

class Player1 : public Players {
public:
    Player1(char myElem, std::string playerName);
    std::string getPlayer() const override;
    void setPlayer(std::string name) override;

private:
    std::string mPlayer1;
};

class Player2 : public Players {
public:
    Player2(char myElem, std::string playerName);
    std::string getPlayer() const override;
    void setPlayer(std::string name) override;

private:
    std::string mPlayer2;
};

class Board {
public:
    Board();
    bool checkPosition(std::size_t i, std::size_t j);
    char getValueAtPosition(int row, int col);
    void setValueAtPosition(int row, int col, char elem);
    void draw();

private:
    //files part
    void writeToFile(std::string str);
    std::string readFromFile();

public: 
    std::string serialize();
    void deserialize(std::string boardStr);

private: 
    char board[3][3];
};


class Game {
public:
    Game(Board board);
    void checkForWinner(std::string name);
    bool winLogic(); 
    void execute();
public:
    void serverSocket();
    void clientSocket();
    static void receiveMessagesForClient(int serverSocket);
    static void receiveMessagesForServer(int serverSocket);
    Board getBoard() {
	    return mBoard;
    }
private:
    Players* mPlayer = nullptr;
    Board mBoard;
    int count = 0;
};

void receiveMessagesForClient(int serverSocket);
void receiveMessagesForServer(int serverSocket);



#endif
