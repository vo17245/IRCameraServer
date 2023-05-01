#include "Socket.h"
#include "Log.h"

namespace ircs
{
    namespace socket
    {
        static const int IRCS_BUFSIZE=41943040; //40M
        static char* ircs_recv_buffer=new char[IRCS_BUFSIZE];
        char* GetLastError()
        {
            return ::strerror(errno);
        }
        int CreateServerSocket(int port)
        {
            int  listenfd;
            struct sockaddr_in  servaddr;
            if( (listenfd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
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
                if(ret==0||ret==-1)
                {
                    return sendCnt;
                }
                sendCnt+=ret;
            }
            return sendCnt;
        }
        void BufferRecv(int clientSocket,int size,Buffer& buffer)
        {
            while(buffer.GetUsed()!=size)
            {
                if (size-buffer.GetUsed()>=IRCS_BUFSIZE)
                {
                    int byteCnt=Recv(clientSocket,ircs_recv_buffer,IRCS_BUFSIZE);
                    if (byteCnt==0)
                    {
                        return;
                    }
                    buffer.Push(byteCnt,ircs_recv_buffer);
                }
                else
                {
                    int byteCnt=Recv(clientSocket,ircs_recv_buffer,size-buffer.GetUsed());
                    if (byteCnt==0)
                    {
                        return;
                    }
                    buffer.Push(byteCnt,ircs_recv_buffer);
                }
            }
        }
    }
    
}
