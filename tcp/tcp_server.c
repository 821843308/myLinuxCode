#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#define _PORT 9999
#define _BACKLOG 10

int main()
{

    //创建socket
    int sock = socket(AF_INET,SOCK_STREAM,0);//ipv4,tcp
    if(sock < 0)
    {
        perror("socket");
        return 1;
    }

    //填写服务端信息
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(_PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);//任意地址

    //绑定服务端端口号
    if(bind(sock,(struct sockaddr*)&server,sizeof(server)) < 0)
    {
        perror("bind");
        return 2;
    }

    //监听套接字,监听有没有请求链接，放入等待队列
    if(listen(sock,_BACKLOG) < 0)//listen的第二个参数，链接队列(等待队列)
    {
        perror("listen");
        return 3;
    }
    
    struct sockaddr_in client;//输出型参数
    while(1)
    {
        socklen_t len = 0;
        int client_sock = accept(sock,(struct sockaddr*)&client,&len);//三次握手后，调用accept接受链接
        if(client_sock < 0)//链接失败
        {
            printf("accept is error,errno is:%d,strerror is %s\n",errno,strerror(errno));//错误原因
            close(sock);
            return 4;
        }

        //获取客户端ip
        char buf_ip[INET_ADDRSTRLEN];
        memset(buf_ip,'\0',sizeof(buf_ip));
        inet_ntop(AF_INET,&client.sin_addr.s_addr,buf_ip,sizeof(buf_ip));

        printf("accept success,ip: %s ,port: %d\n",buf_ip,ntohs(client.sin_port));

        //进行通信
        while(1)
        {
            char buf[1024];
            memset(buf,'\0',sizeof(buf));
            read(client_sock,buf,sizeof(buf));
            printf("client say:%s\n",buf);

            printf("server say:");
            memset(buf,'\0',sizeof(buf));
            fgets(buf,sizeof(buf),stdin);

            buf[strlen(buf)-1] = '\0';
            write(client_sock,buf,strlen(buf)+1);

            printf("please wait...\n");
        }
    }
    close(sock);
    return 0;
}
