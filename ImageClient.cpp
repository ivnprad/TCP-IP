#include "ImageClient.hpp"
#include <thread>
#include <chrono>
#include <algorithm>

ImageClient::ImageClient(const std::string& host,int port):
    communicationDelay_ms(100),
    connectionEstablished(false),
    hysteresis(0),
    hysteresis_limit(10),
    newDataReceived(false),
    host(host),
    port(port){

    stop_flag=false;

}

void ImageClient::process(){
    std::lock_guard<std::mutex> lock(mtx); // automatically unlocked when going out of scope
    // update data to sent
    dataSent =45; // this convert to data received at imageserver
    //operation to update checkSUM
    std::cout << " CLIENT data SENT :"<< dataSent << std::endl;

    if(newDataReceived){
        //update dataReceived
        std::cout<< " CLIENT data received "<< dataReceived << std::endl;
        hysteresis = hysteresis_limit;
        newDataReceived=true;
    }else{
        if (hysteresis--<=0  || !connectionEstablished){
            // log and throw error? // good criteria?
        }
    }

}

void ImageClient::connect(){

    if(!connectionEstablished){
        itsTCPSocket = itsTCPclient.connect(host,port); // here should throw an exception and if it fails close function
        reportCommunication(true);
    }

    //SEND DATA
    {
        std::lock_guard<std::mutex> lock(mtx);
        const int expectedTotalBytesSent = sizeof(dataSent);
        unsigned char sendBuffer[expectedTotalBytesSent];
        // serialize data and put into sendBuffer
        //dataSent = 3.14159; 
        std::copy(reinterpret_cast<const unsigned char*>(&dataSent), 
          reinterpret_cast<const unsigned char*>(&dataSent) + expectedTotalBytesSent, 
          sendBuffer);

        const int actualBytesSent = itsTCPSocket.send(reinterpret_cast<const char*>(&(sendBuffer[0])),expectedTotalBytesSent);
        if(actualBytesSent!=expectedTotalBytesSent){
            std::cout<<" LOG Socket sends failed" << std::endl;
            reportCommunication(false);
            itsTCPSocket.close();
        }
    }

    //RECEIVE DATA
    {
        std::lock_guard<std::mutex> lock(mtx); 
        const int expectedTotalBytesReceived = sizeof(dataReceived);
        unsigned char recvBuffer[expectedTotalBytesReceived];   
        const int actualBytesReceived = itsTCPSocket.receive(reinterpret_cast<char*>(recvBuffer),expectedTotalBytesReceived);

        if(actualBytesReceived==0){
            std::cout<< " LOG actual bytes received zero" << std::endl;
        }else{
            if(actualBytesReceived!=expectedTotalBytesReceived){
                std::cout<< " socket received failed "<< std::endl;
            }
        }

        // deserialize data and verify checksum
        std::copy(recvBuffer, recvBuffer + expectedTotalBytesReceived, reinterpret_cast<unsigned char*>(&dataReceived));
        // checkSum verification if not valid LOG error
        newDataReceived=true;

    }

}

void ImageClient::reportCommunication(const bool communicationOK){

    if(communicationOK && !connectionEstablished){
        connectionEstablished=true;
    }else if(!communicationOK && connectionEstablished){
        connectionEstablished=false;
    }

    return;

}

void ImageClient::startBehaviour(){
    while(!stop_flag){ // atomic exit flag 
        connect();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << " ImageClient leaving while loop"<<std::endl;
}

void ImageClient::stopBehaviour(){
    stop_flag=true;
}
