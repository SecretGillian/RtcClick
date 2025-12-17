#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>

int main() {
    FILE* l_PtrFile_file = fopen("Text.txt", "w");

    fprintf(l_PtrFile_file, "hello test 1 ligne 1\n");
    fprintf(l_PtrFile_file, "hello test 1 ligne 2\n");

    fclose(l_PtrFile_file);
}
