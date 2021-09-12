//
// Created by dxxmwrld on 11.09.2021.
//


#include "Server.h"

Server::Server(std::string dir, ushort port) : dir_(dir)
{
    //checking the correctness of the path format
    if (dir_[dir_.length() - 1] != '/')
        dir_ += "/";
    //checking for the presence of a root directory
    if (!Search("","").second)
    {
        exit(1);
    }
    //creating socket
    if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error while creating socket\n");
        exit(1);
    }
    //binding socket
    int opt(1);
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("Error while setsockopt function");
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = port;
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(socket_, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
    {
        perror("Error while binding socket\n");
        exit(1);
    }
}

void Server::StartServer()
{
    //start listening
    if (listen(socket_, 1) < 0)
    {
        perror("Error while listening socket\n");
        exit(1);
    }
    char s_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server_address.sin_addr), s_addr, INET_ADDRSTRLEN);
    //Server info
    printf("%s%s%s%s%d%s%s%s", "Server info:\n", "Address: ", s_addr, "\nPort: ", server_address.sin_port, "\nRoot-directory: ", dir_.c_str(), "\n");
    while (true)
        Accept();
}

void Server::Accept()
{
    struct sockaddr_in client_address;
    int client_address_length = sizeof(client_address);
    int client_socket;
    //accept client socket
    if ((client_socket = accept(socket_, (struct sockaddr*) &client_address, (socklen_t*)&client_address_length)) < 0)
    {
        perror("Error while accepting socket\n");
        exit(1);
    }
    char buffer[1024] = "";
    char c_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_address.sin_addr), c_addr, INET_ADDRSTRLEN);
    //read command from client
    if (read(client_socket, buffer, 1024) < 0)
    {
        perror("Error while reading socket\n");
        exit(1);
    }
    //spliting data from buffer
    std::string command = strtok(buffer, " ");
    std::string dir = strtok(NULL, " ");
    //checking the correctness of the path format
    if (dir[dir.length() - 1] != '/')
        dir += "/";
    std::string file_name = strtok(NULL, " ");
    //log
    printf("%s %s %s %s %d\n", "Command:", ('"' + command + " " + dir  + file_name + '"').c_str(), "from", c_addr, client_address.sin_port);
    {
        std::string temp("");
        for (char c: command) {
            temp += tolower(c);
        }
        command = temp;
    }
    //handle command
    Handle(command, dir, file_name);
}

void Server::Handle(const std::string &command, const std::string &dir, const std::string &file_name)
{
    if (command == "delete")
        Delete(dir, file_name);
    else if (command == "create")
        Create(dir, file_name);
    else
    {
        printf("Wrong command name\n");
    }
}

void Server::Create(const std::string &dir, const std::string &file_name)
{
    //checking for the existence of a directory and a file
    std::pair<bool, bool> exists = Search(dir, file_name);
    if (!exists.first && exists.second)
    {
        //creating file
        std::ofstream file(dir_ + dir + file_name);
        if(file.is_open())
        {
            printf("File created\n");
        }
        else
        {
            printf("Error while file creating\n");
        }
        file.close();
    }
    else if (exists.second)
    {
        printf("Such already exists\n");
    }
}

void Server::Delete(const std::string &dir, const std::string &file_name)
{
    //checking for the existence of a directory and a file
    std::pair<bool, bool> exists = Search(dir, file_name);
    if (exists.first)
    {
        //delete file
        unlink((dir_ + dir + file_name).c_str());
        printf("File was deleted\n");
    }
    else if (exists.second)
    {
        printf("Such file does not exist\n");
    }
}

std::pair<bool, bool> Server::Search(const std::string &dir, const std::string &file_name)
{
    //trying to open dir
    DIR *directory = opendir((dir_ + dir).c_str());
    if (directory)
    {
        struct dirent *ent;
        //looking for file named @file_name
        while((ent = readdir(directory)) != NULL)
        {
            if (ent->d_name == file_name)
                return {true, true};
        }
    }
    else
    {
        perror("Error while opening directory\n");
        return {false, false};
    }
    return {false, true};
}