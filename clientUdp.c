// clientUdp.c  (Windows / MinGW Compatible)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFSIZE 1024

int main(void) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in serverAddr;
    char sendbuf[BUFSIZE];
    char recvbuf[BUFSIZE];
    int ret;
    int serverAddrLen = sizeof(serverAddr);

    // Init winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Server address setup
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    printf("Type messages to send to server (type 'quit' to exit, 'shutdown' to stop server)\n");

    while (1) {
        printf("> ");
        fgets(sendbuf, BUFSIZE, stdin);

        // Remove newline
        size_t len = strlen(sendbuf);
        if (len > 0 && sendbuf[len-1] == '\n')
            sendbuf[len-1] = '\0';

        // Quit client
        if (strcmp(sendbuf, "quit") == 0)
            break;

        // Send data to server
        ret = sendto(sock, sendbuf, strlen(sendbuf), 0,
                     (struct sockaddr*)&serverAddr, serverAddrLen);

        if (ret == SOCKET_ERROR) {
            printf("sendto failed: %d\n", WSAGetLastError());
            break;
        }

        // Receive reply from server
        ret = recvfrom(sock, recvbuf, BUFSIZE - 1, 0, NULL, NULL);
        if (ret == SOCKET_ERROR) {
            printf("recvfrom failed: %d\n", WSAGetLastError());
            break;
        }

        recvbuf[ret] = '\0';
        printf("Server: %s\n", recvbuf);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
