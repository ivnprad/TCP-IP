#include <iostream>
// #include <string>
// #include <cstring>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <errno.h>
#include "ImageClient.hpp"
#include "ImageServer.hpp"
#include "TCPClient.hpp"
#include "TCPClientIfc.hpp"
#include <thread>
#include <chrono>


int main()
{
    const std::string serverIP="192.168.0.45";
    const int port = 8080;
    ImageServer itsImageServer(serverIP,port);
    ImageClient itsImageClient(serverIP,port);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Start server in a separate thread
    std::thread server_thread(&ImageServer::startBehaviour, &itsImageServer);

    // Start client in a separate thread
    std::thread client_thread(&ImageClient::startBehaviour, &itsImageClient);

    for(size_t i = 0; i<10;++i){
        itsImageServer.process();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        itsImageClient.process();
    }

    std::cout << " stop image server "<<std::endl;
    itsImageServer.stopBehaviour();
    // Wait for threads to finish
    if (server_thread.joinable()){
        server_thread.join();
    }

    std::cout << " stop image client "<< std::endl;
    itsImageClient.stopBehaviour();
    if (client_thread.joinable()){
        client_thread.join();
    }

    
}
