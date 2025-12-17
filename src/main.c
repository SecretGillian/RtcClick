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

    const char l_str_host[]         = "8.8.8.8";
    uint8_t l_uint8_port            = 53;
    int l_int_sock                  = 0;
    struct sockaddr_in l_StockaddrIn_server;
    struct timeval l_timeval_timeout;
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
        fprintf(l_PtrFILE_DebugFile, "écheque de l'ouverture du fichier de debug\n");
        
        return 1;
    }
    /**/

    /**/ //création d'un socket réseau
    do
    {
        l_int_sock = socket(AF_INET, SOCK_STREAM, 0);

        l_uint8_Test++;
    }while (l_int_sock < 0 && l_uint8_Test <= 200);

    l_uint8_Test = 0;
    
    if (l_int_sock < 0) {
        fprintf(l_PtrFILE_DebugFile, "écheque de la création du socket réseau");
        fclose(l_PtrFILE_DebugFile);
        return 2;
    }
    /**/

    /**/ //initialisation du time out pour connect
    l_timeval_timeout.tv_sec = TIME_OUT_CONNECT;
    l_timeval_timeout.tv_usec = 0;

    setsockopt(l_int_sock, SOL_SOCKET, SO_SNDTIMEO, &l_timeval_timeout, sizeof(l_timeval_timeout));
    /**/

    /**/ //initialisation de la strucutre peretant d'effectuer le pig
    l_StockaddrIn_server.sin_family = AF_INET;
    l_StockaddrIn_server.sin_port = htons(l_uint8_port);
    inet_pton(AF_INET, l_str_host, &l_StockaddrIn_server.sin_addr);
    /**/

    // printf("Test de connexion vers %s:%d...\n", l_str_host, l_uint8_port);

    //test la connexion 
    if (connect(l_int_sock, (struct sockaddr*)&l_StockaddrIn_server, sizeof(l_StockaddrIn_server)) == 0) {
        fprintf(l_PtrFILE_DebugFile, "réseau fonctionnel.\n");
        close(l_int_sock);
        fclose(l_PtrFILE_DebugFile);
        return 0;
    } else {
        fprintf(l_PtrFILE_DebugFile, "réseau non fonctionel. debut de l'initialisation par la RTC:\n");

        //initialisation de la RTC
        l_int_return = InitRTC();

        switch (l_int_return)
        {
            case OPEN_FILE_ERROR:
                fprintf(l_PtrFILE_DebugFile, "Erreur ouverture du bus I2C. fonction InitRTC\n");
                close(l_int_sock);
                fclose(l_PtrFILE_DebugFile);
            break;
            
            case SET_SLAVE_ERROR:
                fprintf(l_PtrFILE_DebugFile, "Erreur sélection périphérique I2C. fonction InitRTC\n");
                close(l_int_sock);
                fclose(l_PtrFILE_DebugFile);
            break;

            case SEND_VALUE_ERROR:
                fprintf(l_PtrFILE_DebugFile, "Erreur écriture I2C. fonction InitRTC\n");
                close(l_int_sock);
                fclose(l_PtrFILE_DebugFile);
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
                    close(l_int_sock);
                    fclose(l_PtrFILE_DebugFile);
                break;
                
                case SET_SLAVE_ERROR:
                    fprintf(l_PtrFILE_DebugFile, "Erreur sélection périphérique I2C. fonction GetTimeRTC\n");
                    close(l_int_sock);
                    fclose(l_PtrFILE_DebugFile);
                break;

                case SEND_VALUE_ERROR:
                    fprintf(l_PtrFILE_DebugFile, "Erreur écriture I2C. fonction GetTimeRTC\n");
                    close(l_int_sock);
                    fclose(l_PtrFILE_DebugFile);
                break;

                case RECEIVE_VALUE_ERROR:
                    fprintf(l_PtrFILE_DebugFile, "Erreur lecture I2C. fonction GetTimeRTC\n");
                    close(l_int_sock);
                    fclose(l_PtrFILE_DebugFile);
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
            close(l_int_sock);
            fclose(l_PtrFILE_DebugFile);
            return 3;
        }
        /**/

        close(l_int_sock);
        fclose(l_PtrFILE_DebugFile);
        return 0;
    }
}