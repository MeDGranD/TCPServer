#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

int main(){

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
    inet_aton("127.0.0.1", &(addr.sin_addr));

    connect(sock, (sockaddr*)&addr, sizeof(addr));
    char str[sizeof("Hello, world!")];
    recv(sock, str, sizeof("Hello, world!"), 0);

    for(size_t i = 0; i < sizeof("Hello, world!"); ++i){
        std::cout << str[i];
    }
    std::cout << std::endl;

    close(sock);

    return 0;
}