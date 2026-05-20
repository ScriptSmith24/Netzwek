#include <stdio.h>
#include <server.c>
#include <string.h>
#include "h.h"

typedef struct {
    char username[100];
    char text[500];
} UnencryptedMessage;

typedef struct {
    char username[100];
    char text[500];
} EncryptedMessage;

int encryptionVariable = 0;

// Funktion zum Verschlüsseln
void get_encryptionVariable(Message *u)
{
    encryptionVariable = strlen(Message.username);
}

void Caesar_Encrypt(Message *u)
{
    get_encryptionVariable(Message *u);
    for (int i = 0; Message.text[i] != '\0'; i++)
    {
        Message.text[i] = Message.text[i] + encryptionVariable;
    }

}

void Caesar_Decrypt(Message *u)
{
    for (int i = 0; Message.text[i] != '\0'; i++)
    {
        Message.text[i] = Message.text[i] - encryptionVariable;
    }
}