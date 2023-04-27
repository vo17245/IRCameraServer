#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <iostream>

#include "Image.h"
#include "Buffer.h"

namespace ircs
{
    namespace socket
    {
        char* GetLastError();
        int CreateServerSocket(int port);
        int Accept(int serverSocket);
        int Recv(int clientSocket,char* buf,int size);
        int Close(int sock);
        Buffer BufferRecv(int clientSocket,int size);
        int Send(int clientSocket,const char* buf,int size);
        /*
        *@return
        *   send bytes count
        *   如果发送的数量与传入buffe 长度不符，表明客户端断开连接
        * 
        */
        int BufferSend(int clientSocket,const Buffer& buffer);
    }
    
}
