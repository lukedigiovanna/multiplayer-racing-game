#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <cstring>

#include <thread>

#include <iostream>

#define PORT 8080
#define MAXLINE 1024

#define NUM_OPERATIONS 1

const char* operations[NUM_OPERATIONS] = {
    "JOIN"
};

int main() {
    int sockfd; 
    char buffer[MAXLINE];
    sockaddr_in serverAddress, clientAddress; 

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "Error: socket creation failed. Error code: " << errno
              << " - " << strerror(errno) << std::endl;
        // throw std::runtime_error("Error: socket creation failed.");
        return -1;
    }

    // Clear the structs
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    // Set server information
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT); 
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with this server address
    if (bind(sockfd, (const sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
        throw std::runtime_error("Error: failed to bind socket");
    }

    socklen_t len;
    int n;

    len = sizeof(clientAddress);

    char message[MAXLINE];

    while (true) {
        n = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, (sockaddr *) &clientAddress, &len);
        buffer[n] = '\0';

        std::string msg;

        if (n == 0) {
            msg = "ERROR: Receieved a blank request";
        }
        else { // parse the request
            // The first bit will tell which operation the request is doing
            char operation = buffer[0];
            if (operation >= NUM_OPERATIONS) {
                msg = "ERROR: Received undefined operation: " + std::to_string(operation);
            }
            else {
                msg = "Got JOIN operation!";
            }
        }
        

        // send square number to
        sendto(sockfd, msg.c_str(), msg.length(), 0, (const struct sockaddr *) &clientAddress, len);
        
        std::cout << "Sent " << msg << ".\n";
    }

    return 0;
}