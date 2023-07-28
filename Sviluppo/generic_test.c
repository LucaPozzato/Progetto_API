#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
    FILE *input;
    input = fopen(argv[1], "r");
    char ch;

    do {
        ch = getc_unlocked(input);
        printf("%c", ch);
 
        // Checking if character is not EOF.
        // If it is EOF stop reading.
    } while (ch != EOF);
    fclose(input);
    return 0;
}

