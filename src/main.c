#include "../include/main.h"
#include "../include/RTC.h"
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>

int main(void)
{
    union TimeVal l_TimeVal_SetRtcVal   = {.m_aRtcValue_Tab = {0}};
    time_t l_time_TimeVal               = 0;
    struct tm* l_pTm_temps;
    struct stat st;
    const char file[]                   = "/run/systemd/timesync/synchronized";

    uint32_t l_uint32_delta             = 0;
    uint8_t l_uint8_Test                = 0;

    FILE* l_PtrFILE_DebugFile           = NULL;
    const char l_str_FilePath[]         = "/home/user/Sys/DebugUpdateRTC.txt";
    const char l_str_mode1[]            = "w";//ouverture en écriture (écrase le fichier existant)
    const char l_str_mode2[]            = "a";//ouverture en ajout (ajoute à la fin du fichier)
    char const* l_str_mode              = l_str_mode2;

    union TimeVal l_TimeVal_GetRtcVal    = {.m_aRtcValue_Tab = {0}};


    /**/ //récupération de l'heur de la RTC
    l_TimeVal_GetRtcVal = GetTimeRTC();
    /**/

    /**/    //vérifie la taille du fichier de debug pour choisir le mode d'ouverture
    if(stat(l_str_FilePath, &st) == 0)
    {
        if(st.st_size >= MAX_SIZE_DEBUG_FILE)
        {
            l_str_mode = (char*)l_str_mode1;
        }
    }
    /**/

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

    fprintf(l_PtrFILE_DebugFile, "\n\n");

    if (stat(file, &st) != 0) {
        // perror("stat");
        fprintf(l_PtrFILE_DebugFile, TimePatern "Erreur stat: %s\n", Time(l_TimeVal_GetRtcVal), strerror(errno));
        fclose(l_PtrFILE_DebugFile);
        return 2;
    }

    /**/ // récupération de l'heure système
    l_time_TimeVal = time(NULL);
    l_uint32_delta = (uint32_t)difftime(l_time_TimeVal, st.st_mtime);
    /**/

    fprintf(l_PtrFILE_DebugFile, TimePatern "Delta temps depuis dernière synchro NTP: %u minute %u secondes\n", Time(l_TimeVal_GetRtcVal), l_uint32_delta / 60, l_uint32_delta % 60);

    if (l_uint32_delta < MAX_TIME_NTP_SYNC) {
        fprintf(l_PtrFILE_DebugFile, TimePatern "Synchronisation NTP récente\n", Time(l_TimeVal_GetRtcVal));

        /**/ // récupération de l'heure système
        l_pTm_temps = localtime(&l_time_TimeVal);
        /**/

        /**/ //initialisation de la structure servant à régler l'heure de la RTC
        l_TimeVal_SetRtcVal.m_RtcValue_mDay.m_uint8_val  = l_pTm_temps->tm_mday;
        l_TimeVal_SetRtcVal.m_RtcValue_mon.m_uint8_val   = l_pTm_temps->tm_mon + 1;
        l_TimeVal_SetRtcVal.m_RtcValue_hour.m_uint8_val  = l_pTm_temps->tm_hour;
        l_TimeVal_SetRtcVal.m_RtcValue_min.m_uint8_val   = l_pTm_temps->tm_min;
        l_TimeVal_SetRtcVal.m_RtcValue_sec.m_uint8_val   = l_pTm_temps->tm_sec;
        /**/

        //réglage de l'heure de la RTC
        SetTimeRTC(l_TimeVal_SetRtcVal);

        /**/ // suppression du fichier de validation de synchronisation NTP
        if (unlink(file) != 0) {
            if (errno == ENOENT) {
                fprintf(l_PtrFILE_DebugFile, TimePatern "Le fichier n'existe pas\n", Time(l_TimeVal_GetRtcVal));
                fclose(l_PtrFILE_DebugFile);
                return 3;
            } else if (errno == EACCES || errno == EPERM) {
                fprintf(l_PtrFILE_DebugFile, TimePatern "Permission refusée: %s\n" , Time(l_TimeVal_GetRtcVal), strerror(errno));
                fclose(l_PtrFILE_DebugFile);
                return 4;
            } else {
                fprintf(l_PtrFILE_DebugFile, TimePatern "Erreur unlink: %s\n", Time(l_TimeVal_GetRtcVal), strerror(errno));
                fclose(l_PtrFILE_DebugFile);
                return 5;
            }
        }/**/

        fprintf(l_PtrFILE_DebugFile, TimePatern "Fichier supprimé avec succès\n", Time(l_TimeVal_GetRtcVal));

        fclose(l_PtrFILE_DebugFile);
        return 0;
    } else {
        fprintf(l_PtrFILE_DebugFile, TimePatern "Synchronisation NTP ancienne ou perdue\n", Time(l_TimeVal_GetRtcVal));
        fclose(l_PtrFILE_DebugFile);
        return 6;
    }
    
}