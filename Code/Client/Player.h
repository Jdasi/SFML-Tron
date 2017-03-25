#pragma once

class Player
{
public:
    enum State
    {
        NOTREADY,
        READY,
        ALIVE,
        DEAD
    };

    Player();
    ~Player() = default;

    int getID() const;
    void setID(int _id);

    State getState() const;
    void setState(State _state);

    bool isClient() const;
    void setClient(bool _value);

private:
    int id;
    State state;
    bool is_client;

};
