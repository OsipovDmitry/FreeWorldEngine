#include <ctime>

#include "DateTime.h"

namespace FreeWorldEngine {

Time::Time(uint8 h, uint8 m, uint8 s) : hours(h), minutes(m), seconds(s)
{
}

std::string Time::toString() const
{
	std::string sHours = ((hours < 10) ? "0" : "") + std::to_string((_ULonglong)(hours));
	std::string sMinutes = ((minutes < 10) ? "0" : "") + std::to_string((_ULonglong)(minutes));
	std::string sSeconds = ((seconds < 10) ? "0" : "") + std::to_string((_ULonglong)(seconds));
	return sHours + ":" + sMinutes + ":" + sSeconds;
}

Time Time::current()
{
	time_t current_time = std::time(0);
	std::tm *t = localtime(&current_time);
	return Time(t->tm_hour, t->tm_min, t->tm_sec);
}

Date::Date(uint8 d, Month m, uint16 y) : day(d-1), month(m), year(y)
{
}

std::string Date::toString() const
{ 
	std::string sDay = ((day+1 < 10) ? "0" : "") + std::to_string((_ULonglong)(day+1));
	std::string sMonth = ((month-Jan+1 < 10) ? "0" : "") + std::to_string((_ULonglong)(month-Jan+1));
	return sDay + "." + sMonth + "." + std::to_string((_ULonglong)year);
}

Date Date::current()
{
	time_t current_time = std::time(0);
	std::tm *t = localtime(&current_time);
	return Date(t->tm_mday, (Date::Month)t->tm_mon, t->tm_year+1900);
}

DateTime::DateTime(const Time t, const Date& d) : date(d), time(t)
{
}

std::string DateTime::toString() const
{
	return date.toString() + " " + time.toString();
}

DateTime DateTime::current()
{
	return DateTime(Time::current(), Date::current());
}

}