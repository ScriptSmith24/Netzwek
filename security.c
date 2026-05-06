#include <stdio.h>

// Funktion zum Verschlüsseln
void verschluesseln(char text[])
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        text[i] = text[i] + 3;
    }
}

int main()
{
    char text[] = "hallo";

    // Funktion aufrufen
    verschluesseln(text);

    // Verschlüsselten Text ausgeben
    printf("%s\n", text);

    // ASCII-Werte ausgeben
    for (int i = 0; text[i] != '\0'; i++)
    {
        printf("%d ", text[i]);
    }

    return 0;
}
