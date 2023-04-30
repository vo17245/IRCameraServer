#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "Log.h"
#include "Socket.h"
#include "IRRenderer.h"

int Worker(int clientSocket);
void signal_handler(int sig) {
    std::cout<<std::endl;
    std::cout<<"wait for window to close"<<std::endl;
    ircs::IRRenderer::Stop();
    exit(EXIT_SUCCESS);
}
int main(int argc, char** argv){
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("Cannot register signal handler");
        exit(EXIT_FAILURE);
    }
    int serverSocket=ircs::socket::CreateServerSocket(17245);
    if(serverSocket<0)
    {
        ERROR("无法创建serverSocket,由于 {0}",ircs::socket::GetLastError());
        return -1;
    }
    ircs::IRRenderer::CreateWindow();
    while(true)
    {
        int clientSocket=ircs::socket::Accept(serverSocket);
        DEBUG("client connect!");
        if(clientSocket<0)
        {
            ERROR(ircs::socket::GetLastError());
            continue;
        }
        Worker(clientSocket);
    }
    return 0;
}

int Worker(int clientSocket)
{
    // recv client hello
    Buffer versionBuf=ircs::socket::BufferRecv(clientSocket,1);
    if(versionBuf.GetUsed()!=1)
    {
        ircs::socket::Close(clientSocket);
        return -1;
    }
  
    char version=*((char*)versionBuf.GetData());
    // send reply
    if(version!=0x00)
    {
        Buffer reply=Buffer();
        reply.Push(1,[](char* buf){
            buf[0]=0x01;
        });
        ircs::socket::BufferSend(clientSocket,reply);
        ircs::socket::Close(clientSocket);
        return -1;
    }
    else
    {
        Buffer reply=Buffer();
        reply.Push(1,[](char* buf){
            buf[0]=0x00;
        });
        int ret=ircs::socket::BufferSend(clientSocket,reply);
        if(ret!=reply.GetUsed())
        {
            ircs::socket::Close(clientSocket);
            return -1;
        }
    }

    // recv dng size

    Buffer dngSizeBuf=ircs::socket::BufferRecv(clientSocket,4);
   
    
    if(dngSizeBuf.GetUsed()!=4)
    {
        ircs::socket::Close(clientSocket);
        return -1;
    }
    int dngSize=*((int*)dngSizeBuf.GetData());
    DEBUG("recv dng size stop: {0}",dngSize);
    // recv dng data
    DEBUG("recv dng data start");
    Buffer dngDataBuf=ircs::socket::BufferRecv(clientSocket,dngSize);
    DEBUG("recv dng data stop");

    // TODO: do somethin with dng datas
    DNGImage dng(dngDataBuf.GetData(),dngDataBuf.GetUsed());
    Image img=ircs::IRRenderer::Render(dng);
    DEBUG("dng to png, png width {0} height {1}",img.GetWidth(),img.GetHeight());
    ircs::IRRenderer::ShowImage(img);
    // send recv reply
    Buffer recvReply=Buffer();
    recvReply.Push(1,[](char* buf){
        buf[0]=0x00;
    });
    int ret=ircs::socket::BufferSend(clientSocket,recvReply);
    if(ret!=recvReply.GetUsed())
    {
        ircs::socket::Close(clientSocket);
        return -1;
    }
    // close client socket
    ircs::socket::Close(clientSocket);
    DEBUG("close client connect");
    return 0;
}