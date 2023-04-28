#include <iostream>
#include "Log.h"
#include "Socket.h"

int Worker(int clientSocket);

int main(int argc, char** argv){

    int serverSocket=ircs::socket::CreateServerSocket(17245);
    if(serverSocket<0)
    {
        ERROR("无法创建serverSocket,由于 {0}",ircs::socket::GetLastError());
        return -1;
    }
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
    DEBUG("height: {0}",dng.GetHeight());
    DEBUG("width: {0}",dng.GetWidth());
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