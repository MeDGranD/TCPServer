#include "../include/TCPServerBase.hpp"

void consoleThreadFunc(TCPServerBase& server){

    std::string getterstr;
    while(std::cin >> getterstr){
        if(getterstr == "stop")
            server.stop();
        else if(getterstr == "state"){
            ServerState inf = server.state();
            std::cout << "[]\tIPv4 : " << inf.IPv4 << std::endl;
            std::cout << "[]\tPort : " << inf.port << std::endl;
            std::cout << "[]\tStatus : " << (inf.state == 1 ? "Working" : "Stopped") << std::endl << std::endl;
        }
        else if(getterstr == "log"){
            std::list<ConnectionState> ls = server.getConnections();
            for(ConnectionState state : ls){
                std::cout << "[]\tIPv4 : " << state.IPv4 << std::endl;
                std::cout << "[]\tConnection created at : " << std::asctime(std::localtime(&(state.tm)));
                std::cout << "[]\tHandled on sock : " << state.sock << std::endl << std::endl;
            }
        }
        else{
            std::cout << "[]\tUnknowed command" << std::endl << std::endl;
        }
    }

}

TCPServerBase::TCPServerBase() : _handler(nullptr), _port(5555), _state(stopped){

    char hostname[2048];
    if(gethostname(hostname, 2048) < 0)
        throw std::runtime_error("Cannot get host name");

    hostent* host = gethostbyname(hostname);
    if(host == NULL)
        throw std::runtime_error("Cannot get host by host name");

    if(host->h_addr_list[0] == 0)
        throw std::runtime_error("Cannot get inet interface");

    this->_IPv4 = ((in_addr*)host->h_addr_list[0])->s_addr;

    this->init_socket();

}

TCPServerBase::TCPServerBase(const std::string& address) : _handler(nullptr), _state(stopped){

    std::string IP = address.substr(0, address.find(':'));
    std::string port = address.substr(address.find(':') + 1, address.size() - address.find(':') - 1);
    this->_port = std::stoi(port);

    in_addr addr;
    if(inet_aton(IP.c_str(), &addr) == 0)
        throw std::runtime_error("Cannot convert address");

    this->_IPv4 = addr.s_addr;

    this->init_socket();
    
}

void TCPServerBase::init_socket(){

    this->_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->_port);
    addr.sin_addr.s_addr = this->_IPv4;

    if(bind(this->_socket, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Cannot bind socket");

}

void TCPServerBase::start(){

    this->_state = working;

    if(listen(this->_socket, SOMAXCONN) < 0)
        throw std::runtime_error("Cannot set socket to listen");

    std::thread thr(consoleThreadFunc, std::ref(*this));
    thr.detach();

    while(true){

        sockaddr addr;
        socklen_t len = sizeof(addr);
        int accSock = accept(this->_socket, &addr, &len);
        
        if(this->_state == stopped) break;

        if(accSock < 0)
            throw std::runtime_error("Cannot accept connection");

        ConnectionState conState;
        conState.IPv4 = ((sockaddr_in*)(&addr))->sin_addr.s_addr;
        conState.sock = accSock;
        conState.tm = std::time(nullptr);

        std::thread newThr([](
                            std::shared_ptr<IConnectionHandler> hand, int sock
                            ){
                                hand->handle(sock); 
                            },
                            this->_handler, accSock);
        newThr.detach();

        const std::lock_guard<std::mutex> lock(this->ConncetionMutex);
        this->_connections.push_back(conState);

    }

}

void TCPServerBase::stop(){

    this->_state = stopped;

    if(shutdown(this->_socket, SHUT_RDWR) < 0)
        throw std::runtime_error("Cannot to shutdow server socket");

    if(close(this->_socket) < 0)
        throw std::runtime_error("Cannot to close server socket");

}


void TCPServerBase::setHandler(std::shared_ptr<IConnectionHandler> handler){

    this->_handler = handler;

}

TCPServerBase::~TCPServerBase(){

    if(this->_state == working){
        this->stop();
    }

    std::cout << "Server is shutting down" << std::endl;

}

ServerState TCPServerBase::state() const{
    ServerState tempState;

    tempState.IPv4 = this->_IPv4;
    tempState.port = this->_port;
    tempState.state = this->_state;

    return tempState;
}

TCPServerBase& TCPServerBase::instance(){

    static TCPServerBase signleInstance;
    return signleInstance;

}

TCPServerBase& TCPServerBase::instance(const std::string& address){

    static TCPServerBase signleInstance(address);
    return signleInstance;

}

void TCPServerBase::notify(unsigned long IPv4){
    
    std::lock_guard<std::mutex> lock(this->ConncetionMutex);
    auto it = this->_connections.begin();
    while(it != this->_connections.end()){
        if((*it).IPv4 == IPv4) this->_connections.erase(it);
    }

}

std::list<ConnectionState> TCPServerBase::getConnections() const{
    return (this->_connections);
}