#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;

    printf("Initialisiere Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Fehler: %d\n", WSAGetLastError());
        return 1;
    }

    // Socket erstellen
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket Fehler: %d\n", WSAGetLastError());
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if(bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind Fehler: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 1;
    }

    // Listen
    listen(s, 3);
    printf("Server läuft auf Port 8888. Warte auf Verbindungen...\n");

    c = sizeof(struct sockaddr_in);
    while(1) {
        new_socket = accept(s, (struct sockaddr *)&client, &c);
        if (new_socket == INVALID_SOCKET) continue;

        char recv_buffer[1024] = {0};
        recv(new_socket, recv_buffer, sizeof(recv_buffer), 0);

        char method[10], path[100];
        sscanf(recv_buffer, "%s %s", method, path);

        char *fileName = path + 1; 
        if (strcmp(path, "/") == 0) {
            fileName = "Site.html";
        }

        printf("Anfrage fuer: %s\n", fileName);

        FILE *file = fopen(fileName, "rb"); 
        if (file == NULL) {
            char *error = "HTTP/1.1 404 Not Found\r\n\r\n<h1>404 nicht gefunden</h1>";
            send(new_socket, error, (int)strlen(error), 0);
        } else {
            char *contentType = "text/plain";
            if (strstr(fileName, ".html")) contentType = "text/html";
            else if (strstr(fileName, ".css")) contentType = "text/css";
            else if (strstr(fileName, ".js"))  contentType = "application/javascript";
            else if (strstr(fileName, ".png")) contentType = "image/png";

            char header[256];
            sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nConnection: close\r\n\r\n", contentType);
            send(new_socket, header, (int)strlen(header), 0);

            // 4. Datei senden
            char file_buffer[1024];
            size_t bytes_read;
            while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), file)) > 0) {
                send(new_socket, file_buffer, (int)bytes_read, 0);
            }
            fclose(file);
        }
        closesocket(new_socket);
    }

    closesocket(s);
    WSACleanup();//end
    return 0;
}


