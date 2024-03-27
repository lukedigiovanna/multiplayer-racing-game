#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include <thread>

#include <iostream>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd; 
    char buffer[MAXLINE];
    sockaddr_in serverAddress, clientAddress; 

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw std::runtime_error("Error: socket creation failed.");
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

        // interpret the sent message as an integer
        int t = atoi(buffer);

        // calculate the square
        int t2 = t * t;

        std::string msg = std::to_string(t2);

        // send square number to
        sendto(sockfd, msg.c_str(), msg.length(), 0, (const struct sockaddr *) &clientAddress, len);
        
        std::cout << "received " << t << " and sent " << t2 << ".\n";
    }

    return 0;
}