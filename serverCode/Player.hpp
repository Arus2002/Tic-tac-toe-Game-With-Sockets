#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <string>

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

#endif
