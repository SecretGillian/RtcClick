#ifndef __MAIN_H__
#define __MAIN_H__
    #define MIN                 60
    #define MAX_TIME_NTP_SYNC   1*MIN //temps maximum en seconde depuit la derniere synchronisation NTP
    #define MAX_SIZE_DEBUG_FILE 1024*15 //taille maximale du fichier de debug en octets
    #define TimePatern          "%d/%d  %d:%d|"
    #define Time(TimeVar)       TimeVar.m_RtcValue_mDay.m_uint8_val, \
                                TimeVar.m_RtcValue_mon.m_uint8_val, \
                                TimeVar.m_RtcValue_hour.m_uint8_val, \
                                TimeVar.m_RtcValue_min.m_uint8_val
#endif