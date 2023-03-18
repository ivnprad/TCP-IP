#ifndef TCPSOCKET_H
#define TCPSOCKET_H


#include "TCPSocketIfc.hpp"

class TCPSocket{
    
    public:
        TCPSocket(int fd);
        TCPSocket();
        TCPSocket(const TCPSocket& other);
        virtual ~TCPSocket();
        virtual void close();
        TCPSocket& operator=(const TCPSocket& rhs);
        virtual int32_t receive(char* buffer, int length);
        virtual int32_t send(const char* buffer,int length);

    private:
        void checkValidSockfd();
        mutable int sockfd;

};


#endif 