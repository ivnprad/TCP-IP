#include "TCPClient.hpp"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include "TCPSocket.hpp"

TCPClient::TCPClient(){

}

TCPClient::~TCPClient(){

}

TCPSocket TCPClient::connect(const std::string host, int port){
    struct addrinfo hints, *results,*rp; // common practice insteda of only addrinfo
    int status;
    int sockfd; //-1

    //initialize hints firs to zero and assignt values
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC ; // for both IPv6 and IPv4  
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP; // not necessary since SOCK_STREAM alreday specifies protocol 

    //convert port to string
    std::string str_port = std::to_string(port);
    
    status = getaddrinfo(host.c_str(),str_port.c_str(),&hints,&results);

    //check for errors
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        //log this 
        //throw exceptions
    }

    // Iterate through the results and try to connect to the first available address
    // since getaddrinfo is a "linked list" and results returns the first 
    for (rp = results; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
        if (sockfd == -1) {
            continue;
        }
        if (::connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0) { // if connect succeeds break FOR-Loop
        // :: is to tell the compiler to use <sys/socket.h>
            break;
        }
        close(sockfd); 
    }

    if (rp == NULL) {
        std::cerr << "Failed to connect" << std::endl;
        //log this 
        //throw exceptions
    }

    return TCPSocket(sockfd);

}