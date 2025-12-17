#ifndef __RTC_H__
#define __RTC_H__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <linux/i2c-dev.h>
    #include <stdint.h>
    #include <time.h>
    #include <sys/time.h>

    #define NBR_TIME_ELEMENT    6
    #define MODUL_ADDR          0x50
    #define TIME_REGISTER_ADDR  0X02
    #define MSG_CONF_SIZE       2
    #define TIME_MSG_SIZE       NBR_TIME_ELEMENT
    union RtcValue {
        uint8_t m_uint8_val;
        struct
        {
            uint8_t m_uint4_unite   : 4;
            uint8_t m_uint4_dizaine : 4;
        };
    };

    union TimeVal
    {
        uint8_t m_aRtcValue_Tab[NBR_TIME_ELEMENT];
        struct
        {
            uint8_t m_uint8_addr;
            union RtcValue m_RtcValue_sec;
            union RtcValue m_RtcValue_min;
            union RtcValue m_RtcValue_hour;
            union RtcValue m_RtcValue_mDay;
            union RtcValue m_RtcValue_mon;
        };
    };
    
//     1 si le fichier du periférique I2C n'a pas pu aitre ouvert,
//  * 2 si l'esclave n'a pas pu aitre ransangner,
//  * 3 si il y a u une erreur lors de l'envois de la requet ou
//  * 4 si il y a u une erreur lors de la réseption des valeurs
    enum ERREUR{OPEN_FILE_ERROR, SET_SLAVE_ERROR, SEND_VALUE_ERROR, RECEIVE_VALUE_ERROR, NO_ERROR};

    uint8_t InitRTC(void);
    union TimeVal GetTimeRTC(void);
    void SetTimeRTC(union TimeVal x_TimeVal_SetVal);

#endif