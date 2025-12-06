#include "../include/RTC.h"

void InitRTC(void)
{
    uint8_t l_aUint8_cfg[MSG_CONF_SIZE] = {0};
    int l_int_file                      = 0;
    const char *l_str_FileName          = "/dev/i2c-1";
    int l_int_addr                      = MODUL_ADDR; // ⚠️ Adresse I2C de ton périphérique
    int8_t l_int8_return                = 0;
    
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
    l_int8_return = write(l_int_file, l_aUint8_cfg, MSG_CONF_SIZE);
    if (l_int8_return != MSG_CONF_SIZE) {
        perror("Erreur écriture I2C");
    }
}

union TimeVal GetTimeRTC(void)
{
    int l_int_file                  = 0;
    const char *l_str_FileName      = "/dev/i2c-1";
    int l_int_addr                  = MODUL_ADDR; // ⚠️ Adresse I2C de ton périphérique
    int8_t l_int8_return            = 0;
    union TimeVal l_TimeVal_TimeVal = {.m_aRtcValue_Tab = {0}};

    l_TimeVal_TimeVal.m_aRtcValue_Tab[0] = TIME_REGISTER_ADDR;

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
    l_int8_return = write(l_int_file, l_TimeVal_TimeVal.m_aRtcValue_Tab, 1);
    if (l_int8_return != 1) {
        perror("Erreur écriture I2C");
    }

    l_int8_return = read(l_int_file, &l_TimeVal_TimeVal.m_aRtcValue_Tab[1], TIME_MSG_SIZE - 1);
    if  (l_int8_return != TIME_MSG_SIZE)
    {
        perror("Erreur lecture I2C");
        for(uint8_t i = 0 ; i < NBR_TIME_ELEMENT ; i++)
        {
            l_TimeVal_TimeVal.m_aRtcValue_Tab[i] = 0;
        }
        return l_TimeVal_TimeVal;
    }else
    {
        return l_TimeVal_TimeVal;
    }
}

void SetTimeRTC(union TimeVal x_TimeVal_SetVal)
{
    int l_int_file                  = 0;
    const char *l_str_FileName      = "/dev/i2c-1";
    int l_int_addr                  = MODUL_ADDR; // ⚠️ Adresse I2C de ton périphérique
    int8_t l_int8_return            = 0;
    uint8_t l_uint8_buffer = 0;

    x_TimeVal_SetVal.m_uint8_addr = TIME_REGISTER_ADDR;

    l_uint8_buffer                                  = x_TimeVal_SetVal.m_RtcValue_sec.m_uint8_val;
    x_TimeVal_SetVal.m_RtcValue_sec.m_uint4_unite   = l_uint8_buffer % 10;
    x_TimeVal_SetVal.m_RtcValue_sec.m_uint4_dizaine = l_uint8_buffer / 10;

    l_uint8_buffer                                  = x_TimeVal_SetVal.m_RtcValue_min.m_uint8_val;
    x_TimeVal_SetVal.m_RtcValue_min.m_uint4_unite   = l_uint8_buffer % 10;
    x_TimeVal_SetVal.m_RtcValue_min.m_uint4_dizaine = l_uint8_buffer / 10;

    l_uint8_buffer                                      = x_TimeVal_SetVal.m_RtcValue_hour.m_uint8_val;
    x_TimeVal_SetVal.m_RtcValue_hour.m_uint4_unite      = l_uint8_buffer % 10;
    x_TimeVal_SetVal.m_RtcValue_hour.m_uint4_dizaine    = l_uint8_buffer / 10;

    l_uint8_buffer                                      = x_TimeVal_SetVal.m_RtcValue_mDay.m_uint8_val;
    x_TimeVal_SetVal.m_RtcValue_mDay.m_uint4_unite      = l_uint8_buffer % 10;
    x_TimeVal_SetVal.m_RtcValue_mDay.m_uint4_dizaine    = l_uint8_buffer / 10;

    l_uint8_buffer                                  = x_TimeVal_SetVal.m_RtcValue_mon.m_uint8_val;
    x_TimeVal_SetVal.m_RtcValue_mon.m_uint4_unite   = l_uint8_buffer % 10;
    x_TimeVal_SetVal.m_RtcValue_mon.m_uint4_dizaine = l_uint8_buffer / 10;

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
    l_int8_return = write(l_int_file, x_TimeVal_SetVal.m_aRtcValue_Tab, NBR_TIME_ELEMENT);
    if (l_int8_return != TIME_MSG_SIZE) {
        perror("Erreur écriture I2C");
    }
}