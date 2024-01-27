#pragma once
#include "IConnectionHandler.hpp"
#include <memory>
#include <mutex>
#include <list>
#include <unistd.h>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

enum State{
    stopped = -1,
    working = 1
};

struct ServerState{
    std::string IPv4;
    int port;
    State state;
};

class ITCPServer{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~ITCPServer() = default;
protected:
    virtual void init_socket() = 0;
};

class TCPServerBase : public ITCPServer{
public:
    void start() override;
    void stop() override;
    void setHandler(std::shared_ptr<IConnectionHandler>);

    ServerState state() const;
    std::list<ConnectionState> getConnections() const;
    void notify(unsigned long);

    static TCPServerBase& instance();
    static TCPServerBase& instance(const std::string&);

    ~TCPServerBase() override;

protected:
    TCPServerBase();
    TCPServerBase(const std::string&);
    TCPServerBase& operator=(const TCPServerBase&) = delete;
    TCPServerBase(const TCPServerBase&) = delete;

    void init_socket() override;

    std::shared_ptr<IConnectionHandler> _handler;

    int _socket;
    int _port;
    unsigned long _IPv4;
    State _state;

    std::mutex ConncetionMutex;
    std::list<ConnectionState> _connections;

};

void consoleThreadFunc(TCPServerBase&);