#include "TCPSocketServer.hpp"
#include "TCPSocket.hpp"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

TCPSocketServer::TCPSocketServer():server_socket(-1){

}

TCPSocketServer::~TCPSocketServer(){
    TCPSocketServer::close();
}

TCPSocketServer::TCPSocketServer(const std::string& host,int port): server_socket(-1){
    std::cout << " creating tcp socket server "<< std::endl;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cout << "Error creating socket: " << strerror(errno) << std::endl;
    }

    // enable SO_REUSEADDR option

    /*
    In the case of a server socket, it is common to set the SO_REUSEADDR option using setsockopt(). 
    This option allows the server to quickly restart after a previous instance has terminated without waiting for the operating system to release the socket.
    */

    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {

        TCPSocketServer::close();

        // handle error
        std::cout << "Error on set sockopt: " << strerror(errno) << std::endl;
    }



    // Bind the socket to a local address and port
    struct sockaddr_in server_address;
    memset(&server_address, 0x0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    //server_address.sin_addr.s_addr = INADDR_ANY; // what host ????
    //server_address.sin_addr.s_addr = inet_addr("192.168.0.45"); //???
    server_address.sin_addr.s_addr = inet_addr(host.c_str()); //???
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        TCPSocketServer::close();
        std::cout << "LOG Error binding socket: throw exception " << strerror(errno) << std::endl;
        //return 1;
    }

    int backlog = 5;//function specifies the maximum number of pending connections that can be queued for the server socket
    if(listen(server_socket,backlog)<0){
        std::cout << " warning log listen " << std::endl;
    }

    std::cout << "Server listening on port " << ntohs(server_address.sin_port) << std::endl;

    std::cout << " server_socket "<< server_socket << std::endl;
    //int client_socket = accept(server_socket, NULL, NULL);
    //acceptConnection();



}

TCPSocket TCPSocketServer::acceptConnection(){
//void acceptConnection(int sockfd, struct sockaddr* clientaddr, socklen_t* addrlen) {
    struct sockaddr* clientaddr;
    socklen_t* addrlen;
    //int client_socket = accept(server_socket, clientaddr, addrlen);
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket < 0) {
        //perror("accept() failed");
        // handle error
        std::cout << " CLIENT SOCKET fails " <<  strerror(errno) << std::endl;
    }
    return TCPSocket(client_socket);
//}

}


void TCPSocketServer::close(){
    if (::close(server_socket) < 0) {
        //perror("close() failed");
        // handle error
        std::cout << "LOG Error closing socket throw an error: " << strerror(errno) << std::endl;
    }    
}