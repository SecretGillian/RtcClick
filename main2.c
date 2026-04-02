/* #include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int main() {
    struct stat st;
    const char *file = "/run/systemd/timesync/synchronized";

    if (stat(file, &st) != 0) {
        perror("stat");
        return 1;
    }

    time_t now = time(NULL);
    double delta = difftime(now, st.st_mtime);

    printf("Dernière mise à jour : %.0f secondes\n", delta);

    if (delta < 300) {
        printf("Synchronisation NTP récente\n");
    } else {
        printf("Synchronisation NTP ancienne ou perdue\n");
    }

    return 0;
} */

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 22

int main() {
    const char *path = "test.txt"/* "/run/systemd/timesync/synchronized" */;
    FILE* l_PtrFILE_TimeFile    = NULL;

    uint8_t l_uint8_Test        = 0;

    // struct stat st;
    char l_str_Buffer[BUFFER_SIZE]       = {0};
    char l_str_Date[BUFFER_SIZE]         = {0};
    

    do
    {
        l_PtrFILE_TimeFile = fopen(path, "w");

        l_uint8_Test++;
    } while (l_PtrFILE_TimeFile == NULL && l_uint8_Test <= 200);

    if(l_PtrFILE_TimeFile == NULL)
    {
        perror("fopen");
        return 1;
    }

    fprintf(l_PtrFILE_TimeFile, "27/03/2026 14:46:56\n");
    fprintf(l_PtrFILE_TimeFile, "27/03/2026 15:46:56\n");
    fprintf(l_PtrFILE_TimeFile, "27/03/2026 16:46:56\n");
    fprintf(l_PtrFILE_TimeFile, "27/03/2026 17:46:56\n");
    fclose(l_PtrFILE_TimeFile);

    do
    {
        l_PtrFILE_TimeFile = fopen(path, "r");

        l_uint8_Test++;
    } while (l_PtrFILE_TimeFile == NULL && l_uint8_Test <= 200);

    if(l_PtrFILE_TimeFile == NULL)
    {
        perror("fopen");
        return 1;
    }


    while (fgets(l_str_Buffer, BUFFER_SIZE, l_PtrFILE_TimeFile) != NULL);
    
    // printf("test\n");
    // printf("Ligne lue : %s\n\r", l_str_Buffer);
    strcpy(l_str_Date, l_str_Buffer);
    printf("Ligne copiée : %s\n\r", l_str_Date);
    
    
    printf("Dernière ligne lue : %s\n", l_str_Date);

    return 0;
}
