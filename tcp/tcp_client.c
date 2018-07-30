#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>

#define SERVER_PORT 9999

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        perror("argc");
        return 1;
    }
    int sock = socket(AF_INET,SOCK_STREAM,0);//IPV4,UDP
    if(sock < 0){
        perror("socket");
        return 2;
    }

    struct sockaddr_in server;//IPV4,use struct sockaddr_in
    server.sin_family = AF_INET;//IPV4
    server.sin_port = htons(SERVER_PORT);//本机转网络字节序，s short 短整形16位
    server.sin_addr.s_addr = inet_addr(argv[1]);//点分十进制字符串转 in_addr_t
    

    int ret = connect(sock,(struct sockaddr*)&server,sizeof(server));
    if(ret < 0)
    {
        printf("connect failed,errno is:%d,strerror is:%s\n",errno,strerror(errno));
    }


    char buf[1024];
    memset(buf,'\0',sizeof(buf));

    while(1)
    {
        printf("client says:");
        fgets(buf,sizeof(buf),stdin);

        buf[strlen(buf)-1] = '\0';

        write(sock,buf,sizeof(buf));
        memset(buf,'\0',sizeof(buf));
        if(strncasecmp(buf,"quit",4) == 0)
        {
            printf("client quit\n");
            break;
        }
        printf("please wait ...\n");
        read(sock,buf,sizeof(buf));
        printf("server says:%s\n",buf);
    }
    close(sock);

    return 0;
}
