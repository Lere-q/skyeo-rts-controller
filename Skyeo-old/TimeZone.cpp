#include "TimeZone.h"
#include <Preferences.h>

// ============================================
// SKYEO - Timezone Management Implementation
// ============================================

const char* TimeZoneManager::currentTZ = TZ_EUROPE_BERLIN;
bool TimeZoneManager::initialized = false;

void TimeZoneManager::begin() {
    if (initialized) return;
    
    // Lade gespeicherte Zeitzone
    Preferences prefs;
    prefs.begin("skyeo", true);
    String savedTZ = prefs.getString("timezone", TZ_EUROPE_BERLIN);
    prefs.end();
    
    currentTZ = strdup(savedTZ.c_str());
    
    // Zeitzone setzen
    setenv("TZ", currentTZ, 1);
    tzset();
    
    // Zeit auf 2025-01-01 00:00:00 setzen (als Basis)
    // In einer vollständigen Implementierung würde hier NTP verwendet
    struct tm timeinfo = {0};
    timeinfo.tm_year = 2025 - 1900;  // Jahre seit 1900
    timeinfo.tm_mon = 0;              // Januar
    timeinfo.tm_mday = 1;             // 1.
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1;
    
    time_t t = mktime(&timeinfo);
    timeval tv = { t, 0 };
    settimeofday(&tv, nullptr);
    
    initialized = true;
    
    Serial.printf("[SKYEO] Zeitzone gesetzt: %s\n", currentTZ);
    Serial.printf("[SKYEO] Aktuelle Zeit: %s\n", getFormattedDateTime().c_str());
}

void TimeZoneManager::setTimezone(const char* tz) {
    currentTZ = strdup(tz);
    setenv("TZ", currentTZ, 1);
    tzset();
    
    // Speichern
    Preferences prefs;
    prefs.begin("skyeo", false);
    prefs.putString("timezone", currentTZ);
    prefs.end();
    
    Serial.printf("[SKYEO] Zeitzone geändert zu: %s\n", currentTZ);
}

String TimeZoneManager::getCurrentTimezone() {
    return String(currentTZ);
}

int TimeZoneManager::getHour() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return 0;
    return timeinfo.tm_hour;
}

int TimeZoneManager::getMinute() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return 0;
    return timeinfo.tm_min;
}

int TimeZoneManager::getSecond() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return 0;
    return timeinfo.tm_sec;
}

int TimeZoneManager::getDayOfWeek() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return 0;
    return timeinfo.tm_wday;
}

int TimeZoneManager::getDay() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return 1;
    return timeinfo.tm_mday;
}

int TimeZoneManager::getMonth() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return 1;
    return timeinfo.tm_mon + 1;  // tm_mon ist 0-basiert
}

int TimeZoneManager::getYear() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return 2025;
    return timeinfo.tm_year + 1900;
}

String TimeZoneManager::getFormattedTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return "00:00:00";
    
    char buf[9];
    strftime(buf, sizeof(buf), "%H:%M:%S", &timeinfo);
    return String(buf);
}

String TimeZoneManager::getFormattedDate() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return "01.01.2025";
    
    char buf[11];
    strftime(buf, sizeof(buf), "%d.%m.%Y", &timeinfo);
    return String(buf);
}

String TimeZoneManager::getFormattedDateTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return "01.01.2025 00:00:00";
    
    char buf[20];
    strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", &timeinfo);
    return String(buf);
}

void TimeZoneManager::setTime(time_t timestamp) {
    timeval tv = { timestamp, 0 };
    settimeofday(&tv, nullptr);
}

time_t TimeZoneManager::getTimestamp() {
    return time(nullptr);
}

bool TimeZoneManager::isDST() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return false;
    return timeinfo.tm_isdst > 0;
}
