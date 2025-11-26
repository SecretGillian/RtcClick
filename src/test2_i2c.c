#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int main() {
    int file;
    const char *filename = "/dev/i2c-1";
    int addr = 0x08; // ⚠️ Adresse I2C de ton périphérique

    // 1️⃣ Ouvrir le bus I2C
    if ((file = open(filename, O_RDWR)) < 0) {
        perror("Erreur ouverture du bus I2C");
        exit(1);
    }

    // 2️⃣ Sélectionner le périphérique I2C
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        perror("Erreur sélection périphérique I2C");
        close(file);
        exit(1);
    }

    // 3️⃣ Créer la chaîne à envoyer
    const char message[] = "HELLO I2C!";
    unsigned char buffer[32]; // taille max du message
    int len = strlen(message);

    // Optionnel : si ton périphérique attend un "registre" avant la data
    buffer[0] = 0x00; // registre de départ
    memcpy(&buffer[1], message, len); // copie la chaîne à partir de buffer[1]

    // 4️⃣ Envoyer la trame complète (registre + chaîne)
    int count = write(file, buffer, len + 1);
    if (count != len + 1) {
        perror("Erreur écriture I2C");
    } else {
        printf("Message envoyé : \"%s\" (%d octets)\n", message, len);
    }

    close(file);
    return 0;
}
