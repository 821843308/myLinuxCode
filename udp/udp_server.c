#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        perror("argc");
        return 1;
    }
    int sock = socket(AF_INET,SOCK_DGRAM,0);//IPV4,UDP
    if(sock < 0){
        perror("socket");
        return 2;
    }

    struct sockaddr_in local;
    local.sin_family = AF_INET;//IPV4
    local.sin_port = htons(atoi(argv[2]));//端口号转网络字节序
    local.sin_addr.s_addr = inet_addr(argv[1]);//点分十进制转int转网络字节序
    
    
    //绑定端口号
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
        perror("bind");
        return 3;
    }

    char buf[1024];
    struct sockaddr_in client;//输出型参数
    while(1)
    {
        socklen_t len = sizeof(client);
        ssize_t s = recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&client,&len);
        if(s > 0)
        {
            buf[s] = 0;
            printf("[%s:%d]:%s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);//inet_ntoa将sin_addr结构体中的网络字节序ip转点分十进制,
                                                                                            //结果放在静态缓冲区，第二次调用这个函数会覆盖上一次的结果

            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&client,sizeof(client));
        }
    }
    return 0;    
}
