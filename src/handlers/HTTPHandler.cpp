#include "../../include/HTTPHandler.hpp"

void HTTPHandler::setReqHandler(HTTPans(* func)(HTTPreq&), std::string url){
    this->handlers[url] = func;
}

HTTPans HTTPHandler::_switch_method(HTTPreq& request){

    if(this->handlers.count(request.URL) == 0){
        HTTPans answer;
        answer.ansCode = 404;
        answer.comment = "Not Found";
        answer.version = "HTTP/1.1";
        return answer;
    }

    return this->handlers[request.URL](request);

}

void HTTPHandler::handle(int sock){

    this->_socket = sock;

    std::string data = "";
    while(true){

        char part[BUFSIZ];
        size_t sz;
        if((sz = recv(sock, part, BUFSIZ, 0)) < 0)
            throw std::runtime_error("Cannot receive data");
        for(size_t i = 0; i < sz; ++i) data += part[i];

        if(sz < BUFSIZ) break;
    }

    char* buffer = const_cast<char*>(data.c_str());
    size_t bufsize = data.size();

    HTTPreq request = this->_parse_http(buffer, bufsize);

    HTTPans answer = this->_switch_method(request);

    this->_send_http_ans(answer);

}

HTTPreq HTTPHandler::_parse_http(char* buffer, size_t bufsize){

    HTTPreq returnReq;

    std::string getterstr = "";
    size_t i = 0;
    while(buffer[i] != ' '){

        getterstr += buffer[i];
        ++i;
        if(i >= bufsize)
            throw std::runtime_error("Bad request");
    }
    ++i;

    if(getterstr == "GET")
        returnReq.method = METHOD::GET;
    else if(getterstr == "POST")
        returnReq.method = METHOD::POST;
    else if(getterstr == "HEAD")
        returnReq.method = METHOD::HEAD;
    else if(getterstr == "OPTIONS")
        returnReq.method = METHOD::OPTIONS;
    else if(getterstr == "CONNECT")
        returnReq.method = METHOD::CONNECT;
    else if(getterstr == "DELETE")
        returnReq.method = METHOD::DELETE;
    else if(getterstr == "PATCH")
        returnReq.method = METHOD::PATCH;
    else if(getterstr == "PUT")
        returnReq.method = METHOD::PUT;
    else if(getterstr == "TRACE")
        returnReq.method = METHOD::TRACE;

    getterstr = "";
    while(buffer[i] != ' '){

        getterstr += buffer[i];
        ++i;
        if(i >= bufsize)
            throw std::runtime_error("Bad request");
    }
    ++i;

    returnReq.URL = getterstr;
    getterstr = "";

    while(buffer[i] != '\n'){

        getterstr += buffer[i];
        ++i;
        if(i >= bufsize)
            throw std::runtime_error("Bad request");
    }
    ++i;

    returnReq.version = getterstr;
    getterstr = "";

    while(buffer[i] != '\n' && buffer[i] != '\0' && buffer[i] != (char)13){

        while(buffer[i] != ':'){
            getterstr += buffer[i];
            ++i;
            if(i >= bufsize)
                throw std::runtime_error("Bad request");
        }
        ++i;

        std::transform(getterstr.begin(), getterstr.end(), getterstr.begin(),
                        [](unsigned char c){ return std::tolower(c); });
        
        if(returnReq.headers.count(getterstr) == 0)
            returnReq.headers[getterstr] = "";
        else returnReq.headers[getterstr] += ',';

        while(buffer[i] == ' ' || buffer[i] == '\t'){
            ++i;
            if(i >= bufsize)
                throw std::runtime_error("Bad request");
            continue;
        }

        bool space = false;
        while(buffer[i] != '\n'){

            if(buffer[i] == ' ' || buffer[i] == '\t'){
                if(!space) space = true;
                else{
                    ++i;
                    if(i >= bufsize)
                        throw std::runtime_error("Bad request");
                    continue;
                }
            }

            space = false;

            returnReq.headers[getterstr] += buffer[i];
            if(buffer[i] == ';' && buffer[i + 1] == '\n') ++i;

            ++i;
            if(i >= bufsize)
                throw std::runtime_error("Bad request");

        }

        ++i;
        getterstr = "";
    }
    ++i;

    returnReq.body = "";
    while(i < bufsize){
        returnReq.body += buffer[i];
        ++i;
    }

    return returnReq;

}

void HTTPHandler::_send_http_ans(HTTPans& answer){

    std::string buffer = answer.version + " " + 
                         std::to_string(answer.ansCode) + " " + 
                         answer.comment + "\n";

    for(std::pair<std::string, std::string> pr : answer.headers)
        buffer += pr.first + ":" + " " + pr.second + "\n";

    buffer += "\n";
    buffer += answer.body;

    int n = 0;
    size_t bufsize = buffer.size();
    while(n != bufsize){
        int sz;
        if((sz = send(this->_socket, buffer.c_str() + n, bufsize - n, 0)) < 0)
            throw std::runtime_error("Cannot send data");
        n += sz;
    }

    shutdown(this->_socket, SHUT_RDWR);
    close(this->_socket);

}