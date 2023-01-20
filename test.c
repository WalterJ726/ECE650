#include <stdio.h>
#include <unistd.h>

int main(){
    printf("first sbrk: %p\n", sbrk(0));
    printf("second sbrk: %p\n", sbrk(5));
    printf("third sbrk: %p\n", sbrk(0));
    printf("forth sbrk: %p\n", sbrk(5));
    printf("fifth sbrk: %p\n", sbrk(0));
    return 0;
}
