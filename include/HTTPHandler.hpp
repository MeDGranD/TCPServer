#include "IConnectionHandler.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <unistd.h>

enum METHOD{
    GET,
    POST,
    HEAD,
    OPTIONS,
    PUT,
    PATCH,
    DELETE,
    TRACE,
    CONNECT
};

struct HTTPreq{
    METHOD method;
    std::string URL;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

struct HTTPans{
    std::string version;
    int ansCode;
    std::string comment;
    std::map<std::string, std::string> headers;
    std::string body;
};

class HTTPHandler : public IConnectionHandler{
public:
    void handle(int) override;
    void setReqHandler(HTTPans(*)(HTTPreq&), std::string);
protected:
    HTTPreq _parse_http(char*, size_t);
    void _send_http_ans(HTTPans&);
    HTTPans _switch_method(HTTPreq&);
    std::map<std::string, HTTPans(*)(HTTPreq&)> handlers;
    int _socket;
};