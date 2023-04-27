#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <iostream>
#include "Log.h"
#include "Buffer.h"
#include "Image.h"
const int IRCS_BUFSIZE=41943040; //40M
char* ircs_recv_buffer=new char[IRCS_BUFSIZE];

char* GetLastError();
int CreateServerSocket(int port);
int Accept(int serverSocket);
int Recv(int clientSocket,char* buf,int size);
int Close(int sock);
//如果buffer 数据长度与传入的 size 不符，表明客户端断开连接
Buffer BufferRecv(int clientSocket,int size);
int Send(int clientSocket,const char* buf,int size);
/*
*@return
*   send bytes count
*   如果发送的数量与传入buffe 长度不符，表明客户端断开连接
* 
*/
int BufferSend(int clientSocket,const Buffer& buffer);

int Worker(int clientSocket)
{
    // recv client hello
    Buffer versionBuf=BufferRecv(clientSocket,1);
    if(versionBuf.GetUsed()!=1)
    {
        Close(clientSocket);
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
        BufferSend(clientSocket,reply);
        Close(clientSocket);
        return -1;
    }
    else
    {
        Buffer reply=Buffer();
        reply.Push(1,[](char* buf){
            buf[0]=0x00;
        });
        int ret=BufferSend(clientSocket,reply);
        if(ret!=reply.GetUsed())
        {
            Close(clientSocket);
            return -1;
        }
    }

    // recv dng size

    Buffer dngSizeBuf=BufferRecv(clientSocket,4);

    
    if(dngSizeBuf.GetUsed()!=4)
    {
        Close(clientSocket);
        return -1;
    }
    int dngSize=*((int*)dngSizeBuf.GetData());
  
    // recv dng data
  
    Buffer dngDataBuf=BufferRecv(clientSocket,dngSize);


    // TODO: do somethin with dng datas
    DNGImage dng(dngDataBuf.GetData(),dngDataBuf.GetUsed());
    DEBUG("height: {0}",dng.GetHeight());
    DEBUG("width: {0}",dng.GetWidth());
    // send recv reply
    Buffer recvReply=Buffer();
    recvReply.Push(1,[](char* buf){
        buf[0]=0x00;
    });
    int ret=BufferSend(clientSocket,recvReply);
    if(ret!=recvReply.GetUsed())
    {
        Close(clientSocket);
        return -1;
    }
    // close client socket
    Close(clientSocket);
    return 0;
}

int main(int argc, char** argv){

    int serverSocket=CreateServerSocket(17245);
    if(serverSocket<0)
    {
        ERROR("无法创建serverSocket,由于 {0}",GetLastError());
        return -1;
    }
    while(true)
    {
        int clientSocket=Accept(serverSocket);
  
        if(clientSocket<0)
        {
            ERROR(GetLastError());
            continue;
        }
        Worker(clientSocket);
    }
    return 0;
}


char* GetLastError()
{
    return strerror(errno);
}
int CreateServerSocket(int port)
{
    int  listenfd;
    struct sockaddr_in  servaddr;
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        return -1;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    int opt=1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        
        return -2;
    }
    if( listen(listenfd, 10) == -1){
        return -3;
    }
    
    return listenfd;
}

int Accept(int serverSocket)
{
    
    return accept(serverSocket, (struct sockaddr*)NULL, NULL);
}

int Recv(int clientSocket,char* buf,int size)
{
    return recv(clientSocket, buf, size, 0);
}

int Close(int sock)
{
    close(sock);
    return shutdown(sock,SHUT_RDWR);
}
Buffer BufferRecv(int clientSocket,int size)
{
    

    Buffer buffer;

  
    while(buffer.GetUsed()!=size)
    {
        if (size-buffer.GetUsed()>=IRCS_BUFSIZE)
        {
     
            int byteCnt=Recv(clientSocket,ircs_recv_buffer,IRCS_BUFSIZE);

            if (byteCnt==0)
            {
                return buffer;
            }
            buffer.Push(byteCnt,ircs_recv_buffer);
        }
        else
        {
          
            int byteCnt=Recv(clientSocket,ircs_recv_buffer,size-buffer.GetUsed());
         
            if (byteCnt==0)
            {
                return buffer;
            }
            buffer.Push(byteCnt,ircs_recv_buffer);
        }
       
    }

    return buffer;
    
}
int Send(int clientSocket,const char* buf,int size)
{
    return send(clientSocket, buf, size, MSG_NOSIGNAL);
}
int BufferSend(int clientSocket,const Buffer& buffer)
{
    int sendCnt=0;
    while(sendCnt!=buffer.GetUsed())
    {
        int ret=Send(clientSocket,buffer.GetData()+sendCnt,buffer.GetUsed()-sendCnt);
        if(ret==0)
        {
            return sendCnt;
        }
        sendCnt+=ret;
    }
    return sendCnt;
}