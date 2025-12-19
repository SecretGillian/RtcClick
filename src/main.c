#include "../include/main.h"
#include "../include/RTC.h"
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

int main(void)
{
    union TimeVal l_TimeVal_ValRTC    = {.m_aRtcValue_Tab = {0}};
    time_t l_time_TimeVal           = 0;
    struct tm l_pTm_SetTemps;
    struct timeval l_timeval_SetTime;

    int l_int_return                = 0;

    uint8_t l_uint8_Test            = 0;

    FILE* l_PtrFILE_DebugFile       = NULL;
    const char l_str_FilePath[]     = "/home/user/Sys/DebugInitSysClock.txt";
    const char l_str_mode[]         = "w";

    /**/ //ouverture du fichier de debug
    do
    {
        l_PtrFILE_DebugFile = fopen(l_str_FilePath, l_str_mode);

        l_uint8_Test++;
    } while (l_PtrFILE_DebugFile == NULL && l_uint8_Test <= 200);

    if(l_PtrFILE_DebugFile == NULL)
    {        
        return 1;
    }
    /**/
    
    // printf("Test de connexion vers %s:%d...\n", l_str_host, l_uint8_port);

    //test la connexion 
    fprintf(l_PtrFILE_DebugFile, "debut de l'initialisation par la RTC:\n");

    //initialisation de la RTC
    l_int_return = InitRTC();

    switch (l_int_return)
    {
        case OPEN_FILE_ERROR:
            fprintf(l_PtrFILE_DebugFile, "Erreur ouverture du bus I2C. fonction InitRTC\n");
            fclose(l_PtrFILE_DebugFile);
            return 2;
        break;
        
        case SET_SLAVE_ERROR:
            fprintf(l_PtrFILE_DebugFile, "Erreur sélection périphérique I2C. fonction InitRTC\n");
            fclose(l_PtrFILE_DebugFile);
            return 3;
        break;

        case SEND_VALUE_ERROR:
            fprintf(l_PtrFILE_DebugFile, "Erreur écriture I2C. fonction InitRTC\n");
            fclose(l_PtrFILE_DebugFile);
            return 4;
        break;

        default:
        break;
    }
    
    /**/ //récupération des valeur de la RTC
    l_TimeVal_ValRTC = GetTimeRTC();

    if(l_TimeVal_ValRTC.m_aRtcValue_Tab[0] == 0)
    {
        switch (l_TimeVal_ValRTC.m_aRtcValue_Tab[1])
        {
            case OPEN_FILE_ERROR:
                fprintf(l_PtrFILE_DebugFile, "Erreur ouverture du bus I2C. fonction GetTimeRTC\n");
                fclose(l_PtrFILE_DebugFile);
                return 2;
            break;
            
            case SET_SLAVE_ERROR:
                fprintf(l_PtrFILE_DebugFile, "Erreur sélection périphérique I2C. fonction GetTimeRTC\n");
                fclose(l_PtrFILE_DebugFile);
                return 3;
            break;

            case SEND_VALUE_ERROR:
                fprintf(l_PtrFILE_DebugFile, "Erreur écriture I2C. fonction GetTimeRTC\n");
                fclose(l_PtrFILE_DebugFile);
                return 4;
            break;

            case RECEIVE_VALUE_ERROR:
                fprintf(l_PtrFILE_DebugFile, "Erreur lecture I2C. fonction GetTimeRTC\n");
                fclose(l_PtrFILE_DebugFile);
                return 5;
            break;

            default:
            break;
        }
    }
    /**/
    
    /**/ //initialise la structure de temps
    l_pTm_SetTemps.tm_sec   = l_TimeVal_ValRTC.m_RtcValue_sec.m_uint8_val;
    l_pTm_SetTemps.tm_min   = l_TimeVal_ValRTC.m_RtcValue_min.m_uint8_val;
    l_pTm_SetTemps.tm_hour  = l_TimeVal_ValRTC.m_RtcValue_hour.m_uint8_val;
    l_pTm_SetTemps.tm_mday  = l_TimeVal_ValRTC.m_RtcValue_mDay.m_uint8_val;
    l_pTm_SetTemps.tm_mon   = l_TimeVal_ValRTC.m_RtcValue_mon.m_uint8_val - 1;
    l_pTm_SetTemps.tm_isdst = -1;
    l_pTm_SetTemps.tm_wday  = 0;
    l_pTm_SetTemps.tm_yday  = 0;
    l_pTm_SetTemps.tm_year  = 2025 - 1900;
    /**/

    //converti la structure dans le bon format (nombre de seconde écoulées depuis le 1er janvier 1970)
    l_time_TimeVal = mktime(&l_pTm_SetTemps);

    /**/ //regle l'orloge de la Raspberry
    l_timeval_SetTime.tv_sec = l_time_TimeVal;
    l_timeval_SetTime.tv_usec = 0;
    l_int_return = settimeofday(&l_timeval_SetTime,NULL);
    
    if ( l_int_return < 0 ) {  
        fprintf(l_PtrFILE_DebugFile, "l'heur n'a pas pu aitre reglé. erreur : \"%s\"\n", strerror( errno ) );
        fclose(l_PtrFILE_DebugFile);
        return 6;
    }
    /**/

    fclose(l_PtrFILE_DebugFile);
    return 0;
    
}