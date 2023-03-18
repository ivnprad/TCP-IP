#ifndef IMAGECLIENT_H
#define IMAGECLIENT_H

//#include <string>
#include "TCPClient.hpp"
#include "TCPSocket.hpp"
#include <mutex>
#include <algorithm>
#include <atomic>

class ImageClient{
public:
    ImageClient(const std::string& host, int port);
    void process();    
    void connect();
    void reportCommunication(const bool communicationOK);
    void startBehaviour();
    void stopBehaviour();

private:

    const int communicationDelay_ms;
    bool connectionEstablished;
    std::string host;
    int hysteresis;
    const int hysteresis_limit;
    bool newDataReceived;
    int port;
    TCPClient itsTCPclient;
    TCPSocket itsTCPSocket;
    double dataReceived;
    double dataSent;
    std::mutex mtx;
    std::atomic<bool> stop_flag;

};




#endif