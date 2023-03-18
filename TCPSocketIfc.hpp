#ifndef TCPSOCKETIFC_H
#define TCPSOCKETIFC_H

#include <stdint.h>

class TCPSocketIfc{
    public:
        TCPSocketIfc();
        virtual ~TCPSocketIfc()=0;
        virtual void close()=0;
        virtual int32_t receive(char* buffer, int length)=0;
        virtual int32_t send(const char* buffer,int length)=0;

};


#endif