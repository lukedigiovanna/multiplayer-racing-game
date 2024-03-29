#pragma once

#include <netinet/in.h>  
#include <memory>

class Connection {
    std::unique_ptr<sockaddr_in> clientAddress;
public:
    Connection(sockaddr_in* clientAddress);

    void sendMessage(int sockfd, const void* message, size_t messageLength);
    void printInfo() const;

    bool matches(in_addr_t addr, in_port_t port) const;
};