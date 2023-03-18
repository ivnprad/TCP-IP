#ifndef TCPCLIENTIFC_H
#define TCPCLIENTIFC_H

#include <string>
#include <iostream>

class TCPSocket;

class TCPClientIfc{
    public:
        TCPClientIfc();
        virtual ~TCPClientIfc()=0;
        virtual TCPSocket connect(const std::string host, int port)=0;

};


#endif 