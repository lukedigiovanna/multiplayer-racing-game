#include "Connection.h"

#include <iostream>
#include <string>
#include <sstream>

Connection::Connection(sockaddr_in* clientAddress)  {
    this->clientAddress = std::unique_ptr<sockaddr_in>(clientAddress);
}

std::string formatIPAddress(int addr) {
    std::stringstream ss;
    ss << (addr & 0xFF) << "." << (addr >> 8 & 0xFF) << "." << (addr >> 16 & 0xFF) << "." << (addr >> 24 & 0xFF);
    return ss.str();
}

void Connection::printInfo() const {
    std::cout << "==SOCKET CLIENT CONNECTION INFO==\n";
    std::cout << "addr: " << formatIPAddress(clientAddress->sin_addr.s_addr) << "\n";
    std::cout << "port: " << clientAddress->sin_port << "\n";
    std::cout << "family: " << +clientAddress->sin_family << "\n";
    // std::cout << "Address: " << clientAddress.sin_addr.s_addr << "\n";
}

void Connection::sendMessage(int sockfd, const void* message, size_t messageLength) {
    sendto(sockfd, message, messageLength, 0, (const sockaddr*) this->clientAddress.get(), sizeof(*this->clientAddress));
}

bool Connection::matches(in_addr_t addr, in_port_t port) const {
    return addr == clientAddress->sin_addr.s_addr && port == clientAddress->sin_port;
}