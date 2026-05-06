#include <stdio.h>
#include <server.c>
#include <string.h>

// Funktion zum Verschlüsseln
void get_encryptionVariable(Message)
{
    int encryptionVariable = strlen(username);
}

void Caesar_Encrypt(Message)
{
    get_encryptionVariable(Message);
    for (int i = 0; text[i] != '\0'; i++)
    {
        encryptText[i] = text[i] + encryptionVariable;
    }
    return encryptText
}

void Caesar_Decrypt(Message)
{
    get_encryptionVariable(Message);
    for (int i = 0; text[i] != '\0'; i++)
    {
        encryptText[i] = text[i] - encryptionVariable;
    }
    return encryptText
}