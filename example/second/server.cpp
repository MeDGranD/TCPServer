#include "../../include/TCPServerBase.hpp"
#include "../../include/HTTPHandler.hpp"
#include <fstream>

HTTPans IndexHandler(HTTPreq& request){

    HTTPans answer;
    answer.ansCode = 200;
    answer.comment = "OK";
    answer.version = "HTTP/1.1";
    answer.headers["Content-Type"] = "text/html";

    std::fstream file("index.html");
    answer.body = "";
    char ch;
    while(file >> ch)
        answer.body += ch;
    
    file.close();

    return answer;

}

int main(){

    TCPServerBase* serv = &TCPServerBase::instance("127.0.0.1:5555");
    std::shared_ptr<HTTPHandler> handler = std::make_shared<HTTPHandler>();
    handler->setReqHandler(IndexHandler, "/");
    serv->setHandler(handler);
    serv->start();

    return 0;
}