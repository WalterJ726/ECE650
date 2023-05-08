#include <stdlib.h>
#include <stdio.h>
#include "my_malloc.h"

#ifdef FF
#define MALLOC(sz) ff_malloc(sz)
#define FREE(p)    ff_free(p)
#endif
#ifdef BF
#define MALLOC(sz) bf_malloc(sz)
#define FREE(p)    bf_free(p)
#endif


int main(int argc, char *argv[])
{
    void *a = MALLOC(50);
    FREE(a);
    void* b = MALLOC(10);
    void* c = MALLOC(20);
    FREE(b);
    FREE(c);
  return 0;
}
