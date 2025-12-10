#include "../include/main.h"
#include "../include/RTC.h"
#include <errno.h>

int main(void)
{
    union TimeVal l_TimeVal_Test = {.m_aRtcValue_Tab = {0}};
    time_t l_time_TimeVal   = 0;
    struct tm* l_pTm_temps  = NULL;
    struct tm l_pTm_SetTemps;
    struct timeval l_timeval_SetTime;
    int l_int_return        = 0;

    InitRTC();
    
    l_TimeVal_Test = GetTimeRTC();
    printf("on est le %d/%d/2025 et il est %d:%d:%d\n", l_TimeVal_Test.m_RtcValue_mDay.m_uint8_val, l_TimeVal_Test.m_RtcValue_mon.m_uint8_val, l_TimeVal_Test.m_RtcValue_hour.m_uint8_val, l_TimeVal_Test.m_RtcValue_min.m_uint8_val, l_TimeVal_Test.m_RtcValue_sec.m_uint8_val);

    l_pTm_SetTemps.tm_sec   = l_TimeVal_Test.m_RtcValue_sec.m_uint8_val;
    l_pTm_SetTemps.tm_min   = l_TimeVal_Test.m_RtcValue_min.m_uint8_val;
    l_pTm_SetTemps.tm_hour  = l_TimeVal_Test.m_RtcValue_hour.m_uint8_val;
    l_pTm_SetTemps.tm_mday  = l_TimeVal_Test.m_RtcValue_mDay.m_uint8_val;
    l_pTm_SetTemps.tm_mon   = l_TimeVal_Test.m_RtcValue_mon.m_uint8_val - 1;
    l_pTm_SetTemps.tm_isdst = -1;
    l_pTm_SetTemps.tm_wday  = 0;
    l_pTm_SetTemps.tm_yday  = 0;
    l_pTm_SetTemps.tm_year  = 2025 - 1900;

    printf("mktime \n");
    l_time_TimeVal = mktime(&l_pTm_SetTemps);
      
    /* Now set the clock to this time */
    l_timeval_SetTime.tv_sec = l_time_TimeVal;
    l_timeval_SetTime.tv_usec = 0;

    printf("settimeofday taille int : %d \n", sizeof(int));
    l_int_return = settimeofday(&l_timeval_SetTime,NULL);

    /* Check that the change was successful */
    if ( l_int_return < 0 ) {  
        printf("Unable to change the system time. Did you run the program as an administrator?\n");
        printf("The operation returned the error message \"%s\"\n", strerror( errno ) );
        exit (-1);
    }


    // l_time_TimeVal  = time(NULL);
    // l_pTm_temps     = localtime(&l_time_TimeVal);

    // l_TimeVal_Test.m_RtcValue_mDay.m_uint8_val = l_pTm_temps->tm_mday;
    // l_TimeVal_Test.m_RtcValue_mon.m_uint8_val = l_pTm_temps->tm_mon;
    // printf("mois : %d\n", l_pTm_temps->tm_mon);
    // l_TimeVal_Test.m_RtcValue_hour.m_uint8_val = l_pTm_temps->tm_hour;
    // l_TimeVal_Test.m_RtcValue_min.m_uint8_val = l_pTm_temps->tm_min;
    // l_TimeVal_Test.m_RtcValue_sec.m_uint8_val = l_pTm_temps->tm_sec;

    // SetTimeRTC(l_TimeVal_Test);
    // printf("on est le %d/%d/2025 et il est %d:%d:%d\n", l_pTm_temps->tm_mday, l_pTm_temps->tm_mon, l_pTm_temps->tm_hour, l_pTm_temps->tm_min, l_pTm_temps->tm_sec);
}