#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }
    
    const char *IP = argv[1];
    const int PORT = atoi(argv[2]);

    fprintf();
}
