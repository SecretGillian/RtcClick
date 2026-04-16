#include "../include/main.h"
#include "../include/RTC.h"
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>


int main(void)
{
    // union TimeVal l_TimeVal_SetRtcVal   = {.m_aRtcValue_Tab = {0}};
    time_t l_time_TimeVal               = 0;
    // struct tm* l_pTm_temps;
    struct stat st;
    const char l_str_PosTimFille[]      = "test.txt";
    FILE* l_PtrFILE_TimeFile            = NULL;

    char l_str_Buffer[BUFFER_SIZE]       = {0};

    struct tm l_tm_SetTemps;

    int l_int_RtcFile                   = 0;
    const char l_str_RtcSysFile[]       = "/dev/rtc0";
    struct rtc_time l_RtcTime_val;
    // time_t l_time_RtcTimeVal            = 0;
    struct timeval l_timeval_SetTime;

    uint32_t l_uint32_delta             = 0;
    uint8_t l_uint8_Test                = 0;

    FILE* l_PtrFILE_DebugFile           = NULL;
    const char l_str_PosDebugFile[]         = "DebugUpdateTime.txt";
    const char l_str_mode1[]            = "w";//ouverture en écriture (écrase le fichier existant)
    const char l_str_mode2[]            = "a";//ouverture en ajout (ajoute à la fin du fichier)
    char const* l_str_mode              = l_str_mode2;

    union TimeVal l_TimeVal_GetRtcVal    = {.m_aRtcValue_Tab = {0}};


    // /**/ //récupération de l'heur de la RTC
    // l_TimeVal_GetRtcVal = GetTimeRTC();
    // /**/

    printf("verification de la taille du fichier de debug\n");
    /**/    //vérifie la taille du fichier de debug pour choisir le mode d'ouverture
    if(stat(l_str_PosDebugFile, &st) == 0)
    {
        if(st.st_size >= MAX_SIZE_DEBUG_FILE)
        {
            l_str_mode = (char*)l_str_mode1;
        }
    }
    /**/
    l_str_mode = (char*)l_str_mode1;

    printf("Ouverture du fichier de debug\n");
    /**/ //ouverture du fichier de debug
    do
    {
        l_PtrFILE_DebugFile = fopen(l_str_PosDebugFile, l_str_mode);

        l_uint8_Test++;
    } while (l_PtrFILE_DebugFile == NULL && l_uint8_Test <= 200);

    if(l_PtrFILE_DebugFile == NULL)
    {
        return 1;
    }
    /**/

    fprintf(l_PtrFILE_DebugFile, "\n\n");

    printf("Ouverture du fichier de la RTC\n");
    /**/ //ouverture du fichier de la RTC
    l_uint8_Test = 0;
    do
    {
        l_int_RtcFile = open(l_str_RtcSysFile, O_RDWR);

        l_uint8_Test++;
    } while (l_PtrFILE_DebugFile == NULL && l_uint8_Test <= 200);
    /**/

    /**/  //vérifie que le fichier de la RTC a pu être ouvert
    if(l_int_RtcFile < 0)
    {
        fprintf(l_PtrFILE_DebugFile, "Erreur ouverture du fichier de la RTC: %s\n", strerror(errno));
        fclose(l_PtrFILE_DebugFile);
        return 2;
    }
    /**/

    /**/ //ouverture du fichier de mise à jour de l'heure
    do
    {
        l_PtrFILE_TimeFile = fopen(l_str_PosTimFille, "r");

        l_uint8_Test++;
    } while (l_PtrFILE_TimeFile == NULL && l_uint8_Test <= 200);

    if(l_PtrFILE_TimeFile == NULL)
    {
        perror("fopen");
        return 1;
    }
    /**/

    printf("Lecture de l'heure de la RTC\n");
    /**/ //récupération de l'heur de la RTC
    if(ioctl(l_int_RtcFile, RTC_RD_TIME, &l_RtcTime_val) < 0)
    {
        fprintf(l_PtrFILE_DebugFile, "Erreur lecture de l'heure de la RTC: %s\n", strerror(errno));
        close(l_int_RtcFile);
        fclose(l_PtrFILE_DebugFile);
        return 3;
    }
    /**/

    printf("Heure de la RTC:"TIME_PATTERN"\n", TIME(l_RtcTime_val));

    printf("Vérification de l'existence du fichier de mise à jour de l'heure\n");
    /**/ //verification de l'existence du fichier de validation de synchronisation NTP
    if (stat(l_str_PosTimFille, &st) != 0) {
        // perror("stat");
        fprintf(l_PtrFILE_DebugFile, TIME_PATTERN "Erreur stat: %s\n", TIME(l_RtcTime_val), strerror(errno));
        fclose(l_PtrFILE_DebugFile);
        return 4;
    }
    /**/

    /**/ // récupération de l'heure système
    l_time_TimeVal = time(NULL);
    l_uint32_delta = (uint32_t)difftime(l_time_TimeVal, st.st_mtime);
    /**/

    fprintf(l_PtrFILE_DebugFile, TimePatern "Delta temps depuis dernière synchro : %u minute %u secondes\n", Time(l_TimeVal_GetRtcVal), l_uint32_delta / 60, l_uint32_delta % 60);
    
    printf("Delta temps depuis dernière synchro : %u minute %u secondes\n", l_uint32_delta / 60, l_uint32_delta % 60);
    
    printf("test //////////////////////////////////////////////////////\n");

    if (l_uint32_delta < MAX_TIME_NTP_SYNC) {
        fprintf(l_PtrFILE_DebugFile, TIME_PATTERN "Synchronisation de temps récente\n", TIME(l_RtcTime_val));
        /**/ //recupération de l'heure dans le fichier
        while (fgets(l_str_Buffer, BUFFER_SIZE, l_PtrFILE_TimeFile) != NULL);
        sscanf(l_str_Buffer, "%d/%d/%d %d:%d:%d", &l_RtcTime_val.tm_mday, &l_RtcTime_val.tm_mon, &l_RtcTime_val.tm_year, &l_RtcTime_val.tm_hour, &l_RtcTime_val.tm_min, &l_RtcTime_val.tm_sec);
        /**/

        printf("Heure lue dans le fichier:"TIME_PATTERN"\n", TIME(l_RtcTime_val));

        /**/ //réglage de l'offset pour leurs
        l_RtcTime_val.tm_year -= 1900;
        l_RtcTime_val.tm_mon -= 1;
        /**/

        /**/ //initialisation de la structure servant à régler l'heure système
        l_tm_SetTemps.tm_mday  = l_RtcTime_val.tm_mday;
        l_tm_SetTemps.tm_mon   = l_RtcTime_val.tm_mon;
        l_tm_SetTemps.tm_year  = l_RtcTime_val.tm_year;
        l_tm_SetTemps.tm_hour  = l_RtcTime_val.tm_hour;
        l_tm_SetTemps.tm_min   = l_RtcTime_val.tm_min;
        l_tm_SetTemps.tm_sec   = l_RtcTime_val.tm_sec;
        l_tm_SetTemps.tm_isdst = -1;
        l_tm_SetTemps.tm_wday  = 0;
        l_tm_SetTemps.tm_yday  = 0;
        /**/

        /**/    //réglage de l'offset pour leurs de la RTC
            l_RtcTime_val.tm_hour -= 1;
        /**/

        /**/ //conversion et envoi de l'heure système
        l_time_TimeVal = mktime(&l_tm_SetTemps);

        l_timeval_SetTime.tv_sec = l_time_TimeVal;
        l_timeval_SetTime.tv_usec = 0;

        if(settimeofday(&l_timeval_SetTime,NULL) < 0)
        {
            fprintf(l_PtrFILE_DebugFile, TIME_PATTERN "Erreur réglage de l'heure système: %s\n", TIME(l_RtcTime_val), strerror(errno));
            fclose(l_PtrFILE_DebugFile);
            return 9;
        }
        /**/

        // if (ioctl(fd, RTC_SET_TIME, &rtc) == -1) {
        // perror("ioctl RTC_SET_TIME");
        // close(fd);
        // return 1;
        // }

        /**/ //envoi de l'heure à la RTC
        if(ioctl(l_int_RtcFile, RTC_SET_TIME, &l_RtcTime_val) < 0)
        {
            fprintf(l_PtrFILE_DebugFile, TIME_PATTERN "Erreur réglage de l'heure de la RTC: %s\n", TIME(l_RtcTime_val), strerror(errno));
            close(l_int_RtcFile);
            fclose(l_PtrFILE_DebugFile);
            return 10;
        }
        /**/

        // /**/ //initialisation de la structure servant à régler l'heure de la RTC
        // l_TimeVal_SetRtcVal.m_RtcValue_mDay.m_uint8_val  = l_pTm_temps->tm_mday;
        // l_TimeVal_SetRtcVal.m_RtcValue_mon.m_uint8_val   = l_pTm_temps->tm_mon + 1;
        // l_TimeVal_SetRtcVal.m_RtcValue_hour.m_uint8_val  = l_pTm_temps->tm_hour;
        // l_TimeVal_SetRtcVal.m_RtcValue_min.m_uint8_val   = l_pTm_temps->tm_min;
        // l_TimeVal_SetRtcVal.m_RtcValue_sec.m_uint8_val   = l_pTm_temps->tm_sec;
        // /**/

        //réglage de l'heure de la RTC
        // SetTimeRTC(l_TimeVal_SetRtcVal);

        /**/ // suppression du fichier de validation de synchronisation NTP
        if (unlink(l_str_PosTimFille) != 0) {
            if (errno == ENOENT) {
                fprintf(l_PtrFILE_DebugFile, TIME_PATTERN "Le fichier n'existe pas\n", TIME(l_RtcTime_val));
                fclose(l_PtrFILE_DebugFile);
                return 5;
            } else if (errno == EACCES || errno == EPERM) {
                fprintf(l_PtrFILE_DebugFile, TIME_PATTERN "Permission refusée: %s\n" , TIME(l_RtcTime_val), strerror(errno));
                fclose(l_PtrFILE_DebugFile);
                return 6;
            } else {
                fprintf(l_PtrFILE_DebugFile, TIME_PATTERN "Erreur unlink: %s\n", TIME(l_RtcTime_val), strerror(errno));
                fclose(l_PtrFILE_DebugFile);
                return 7;
            }
        }/**/

        fprintf(l_PtrFILE_DebugFile, TIME_PATTERN "Fichier supprimé avec succès\n", TIME(l_RtcTime_val));

        printf("test1");

        fclose(l_PtrFILE_DebugFile);
        return 0;
    } else {
        fprintf(l_PtrFILE_DebugFile, TIME_PATTERN "Synchronisation ancienne ou perdue\n", TIME(l_RtcTime_val));
        fclose(l_PtrFILE_DebugFile);
        return 8;
    }
    
}