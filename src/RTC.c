#include "../include/RTC.h"

void InitRTC(void)
{
    uint8_t l_aUint8_cfg[2]     = {0};
    int l_int_file              = 0;
    const char *l_str_FileName  = "/dev/i2c-1";
    int l_int_addr              = MODUL_ADDR; // ⚠️ Adresse I2C de ton périphérique
    int8_t l_int8_return        = 0;
    
    l_aUint8_cfg[1] |= (1 << 3);
    
    // 1️⃣ Ouvrir le bus I2C
    l_int_file = open(l_str_FileName, O_RDWR);
    if (l_int_file < 0) {
        perror("Erreur ouverture du bus I2C");
        exit(1);
    }
    
    // 2️⃣ Sélectionner le périphérique I2C
    l_int8_return = ioctl(l_int_file, I2C_SLAVE, l_int_addr);
    if (l_int8_return < 0) {
        perror("Erreur sélection périphérique I2C");
        close(l_int_file);
        exit(1);
    }
    
    // 4️⃣ Envoyer la trame complète (registre + chaîne)
    l_int8_return = write(l_int_file, l_aUint8_cfg, 2);
    if (l_int8_return != 2) {
        perror("Erreur écriture I2C");
    }
}

// union TimeVal GetTimeRTC(void)
// {
//     int l_int_file              = 0;
//     const char *l_str_FileName  = "/dev/i2c-1";
//     int l_int_addr              = MODUL_ADDR; // ⚠️ Adresse I2C de ton périphérique
//     int8_t l_int8_return        = 0;
//     uint8_t l_aUint8_TimeVal    = {0};

//     l_aUint8_TimeVal[0] = 0x02;

//     // 1️⃣ Ouvrir le bus I2C
//     l_int_file = open(l_str_FileName, O_RDWR);
//     if (l_int_file < 0) {
//         perror("Erreur ouverture du bus I2C");
//         exit(1);
//     }
    
//     // 2️⃣ Sélectionner le périphérique I2C
//     l_int8_return = ioctl(l_int_file, I2C_SLAVE, l_int_addr);
//     if (l_int8_return < 0) {
//         perror("Erreur sélection périphérique I2C");
//         close(l_int_file);
//         exit(1);
//     }
    
//     // 4️⃣ Envoyer la trame complète (registre + chaîne)
//     l_int8_return = write(l_int_file, &l_uint8_cfg, 1);
//     if (l_int8_return != 1) {
//         perror("Erreur écriture I2C");
//     }
// }