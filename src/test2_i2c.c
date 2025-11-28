#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c %c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
    ((byte) & 0x80 ? '1' : '0'), \
    ((byte) & 0x40 ? '1' : '0'), \
    ((byte) & 0x20 ? '1' : '0'), \
    ((byte) & 0x10 ? '1' : '0'), \
    ((byte) & 0x08 ? '1' : '0'), \
    ((byte) & 0x04 ? '1' : '0'), \
    ((byte) & 0x02 ? '1' : '0'), \
    ((byte) & 0x01 ? '1' : '0')

int main() {
    int file;
    const char *filename = "/dev/i2c-1";
    int addr = 0x50; // ⚠️ Adresse I2C de ton périphérique

    // 1️⃣ Ouvrir le bus I2C
    printf("__________connection au ficher\n");
    if ((file = open(filename, O_RDWR)) < 0) {
        perror("Erreur ouverture du bus I2C");
        exit(1);
    }

    // 2️⃣ Sélectionner le périphérique I2C
    printf("__________set esclave\n");
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        perror("Erreur sélection périphérique I2C");
        close(file);
        exit(1);
    }

    // 3️⃣ Créer la chaîne à envoyer
    // const char message[] = "HELLO I2C!";
    unsigned char buffer[32] = {0}; // taille max du message
    // int len = strlen(message);

    // Optionnel : si ton périphérique attend un "registre" avant la data
    buffer[0] = 0x02; // registre de départ
    // memcpy(&buffer[1], message, len); // copie la chaîne à partir de buffer[1]

    // 4️⃣ Envoyer la trame complète (registre + chaîne)
    printf("__________envoi\n");
    int count = write(file, buffer, 1);
    if (count != 1) {
        perror("Erreur écriture I2C");
    } else {
        printf("Message envoyé : \"%s\" (%d octets)\n", buffer, 1);
    }

    printf("__________reception\n");
    count = read(file, buffer, 4);
    if (count != 4) {
        perror("Erreur écriture I2C");
    } else {
        for(uint8_t i = 0 ; i < 32 ; i++)
        {
            printf("valeur ressu case %2d : "BYTE_TO_BINARY_PATTERN" | %02X \n", i, BYTE_TO_BINARY(buffer[i]), buffer[i]);
        }
    }

    close(file);
    return 0;
}
