#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

typedef struct {
    char username[100];
    char text[500];
} Message;
Message unencry[100];
void encrypt(unencry){
    

}
Message messages[100];
int message_count = 0;

void add_message(char *username, char *text) {
    if (message_count >= 100) {
        return;
    }

    strcpy(messages[message_count].username, username);
    strcpy(messages[message_count].text, text);
    message_count++;

    printf("Nachricht hinzugefuegt: %s - %s\n", username, text);
}

void create_messages_json(char *json_buffer) {
    strcpy(json_buffer, "[");

    for (int i = 0; i < message_count; i++) {
        if (i > 0) {
            strcat(json_buffer, ",");
        }

        char message_json[700];
        sprintf(message_json, "{\"username\":\"%s\",\"text\":\"%s\"}",
                messages[i].username, messages[i].text);
        strcat(json_buffer, message_json);
    }

    strcat(json_buffer, "]");
}

void handle_send_request(char *request_body) {
    char username[100] = {0};
    char messagetext[500] = {0};

    char *username_pos = strstr(request_body, "username=");
    if (username_pos) {
        sscanf(username_pos, "username=%99s", username);
        char *and_pos = strchr(username, '&');
        if (and_pos) *and_pos = '\0';
    }

    char *message_pos = strstr(request_body, "message=");
    if (message_pos) {
        sscanf(message_pos, "message=%499s", messagetext);
    }

    if (strlen(username) > 0 && strlen(messagetext) > 0) {
        add_message(username, messagetext);
    }
}

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
                    //IPv4   TCP
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket Fehler: %d\n", WSAGetLastError());
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if(bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind Fehler: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return 1;
    }

    listen(s, 3);
    printf("Server laeuft auf Port 8888. Warte auf Verbindungen...\n");

    c = sizeof(struct sockaddr_in);

    while(1) {
        new_socket = accept(s, (struct sockaddr *)&client, &c);
        if (new_socket == INVALID_SOCKET) continue;

        char recv_buffer[2048] = {0};
        recv(new_socket, recv_buffer, sizeof(recv_buffer), 0);

        char method[10], path[100];
        sscanf(recv_buffer, "%s %s", method, path);

        printf("Anfrage: %s %s\n", method, path);

        if (strcmp(path, "/api/messages") == 0) {
            char json_buffer[10000] = {0};
            create_messages_json(json_buffer);

            char header[256];
            sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n");
            send(new_socket, header, (int)strlen(header), 0);

            send(new_socket, json_buffer, (int)strlen(json_buffer), 0);
        }
        else if (strcmp(path, "/api/send") == 0) {
            char *body_start = strstr(recv_buffer, "\r\n\r\n");
            if (body_start) {
                body_start += 4;
                handle_send_request(body_start);
            }

            char *response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n{\"status\":\"ok\"}";
            send(new_socket, response, (int)strlen(response), 0);
        }
        else {
            char *fileName = path + 1;
            if (strcmp(path, "/") == 0) {
                fileName = "Site.html";
            }

            printf("Datei angefordert: %s\n", fileName);

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

                char file_buffer[1024];
                size_t bytes_read;
                while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), file)) > 0) {
                    send(new_socket, file_buffer, (int)bytes_read, 0);
                }
                fclose(file);
            }
        }
        closesocket(new_socket);
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
