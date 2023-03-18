#ifndef TCPSOCKETSERVERIFC_H
#define TCPSOCKETSERVERIFC_H

class TCPSocket;

class TCPSocketServerIfc{
public:
    TCPSocketServerIfc();
    virtual ~TCPSocketServerIfc()=0;
    virtual TCPSocket acceptConnection()=0;
};


#endif