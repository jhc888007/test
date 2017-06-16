#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLEN 1024

#define IP "127.0.0.1"
#define PORT 7777

int main(int argc,char **argv)
{
    int socket_id;
    if ((socket_id = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        printf("Socket Add Error: %d", socket_id);
        exit(0);
    }
    printf("Socket Add: %d", socket_id);

    struct sockaddr_in sockaddr;
    const char *servInetAddr = IP;
    memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT);
    inet_pton(AF_INET,servInetAddr,&sockaddr.sin_addr);
    
    if ((connect(socket_id,(struct sockaddr*)&sockaddr,sizeof(sockaddr))) < 0) {
        printf("Socket Connect Error %s Errno: %d", strerror(errno),errno);
        exit(0);
    }
    printf("Socket Connected");

    for (int i = 0; i < 20; i++) {
        char sendline[MAXLEN] = "Send Msg";
        int n;
        if ((send(socket_id,sendline,strlen(sendline),0)) < 0) {
            printf("Send Msg Error %s Errno: %d", strerror(errno),errno);
            exit(0);
        }
        printf("Socket Send Msg");
        sleep(3);
    }

    close(socket_id);
    exit(0);
}


