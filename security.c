#include <stdio.h>
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
int get_encryptionVariable(const Message *u)
{
    encryptionVariable = strlen(u->username);
}

void Caesar_Encrypt(Message *u)
{
    int encryptionVariable= get_encryptionVariable(u);
    for (int i = 0; u->text[i] != '\0'; i++)
    {
        u->text[i] = u->text[i] + encryptionVariable;
    }

}

void Caesar_Decrypt(Message *u)
{
    int encryptionVariable= get_encryptionVariable(u);
    for (int i = 0; u->text[i] != '\0'; i++)
    {
        u->text[i] = u->text[i] - encryptionVariable;
    }
}