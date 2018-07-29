#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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

    struct sockaddr_in server;//IPV4,use struct sockaddr_in
    server.sin_family = AF_INET;//IPV4
    server.sin_port = htons(atoi(argv[2]));//本机转网络字节序，s short 短整形16位
    server.sin_addr.s_addr = inet_addr(argv[1]);//点分十进制字符串转 in_addr_t
    
    struct sockaddr_in peer;
    char buf[1024];
    while(1)
    {
        socklen_t len = sizeof(peer);
        printf("Please Enter:");
        fflush(stdout);
        ssize_t s = read(0,buf,sizeof(buf)-1);
        if(s > 0)
        {
            buf[s-1] = 0;
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));
            ssize_t _s = recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&len);//peer一定和server相同
            if(_s > 0)
            {
                buf[_s] = 0;
                printf("server say:%s\n",buf);
            }
        }
    }
    return 0;
}
