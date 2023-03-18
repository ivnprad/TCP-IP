#include "ImageServer.hpp"
#include "TCPSocket.hpp"
#include <thread>
#include <chrono>

ImageServer::ImageServer(const std::string& host,int port):
itsTCPSocketServer(host,port), 
hysteresisLimit(100),
hysteresis(0),
connectionEstablished(false),
newDataReceived(false){

    stop_flag = false;

}


// ImageServer::~ImageServer(){


// }

void ImageServer::process(){
    std::lock_guard<std::mutex> lock(mtx); // automatically unlocked when going out of scope
    
    if (newDataReceived){ // this value is set in other functions
        //save value data received data
        //dataReceived; // do sth with this new data
        std::cout << " SERVER data received "<< dataReceived << std::endl;
        hysteresis=hysteresisLimit;
        newDataReceived = false;
    }else{
        // decrement hysteresis
        if (hysteresis--<=0  || !connectionEstablished){
            // log and throw error? // good criteria?
        }
    }
      
    // update data to be sent 
    dataSent = 10.0;
    std::cout << " SERVER DATA  sent "<< dataSent << std::endl;
    // dataSent should be a class and update its checkSum;

}


void ImageServer::handleConnection(){
    TCPSocket client_socket = itsTCPSocketServer.acceptConnection(); // creates socket client
    reportCommunication(true);
    sockOk.store(true);

    while (sockOk){
        // RECEIVE DATA FROM SOCKET
        const int expectedTotalBytesReceived = sizeof(dataReceived);
        unsigned char recvBuffer[expectedTotalBytesReceived];
        int totalBytesReceived = client_socket.receive(
            reinterpret_cast<char*>(&(recvBuffer[0])),
            expectedTotalBytesReceived);


        if(totalBytesReceived==0){
            std::cout<<" no DATA RECEIVED YET"<< std::endl;
        }else{
            if(totalBytesReceived!=expectedTotalBytesReceived){
                reportCommunication(false);
                client_socket.close();
                return;
            }
            //std::cout <<" correct number of bytes received "<< std::endl;

            
            {
                // desesialize data and verify checksum
                // HERE deserialize(recvBuffer)-> dataReceived
                std::lock_guard<std::mutex> lock(mtx);
                //deseralize data 
                std::copy(recvBuffer, recvBuffer + expectedTotalBytesReceived, reinterpret_cast<unsigned char*>(&dataReceived));
                bool checkSumValid=true; // assume checkSum is valid
                newDataReceived=true;

                if(!checkSumValid){
                    std::cout << " LOG checksum error "<< std::endl;
                    reportCommunication(false);
                }
            }

        }

        // SEND DATA to CLIENT
        {
            std::lock_guard<std::mutex> lock(mtx);
            const int expectedTotalBytesSent = sizeof(dataSent);
            unsigned char sendBuffer[expectedTotalBytesSent];
            // serialize data
            std::copy(reinterpret_cast<const unsigned char*>(&dataSent), 
            reinterpret_cast<const unsigned char*>(&dataSent) + expectedTotalBytesSent, 
            sendBuffer);

            const int actualBytesSent = client_socket.send(reinterpret_cast<char*>(&(sendBuffer[0])),expectedTotalBytesSent);
            if (actualBytesSent!=expectedTotalBytesSent){
                std::cout << " client socket sent failed "<< std::endl;
                reportCommunication(false);
                client_socket.close();
            }
            
        }

    }

}

void ImageServer::reportCommunication(const bool communicationOK){

    if(communicationOK && !connectionEstablished){
        connectionEstablished=true;
    }else if(!communicationOK && connectionEstablished){
        connectionEstablished=false;
    }

    return;

}

void ImageServer::startBehaviour(){
    while(!stop_flag){ // atomic exit flag 
        handleConnection();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << " ImageServer leaving while loop"<<std::endl;
}


void ImageServer::stopBehaviour(){
    //stop_flag=true;
    stop_flag.store(true);
    sockOk.store(false);
    std::cout << " stopping SERVERRRR "<<std::endl;
}
