//
// Created by Friedemann Grothe on 20.05.26.
//

#ifndef NETZWEK_H_H
#define NETZWEK_H_H

typedef struct {
    char username[100];
    char text[500];
} Message;

void Caesar_Encrypt(Message *u);
void Caesar_Decrypt(Message *u);

#endif //NETZWEK_H_H
