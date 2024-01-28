#include "../include/TCPServerBase.hpp"

class Handler: public IConnectionHandler{
    void handle(int sock) override{

        if(send(sock, "Hello, world!", sizeof("Hello, world!"), 0) < 0)
            throw std::runtime_error("Send error");

        close(sock);

    }
};

int main(){

    TCPServerBase* serv = &TCPServerBase::instance("127.0.0.1:5555");
    serv->setHandler(std::make_shared<Handler>());
    serv->start();

    return 0;

}