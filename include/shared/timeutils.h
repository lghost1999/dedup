#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <stdint.h>
#include <string>

class TimesUtil
{
public:
    // 获取当前系统时间(精确到纳秒）tv_nsec精确到纳秒（编译时加上-lrt）
	static int64_t getTimeNs();

    // 获取当前系统时间(精度微秒）tv_usec精确到微秒
	static int64_t getTimeUs();
	
    // 获取当前系统时间(精度毫秒）tv_usec精确到微秒
	static int64_t getTimeMs();
	
    // 获取当前系统时间(精度秒)，YY年 mm月 dd日 HH时 MM分 SS秒
	static std::string getDateTime();
};


#endif
