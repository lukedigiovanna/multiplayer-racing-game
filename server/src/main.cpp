#include <stdlib.h> 
// #include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
// #include <arpa/inet.h> 
#include <netinet/in.h> 
#include <cstring>

#include <thread>
#include <atomic>

#include <iostream>

#include "Connection.h"
#include "Player.h"

#define PORT 8080
#define MAXLINE 1024

#define NUM_OPERATIONS 3
#define OPCODE_JOIN 0
#define OPCODE_LEAVE 1
#define OPCODE_KEY 2

#define SUCCESS 0
#define ERROR_BLANK 1
#define ERROR_UNDEFINED_OP 2

std::atomic<bool> serverShutdown = false;

const char* operations[NUM_OPERATIONS] = {
    "JOIN",
    "LEAVE",
    "KEY"
};

inline void build_error_message(char* msgBuffer, size_t& t, int errorCode) {
    msgBuffer[0] = errorCode;
    msgBuffer[1] = '\0';
    t = 2;
}

std::vector<std::pair<Connection, Player>> connections;

void accept_requests(int sockfd) {
    static char buffer[MAXLINE];
    static char msg[MAXLINE];
    size_t l;
    while (!serverShutdown) {
        sockaddr_in* clientAddress = (sockaddr_in*) malloc(sizeof(sockaddr_in));
        memset(clientAddress, 0, sizeof(sockaddr_in));
        socklen_t len = sizeof(clientAddress);
        int n = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, (sockaddr *) clientAddress, &len);
        // process request
        if (n == 0) {
            build_error_message(msg, l, ERROR_BLANK);
            std::cerr << "ERROR: Blank message\n";
        }
        else { // parse the request
            // The first bit will tell which operation the request is doing
            char operation = buffer[0];
            if (operation >= NUM_OPERATIONS) {
                build_error_message(msg, l, ERROR_UNDEFINED_OP);
                std::cerr << "ERROR: Undefined operation\n";
            }
            else {
                if (operation == OPCODE_JOIN) {
                    connections.push_back({Connection(clientAddress),Player()});
                    std::cout << "A client has connected -- " << connections.size() << " connections active\n";
                    connections[connections.size() - 1].first.printInfo();
                }
                else if (operation == OPCODE_LEAVE) {
                    // find matching connection
                    in_addr_t addr = clientAddress->sin_addr.s_addr;
                    in_port_t port = clientAddress->sin_port;
                    // find a matching connection
                    for (size_t i = 0; i < connections.size(); i++) {
                        auto& connection = connections[i];
                        if (connection.first.matches(addr, port)) {
                            // Remove this connection
                            connections.erase(connections.begin() + i);
                            break;
                        }
                    }
                }
                else if (operation == OPCODE_KEY) {

                }
                msg[0] = SUCCESS; // join success
                msg[1] = '\0';
                l = 2;
            }
        }
        sendto(sockfd, msg, l, 0, (const sockaddr*) clientAddress, len);
    }
}

void update_clients(int sockfd) {
    while (!serverShutdown) {
        for (auto &connection : connections) {
            void* info = connection.second.serialize();
            connection.first.sendMessage(sockfd, info, PLAYER_SERIALIZATION_SIZE);
            free(info);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    int sockfd; 
    char buffer[MAXLINE];
    sockaddr_in serverAddress; 

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw std::runtime_error("Error: socket creation failed.");
    }

    memset(&serverAddress, 0, sizeof(serverAddress));

    // Set server information
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT); 
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with this server address
    if (bind(sockfd, (const sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
        throw std::runtime_error("Error: failed to bind socket");
    }

    std::thread acceptRequestsThread(accept_requests, sockfd);
    std::thread updateClientsThread(update_clients, sockfd);

    acceptRequestsThread.join();
    updateClientsThread.join();

    return 0;
}