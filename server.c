#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "h.h"

#pragma comment(lib, "ws2_32.lib")

Message messages[100];
int message_count = 0;

void url_decode(const char *src, char *dst, size_t dst_size) {
    size_t i = 0;
    while (*src != '\0' && i + 1 < dst_size) {
        if (*src == '+') {
            dst[i++] = ' ';
            src++;
            continue;
        }

        if (*src == '%' && isxdigit((unsigned char)src[1]) && isxdigit((unsigned char)src[2])) {
            char hex[3] = { src[1], src[2], '\0' };
            dst[i++] = (char)strtol(hex, NULL, 16);
            src += 3;
            continue;
        }

        dst[i++] = *src++;
    }
    dst[i] = '\0';
}

void add_message(char *username, char *text) {
    if (message_count >= 100) {
        return;
    }

    strncpy(messages[message_count].username, username, sizeof(messages[message_count].username) - 1);
    messages[message_count].username[sizeof(messages[message_count].username) - 1] = '\0';
    strncpy(messages[message_count].text, text, sizeof(messages[message_count].text) - 1);
    messages[message_count].text[sizeof(messages[message_count].text) - 1] = '\0';
    message_count++;

    printf("Nachricht hinzugefuegt: %s - %s\n", username, text);
}

void append_json_escaped(char *dest, size_t dest_size, const char *src) {
    size_t len = strlen(dest);
    while (*src != '\0' && len + 1 < dest_size) {
        unsigned char c = (unsigned char)*src++;
        if (c == '"' || c == '\\') {
            if (len + 2 >= dest_size) break;
            dest[len++] = '\\';
            dest[len++] = c;
        } else if (c == '\b' || c == '\f' || c == '\n' || c == '\r' || c == '\t') {
            if (len + 2 >= dest_size) break;
            dest[len++] = '\\';
            dest[len++] = (c == '\b' ? 'b' : c == '\f' ? 'f' : c == '\n' ? 'n' : c == '\r' ? 'r' : 't');
        } else if (c < 0x20) {
            if (len + 6 >= dest_size) break;
            sprintf(dest + len, "\\u%04x", c);
            len += 6;
        } else {
            dest[len++] = c;
        }
    }
    dest[len] = '\0';
}

void create_messages_json(char *json_buffer) {
    strcpy(json_buffer, "[");

    for (int i = 0; i < message_count; i++) {
        if (i > 0) {
            strcat(json_buffer, ",");
        }

        char message_json[1400] = {0};
        strcat(message_json, "{\"username\":\"");
        append_json_escaped(message_json, sizeof(message_json), messages[i].username);
        strcat(message_json, "\",\"text\":\"");
        append_json_escaped(message_json, sizeof(message_json), messages[i].text);
        strcat(message_json, "\"}");
        strcat(json_buffer, message_json);
    }

    strcat(json_buffer, "]");
}

void handle_send_request(char *request_body) {
    char username[100] = {0};
    char messagetext[500] = {0};
    char encoded_username[200] = {0};
    char encoded_message[1000] = {0};

    char *username_pos = strstr(request_body, "username=");
    if (username_pos) {
        username_pos += strlen("username=");
        char *and_pos = strchr(username_pos, '&');
        size_t len = and_pos ? (size_t)(and_pos - username_pos) : strlen(username_pos);
        if (len >= sizeof(encoded_username)) {
            len = sizeof(encoded_username) - 1;
        }
        strncpy(encoded_username, username_pos, len);
        encoded_username[len] = '\0';
        url_decode(encoded_username, username, sizeof(username));
    }

    char *message_pos = strstr(request_body, "message=");
    if (message_pos) {
        message_pos += strlen("message=");
        size_t len = strlen(message_pos);
        if (len >= sizeof(encoded_message)) {
            len = sizeof(encoded_message) - 1;
        }
        strncpy(encoded_message, message_pos, len);
        encoded_message[len] = '\0';
        url_decode(encoded_message, messagetext, sizeof(messagetext));
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
            sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\nConnection: close\r\n\r\n");
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
