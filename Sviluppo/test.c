#include <stdio.h>
#include <stdlib.h>

int main () 
{
    char i = 0;
    do {
        i = getchar();
        putchar(i);
    }
    while (i != 'a');
}