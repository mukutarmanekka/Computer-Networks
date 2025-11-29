// serverUdp.c  (Windows / MinGW Compatible)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#define PORT 8080
#define BUFSIZE 1024

int main(void) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFSIZE];
    int ret;
    int clientAddrLen = sizeof(clientAddr);

    // Start winsock
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

    // Bind settings
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("UDP Server running on port %d...\n", PORT);

    // Main loop
    while (1) {
        ret = recvfrom(sock, buffer, BUFSIZE - 1, 0,
                       (struct sockaddr*)&clientAddr, &clientAddrLen);

        if (ret == SOCKET_ERROR) {
            printf("recvfrom failed: %d\n", WSAGetLastError());
            break;
        }

        buffer[ret] = '\0';

        printf("Client [%s:%d]: %s\n",
               inet_ntoa(clientAddr.sin_addr),
               ntohs(clientAddr.sin_port),
               buffer);

        // If client requests shutdown
        if (strcmp(buffer, "shutdown") == 0) {
            printf("Shutdown command received. Server exiting...\n");
            break;
        }

        // Reply
        const char *reply = "Message received";
        sendto(sock, reply, strlen(reply), 0,
               (struct sockaddr*)&clientAddr, clientAddrLen);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
