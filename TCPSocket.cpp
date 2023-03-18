#include "TCPSocket.hpp"
//#include <cstring>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <iostream>

TCPSocket::TCPSocket(int fd):sockfd(fd){

    checkValidSockfd();

}

TCPSocket::TCPSocket(): sockfd(-1){

}

TCPSocket::TCPSocket(const TCPSocket& other):sockfd(other.sockfd){
    other.sockfd=-1; // this is why we need sockfd to be mutable
}

TCPSocket::~TCPSocket(){
    //if( !(sockfd<0)){
        try{
            TCPSocket::close();
        }catch(...){

        }
    //}
}

void TCPSocket::close(){
    if(!(sockfd<0)){
        if( ::close(sockfd) == -1){//Error 
           //write error log  
        }
        sockfd=-1; // no need but anyway
    }
}

TCPSocket& TCPSocket::operator=(const TCPSocket& rhs){
    if (this!=&rhs){
        TCPSocket::close(); // close before changing
        sockfd = rhs.sockfd;
        rhs.sockfd=-1;
    }

    return *this;
}

int32_t TCPSocket::receive(char* buffer, int length){

/*  
    The purpose of the loop in the original code is to ensure that all of the requested data is received by calling recv() repeatedly until either all of the data has been received or an error occurs.
    The recv() function can return fewer bytes than requested, even if the connection is still open, so it's necessary to call recv() repeatedly until all of the requested data has been received. The loop in the original code accomplishes this by repeatedly calling recv() and checking how much data was actually received in each call.
*/
    checkValidSockfd();

    int totalBytesReceived = 0; // keep track of total bytes received so far
    int bytesReceived = 1; // number of bytes received in each call to recv()

    while (bytesReceived>0 && totalBytesReceived < length) {

        // calculate remaining bytes to receive
        int remainingBytes = length - totalBytesReceived; 

        // since we do not if length == sizeof(buffer)
        int recvSize = (remainingBytes > sizeof(buffer)) ? sizeof(buffer) : remainingBytes;

        //update recvBuffer 
        char* recvBuffer = buffer + totalBytesReceived;

        // receive data into buffer, up to sizeof(buffer) bytes
        bytesReceived = ::recv(sockfd, recvBuffer, recvSize, 0x00);

        // check for errors
        if (bytesReceived < 0) {
            //throw std::runtime_error("recv() failed");
            //log it 
        }

        // update total bytes received
        totalBytesReceived += bytesReceived;
    }

    return totalBytesReceived;

}


int32_t TCPSocket::send(const char* buffer, int length){
    checkValidSockfd();
    int totalBytesSENT = 0; // keep track of total bytes SENT so far
    int bytesSENT = 1; // number of bytes SENT in each call to recv()

    while( bytesSENT>0 && totalBytesSENT<length){

        //update sendBuffer 
        const char* sendBuffer = buffer + totalBytesSENT;

        // calculate remaining bytes to receive
        int remainingBytes = length - totalBytesSENT; 

        // since we do not if length == sizeof(buffer)
        int sendSize = (remainingBytes > sizeof(buffer)) ? sizeof(buffer) : remainingBytes;

        bytesSENT = ::send(sockfd,sendBuffer,sendSize,0x00);
        // check for errors
        if (bytesSENT < 0) {
            //throw std::runtime_error("recv() failed");
            //log it 
        }

        // update total bytes received
        totalBytesSENT += bytesSENT;
    }
    return totalBytesSENT;
}

void TCPSocket::checkValidSockfd(){
    if(sockfd<0){
        //log
        // throw exception
        std::cout << " INVALID SOCKET "<< std::endl;
    }
}
