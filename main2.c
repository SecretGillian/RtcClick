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
#include <errno.h>

int main() {
    const char *path = "/home/user/test_i2c/test.txt"/* "/run/systemd/timesync/synchronized" */;

    if (unlink(path) == 0) {
        printf("Fichier supprimé avec succès\n");
    } else {
        if (errno == ENOENT) {
            printf("Le fichier n'existe pas\n");
        } else if (errno == EACCES || errno == EPERM) {
            perror("Permission refusée");
        } else {
            perror("Erreur unlink");
        }
    }

    return 0;
}
