#include <stdio.h>
#define len 64

int main()
{
    int index = 7;
    int arr[len] = {0};
    int nxt_index = 0;
    for (int i = 0; i < len; i++){
        nxt_index = (index + (int)((1/((double)2))*i + (1/((double)2))*(i*i)))%len;
        arr[nxt_index] = 1;
    }
    for (int i = 0; i < len; i++){
        printf("%d, ", arr[i]);
    }
    printf("\n");
}