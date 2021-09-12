//
// Created by dxxmwrld on 11.09.2021.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

class Client {
public:
    Client(char* address, ushort port, std::string command, std::string dir, std::string file_name);
    ~Client() = default;
    void ConnectToServer();
private:
    int socket_;
    std::string dir_;
    std::string file_name_;
    std::string command_;
    struct sockaddr_in server_address_;
};


#endif //CLIENT_CLIENT_H
