#ifndef __DATETIME__
#define __DATETIME__

#include <string>

#include "Types.h"
#include "UtilitySettings.h"

namespace FreeWorldEngine {

struct UTILITY_DLL Time {
	Time(uint8 h = 0, uint8 m = 0, uint8 s = 0);
	
	std::string toString() const;
	static Time current();

	uint8 hours, minutes, seconds;
};

struct UTILITY_DLL Date {
	enum Month { Jan, Feb, Mar, Apr, May, June, July, Aug, Sept, Oct, Nov, Dec };

	// d >= 1, m = [Jan..Dec], y >= 0
	Date(uint8 d = 29, Month m = Nov, uint16 y = 1992);
	
	std::string toString() const;
	static Date current();

	uint16 year;
	Month month;
	uint8 day;
};

struct UTILITY_DLL DateTime {
	DateTime(const Time t = Time(), const Date& d = Date());
	
	std::string toString() const;
	static DateTime current();

	Date date;
	Time time;
};

} // namespace

#endif // __DATETIME__