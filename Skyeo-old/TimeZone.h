#ifndef TIMEZONE_H
#define TIMEZONE_H

#include <Arduino.h>
#include <time.h>

// ============================================
// SKYEO - Timezone Management
// Mit deutscher Zeit (CET/CEST automatisch)
// ============================================

class TimeZoneManager {
public:
    static void begin();
    static void setTimezone(const char* tz);
    static String getCurrentTimezone();
    
    // Zeit holen
    static int getHour();
    static int getMinute();
    static int getSecond();
    static int getDayOfWeek();  // 0=Sonntag, 1=Montag, ..., 6=Samstag
    static int getDay();
    static int getMonth();
    static int getYear();
    
    // Formatierte Zeit
    static String getFormattedTime();
    static String getFormattedDate();
    static String getFormattedDateTime();
    
    // Zeit setzen (von NTP oder manuell)
    static void setTime(time_t timestamp);
    static time_t getTimestamp();
    
    // Prüfen ob Sommerzeit aktiv ist
    static bool isDST();
    
private:
    static const char* currentTZ;
    static bool initialized;
};

// Deutsche Zeitzone: CET-1CEST,M3.5.0,M10.5.0/3
// Format: https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
#define TZ_EUROPE_BERLIN "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_UTC "UTC0"

#endif
