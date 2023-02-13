#include "timeutils.h"

#include <ctime>
#include <sys/time.h> 

int64_t TimesUtil::getTimeNs() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

    
int64_t TimesUtil::getTimeUs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1e6 + tv.tv_usec;
}
	

int64_t TimesUtil::getTimeMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1e3 + tv.tv_usec / 1e3;
}



std::string TimesUtil::getDateTime() {
    time_t now = time(0);
    tm  *ltm = localtime(&now);

    char myDate[40], myTime[40];
    strftime(myDate, 40, "%Y-%m-%d", ltm);
    strftime(myTime, 40, "-%H:%M:%S", ltm);
    
    return std::string(myDate) + std::string(myTime);
}
