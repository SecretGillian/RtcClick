#include "../include/main.h"
#include "../include/RTC.h"

int main(void)
{
    union TimeVal l_TimeVal_Test = {.m_aRtcValue_Tab = {0}};
    time_t l_time_TimeVal = 0;
    struct tm* l_pTm_temps;

    // InitRTC();
    
    // l_TimeVal_Test = GetTimeRTC();
    // printf("on est le %x/%x/2025 et il est %x:%x:%x\n", l_TimeVal_Test.m_RtcValue_mDay, l_TimeVal_Test.m_RtcValue_mon, l_TimeVal_Test.m_RtcValue_hour, l_TimeVal_Test.m_RtcValue_min, l_TimeVal_Test.m_RtcValue_sec);

    l_time_TimeVal  = time(NULL);
    l_pTm_temps     = localtime(&l_time_TimeVal);

    l_TimeVal_Test.m_RtcValue_mDay.m_uint8_val = l_pTm_temps->tm_mday;
    l_TimeVal_Test.m_RtcValue_mon.m_uint8_val = l_pTm_temps->tm_mon;
    l_TimeVal_Test.m_RtcValue_hour.m_uint8_val = l_pTm_temps->tm_hour;
    l_TimeVal_Test.m_RtcValue_min.m_uint8_val = l_pTm_temps->tm_min;
    l_TimeVal_Test.m_RtcValue_sec.m_uint8_val = l_pTm_temps->tm_sec;

    SetTimeRTC(l_TimeVal_Test);
    // printf("on est le %d/%d/2025 et il est %d:%d:%d\n", l_pTm_temps->tm_mday, l_pTm_temps->tm_mon, l_pTm_temps->tm_hour, l_pTm_temps->tm_min, l_pTm_temps->tm_sec);
}