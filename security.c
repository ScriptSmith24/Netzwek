#include <stdio.h>
#include <server.c>
#include <string.h>

// Funktion zum Verschlüsseln
int get_encryptionVariable(unencry)
{
    int encryptionVariable = strlen(username);
    return encryptionVariable;
}

void Caesar_Encrypt(unencry)
{
    get_encryptionVariable(unencry);
    for (int i = 0; text[i] != '\0'; i++)
    {
        encryptText[i] = text[i] + encryptionVariable;
    }
    return encryptText
}

void Caesar_Decrypt(unencry)
{
    get_encryptionVariable(unencry);
    for (int i = 0; text[i] != '\0'; i++)
    {
        encryptText[i] = text[i] - encryptionVariable;
    }
    return encryptText
}