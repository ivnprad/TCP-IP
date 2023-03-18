#ifndef IMAGESERVER_H
#define IMAGESERVER_H

#include <mutex>
#include <atomic>
#include "TCPSocketServer.hpp"


class ImageServer{

    public:
        ImageServer(const std::string& host,int port);
        void process();
        void handleConnection();
        void reportCommunication(const bool communicationOK);
        void startBehaviour();
        void stopBehaviour();


    private:
        TCPSocketServer itsTCPSocketServer;
        bool connectionEstablished;
        int hysteresis;
        const int hysteresisLimit;
        std::mutex mtx;
        bool newDataReceived;
        double dataReceived;
        double dataSent;
        std::atomic<bool> stop_flag;
        std::atomic<bool> sockOk;

};


#endif 