//
// Created by dxxmwrld on 11.09.2021.
//

#include "Client.h"

Client::Client(char* address, ushort port, std::string command, std::string dir, std::string file_name)
: command_(command), file_name_(file_name), dir_(dir)
{
    //creating socket
    if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error while creating socket");
    }
    server_address_.sin_family = AF_INET;
    server_address_.sin_port = port;
    inet_pton(AF_INET, address, &server_address_.sin_addr);
}

void Client::ConnectToServer()
{
    //trying to connect with server
    if (connect(socket_, (struct sockaddr*) &server_address_, sizeof(server_address_)) < 0)
    {
        perror("Error while connecting socket");
        exit(1);
    }
    char buffer[1024];
    strcpy(buffer, (command_ + " " + dir_ + " " + file_name_).c_str());
    //trying to send data to server
    if (send(socket_, buffer, strlen(buffer), 0) < 0)
    {
        perror("Error while sending socket");
        exit(1);
    }
}