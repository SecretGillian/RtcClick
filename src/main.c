#include "../include/main.h"
#include "../include/RTC.h"

int main(void)
{
    union TimeVal l_TimeVal_Test = {.m_aRtcValue_Tab = {0}};

    // InitRTC();
    
    l_TimeVal_Test = GetTimeRTC();
    printf("on est le %x/%x/2025 et il est %x:%x:%x\n", l_TimeVal_Test.m_RtcValue_mDay, l_TimeVal_Test.m_RtcValue_mon, l_TimeVal_Test.m_RtcValue_hour, l_TimeVal_Test.m_RtcValue_min, l_TimeVal_Test.m_RtcValue_sec);
}