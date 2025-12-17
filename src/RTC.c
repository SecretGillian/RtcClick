#include "../include/RTC.h"


/**
 * @brief fonciton permetant d'initialiser la RTC
 * 
 * @return NO_ERROR si il n'y a pas u de broblème, 
 * OPEN_FILE_ERROR si le fichier du periférique I2C n'a pas pu aitre ouvert, 
 * SET_SLAVE_ERROR si l'esclave n'a pas pu aitre ransangner ou 
 * SEND_VALUE_ERROR si il y a u une erreur lors de l'envois des paramètre
 */
uint8_t InitRTC(void)
{
    uint8_t l_aUint8_cfg[MSG_CONF_SIZE] = {0};          //tableau de valeur à envoyer sur la liaison I2C
    int l_int_file                      = 0;            //variable peretant de stocker l'id du périphérique ouvert
    const char *l_str_FileName          = "/dev/i2c-1"; // chemain vers le fichier de controle du périphérique I2C
    int l_int_addr                      = MODUL_ADDR;   // Adresse I2C de de l'esclave
    int8_t l_int8_return                = 0;            //valeur stockant les valeur retourner par le fonction de gestion de périphérique
    
    l_aUint8_cfg[1] |= (1 << 3);
    
    l_int_file = open(l_str_FileName, O_RDWR); //ouvre le fichier I2C
    if (l_int_file < 0) {
        return OPEN_FILE_ERROR;
    }
    
    l_int8_return = ioctl(l_int_file, I2C_SLAVE, l_int_addr); //ransaigne l'adresse I2C de l'esclave
    if (l_int8_return < 0) {
        close(l_int_file);
        return SET_SLAVE_ERROR;
    }
    
    l_int8_return = write(l_int_file, l_aUint8_cfg, MSG_CONF_SIZE); //envoi le tableau de configuration sur la laison I2C
    if (l_int8_return != MSG_CONF_SIZE) {
        close(l_int_file);
        return SEND_VALUE_ERROR;
    }

    close(l_int_file);
    return NO_ERROR;
}

/**
 * @brief fonction permetant de récupéré l'heur et la date depui la RTC
 * 
 * @return retourne la strucutre contenant les différantes valeur retourné par la RTC 
 * ou si la case 0 du membre m_aRtcValue_Tab est égale à 0 alor la case 1 est égale à
 * OPEN_FILE_ERROR si le fichier du periférique I2C n'a pas pu aitre ouvert,
 * SET_SLAVE_ERROR si l'esclave n'a pas pu aitre ransangner,
 * SEND_VALUE_ERROR si il y a u une erreur lors de l'envois de la requet ou
 * RECEIVE_VALUE_ERROR si il y a u une erreur lors de la réseption des valeurs
 */
union TimeVal GetTimeRTC(void)
{
    int l_int_file                  = 0;                        //variable peretant de stocker l'id du périphérique ouvert
    const char *l_str_FileName      = "/dev/i2c-1";             //chemain vers le fichier de controle du périphérique I2C
    int l_int_addr                  = MODUL_ADDR;               //Adresse I2C de ton périphérique
    int8_t l_int8_return            = 0;                        //valeur stockant les valeur retourner par le fonction de gestion de périphérique
    union TimeVal l_TimeVal_TimeVal = {.m_aRtcValue_Tab = {0}}; //tructure permetant de stocker les valeur renvoyer par la RTC 

    l_TimeVal_TimeVal.m_uint8_addr  = TIME_REGISTER_ADDR; //ransaigne l'adresse du regitre de la RTC

    l_int_file = open(l_str_FileName, O_RDWR); //ouvre le fichier I2C
    if (l_int_file < 0) {
        l_TimeVal_TimeVal.m_aRtcValue_Tab[0] = 0;
        l_TimeVal_TimeVal.m_aRtcValue_Tab[1] = OPEN_FILE_ERROR;
        return l_TimeVal_TimeVal;
    }
    
    l_int8_return = ioctl(l_int_file, I2C_SLAVE, l_int_addr); //ransaigne l'adresse I2C de l'esclave
    if (l_int8_return < 0) {
        close(l_int_file);
        l_TimeVal_TimeVal.m_aRtcValue_Tab[0] = 0;
        l_TimeVal_TimeVal.m_aRtcValue_Tab[1] = SET_SLAVE_ERROR;
        return l_TimeVal_TimeVal;
    }
    
    l_int8_return = write(l_int_file, l_TimeVal_TimeVal.m_aRtcValue_Tab, 1); //envois de la requet pour que la RTC renvois la date et l'heur
    if (l_int8_return != 1) {
        close(l_int_file);
        l_TimeVal_TimeVal.m_aRtcValue_Tab[0] = 0;
        l_TimeVal_TimeVal.m_aRtcValue_Tab[1] = SEND_VALUE_ERROR;
        return l_TimeVal_TimeVal;
    }

    l_int8_return = read(l_int_file, &l_TimeVal_TimeVal.m_aRtcValue_Tab[1], TIME_MSG_SIZE - 1); //lecture des valeur renvoyer par la RTC
    if  (l_int8_return != TIME_MSG_SIZE - 1)
    {
        close(l_int_file);
        l_TimeVal_TimeVal.m_aRtcValue_Tab[0] = 0;
        l_TimeVal_TimeVal.m_aRtcValue_Tab[1] = RECEIVE_VALUE_ERROR;
        return l_TimeVal_TimeVal;
    }else
    {
        l_TimeVal_TimeVal.m_RtcValue_sec.m_uint8_val = l_TimeVal_TimeVal.m_RtcValue_sec.m_uint4_unite + l_TimeVal_TimeVal.m_RtcValue_sec.m_uint4_dizaine * 10;
        l_TimeVal_TimeVal.m_RtcValue_min.m_uint8_val = l_TimeVal_TimeVal.m_RtcValue_min.m_uint4_unite + l_TimeVal_TimeVal.m_RtcValue_min.m_uint4_dizaine * 10;
        l_TimeVal_TimeVal.m_RtcValue_hour.m_uint8_val = l_TimeVal_TimeVal.m_RtcValue_hour.m_uint4_unite + l_TimeVal_TimeVal.m_RtcValue_hour.m_uint4_dizaine * 10;
        l_TimeVal_TimeVal.m_RtcValue_mDay.m_uint8_val = l_TimeVal_TimeVal.m_RtcValue_mDay.m_uint4_unite + l_TimeVal_TimeVal.m_RtcValue_mDay.m_uint4_dizaine * 10;
        l_TimeVal_TimeVal.m_RtcValue_mon.m_uint8_val = l_TimeVal_TimeVal.m_RtcValue_mon.m_uint4_unite + l_TimeVal_TimeVal.m_RtcValue_mon.m_uint4_dizaine * 10;
        
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

    /**/ //met les différante valeur donner par l'utilisateur au format utiliser par la RTC
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
    /**/

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