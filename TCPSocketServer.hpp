#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#include "TCPSocketServerIfc.hpp"
#include <iostream>

class TCPSocket;

class TCPSocketServer: public TCPSocketServerIfc{
    public:
        TCPSocketServer(const std::string& host,int port);
        virtual ~TCPSocketServer();
        virtual TCPSocket acceptConnection();
        void close();
    private:
        TCPSocketServer();
        int server_socket;

};


#endif 