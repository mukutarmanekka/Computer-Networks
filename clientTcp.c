// Windows TCP Client (C)
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    char buffer[1024];

    // 1. Initialize Winsock
    WSAStartup(MAKEWORD(2,2), &wsa);

    // 2. Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 3. Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 4. Connect to server
    if (connect(client_socket, (struct sockaddr*)&server, sizeof(server)) != 0) {
        printf("Connection failed!\n");
        return 1;
    }

    // 5. Send message
    char msg[] = "Hello from Windows Client!";
    send(client_socket, msg, strlen(msg), 0);

    // 6. Receive reply
    int r = recv(client_socket, buffer, 1024, 0);
    buffer[r] = '\0';
    printf("Server says: %s\n", buffer);

    // 7. Close
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
