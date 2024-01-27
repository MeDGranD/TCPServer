#pragma once
#include <string>
#include <ctime>

struct ConnectionState{
    unsigned long IPv4;
    int sock;
    std::time_t tm;
};

class IConnectionHandler{
public:
    virtual void handle(int) = 0;
}; 