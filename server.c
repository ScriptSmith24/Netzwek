#include <windows.h>
#include <stdio.h>
#include <winsock2.h> //-> Documentaion https://learn.microsoft.com/en-us/windows/win32/api/winsock2/
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") 


int main() {
    // 1. Create a data structure to hold Winsock info
    WSADATA wsa;

    printf("Initializing Winsock...\n");

    // 2. Pass the version (2.2) and the address of your 'wsa' structure
    // MAKEWORD(2,2) creates the version number needed
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    printf("Initialised successfully!\n");
    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP
    if (s == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    server.sin_port = htons(8888);       // Port number (htons converts it to network byte order)

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
    }

    listen(s, 3); // 3 is the "backlog" (how many people can wait in line)

printf("Waiting for incoming connections...\n");

    while(1) {
        struct sockaddr_in client;
        int c = sizeof(struct sockaddr_in);
        SOCKET new_socket = accept(s, (struct sockaddr *)&client, &c);

        if (new_socket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            continue; // Keep trying for the next connection
        }

        printf("Connection accepted!\n");

        // The HTTP Response - Must have \r\n (Carriage Return + Line Feed)
        char *response = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "Content-Length: 35\r\n"
                         "Connection: close\r\n"
                         "\r\n"
                         "<html><body>Hello World</body></html>";

        send(new_socket, response, (int)strlen(response), 0);
        
        // Close the specific client socket, but NOT the main listener 's'
        closesocket(new_socket);
        printf("Response sent and connection closed. Waiting for next...\n");
    }

    // This part is rarely reached now because of while(1)
    closesocket(s);
    WSACleanup();
    return 0;
}
