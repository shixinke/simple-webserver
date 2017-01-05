#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BACKLOG 5
#define BUF_SIZE 1024
#define DEFAULT_PORT 9669

int start(char *ip, int port)
{
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        printf("create socket server failed, the reason is:%s\n", strerror(errno));
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    int ret = bind(server_sock, (struct sockaddr *) &addr,sizeof(addr));
    if (ret < 0) {
        printf("bind socket server failed, the reason is:%s\n", strerror(errno));
        return -1;
    }
    int res = listen(server_sock, BACKLOG);
    if (res < 0) {
        printf("listen socket server failed, the reason is:%s\n", strerror(errno));
        return -1;
    }
    int client;
    char buf[BUF_SIZE];
    char *response;
    memset(buf, '\0', BUF_SIZE);
    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    while(1) {
        client = accept(server_sock, (struct sockaddr *) &client_addr, &client_len);
        if (client < 0) {
            printf("accept socket server failed, the reason is:%s\n", strerror(errno));
            return -1;
        }
        ret = recv(client, buf, BUF_SIZE, 0);
        if (res < 0) {
            printf("receive data failed, the reason is:%s\n", strerror(errno));
            return -1;
        }
        printf("receive:\n%s", buf);
        response = "It works, powered by socket server";
        int ret = send(client, response, sizeof(response), 0);
        if (res < 0) {
            printf("send data failed, the reason is:%s\n", strerror(errno));
        }
        close(client);
    }
    close(server_sock);
    return 0;
}

int main(int argc, char *argv[])
{
    char *ip;
    int port;
    if (argc == 3) {
        ip = argv[1];
        port = atoi(argv[2]);
    } else if(argc == 2) {
        ip = argv[1];
        port = DEFAULT_PORT;
    } else {
        ip = "127.0.0.1";
        port = DEFAULT_PORT;
    }
    start(ip, port);
    return 0;
}