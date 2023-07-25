#include <stdio.h>
#include <stdlib.h>

int main () 
{
    char i = 0;
    do {
        scanf("%c", &i);
        printf("%c", i);
    }
    while (i != 'a');
    printf("\n");
}
