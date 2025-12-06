#include "../include/RTC.h"


/**
 * @bief fonciton permetant d'initialiser la RTC
 */
void InitRTC(void)
{
    uint8_t l_aUint8_cfg[MSG_CONF_SIZE] = {0};          //tableau de valeur à envoyer sur la liaison I2C
    int l_int_file                      = 0;            //variable peretant de stocker l'id du périphérique ouvert
    const char *l_str_FileName          = "/dev/i2c-1"; // chemain vers le fichier de controle du périphérique I2C
    int l_int_addr                      = MODUL_ADDR;   // Adresse I2C de de l'esclave
    int8_t l_int8_return                = 0;            //valeur stockant les valeur retourner par le fonction de gestion de périphérique
    
    l_aUint8_cfg[1] |= (1 << 3);
    
    l_int_file = open(l_str_FileName, O_RDWR); //ouvre le fichier I2C
    if (l_int_file < 0) {
        perror("Erreur ouverture du bus I2C");
        exit(1);
    }
    
    l_int8_return = ioctl(l_int_file, I2C_SLAVE, l_int_addr); //ransaigne l'adresse I2C de l'esclave
    if (l_int8_return < 0) {
        perror("Erreur sélection périphérique I2C");
        close(l_int_file);
        exit(1);
    }
    
    l_int8_return = write(l_int_file, l_aUint8_cfg, MSG_CONF_SIZE); //envoi le tableau de configuration sur la laison I2C
    if (l_int8_return != MSG_CONF_SIZE) {
        perror("Erreur écriture I2C");
    }
}

/**
 * @brief fonction permetant de récupéré l'heur et la date depui la RTC
 * 
 * @return retourne la strucutre contenant les différantes valeur retourné par la RTC
 */
union TimeVal GetTimeRTC(void)
{
    int l_int_file                  = 0;                        //variable peretant de stocker l'id du périphérique ouvert
    const char *l_str_FileName      = "/dev/i2c-1";             //chemain vers le fichier de controle du périphérique I2C
    int l_int_addr                  = MODUL_ADDR;               //Adresse I2C de ton périphérique
    int8_t l_int8_return            = 0;                        //valeur stockant les valeur retourner par le fonction de gestion de périphérique
    union TimeVal l_TimeVal_TimeVal = {.m_aRtcValue_Tab = {0}}; //tructure permetant de stocker les valeur renvoyer par la RTC 

    l_TimeVal_TimeVal.m_aRtcValue_Tab[0] = TIME_REGISTER_ADDR; //ransaigne l'adresse du regitre de la RTC

    l_int_file = open(l_str_FileName, O_RDWR); //ouvre le fichier I2C
    if (l_int_file < 0) {
        perror("Erreur ouverture du bus I2C");
        exit(1);
    }
    
    l_int8_return = ioctl(l_int_file, I2C_SLAVE, l_int_addr); //ransaigne l'adresse I2C de l'esclave
    if (l_int8_return < 0) {
        perror("Erreur sélection périphérique I2C");
        close(l_int_file);
        exit(1);
    }
    
    l_int8_return = write(l_int_file, l_TimeVal_TimeVal.m_aRtcValue_Tab, 1); //envois de la requet pour que la RTC renvois la date et l'heur
    if (l_int8_return != 1) {
        perror("Erreur écriture I2C");
    }

    l_int8_return = read(l_int_file, &l_TimeVal_TimeVal.m_aRtcValue_Tab[1], TIME_MSG_SIZE - 1); //lecture des valeur renvoyer par la RTC
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

/**
 * @brief fonction permetant de changer la date et l'heur de la RTC
 * 
 * @param x_TimeVal_SetVal est la strucutre contenant les nouvel valeur avec les quel on souette raigler la RTC
 */
void SetTimeRTC(union TimeVal x_TimeVal_SetVal)
{
    int l_int_file                  = 0;            //variable peretant de stocker l'id du périphérique ouvert
    const char *l_str_FileName      = "/dev/i2c-1"; //chemain vers le fichier de controle du périphérique I2C
    int l_int_addr                  = MODUL_ADDR;   //Adresse I2C de ton périphérique
    int8_t l_int8_return            = 0;            //valeur stockant les valeur retourner par le fonction de gestion de périphérique
    uint8_t l_uint8_buffer = 0;

    x_TimeVal_SetVal.m_uint8_addr = TIME_REGISTER_ADDR; //ransaigne l'adresse du regitre de la RTC

    //met les différante valeur donner par l'utilisateur au format utiliser par la RTC
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

    l_int_file = open(l_str_FileName, O_RDWR); //ouvre le fichier I2C
    if (l_int_file < 0) {
        perror("Erreur ouverture du bus I2C");
        exit(1);
    }
    
    l_int8_return = ioctl(l_int_file, I2C_SLAVE, l_int_addr); //ransaigne l'adresse I2C de l'esclave
    if (l_int8_return < 0) {
        perror("Erreur sélection périphérique I2C");
        close(l_int_file);
        exit(1);
    }
    
    l_int8_return = write(l_int_file, x_TimeVal_SetVal.m_aRtcValue_Tab, NBR_TIME_ELEMENT); //envois à la RTC la trame pour changer la date et l'heur
    if (l_int8_return != TIME_MSG_SIZE) {
        perror("Erreur écriture I2C");
    }
}