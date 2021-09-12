#include <iostream>
#include "Client.h"
int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cout << "Wrong parameter\n" << "Example usage ./Client address port command dir filename" << std::endl;
        return -1;
    }
    char* address = argv[1];
    ushort port = std::atoi(argv[2]);
    char* command = argv[3];
    char* dir = argv[4];
    char* file_name = argv[5];
    Client client(address, port, command, dir, file_name);
    client.ConnectToServer();
}
