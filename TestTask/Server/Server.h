//
// Created by dxxmwrld on 11.09.2021.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <algorithm>
#include <dirent.h>
#include <fstream>

class Server {
public:
    Server(std::string dir, ushort port);
    ~Server() = default;
    void StartServer();
    void Accept();
    void Handle(const std::string& command, const std::string& dir, const std::string& file_name);
    void Create(const std::string& dir, const std::string& file_name);
    void Delete(const std::string& dir, const std::string& file_name);
    std::pair<bool, bool> Search(const std::string &dir, const std::string &file_name);
private:
    int socket_;
    struct sockaddr_in server_address;
    std::string dir_;
};


#endif //SERVER_SERVER_H
