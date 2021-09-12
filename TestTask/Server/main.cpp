#include "Server.h"

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        std::cout << "Wrong parameter\n" << "Example usage ./Server dir port" << std::endl;
        return -1;
    }
    std::string dir = argv[1];
    ushort port = std::atoi(argv[2]);
    Server server(dir, port);
    server.StartServer();
    return 0;
}
