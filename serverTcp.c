// Windows TCP Server (C)
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    char buffer[1024];
    int client_len = sizeof(client);

    // 1. Initialize Winsock
    printf("Initializing Winsock...\n");
    WSAStartup(MAKEWORD(2,2), &wsa);

    // 2. Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Could not create socket\n");
        return 1;
    }

    // 3. Setup server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // 4. Bind
    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        return 1;
    }

    // 5. Listen
    listen(server_socket, 3);
    printf("Server listening on port 8080...\n");

    // 6. Accept client
    client_socket = accept(server_socket, (struct sockaddr*)&client, &client_len);
    printf("Client connected!\n");

    // 7. Receive
    int r = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[r] = '\0';
    printf("Client says: %s\n", buffer);

    // 8. Send response
    char msg[] = "Hello from Windows Server!";
    send(client_socket, msg, strlen(msg), 0);

    // 9. Close sockets
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
