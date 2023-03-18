#ifndef TCPCLIENT_H
#define TCPCLIENT_H


#include "TCPClientIfc.hpp"


class TCPSocket;

class TCPClient: public TCPClientIfc{
    public:
        TCPClient();
        virtual ~TCPClient() override;
        virtual TCPSocket connect(const std::string host, int port) override;
};


#endif 