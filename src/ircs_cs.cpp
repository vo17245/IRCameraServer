#include "Socket.h"
#include "Log.h"

const int IRCS_BUFSIZE=41943040; //40M
char* ircs_recv_buffer=new char[IRCS_BUFSIZE];

size_t getFileSize(const char* filepath);


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
  
    // recv dng data
  
    Buffer dngDataBuf=ircs::socket::BufferRecv(clientSocket,dngSize);


    // TODO: render dng to create png image
    DNGImage dng(dngDataBuf.GetData(),dngDataBuf.GetUsed());
    char pngPath[]="res/suiginton.png";
    int pngSize=getFileSize(pngPath);
    Buffer pngDataBuffer;
    pngDataBuffer.Push(pngSize,[pngSize](char* data){
        FILE* fp;
        fp=fopen("res/suiginton.png","rb");
        fread(data,pngSize,1,fp);
        fclose(fp);
    });
    
    Buffer pngSizeBuffer;
    pngSizeBuffer.Push(4,(const char*)&pngSize);
    // send png size
    int ret=ircs::socket::BufferSend(clientSocket,pngSizeBuffer);
    if(pngSizeBuffer.GetUsed()!=ret)
    {
        ircs::socket::Close(clientSocket);
        return -1;
    }
    DEBUG("send png size: {0}",pngSize);
    // send png data
    
    ret=ircs::socket::BufferSend(clientSocket,pngDataBuffer);
    if(pngDataBuffer.GetUsed()!=ret)
    {
        ircs::socket::Close(clientSocket);
        return -1;
    }

    DEBUG("send png data");
    // recv client reply
    Buffer clientRecvReply=ircs::socket::BufferRecv(clientSocket,1);
    if(clientRecvReply.GetUsed()!=1)
    {
        ircs::socket::Close(clientSocket);
        return -1;
    }
    //close connect
    ircs::socket::Close(clientSocket);
    DEBUG("close connect");
    return 0;
}

int main(int argc, char** argv){

    int serverSocket=ircs::socket::CreateServerSocket(17246);
    if(serverSocket<0)
    {
        ERROR("无法创建serverSocket,由于 {0}",ircs::socket::GetLastError());
        return -1;
    }
    while(true)
    {
        int clientSocket=ircs::socket::Accept(serverSocket);
  
        if(clientSocket<0)
        {
            ERROR(ircs::socket::GetLastError());
            continue;
        }
        Worker(clientSocket);
    }
    return 0;
}



size_t getFileSize(const char* filepath) 
{
	struct stat statbuf;
	stat(filepath, &statbuf);
	return statbuf.st_size;
}