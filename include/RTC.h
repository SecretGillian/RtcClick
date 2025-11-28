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

    #define NbrTimeElement 5

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
        uint8_t m_RtcValue_Tab[NbrTimeElement];
        struct
        {
            union RtcValue m_RtcValue_sec;
            union RtcValue m_RtcValue_min;
            union RtcValue m_RtcValue_hour;
            union RtcValue m_RtcValue_mDay;
            union RtcValue m_RtcValue_mon;
        };
    };
    

    void InitRTC(void);
    
#endif