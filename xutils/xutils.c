/*
 * xutils.c
 *
 * Created by Jackie Xie on 2011-08-20.
 * Copyright 2011 Jackie Xie. All rights reserved.
 *
 */

#include <xutils.h>
#include <errno.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#ifdef CHK_MEMLEAK
void * debug_malloc(size_t size, const char *file, int line, const char *func)
{
	void *p;

	p = malloc(size);
	printf("%s:%d:%s:malloc(%ld): p=0x%lx\n",  file, line, func, size, (unsigned long)p);
	return p;
}

#define malloc(s) debug_malloc(s, __FILE__, __LINE__, __func__)
#define free(p)  do {						   \
	printf("%s:%d:%s:free(0x%lx)\n", __FILE__, __LINE__,	    \
	    __func__, (unsigned long)p);				\
	free(p);							\
} while (0)
#endif

int years[13] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool isLeapYear (int year)
{
	if(((year % 4==0) && (year%100 != 0)) || (year % 400 == 0))
	   return true;
	else
		return false;	
}

bool IsLeapYear(int year)	
{					
	if(year%4==0) //必須能被4整除	
	{	
		 if(year%100==0)	
		 {	
				if(year%400==0)
					return	true;//如果以00結尾,還要能被400整除			 
				else
					return false;		
		 }
		 else
		 	return true;		
	}
	else
		return false;	
}	

/* ======================(function header)========================
Function Name : Days get_day_code (int year){}
Description : The codes are: day_code (0 = Sun, 1 = Mon, etc.).
Arguments : a year as a decimal number with century.
Return : an user defined type 'Days' value.
written by Jackie Xie
Date : 2011/08/28
================================================================*/
Days get_day_code (int year)
{
	int day_code;
	int x1, x2, x3;
	x1 = (year - 1.)/ 4.0;
	x2 = (year - 1.)/ 100.;
	x3 = (year - 1.)/ 400.;
	day_code = (year + x1 - x2 + x3) %7;
	return day_code;
}

/* ======================(function header)========================
Function Name : int dayOfMonth(datetime dt){}
Description : To convert the day(Sun...Sat) of week(1~5) of 
					month(1~12) into the day of the month as a decimal 
					number(01-31).
Arguments : a user defined type 'datetime' dt.
Return : an interger value which range is from 1 to 31.
written by Jackie Xie
Date : 2011/08/28
================================================================*/
int dayOfMonth(datetime dt)
{
	int days, month;
	Days day_code;

	if(isLeapYear(dt.year))
		years[February] = 29;
	else
		years[February] = 28;
	
	//printf("February have %d days in %d\n", years[February], dt.year);
	
	day_code = get_day_code(dt.year);
	for ( month = 1; month < dt.month; month++ )
	{
		/* set day_code for next month to begin */
		day_code = ( day_code + years[month] ) % 7;	
	}
	
	days = (dt.day >= day_code)?(dt.day - day_code + 1)+7*(dt.week-1): (7 - day_code + 1) + dt.day + 7*(dt.week-1);
	
	while(days > years[dt.month])
		days -= 7;
	
	return days;
}

/* ======================(function header)========================
Function Name :t ime_t str2datetime(char *datetime){}
Description : To Convert the string of datetime format into an interger 
					which unit is a time 'second'.
Arguments : char * tz
Return : an integer of type 'time_t '
written by Jackie Xie
Date : 2011/08/29
================================================================*/
time_t str2datetime(char *datetime)
{
	int year, month, day, hour, min, sec;
	struct tm *when;
	time_t tme;
	char tmp_time[32];
	strcpy(tmp_time, datetime);

	sscanf(tmp_time, "%4d-%02d-%02dT%02d:%02d:%02dZ", &year, &month, &day, &hour, &min, &sec);

	time(&tme);
	when = localtime(&tme);
	when->tm_year = year - 1900;
	when->tm_mon = month-1;
	when->tm_mday = day;
	when->tm_hour = hour;
	when->tm_min = min;
	when->tm_sec = sec;

	return(mktime(when));
}

/*
 * Convert a date to days since 1970.
 * written by jackiexie
 * Date : 2012/02/08
 */
int date2jds(char* datetime)
{
	time_t when;
	struct tm	tm;
	char	*end;

	memset(&tm, 0, sizeof(tm));
	end = strptime(datetime, "%Y-%m-%d", &tm);
	if (end == NULL || *end) {
		fprintf(stderr, "could not parse: %s\n", datetime);
		return 0;
	}

	when = mktime(&tm) / 86400;
	//printf("%u\n", (unsigned) (when));
	return when;
}

/*
 * Convert days since 1970 to a date.
 * written by jackiexie
 * Date : 2012/02/08
 */
int jds2date(int days, char* datetime)
{
	time_t when;
	struct tm	*whentm;
	char	res[64];

	when = days * 24 * 60 * 60;
	if ((whentm = localtime(&when)) == NULL) {
		fprintf(stderr, "localtime: %s\n", strerror(errno));
		return -1;
	}
#if 0
	strftime(res, sizeof(res) - 1, "%A, %d %B %Y", whentm);
	res[sizeof(res) - 1] = '\0';
	printf("%s\n", res);
	sprintf(datetime, "%s", res);
#endif
	strftime(res, sizeof(res) - 1, "%Y-%m-%d", whentm);
	res[sizeof(res) - 1] = '\0';
	//printf("%s\n", res);
	sprintf(datetime, "%s", res);

	return 0;
}

/*
 * Convert [J]<days> to M<month>,<week of month>,<week day> format.
 * written by jackiexie
 * Date : 2012/02/08
 */
int jds2mds(int days, char* dst)
{
	time_t when;
	struct tm	*whentm;
	char	month[5], DayOfMonth[4], weekday[4];
	int WeekOfMonth;

	when = days * 24 * 60 * 60;
	if ((whentm = localtime(&when)) == NULL) {
		fprintf(stderr, "localtime: %s\n", strerror(errno));
		return -1;
	}

	strftime(month, sizeof(month) - 1, "M%m", whentm);
	month[sizeof(month) - 1] = '\0';

	strftime(DayOfMonth, sizeof(DayOfMonth) - 1, "%d", whentm);
	DayOfMonth[sizeof(DayOfMonth) - 1] = '\0';
	/* To find current 'week of month'(by 'day of month'(DayOfMonth) */
	WeekOfMonth = ceil(atof(DayOfMonth) / 7);

	strftime(weekday, sizeof(weekday) - 1, "%w", whentm);
	weekday[sizeof(weekday) - 1] = '\0';

	//printf("%s.%d.%s\n", month, WeekOfMonth, weekday);
	sprintf(dst, "%s.%d.%s", month, WeekOfMonth, weekday);

	return 0;
}

/*
 * Convert days to a date format.
 * written by jackiexie
 * Date : 2012/02/08
 */
int days2date(int Days, char* dst)
{
	time_t baseTime;
	struct tm* pLocalTime=NULL;
	char dt[20];
	int jdays = 0;

	baseTime = time(NULL);
	pLocalTime = localtime(&baseTime);
	pLocalTime->tm_year += 1900;

	sprintf(dt, "%d-1-1", pLocalTime->tm_year);
	jdays = Days + date2jds(dt);
	jdays = (isLeapYear(pLocalTime->tm_year))? jdays+1:jdays;
	jds2date(jdays, dst);
	//return jds2mds(jdays, dst);
	return jdays;
}

/*
 * Convert date to a days format.
 * written by jackiexie
 * Date : 2012/02/08
 */
 int date2days(char* date)
 {
	time_t when;
	struct tm	tm;
	char	*end, dt[20], *year, *month, *day;
	int days = 0, day_of_month = 0;

	year = StrDup(date);
	month = StrDup(date);
	day  = StrDup(date);
	strcutail(year, "-", -1);
	GetStrBetweenStr(month, "-", "-");
	strmhead(day, "-", -2);
	day_of_month = atoi(day);
	sprintf(dt, "%s-1-1", year);
	days = date2jds(dt);
	memset(&tm, 0, sizeof(tm));
	end = strptime(date, "%Y-%m-%d", &tm);
	if (end == NULL || *end || (!strcmp(month, "2") && 
		((day_of_month >= 29 && !isLeapYear(atoi(year))) || 
			(day_of_month > 29 && isLeapYear(atoi(year)))))) {
		fprintf(stderr, "could not parse: %s\n", date);
		return -1;
	}

	when = mktime(&tm) / 86400 - days + 1;
	//when = (!isLeapYear(atoi(year)) && when >= 59)? ++when: when;
	if(isLeapYear(atoi(year))) --when;
	printf("date2days = %u\n", (unsigned) (when));
	StrFree(year);

	return when;
 }

/* ======================(function header)========================
Function Name:int getTZ(char **tz){}
Description : To find the time difference[Coordinated Universal Time
			(UTC)] between GMT and Local time zone.
Arguments : char ** tz
Return : a string with format "[+|-]hh:mm"
written by jackiexie
Date : 2011/06/02
Example :
	int main(int argc, char** argv)
	{
		char *tz;
	
		getTZ(&tz);
		printf("UTC%s", tz);
	
		return 0;
	}
================================================================*/
int getTZ(char **tz)
{
	struct tm *tptr;
	char UTC[20];
	time_t secs,l_secs,gmt_secs;
	float utc;
	char *min, *sign;
	int tmp, res = 0;
	
	time(&secs);
	tptr = localtime(&secs);
	l_secs = mktime(tptr);
	tptr = gmtime(&secs);
	gmt_secs = mktime(tptr);
	utc = l_secs - gmt_secs;
	
	utc = utc/3600.0;
	//utc = -9.9;//-2.5;-3.5;-4.5;-9.5;-12;-10;5.75;8.75;12.75;13.75;14

	if(utc < 0)
		sign = StrDup("-");
	else if(utc > 0)
		sign = StrDup("+");

	tmp = utc * 60;
	tmp = abs(tmp) % 60;
	
	if(tmp == 30)
		min = StrDup("30");
	else if(tmp == 45)
		min = StrDup("45");
	else if(tmp != 0)
		min = StrDup(itos(tmp));
	else
		min = StrDup("00");

	tmp = (int)abs(utc);
	if(tmp <= 14 && tmp >= 10)
		sprintf(UTC, "%s%d:%s", sign, tmp, min);
	else if(tmp < 10 && tmp > 0)
		sprintf(UTC, "%s0%d:%s", sign, tmp, min);
	else
		sprintf(UTC, "00:00");

	UTC[sizeof(UTC) - 1] = '\0';
	*tz = StrDup(UTC);
	//printf("%s(%s)\n", UTC, *tz);
	if(*tz == NULL)
		res = -1;
	
	StrFree(min);
	StrFree(sign);
	
	return res;
}

/* ======================(function header)========================
Function Name:int setTZ(char **tz){}
Description : To Setup the system date、time by timezone information.
Arguments : char * tz
Return : a string with format "[+|-]hh:mm:ss"
written by Jackie Xie
Date : 2011/08/18
================================================================*/
int setTZ(char *timezone)
{
	struct tm *tptr, time_to_set;
	struct timeval tv;
	struct timezone tz;
	time_t secs;
	int utc = 0;
	int hsecs, msecs = 0, ssecs = 0, tzhour, tzminute;
	int rc;
	char *sHour = StrDup(timezone);
	char *sMin = StrDup(timezone);
	char *sSec = StrDup(timezone);
	char *sTZ = StrDup(timezone);
	bool tz_is_minus = false;

	if(!timezone)
		return -1;

	if(strstr(timezone, "-")) tz_is_minus = true;
	(void) time(&secs);
	tptr = localtime(&secs);
	printf("The timezone is：UTC%s\n", timezone);
	printf("The current date/time：\n");
	printf("	Date：%02d-%02d-%02d\n", 1900+tptr->tm_year, tptr->tm_mon+1, tptr->tm_mday);
	printf("	Time：%02d:%02d:%02d\n\n", tptr->tm_hour, tptr->tm_min, tptr->tm_sec);

	if(strstr(timezone,":") && matchStrPosAt(":", timezone, -1) == 1){
		/*hsecs = atoi(strcutail(sHour, ":", -100)) * 3600;
		msecs = atoi(strmhead(sMin, ":", -100)) * 60;*/
		hsecs = atoi(index_str(sHour, ":", 1)) * 3600;
		msecs = atoi(index_str(sMin, ":", 2)) * 60;
		tzminute = atoi(sMin);
		if(tz_is_minus)
			msecs = msecs * (-1);
	}
	else if(strstr(timezone, ":") && matchStrPosAt(":", timezone, -1) == 2){
		hsecs = atoi(index_str(sHour, ":", 1)) * 3600;
		msecs = atoi(index_str(sMin, ":", 2)) * 60;
		ssecs = atoi(index_str(sSec, ":", 3));
		tzminute = atoi(sMin);
		if(tz_is_minus){
			msecs = msecs * (-1);
			ssecs = ssecs * (-1);
		}
	}
	else{
		hsecs = atoi(sHour) * 3600;
		msecs = 0;
	}

	if(msecs == 0 && ssecs == 0){
		if(hsecs > 0)
			sprintf(sTZ, "GMT+%02d", hsecs/3600);
		else
			sprintf(sTZ, "GMT%02d", hsecs/3600);
	}
	else if(ssecs == 0){
		if(hsecs > 0)
			sprintf(sTZ, "GMT+%02d:%02d", hsecs/3600, msecs/60);
		else
			sprintf(sTZ, "GMT%02d:%02d", hsecs/3600, abs(msecs/60));
	}
	else{
		if(hsecs > 0)
			sprintf(sTZ, "GMT+%02d:%02d:%02d", hsecs/3600, msecs/60, ssecs);
		else
			sprintf(sTZ, "GMT%02d:%02d:%02d", hsecs/3600, abs(msecs/60), abs(ssecs));
	}

	tzhour = atoi(sHour);
	switch(tzhour)
	{
		case -12: nvram_set("ntp_hidden_select", "0"); break;
		case -11: nvram_set("ntp_hidden_select", "1"); break;
		case -10: nvram_set("ntp_hidden_select", "2"); break;
		case -9: nvram_set("ntp_hidden_select", "3"); break;
		case -8: nvram_set("ntp_hidden_select", "4"); break;
		case -7: nvram_set("ntp_hidden_select", "6"); break;
		case -6: nvram_set("ntp_hidden_select", "8"); break;
		case -5: nvram_set("ntp_hidden_select", "10"); break;
		case -4: nvram_set("ntp_hidden_select", "11"); break;
		case -3: if(tzminute == 30) nvram_set("ntp_hidden_select", "12"); else nvram_set("ntp_hidden_select", "13"); break;
		case -2: nvram_set("ntp_hidden_select", "14"); break;
		case -1: nvram_set("ntp_hidden_select", "15"); break;
		case 0: nvram_set("ntp_hidden_select", "17"); break;
		case 1: nvram_set("ntp_hidden_select", "21"); break;
		case 2: nvram_set("ntp_hidden_select", "24"); break;
		case 3: nvram_set("ntp_hidden_select", "26"); break;
		case 4: nvram_set("ntp_hidden_select", "27"); break;
		case 5: if(tzminute == 30) nvram_set("ntp_hidden_select", "29"); else nvram_set("ntp_hidden_select", "28"); break;
		case 6: nvram_set("ntp_hidden_select", "30"); break;
		case 7: nvram_set("ntp_hidden_select", "31"); break;
		case 8: nvram_set("ntp_hidden_select", "33"); break;
		case 9: nvram_set("ntp_hidden_select", "34"); break;
		case 10: nvram_set("ntp_hidden_select", "36"); break;
		case 11: nvram_set("ntp_hidden_select", "37"); break;
		case 12: nvram_set("ntp_hidden_select", "38"); break;
		default:
			break;
	}
	nvram_set("time_zone", sTZ);
	
	utc = hsecs + msecs + ssecs;
	 (void) time(&secs);
	
	if(utc==0)
		tptr = localtime(&secs);
	else{
		secs += utc;
		tptr = gmtime(&secs);
	}

	// Make new system time.
	if ((tv.tv_sec = mktime(tptr)) == (time_t)-1)
	{
		printf("Cannot convert system time\n");
		return -1;
	}

	tz.tz_minuteswest = -(secs / 60);	
	tv.tv_usec = 0;
	rc = settimeofday(&tv, &tz);
	if ( rc != 0)
	{
		printf("Cannot set system time\n");
		printf("settimeofday() failed, " "errno = %d\n",errno);
		return -1;
	}
	else
		printf("settimeofday() successful.\n");


	printf("新時區是：UTC%s\n", timezone);
	printf("更新後的日期和時間分別為:\n");
	printf("	日期: %02d年%02d月%02d日\n", 1900+tptr->tm_year, tptr->tm_mon+1, tptr->tm_mday);
	printf("	時間: %02d:%02d:%02d\n\n", tptr->tm_hour, tptr->tm_min, tptr->tm_sec);

	StrFree(sMin);
	StrFree(sHour);
	StrFree(sSec);
	StrFree(sTZ);
	
	return 0;
}

/* ======================(function header)===========================
Function Name:int setTZName(char *tzname){}
Description : To Setup the system date、time by a specified local timezone name which 
					is encoded according to IEEE 1003.1 (POSIX).
					
					Specifying the Time Zone with TZ
					
					In POSIX systems, a user can specify the time zone by means of the TZ environment variable. For information about how to set environment variables, see section Environment Variables. The functions for accessing the time zone are declared in `time.h'.
					
					You should not normally need to set TZ. If the system is configured properly, the default time zone will be correct. You might set TZ if you are using a computer over a network from a different time zone, and would like times reported to you in the time zone local to you, rather than what is local to the computer.
					
					In POSIX.1 systems the value of the TZ variable can be in one of three formats. With the GNU C library, the most common format is the last one, which can specify a selection from a large database of time zone information for many regions of the world. The first two formats are used to describe the time zone information directly, which is both more cumbersome and less precise. But the POSIX.1 standard only specifies the details of the first two formats, so it is good to be familiar with them in case you come across a POSIX.1 system that doesn't support a time zone information database.
					
					The first format is used when there is no Daylight Saving Time (or summer time) in the local time zone:
					
					std offset
					The std string specifies the name of the time zone. It must be three or more characters long and must not contain a leading colon, embedded digits, commas, nor plus and minus signs. There is no space character separating the time zone name from the offset, so these restrictions are necessary to parse the specification correctly.
					
					The offset specifies the time value you must add to the local time to get a Coordinated Universal Time value. It has syntax like [+|-]hh[:mm[:ss]]. This is positive if the local time zone is west of the Prime Meridian and negative if it is east. The hour must be between 0 and 23, and the minute and seconds between 0 and 59.
					
					The minutes ( mm) and seconds ( ss) are optional. The hour ( hh) shall be required and may be a single digit. The offset following std shall be required. If no offset follows dst, the alternative time is assumed to be one hour ahead of standard time. One or more digits may be used; the value is always interpreted as a decimal number. The hour shall be between zero and 24, and the minutes (and seconds)-if present-between zero and 59. The result of using values outside of this range is unspecified. If preceded by a '-', the timezone shall be east of the Prime Meridian; otherwise, it shall be west (which may be indicated by an optional preceding '+' ).
					
					For example, here is how we would specify Eastern Standard Time, but without any Daylight Saving Time alternative:
					
					EST+5
					The second format is used when there is Daylight Saving Time:
					
					std offset dst [offset],start[/time],end[/time]
					The initial std and offset specify the standard time zone, as described above. The dst string and offset specify the name and offset for the corresponding Daylight Saving Time zone; if the offset is omitted, it defaults to one hour ahead of standard time.
					
					The remainder of the specification describes when Daylight Saving Time is in effect. The start field is when Daylight Saving Time goes into effect and the end field is when the change is made back to standard time. The following formats are recognized for these fields:
					
					"FST+3FDT02:00,J60/00,J304/02"

					This specification describes a fictitious zone that lies three hours east of GMT. It makes a two hour shift forward for daylight savings which begins on March 1st at midnight, and ends on October 31st at 2am. The 'J' designation in the start/end specs signifies that counting starts at one and February 29th is never counted.

     				"FST+3FDT,59,304"

					This specification is significant because of the '59'. The lack of 'J' for the start and end dates, indicates that the Julian day-count begins at zero and ends at 365. If you do the math, you'll see that allows for a total of 366 days. This is fine in leap years, but in non-leap years '59' (Feb-29) does not exist.
					
					Jn
					This specifies the Julian day, with n between 1 and 365. February 29 is never counted, even in leap years.

					n
					This specifies the Julian day, with n between 0 and 365. February 29 is counted in leap years.

					Mm.w.d
					This specifies day d of week w of month m. The day d must be between 0 (Sunday) and 6. The week w must be between 1 and 5; week 1 is the first week in which day d occurs, and week 5 specifies the last d day in the month. The month m should be between 1 and 12.
					
					The time fields specify when, in the local time currently in effect, the change to the other time occurs. If omitted, the default is 02:00:00.
					
					For example, here is how you would specify the Eastern time zone in the United States, including the appropriate Daylight Saving Time and its dates of applicability. The normal offset from UTC is 5 hours; since this is west of the prime meridian, the sign is positive. Summer time begins on the first Sunday in April at 2:00am, and ends on the last Sunday in October at 2:00am.
					
					EST+5EDT,M4.1.0/2,M10.5.0/2

Arguments : char * tz
Return : a string with format "std offset dst [offset],start[/time],end[/time]"
written by Jackie Xie
Date : 2011/08/28
===================================================================*/
int setTZName(char *tzname)
{
	char *offsets = StrDup(tzname);
	char stz[12]={0};
	char dtz[12]={0};
	char stz_offset[12]={0};
	char dtz_offset[12]={0};
	char *start_dst = NULL;
	char *end_dst = NULL;
	int len, i = -1, chstr = 0, chnum = 0;
	int stdlen = 0, dstlen = 0, sofflen = 0, dofflen = 0;
	char *times, *day, *week, *month, *year, *tmp;
	datetime currtime, sdst, edst;
	time_t ptime = 0;
	char data[32];
	
	if(!tzname)
		return -1;

	if(strstr(tzname, ","))
	{
		time_t currTime;
		struct tm* pLocalTime=NULL;
	
		times = StrDup("00:00:00");
		day = StrDup("00");
		week = StrDup("0");
		month = StrDup("00");
		year = StrDup("0000");

		currTime = time(NULL);
		pLocalTime = localtime(&currTime);
		strftime(times, 10 ,"%H:%M:%S", pLocalTime);
		strftime(day, 3 ,"%d", pLocalTime);
		strftime(week, 2 ,"%w", pLocalTime);
		strftime(month, 3 ,"%m", pLocalTime);
		strftime(year, 5, "%Y", pLocalTime);
		
		tmp = StrDup(times); currtime.time.hour = atoi(strcutail(tmp, ":", -1));
		tmp = StrDup(times); currtime.time.minute = atoi(GetStrBetweenStr(tmp, ":", ":"));
		tmp = StrDup(times); currtime.time.second = atoi(strmhead(tmp, ":", -2));
		currtime.day = atoi(day);
		currtime.week = atoi(week);
		currtime.month = atoi(month);
		currtime.year = atoi(year);
		printf("current time is %d-%02d-%02dT%02d:%02d:%02dZ\n", currtime.year, currtime.month, currtime.day, currtime.time.hour, currtime.time.minute, currtime.time.second);

		start_dst = StrDup(tzname);
		end_dst = StrDup(tzname);
		tmp = StrDup(tzname);
		strcutail(offsets, ",", -1);
		//printf("offsets = %s\n", offsets);
		start_dst = StrDup(GetStrBetweenStr(tmp, ",", ","));

		/* To convert 'Jn' to 'Mm.w.d' format if '[J]n' is used */	
		if(strstr(start_dst, "J") || !strstr(start_dst, "M")){
			char *tmp = StrDup(start_dst);
			char dst[64], *timetmp = NULL;
			int _days;

			if(strstr(tmp, "/")){
				timetmp = StrDup(start_dst);
				strmhead(timetmp, "/", 1);
				strcutail(tmp, "/", -1);
			}
			if(strstr(tmp, "J"))
				strmhead(tmp, "J", -1);
			_days = atoi(tmp);
			//printf("days = %d\n", _days);
			_days = days2date(_days, dst);
			//printf("days = %d[%s]\n", _days, dst);
			jds2mds(_days, dst);
			dst[sizeof(dst) - 1] = '\0';
			if(timetmp)
				sprintf(start_dst, "%s%s", dst, timetmp);
			else
				sprintf(start_dst, "%s", dst);
			//printf("new start_dst is %s[%s]\n", start_dst, dst);
			StrFree(tmp); StrFree(timetmp);
		}

		/* Parsing Mm.w.d format */		
		if(!strstr(start_dst, "/"))
			sprintf(start_dst, "%s/2", start_dst);
		
		times = StrDup(start_dst);
		day = StrDup(start_dst);
		week = StrDup(start_dst);
		month = StrDup(start_dst);
		
		//sdst.time = atoi(strmhead(times, "/", -1));
		tmp = StrDup(strmhead(times, "/", -1));
		if(strstr(tmp, ":"))
		{
			times = StrDup(tmp); sdst.time.hour = atoi(strcutail(times, ":", -1));
			times = StrDup(tmp); //printf("times : %s\n", times);
			if(strstr(strmhead(times, ":", -1), ":")) sdst.time.minute = atoi(strcutail(times, ":", -1));
			else sdst.time.minute = atoi(times);
			times = StrDup(tmp);
			if(strstr(strmhead(times, ":", -1), ":")) sdst.time.second = atoi(strmhead(times, ":", -1));
			else sdst.time.second = 0;
		}
		else
		{
			sdst.time.hour = atoi(tmp);
			sdst.time.minute = 0;
			sdst.time.second = 0;
		}
			
		sdst.day = atoi(strcutail(strmhead(day, ".", -2), "/", -1));
		sdst.week = atoi(GetStrBetweenStr(week, ".", "."));
		sdst.month = atoi(GetStrBetweenStr(month, "M", "."));

		strmhead(end_dst, ",", -2);

		/* To convert 'Jn' to 'Mm.w.d' format if '[J]n' is used */	
		if(strstr(end_dst, "J") || !strstr(end_dst, "M")){
			char *tmp = StrDup(end_dst);
			char dst[64], *timetmp = NULL;
			int _days;

			if(strstr(tmp, "/")){
				timetmp = StrDup(end_dst);
				strmhead(timetmp, "/", 1);
				strcutail(tmp, "/", -1);
			}
			if(strstr(tmp, "J"))
				strmhead(tmp, "J", -1);
			_days = atoi(tmp);
			//printf("days = %d\n", _days);
			_days = days2date(_days, dst);
			//printf("days = %d[%s]\n", _days, dst);
			jds2mds(_days, dst);
			dst[sizeof(dst) - 1] = '\0';
			if(timetmp)
				sprintf(end_dst, "%s%s", dst, timetmp);
			else
				sprintf(end_dst, "%s", dst);
			//printf("new end_dst is %s[%s]\n", end_dst, dst);
			StrFree(tmp); StrFree(timetmp);
		}

		/* Parsing Mm.w.d format */		
		if(!strstr(end_dst, "/"))
			sprintf(end_dst, "%s/2", end_dst);
		
		times = StrDup(end_dst);
		day = StrDup(end_dst);
		week = StrDup(end_dst);
		month = StrDup(end_dst);
		
		//edst.time = atoi(strmhead(times, "/", -1));
		tmp = StrDup(strmhead(times, "/", -1));
		if(strstr(tmp, ":"))
		{
			times = StrDup(tmp); edst.time.hour = atoi(strcutail(times, ":", -1));
			times = StrDup(tmp);
			if(strstr(strmhead(times, ":", -1), ":")) edst.time.minute = atoi(strcutail(times, ":", -1));
			else edst.time.minute = atoi(times);
			times = StrDup(tmp);
			if(strstr(strmhead(times, ":", -1), ":")) edst.time.second = atoi(strmhead(times, ":", -1));
			else edst.time.second = 0;
		}
		else
		{
			edst.time.hour = atoi(tmp);
			edst.time.minute = 0;
			edst.time.second = 0;
		}

		edst.day = atoi(strcutail(strmhead(day, ".", -2), "/", -1));
		edst.week = atoi(GetStrBetweenStr(week, ".", "."));
		edst.month = atoi(GetStrBetweenStr(month, "M", "."));

		/* To confirm the year for start and end of the daylight saving time */
		if(sdst.month > edst.month)
		{
			if(currtime.month >= sdst.month){
				sdst.year = currtime.year;
				edst.year = currtime.year + 1;
			}
			else if(currtime.month <= edst.month || sdst.month > currtime.month){
				sdst.year = currtime.year - 1;
				edst.year = currtime.year;
			}
			else
				sdst.year = edst.year = currtime.year;
		}
		else if(sdst.month == edst.month)
		{
			if(sdst.week > edst.week || (sdst.week == edst.week && (sdst.day > edst.day || 
				(sdst.day == edst.day && (sdst.time.hour > edst.time.hour || 
				(sdst.time.hour == edst.time.hour && (sdst.time.minute > edst.time.minute ||
				(sdst.time.minute == edst.time.minute && sdst.time.second > edst.time.second))))))))
			{
				sdst.year = currtime.year;
				edst.year = currtime.year + 1;
			}
			else
				sdst.year = edst.year = currtime.year;				
		}
		else
			sdst.year = edst.year = currtime.year;

		printf("start daylight saving time = %s[%4d-%02d-%02dT%02d:%02d:%02dZ]\n", start_dst, sdst.year, sdst.month, dayOfMonth(sdst), sdst.time.hour, sdst.time.minute, sdst.time.second);
		printf("end daylight saving time = %s[%4d-%02d-%02dT%02d:%02d:%02dZ]\n", end_dst, edst.year, edst.month, dayOfMonth(edst), edst.time.hour, edst.time.minute, edst.time.second);
		
		sprintf(data, "%4d-%02d-%02dT%02d:%02d:%02dZ\n", currtime.year, currtime.month,  currtime.day, currtime.time.hour, currtime.time.minute, currtime.time.second);
		ptime = currtime.ptime = str2datetime(data);

		sprintf(data, "%4d-%02d-%02dT%02d:%02d:%02dZ\n", sdst.year, sdst.month,  sdst.day, sdst.time.hour, sdst.time.minute, sdst.time.second);
		sdst.ptime = str2datetime(data);

		sprintf(data, "%4d-%02d-%02dT%02d:%02d:%02dZ\n", edst.year, edst.month,  edst.day, edst.time.hour, edst.time.minute, edst.time.second);
		edst.ptime = str2datetime(data);

		StrFree(times); StrFree(tmp);
	}

	len = strlen(offsets);
	
	chstr = 0; chnum = 0;
	while (offsets[++i] != '\0')
	{
		offsets[stdlen] = toupper(offsets[stdlen]);
		if(isalpha(offsets[i]))
		{
			offsets[stdlen] = toupper(offsets[stdlen]);
			if(!chstr)
			{
				stz[stdlen] = offsets[i];
				stdlen++ ;
			}
			else
			{
				chnum = 1;
				dtz[dstlen] = offsets[i];
				dstlen++ ;
			}
		}
		else if(isdigit(offsets[i]) || offsets[i] == '+' || offsets[i] == '-' || offsets[i] == ':')
		{
			/* If preceded by a '-', the timezone shall be east of the Prime Meridian; 
			 * otherwise, it shall be west (which may be indicated by an optional preceding '+' ).
			 * Note: The +,- definition specifies east and west and not addition or subtraction, 
			 * so be careful. (UTC+1 translates to <Timezone name>-1) 
			 */
			chstr = 1;
			if(!chnum)
			{
				stz_offset[sofflen] = offsets[i];
				sofflen++ ;
			}
			else
			{
				dtz_offset[dofflen] = offsets[i];
				dofflen++ ;
			}
		}
	}
	
	printf("Standard time zone is '%s%s'.\n", stz, stz_offset);
	//printf("Daylight Saving Time zone is '%s%s'.\n", dtz, dtz_offset);
	
	{	
		/* Parsing standard/daylight-saving time zone name */
		char *shour = StrDup(stz_offset);
		char *sminute = StrDup(stz_offset);
		char *ssecond = StrDup(stz_offset);
		char *dhour = StrDup(dtz_offset);
		char *dminute = StrDup(dtz_offset);
		char *dsecond = StrDup(dtz_offset);
		int stzhour = 0,  dtzhour = 0, stzminute = 0, dtzminute = 0, stzsecond = 0, dtzsecond = 0;
		bool stz_is_minus = false, dtz_is_minus = false;

		strcutail(shour, ":", -1);
		if(matchStrPosAt(":", sminute, -1) == 2)
		stzminute = atoi(GetStrBetweenStr(sminute, ":", ":"));
		else if(matchStrPosAt(":", sminute, -1) == 1)
			stzminute = atoi(strmhead(sminute, ":", -1));
		if(strstr(ssecond, ":") && matchStrPosAt(":", ssecond, -1) == 2)
			stzsecond = atoi(strmhead(ssecond, ":", -2));
		else
			stzsecond = 0;

#ifdef ENABLE_TIMEZONE_OFFSET
		if(strstr(dtz_offset, "-")) dtz_is_minus = true;
#else
		if(strstr(dtz_offset, "-")) dtz_is_minus = false;
#endif
		dtzhour = atoi(strcutail(dhour, ":", -1));
		if(matchStrPosAt(":", dminute, -1) == 2)
		dtzminute = atoi(GetStrBetweenStr(dminute, ":", ":"));
		else if(matchStrPosAt(":", dminute, -1) == 1)
			dtzminute = atoi(strmhead(dminute, ":", -1));
		if(strstr(dsecond, ":") && matchStrPosAt(":", dsecond, -1) == 2)
			dtzsecond = atoi(strmhead(dsecond, ":", -2));
		else
			dsecond = 0;
/*		
		shour = index_str(shour, ":", 1);
		stzminute = atoi(index_str(sminute, ":", 2));
		stzsecond = atoi(index_str(ssecond, ":", 3));
		dtzhour = atoi(index_str(dhour, ":", 1));
		dtzminute = atoi(index_str(dminute, ":", 2));
		dtzsecond = atoi(index_str(dsecond, ":", 3));
*/
		if(dtz_is_minus){
			dtzminute = dtzminute * (-1);
			dtzsecond = dtzsecond * (-1);
		}

		if(strstr(stz, "GMT") || strstr(stz, "UTC") || !strcmp(stz, ""))
			stzhour = atoi(shour);
		else
			stzhour = atoi(shour) * (-1);

#ifdef ENABLE_TIMEZONE_OFFSET
		if(stzhour < 0 || ((strstr(stz, "GMT") || strstr(stz, "UTC") || !strcmp(stz, "")) && strstr(shour, "-"))
			|| (!strcmp(shour, "00") && !strstr(shour, "-") && !(strstr(stz, "GMT") || strstr(stz, "UTC") || !strcmp(stz, ""))))
			stz_is_minus = true;

		if(stz_is_minus){
			stzminute = stzminute * (-1);
			stzsecond = stzsecond * (-1);
		}

		if((stz_is_minus && (abs(stzhour) > abs(dtzhour) || ((abs(dtzhour) - abs(stzhour) == 0) && (abs(stzminute) > abs(dtzminute)
			|| ((stzminute - dtzminute == 0) && abs(stzsecond) > abs(dtzsecond))))))
				|| (dtz_is_minus && (abs(stzhour) < abs(dtzhour) || ((abs(dtzhour) - abs(stzhour) == 0) && (abs(stzminute) < abs(dtzminute)
					|| ((stzminute - dtzminute == 0) && abs(stzsecond) < abs(dtzsecond)))))))
			dtz_is_minus = true;
		else
			dtz_is_minus = false;

		dtzsecond += stzsecond;
		if(dtzsecond >= 60){
			dtzsecond %= 60;
			++dtzminute;
		}
		else if((!dtz_is_minus && dtzsecond < 0 && dtzhour != 0) || (dtz_is_minus && abs(dtzsecond) >60)){
			dtzsecond += 60;
			--dtzminute;
		}
		else if(dtz_is_minus && dtzsecond > 0){
			dtzsecond -= 60;
			++dtzminute;
		}

		dtzminute += stzminute;
		if(dtzminute >= 60){
			dtzminute %= 60;
			++dtzhour;
		}
		else if((!dtz_is_minus && dtzminute < 0 && dtzhour != 0) || (dtz_is_minus && abs(dtzminute) >60)){
			dtzminute += 60;
			--dtzhour;
		}
		else if(dtz_is_minus && dtzminute > 0){
			dtzminute -= 60;
			++dtzhour;
		}

		if(!dtz_is_minus && dstlen && !dofflen || (dtzhour == 0 && dtzminute ==0 && dtzsecond == 0))
			dtzhour += stzhour + 1; 
		else if(dtzhour != 0)
			dtzhour += stzhour;
		else 
			dtzhour = stzhour;

#else
		if(!dtz_is_minus) dtzhour = dtzhour * (-1);
		if((dstlen && !dofflen) || dtzhour == 0){
			dtzhour = stzhour + 1;
			dtzminute = stzminute;
			dtzsecond = stzsecond;
		}
#endif

		if(dtzhour == 0 && (dtzminute < 0 || dtzsecond < 0))
			dtz_is_minus = true;

		if((strstr(stz_offset, ":") || strstr(dtz_offset, ":")) && ((dtzminute != 0 || dtzsecond != 0) || (stzminute != 0 || stzsecond != 0)))
		{
			if(dtzhour > 0)
				sprintf(dtz_offset, "+%02d:%02d:%02d", dtzhour, dtzminute, dtzsecond);
			else if(dtzhour == 0 && dtz_is_minus)
				sprintf(dtz_offset, "-%02d:%02d:%02d", dtzhour, abs(dtzminute), abs(dtzsecond));
			else
				sprintf(dtz_offset, "%02d:%02d:%02d", dtzhour, abs(dtzminute), abs(dtzsecond));
			if(stzhour > 0)
				sprintf(stz_offset, "+%02d:%02d:%02d", stzhour, stzminute, stzsecond);
			else if(stzhour == 0 && stz_is_minus)
				sprintf(stz_offset, "-%02d:%02d:%02d", stzhour, abs(stzminute), abs(stzsecond));
			else
				sprintf(stz_offset, "%02d:%02d:%02d", stzhour, abs(stzminute), abs(stzsecond));
		}
		else
		{
			if(dtzhour > 0)
				sprintf(dtz_offset, "+%02d", dtzhour);
			else
				sprintf(dtz_offset, "%02d", dtzhour);
			if(stzhour > 0)
				sprintf(stz_offset, "+%02d", stzhour);
			else
				sprintf(stz_offset, "%02d", stzhour);
		}
		StrFree(shour); StrFree(sminute);
	}
	
	if(dstlen) printf("Daylight Saving Time zone is '%s%s'.\n\n", dtz, dtz_offset);
	/* improve accuracy in timezone configuration */
	bool duringTheDST = (currtime.year > sdst.year && (currtime.year == edst.year && 
		(currtime.month < edst.month || (currtime.month == edst.month &&
			(currtime.day < dayOfMonth(edst) || (currtime.day == dayOfMonth(edst) && 
				(currtime.time.hour < edst.time.hour || (currtime.time.hour == edst.time.hour && 
					(currtime.time.minute < edst.time.minute || (currtime.time.minute == edst.time.minute && currtime.time.second < edst.time.second)))))))))) ||
		(currtime.year == sdst.year && 
			(currtime.month > sdst.month || (currtime.month == sdst.month && 
				(currtime.day > dayOfMonth(sdst) || (currtime.day == dayOfMonth(sdst) && 
					(currtime.time.hour > sdst.time.hour || (currtime.time.hour == sdst.time.hour && (currtime.time.minute > sdst.time.minute || 
						(currtime.time.minute == sdst.time.minute && currtime.time.second > sdst.time.second)))))))) &&
		(currtime.year < edst.year || (currtime.year == edst.year &&
			(currtime.month < edst.month || (currtime.month == edst.month && 
				(currtime.day < dayOfMonth(edst) || (currtime.day == dayOfMonth(edst) && 
					(currtime.time.hour < edst.time.hour) || (currtime.time.hour == edst.time.hour && (currtime.time.minute < edst.time.minute || 
						(currtime.time.minute == edst.time.minute && currtime.time.second < edst.time.second))))))))));
	if(duringTheDST){
	/* if(ptime >= sdst.ptime && ptime <= edst.ptime) */
	/*if((currtime.year > sdst.year && currtime.year == edst.year && currtime.day <= dayOfMonth(edst)) ||
		(currtime.year == sdst.year && currtime.month >= sdst.month && currtime.day >= dayOfMonth(sdst))) */
		setTZ(dtz_offset);
		nvram_set("localtimezone", dtz_offset);
		nvram_set("daylight_savings_used", "true");
		nvram_set("ntpadjust", "1");
	}
	else{
		setTZ(stz_offset);
		nvram_set("localtimezone", stz_offset);
		nvram_set("daylight_savings_used", "false");
		nvram_set("ntpadjust", "0");
	}

	StrFree(offsets); StrFree(start_dst); StrFree(end_dst);
	return 0;
}

/* ======================(function header)========================
Function Name:char* itos (){}
Description:to convert integer to string
Arguments:unsigned n
Return:a string
written by jackiexie
Date:  2007/07/10
================================================================*/
char tmpbuf[17];
char* itos (int n)
{
  int i=0,j;
  //char* s;
  //char* u;
  char s[17];
  //s= (char*) malloc(17);
  //u= (char*) malloc(17);
  
  do{
    s[i++]=(char)( n%10+48 );
    n-=n%10;
  }
  while((n/=10)>0);
  for (j=0;j<i;j++)
  tmpbuf[i-1-j]=s[j];

  tmpbuf[j]='\0';
  return tmpbuf;
}

int isLetters(const char *text)
{
	if (!text) return 0;
	while (*text) {
		if (!isalpha(*text)) return 0;
		++text;
	}
	return 1;
}

/* ======================(function header)========================
Function Name:time_t time(time_t *timer){}
Description:get current time value 
Arguments:time_t *timer
Return:time_t
written by jackie xie
Date:   2007/07/10
================================================================*/
/****************************************************************
 *
 * time					2007/07/10  jackie xie
 *
 ****************************************************************/
time_t _time(time_t *timer)
{
   struct timeval xCurrTime;
   xCurrTime.tv_usec = *timer;
   gettimeofday(&xCurrTime, NULL);
   return((xCurrTime.tv_sec << 1000) + (xCurrTime.tv_usec >> 1000));
}

time_t get_time(time_t *t)
{ 
  time_t tRet;
  struct timeval tv;

  gettimeofday(&tv, NULL);

  tRet = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
  
  if (t) {
    *t = tRet;
  }
  return tRet;
}

/* ======================(function header)========================
Function Name:int number_gen(){}
Description:Get a random number
Arguments:none
Return:integer
written by jackie xie
Date:   2007/07/10
================================================================*/
/****************************************************************
 *
 * number_gen				2007/07/10   jackie xie
 *
 ****************************************************************/
int number_gen( void )
{
    int *piState;
    int iState1;
    int iState2;
    int iRand;

    piState		= &rgiState[2];
    iState1	 	= piState[-2];
    iState2	 	= piState[-1];
    iRand	 	= ( piState[iState1] + piState[iState2] )
    			& ( ( 1 << 30 ) - 1 );
    piState[iState1]	= iRand;
    if ( ++iState1 == 55 )
    	iState1 = 0;
    if ( ++iState2 == 55 )
    	iState2 = 0;
    piState[-2]		= iState1;
    piState[-1]		= iState2;
    return iRand >> 6;
}

/* ======================(function header)========================
Function Name:int number_range(){}
Description:Get a generated random number
Arguments:two integer numbers for a range ( int from, int to )
Return:integer
written by jackie xie
Date:   2007/07/10
================================================================*/
/****************************************************************
 *
 * Generate a random number		2007/06/28 jackie xie
 *
 ****************************************************************/
int number_range( int from, int to )
{
    int power;
    int number;
    if ( ( to = to - from + 1 ) <= 1 )
    	return from;
    for ( power = 2; power < to; power <<= 1 )
    	;
    while ( ( number = number_gen( ) & ( power - 1 ) ) >= to )
    	;
    return from + number;
}

/* ======================(function header)========================
Function Name:void init_gen( ){}
Description:seed the number generator
Arguments:none
Return:none
written by jackie xie
Date:   2007/07/10
================================================================*/
/****************************************************************
*
* This function is follow the Mitchell-Moore algorithm from 								
* Knuth Volume II.			2007/07/10   jackie xie
*
****************************************************************/
void init_gen( )
{
    int *piState;
    int iState;
    piState	= &rgiState[2];

    piState[-2]	= 55 - 55;
    piState[-1]	= 55 - 24;
    //cout<<"current time : "<<time( NULL )<<endl;
    //printf("current time :  %ld\n", (long)get_time( NULL ));
    piState[0]	= ( (int) time( NULL ) ) & ( ( 1 << 30 ) - 1 );
    piState[1]	= 1;
    for ( iState = 2; iState < 55; iState++ )
    {
        	piState[iState] = ( piState[iState-1] + piState[iState-2] )
        			& ( ( 1 << 30 ) - 1 );
    }
    return;
}

// To generate alphanumeric Random String
void random_string(char *string, const unsigned int length) {
	int i;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	/* Seed number for rand() */
	srand((unsigned int) time(0) + getpid());

	for (i = 0; i < length; ++i) {
		string[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	string[i] = '\0';	
}

/*Shell Sort*/
void Shell_Sort( int *array , int index )
{
  int i ;
  int tmp ;
  int length;
  int process;

  length = index / 2;

  while ( length != 0 )
  {
    for ( i = length ; i < index ; i++ )
    {
      tmp = array[i];
      process = i - length;

      while ( tmp < array[process] && process >= 0 && i <= index )
      {
        array[ process + length ] = array[process];
        process = process - length;
      }
      array[ process + length ] = tmp;
    }
    length = length / 2;
  }
}

int lowercase (char *sPtr)
{
	while ( *sPtr != '\0' ) {
		*sPtr = tolower((unsigned char) *sPtr);
		++sPtr;
	}
	return 0;
}

int uppercase (char *sPtr)
{
	while ( *sPtr != '\0' ) {
		*sPtr = toupper((unsigned char)*sPtr);
		++sPtr;
	}
	return 0;
}

/* ======================(function header)========================
Function Name:int trim(char *)()
Description:elimination the space of the
            string.
Arguments:char* str
Return:none-space string
written by jackie xie
Date:   2007/07/10
================================================================*/
int xtrim(char *s)
{
   char *esp,*sp=s,c; esp=sp;
   if(s!=NULL && *s)
   {
      do ; while( (*sp++)==32 );
      sp--;
      do {
         c=*sp++;
         *s++=c;
         if( (c!=32) && (c>0) ) esp=s;
         do ; while( (*sp++)==32 );
         sp--;
      }while (c>0);
      *esp=0;
   }
   return 0;
}

/* Remove leading whitespaces */
char *ltrim(char *const s)
{
	size_t len;
	char *cur;

	if(s && *s) {
		len = strlen(s);
		cur = s;

		while(*cur && isspace(*cur))
			++cur, --len;

		if(s != cur)
			memmove(s, cur, len + 1);
	}

	return s;
}

/* Remove trailing whitespaces */
char *rtrim(char *const s)
{
	size_t len;
	char *cur;

	if(s && *s) {
		len = strlen(s);
		cur = s + len - 1;

		while(cur != s && isspace(*cur))
			--cur, --len;

		cur[isspace(*cur) ? 0 : 1] = '\0';
	}

	return s;
}

/* Remove leading and trailing whitespaces */
char *trim(char *const s)
{
	rtrim(s);  // order matters
	ltrim(s);

	return s;
}

/* ======================(function header)========================
Function Name: char *GetStrBetweenStr(char *,
                                char *,char *)
Description: retrieve a string from between left position
                 and right position string.
Arguments:char *s,char *lstr,char *rstr
Return: string
written by jackie xie
Date:   2007/07/10
================================================================*/
char *GetStrBetweenStr(char *s,char *lstr,char *rstr)
{
   char *p=s,*lp=s,*rp;
   do
   {
      /*trim(lstr);
      trim(rstr);*/
      lp=strstr(lp,lstr);
      if(lp)
      {
         rp=lstr;
         do lp++; while(*rp++);
         lp--;
         rp=strstr(lp,rstr);
         if(rp)
         {
            if(lp!=rp)
            {
               do *p++=*lp++; while(lp<rp);
               lp--;
            }
            rp=rstr;
            do lp++; while(*rp++);
            lp--;
         }
      }
   } while(lp);
   *p=0;
   return s;
}

char *StrDup(const char *s)
{
  char *d = NULL;
  if (s) {
    d = (char *)malloc(sizeof(char*) * strlen(s) + 1);
    strcpy(d, s);
  }
  return d;
}

void StrFree(void *ptr)
{
  if (ptr) {
    free(ptr);
  }
}

/* ======================(function header)========================
Function Name: char *StrCat(char *, const char *){}
Description: copy string function. using this function
            before you must initial the source string
Arguments: char *src, const char *dest
Return: string
written by jackie xie
Date   :  2007/07/10
================================================================*/
char *StrCat(char *src, const char *dest)
{
	char *newstr = src;
	int length;

	length = (unsigned) strlen(src) + (unsigned) strlen(dest) + 1;
	src = (char *) realloc(src,length);

	while (*newstr)
	{
		newstr++;
	}

	while ((*(newstr++) = (*(dest++))))
	{
		;
	}
	return src;
}

unsigned char* ConvertStringIntoByte(char *pszStr, unsigned char* pbyNum)
{
	//const char cSep0 = NULL ; //Bytes separator in string like 00aabbccddee
	const char cSep1 = '-'; 	//Bytes separator in string like 00-aa-bb-cc-dd-ee
	const char cSep2 = ' '; 	//Bytes separator in string like 00 aa bb cc dd ee
	const char cSep3 = ':'; 	//Bytes separator in string like 00:aa:bb:cc:dd:ee
	const char cSep4 = '.'; 	//Bytes separator in string like 00.aa.bb.cc.dd.ee
	const char cSep5 = ','; 	//Bytes separator in string like 00,aa,bb,cc,dd,ee
	const char cSep6 = '/'; 	//Bytes separator in string like 00/aa/bb/cc/dd/ee	
	int iConunter;
	bool exist_delimiter = false;
	int strLength;
		
	strLength = strlen(pszStr);
		
	if(strLength > 13)
		exist_delimiter = true;
	
	for (iConunter = 0; iConunter < 6; ++iConunter)
	{
		unsigned int iNumber = 0;
		char ch;
			
		//Convert letter into lower case.
		ch = tolower (*pszStr++);

		if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
		{
			//printf("delimiter is %c\n", ch);
			return NULL;
		}

		//Convert into number. 
		//         a. If chareater is digit then ch - '0'
		//		   b. else (ch - 'a' + 10) it is done because addition of 10 takes correct value.
		iNumber = isdigit (ch) ? (ch - '0') : (ch - 'a' + 10);
		ch = tolower (*pszStr);

		if ((iConunter < 5 && (ch != cSep1 && ch != cSep2 && ch != cSep3 && ch != cSep4 && ch != cSep5 && ch != cSep6)) || (iConunter == 5 && ch != '\0' && !isspace(ch)))
		{
			++pszStr;

			if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
			{
				return NULL;
			}

			iNumber <<= 4;
			iNumber += isdigit (ch) ? (ch - '0') : (ch - 'a' + 10);
			ch = *pszStr;
		
			if(exist_delimiter)
				if (iConunter < 5 && (ch != cSep1 && ch != cSep2 && ch != cSep3 && ch != cSep4 && ch != cSep5 && ch != cSep6))
				{
					return NULL;
				}
		}
		/* Store result.  */
		pbyNum[iConunter] = (unsigned char) iNumber;
		/* Skip cSep.  */
		if(exist_delimiter)
			++pszStr;			
	}
	return pbyNum;
}

int xasprintf(char **bufp, const char *format, ...)
{
  va_list ap, ap1;
  int rv;
  int bytes;
  char *p;

  va_start(ap, format);
  va_copy(ap1, ap);

  bytes = vsnprintf(NULL, 0, format, ap1) + 1;
  va_end(ap1);

  *bufp = p = malloc(bytes);
  if ( !p )
    return -1;

  rv = vsnprintf(p, bytes, format, ap);
  va_end(ap);

  return rv;
}

#if defined(__FreeBSD__) || defined(__APPLE__) || defined(MACOSX) || defined(darwin)
ssize_t getdelim(char **lineptr, size_t *n, int delim, FILE *stream)
{
	ssize_t linelen;
	char *newlineptr;
	char c;

	if (!lineptr || !n || !stream)
	{
		errno = EINVAL;
		return -1;
	}

	if (!*lineptr)
	{
		*n = GETDELIM_CHUNKSIZE;
		*lineptr = malloc(*n);
	}
	if (!*lineptr)
		return -1;

	linelen = 0;
	while (1)
	{
		/*
		 * Ensure adequate length. This has to be matched to the
		 * subsequent for loop.
		 */
		if (linelen + 2 > *n)
		{
			*n += GETDELIM_CHUNKSIZE;
			newlineptr = realloc(*lineptr, *n);
			if (!newlineptr)
			{
				*n -= GETDELIM_CHUNKSIZE;
				return -1;
			}
			*lineptr = newlineptr;
		}

		/*
		 * Ensure we have space for two new chars: the one we read and
		 * the terminating NUL.
		 */
		errno = 0;
		for (; linelen < *n - 1; linelen ++)
		{
			c = fgetc(stream);
			if (c == EOF)
			{
				if (errno)
				{
					/*
					 * Should we just set EINVAL for everything? We're
					 * supposed to set EINVAL for a bad stream
					 * descriptor... and I assume/hope that EBADF fits
					 * the description... --binki
					 */
					if (errno == EBADF)
						errno = EINVAL;
					return -1;
				}
				if (feof(stream))
					if (linelen)
					{
						/* we hit EOF _instead_ of a delimiter */
						(*lineptr)[linelen] = '\0';
						return linelen;
					}
			else
				/* since we've read no data, time to tell the caller it's EOF */
				return -1;
			}
			(*lineptr)[linelen] = c;
			if (c == delim)
			{
				linelen ++;
				/* our for loop ensures that we have this second char to put the '\0' into */
				(*lineptr)[linelen] = '\0';
				return linelen;
			}
		}
	}
}

#if 1
ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
	return getdelim(lineptr, n, '\n', stream);
}
#else		/* An other implemenation of getline */
ssize_t getline(char **linep, size_t *np, FILE *stream)
{
	char *p = NULL;
	size_t i = 0;
	int ch = 0;

	if (!linep || !np) {
		errno = EINVAL;
		return -1;
	}

	if (!(*linep) || !(*np)) {
		*np = 120;
		*linep = (char *)malloc(*np);
		if (!(*linep)) {
			return -1;
		}
	}

	flockfile(stream);

	p = *linep;
	for (ch = 0; (ch = getc_unlocked(stream)) != EOF;) {
		if (i > *np) {
			/* Grow *linep. */
			size_t m = *np * 2;
			char *s = (char *)realloc(*linep, m);

			if (!s) {
				int error = errno;
				funlockfile(stream);
				errno = error;
				return -1;
			}

			*linep = s;
			*np = m;
		}

		p[i] = ch;
		if ('\n' == ch) break;
		i += 1;
	}
	funlockfile(stream);

	/* Null-terminate the string. */
	if (i > *np) {
		/* Grow *linep. */
			size_t m = *np * 2;
			char *s = (char *)realloc(*linep, m);

			if (!s) {
				return -1;
			}

			*linep = s;
			*np = m;
	}

	p[i + 1] = '\0';
	return ((i > 0)? i : -1);
}
#endif
#endif

char *readin(FILE *in)
{
    char tmp[80];
    char *result="";
    while( fgets( tmp, 80, in)!=NULL ) {
        xasprintf(&result, "%s%s", result, tmp);
    }
    return result;
}

int copyFile(const char *Fin, const char *Fout)
{
	int inF, ouF;
	char line[2048];
	int bytes;

	if((inF = open(Fin, O_RDONLY)) == -1) {
		printf("Failed to open file %s\n", Fin);
		return -1;
	}

	unlink(Fout);													
	if((ouF = open(Fout, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
		printf("Failed to open file %s\n", Fout);
		return -1;
	}
	while((bytes = read(inF, line, sizeof(line))) > 0)
		write(ouF, line, bytes);	
	close(inF);
	close(ouF);
	return 0;
}

int getFileSize(char* filename, long *filesize)
{
	FILE *pFile;
	long size;
	
	pFile = fopen (filename,"rb");
	if (pFile==NULL){
		perror ("Error opening file");
		return -1;
	}
	else
	{
		fseek (pFile, 0, SEEK_END);
		size=ftell (pFile);
		fclose (pFile);
	}
	//printf ("Size of %s : %ld bytes.\n", filename, size);
	*filesize = size;
	
	return 0;
}

/* ======================(function header)========================
Function Name: int matchStrPosAt (const char *, const char *)
Description: find the sub-string from the source string.
Arguments: const char *, const char *
Return: Returns "the array element number"(or "position") where 
      "sub-string" matches before in "original string", or return 
      "-1" if substr is not present in "original string".
written by jackie xie
Date   :  2007/07/10
================================================================*/
/* 
 *	delimiters = ±n：return position before n-th matched substr.
 *	delimiters =   0 ：do nothing.
 *	delimiters =  -1 ：returns the array element number where "substr" occurs in "str".
 */
int matchStrPosAt (const char * substr, char * str, int delimiters)
{
	int i = -1, k, str_index, total = 0;
	int substr_len = strlen(substr), str_len = strlen(str);
	int matchpos = -1;
	int delimiter[str_len], ind_delim = 0;
	//int n = abs(delimiters);

	for(k=0; k<=str_len ; k++)
		delimiter[k] = -1;

	if (str == NULL || substr == NULL) return -1;

	/* substr has to be smaller than str */
	if (substr_len > str_len) return -1;

	/* look through str for substr, stopping at end of string or when the length
	 * of substr exceeds the remaining number of characters to be searched */ 
	while (str[++i] != '\0' && (str_len - i + 1) > substr_len) 
	{
		for (str_index = 0; str_index < substr_len; ++str_index)
		{
			/* definitely not at location i */
			if (str[i + str_index] != substr[str_index])
				break;
			/* if last letter matches, then we know the whole thing matched */
			else if (str_index == substr_len - 1)
			{
				if(delimiters >= 1)
				{
					i += substr_len -1;
					delimiter[++ind_delim] = i;
					//printf("delimiter[%d] = %d\n", ind_delim, delimiter[ind_delim]);
				}
				else if(delimiters == 0)
					return i;
				else if(delimiters == -1)
					total++;
			}
		}
	}

	if(delimiters == 0)
		return -1;						/* substr not present in str */
	else if(delimiters == -1)
		return total;					/* the total numbers of substr */
	else if(delimiters < ind_delim)
		matchpos = delimiter[delimiters];
	else /*if(delimiters == ind_delim)*/
		matchpos = delimiter[ind_delim];

	return matchpos;
}

/* replaces the first occurence of "in" within the string "source" with
 * the string "new" and returns empty string if no replacement was made
 * limitation ---> if the in is a subset of out and strlen(out) is greater than 
 *						  strlen(in) then we'll got the unexpected result. 
 */
char *replacestr(const char *s, char *in, char *out, int delimiters)
{
	int replace_index;
	int source_len, in_len, out_len, in_idx;
 	int total_len;
	int i = delimiters, delta = 0;
	char *source;
	char *_new;

	if(!strcmp(in ,out))
		return (char *)s;

	in_len = strlen(in);
	out_len = strlen(out);
	delta = abs(in_len - out_len);
	if(i == -2) delimiters = 1;
	in_idx = matchStrPosAt(in, (char *)s, delimiters);
	if(in_len < out_len){
		if(i != -2)
			replace_index = in_idx;
		else
 			replace_index = in_idx - in_len + 1;
#if defined(___DEBUG___)
 		printf("cond1\n");
#endif
 	}
	else if (((in_len > 1) && (out_len > 1) && (delta == 0)) || (in_len > out_len)){
		if(i != -2)
			replace_index = in_idx;
		else
 			replace_index = in_idx - out_len - (delta - 1);
#if defined(___DEBUG___)
 		printf("cond2\n");
#endif
 	}
 	else{
 		replace_index = in_idx;
#if defined(___DEBUG___)
 		printf("cond3\n");
#endif
	}

#if defined(___DEBUG___)
	printf("in_len = %d\tout_len = %d\tdelta = %d\tposition = %d\n", in_len, out_len, delta, replace_index);
#endif

	if (replace_index < 0)
		return "";
	else
	{
		source = StrDup (s);
		source_len = strlen(source);
		total_len = sizeof(char*) * (source_len - in_len + out_len) + 1;
		//total_len = source_len - in_len + out_len + 1;

		/*if(total_len == source_len)
			total_len = total_len + 1;*/

		//if ((*_new = (char *)calloc(total_len,sizeof(char))) == NULL)

		if ((_new = (char *)malloc(sizeof(char*) * total_len + 1)) == NULL)
			//my_perror("malloc", FATAL);
			return NULL;

		(_new)[total_len - 1] = '\0';

		/* copy part before match */
		for (i = 0; i < replace_index; ++i)
			(_new)[i] = source[i];

		/* insert new string */
		for (i = replace_index; i < replace_index + out_len; ++i)
			(_new)[i] = out[i - replace_index];

 		/* copy part after match */
		for (i = replace_index + out_len; i < total_len - 1; ++i)
		(_new)[i] = source[(i - out_len + in_len)];
	}
	StrFree(source);
	source = NULL;
	return _new;
}

/* replaces at most 'max' occurences of "in" within the string "source" with
** the string "out".  A replacement may contain another occurence of 
** "in", in which case that will be replaced before an following occurence
** (i.e., it works left to right) */ 
char *replaceall(const char *source, char *in, char *out, int delimiters, int max)
{
	int i;
	char *_new;

	if (max < 1) return (char *)source;

	_new = replacestr(source, in, out, delimiters);

	for (i = 1; i < max; ++i)
	{
		_new = replacestr(StrDup(_new), in, out, delimiters);
		if (!strcmp(_new,"") || _new == NULL)
			break;
	}
	return _new;
}

unsigned long iptoul(char *ip)
{
	unsigned long ul = 0, t;
	char *p;
	
	do{
		t = strtoul(ip, &p, 0);
		ul = ul * 256 + t;
		ip = p + 1;
	} while (*p == '.');
	
	return ul;
}
	
char *ultoip(unsigned long ul)
{
	static char ip[16];
	char t[16];
	char *tp;
	unsigned long tl;
	ip[0] = '\0';
	
	do{
		tp = t;
		tl = ul % 256;
		ul /= 256;
		if (ul) *t = '.', ++tp;
		sprintf(tp, "%lu", tl);
		strcat(t, ip);
		strcpy(ip, t);
	} while (ul);
	
	return ip;
}

/* ======================(function header)========================
Function Name: int strcutail (char *str, const char *n, int pos)
Description: To remove the sub-string which is starting at 
			 n-th delimiter(or include it) to the end of input string.
Arguments: char *, const char *
Return: Returns the string which was cut the tailed sub-string off.
written by Jackie Xie
Date   :  2011/07/15
================================================================*/
char *strcutail (char *str, const char *n, int pos)
{
	int i, _newStrLen = (pos != 0)?matchStrPosAt(n, str, abs(pos)): -1;
	char *_new;
	
	if(_newStrLen >= 0){
		if (pos >= 0)
			++_newStrLen;			
		if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
			return NULL;

		(_new)[_newStrLen] = '\0';		

		/* copy sub-string from the head of the string to the specified delimiter */
		for (i = 0; i < _newStrLen; ++i)
			(_new)[i] = str[i];

		strcpy(str, _new);
#ifndef TARGET_DEVICE
		StrFree(_new);	
#endif
		_new = NULL;
	}
	return str;
}


/* ======================(function header)========================
Function Name: int strmhead (char *str, const char *n, int pos)
Description: To cut the front of sub-string which is starting at the
			  matched delimiter and ending of the input string.
Arguments: char *, const char *
Return: The string which was cut the front of input string till the 
				specified delimiter.
written by Jackie Xie
Date   :  2011/08/12
================================================================*/
char *strmhead (char *str, const char *n, int pos)
{
	int i, _matchedStrLen = (pos != 0)?matchStrPosAt(n, str, abs(pos)): -1;
	int str_len = strlen(str);
	int _newStrLen = 0;
	
	char *_new;
	
	if(_matchedStrLen >= 0){
		if (pos >= 0)
			_matchedStrLen -= strlen(n);
			
		_newStrLen = str_len - _matchedStrLen;

		if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
			return NULL;

		(_new)[_newStrLen+1] = '\0';		

		/* copy sub-string from specified delimiter */
		for (i = 0; i < _newStrLen; ++i)
			(_new)[i] = str[_matchedStrLen +1 + i];
		strcpy(str, _new);
		StrFree(_new); _new = NULL;
	}
	
	return str;
}

char *index_str (char *str, const char *n, int index)
{
	int i, str_len = strlen(str), delm_len = strlen(n);
	int _newStrLen = 0;
	int idx1, idx2;
	char *_new;
	
	index = abs(index);
	if(index == 0 || index > matchStrPosAt(n, str, -1) + 1)
		return NULL;
	else if(index == 1){
		idx1 = 0;
		idx2 = matchStrPosAt(n, str, index) - delm_len - 1;
	}
	else if(index > 1 && index <= matchStrPosAt(n, str, -1)){
		idx1 = matchStrPosAt(n, str, index - 1) + 1;
		idx2 = matchStrPosAt(n, str, index) - delm_len - 1;
	}
	else if(index == matchStrPosAt(n, str, -1) + 1){
		idx1 = matchStrPosAt(n, str, index - 1) + 1;
		idx2 = 	str_len - 1;
	}
	_newStrLen = idx2 - idx1 + 1;
	if(_newStrLen < 0)
		_newStrLen = str_len - 1;
	if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
		return NULL;
	(_new)[_newStrLen + 1] = '\0';		
	for (i = 0; i <= _newStrLen; ++i)
		(_new)[i] = str[idx1+i];

	strcpy(str, _new);
	//StrFree(_new); _new = NULL;
	return str;
}

char *insert_str(char *str, char *value, char *delm, int index)
{
	int i, idx, tokens;
	int str_len = strlen(str), val_len = strlen(value), delm_len = strlen(delm);
	int _newStrLen = str_len + val_len + delm_len + 1;
	char *_new;

	if(index == 0) return str;
	if(str_len == 0){
		_newStrLen = val_len;
		delm_len = 0;
	}

	index = abs(index);
	tokens = matchStrPosAt(delm, str, -1) + 1;
	idx = matchStrPosAt(delm, str, index-1) + 1;
	if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
		return NULL;
	(_new)[_newStrLen] = '\0';		

	for(i = 0; i <= _newStrLen; i++){
		if(index == 1){
			if(i <= val_len - 1)
				(_new)[i] = value[i];
			else if(i > val_len - 1 && i <= val_len + delm_len - 1 && str_len != 0)
				(_new)[i] = delm[i - val_len];
			else if(i > val_len + delm_len - 1 && str_len != 0)
				(_new)[i] = str[i - val_len - delm_len];
		}
		else if(index > 1 && index <= tokens){
			if(i <= idx -1)
				(_new)[i] = str[i];
			else if(i >= idx && i <= idx + val_len -1)
				(_new)[i] = value[i - idx];
			else if(i > idx + val_len -1 && i < idx + val_len + delm_len)
				(_new)[i] = delm[i - idx - val_len];
			else if(i >= idx + val_len + delm_len)
				(_new)[i] = str[i - val_len - delm_len];
		}
		else if(index > tokens){
			if(i <= str_len - 1)
				(_new)[i] = str[i];
			else if(i > str_len - 1 && i <= str_len + delm_len - 1)
				(_new)[i] = delm[i - str_len];
			else if(i > str_len + delm_len - 1)
				(_new)[i] = value[i - str_len - delm_len];
		}
	}
	str = StrDup(_new);
	StrFree(_new); _new = NULL;
	return str;	
}

/*
────────────────────────────────────────────────
			index		1	2	3		4			5			6				7				8					9					10
────────────────────────────────────────────────
delete					0 11 222 3333 44444 555555 6666666 77777777 888888888 9999999999
────────────────────────────────────────────────
index '0'	: 			(null)
index '1' 	: 			11 222 3333 44444 555555 6666666 77777777 888888888 9999999999
index '2' 	: 			0 222 3333 44444 555555 6666666 77777777 888888888 9999999999
index '3' 	: 			0 11 3333 44444 555555 6666666 77777777 888888888 9999999999
index '4' 	: 			0 11 222 44444 555555 6666666 77777777 888888888 9999999999
index '5' 	: 			0 11 222 3333 555555 6666666 77777777 888888888 9999999999
index '6' 	: 			0 11 222 3333 44444 6666666 77777777 888888888 9999999999
index '7' 	: 			0 11 222 3333 44444 555555 77777777 888888888 9999999999
index '8' 	: 			0 11 222 3333 44444 555555 6666666 888888888 9999999999
index '9' 	: 			0 11 222 3333 44444 555555 6666666 77777777 9999999999
index '10'	: 			0 11 222 3333 44444 555555 6666666 77777777 888888888
index '11'	: 			(null)
────────────────────────────────────────────────
*/
char *delete_str(char *name, char *delm, int index)
{
	int i, idx, idx1, idx2, newpos = 0;
	int name_len = strlen(name), delm_len = strlen(delm);
	int _newStrLen;
	char *_new;

	idx = matchStrPosAt(delm, name, -1);
	//printf("idx = %d\n", idx);
	index = abs(index);
	if(idx <= 0)
		return "";
	else if(index == 0 || idx +1 < index)
		return name;
	else if(index == 1){
		//return strmhead(name, delm, 1);
		idx1 = matchStrPosAt(delm, name, index) + 1;
		_newStrLen = name_len - idx1;
		if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
			return NULL;
		(_new)[_newStrLen + 1] = '\0';
		for(i = 0; i <= _newStrLen; i++)
			(_new)[i] = name[i+idx1];
	}
	else if(index >= idx + 1){
		//return strcutail(name, delm, matchStrPosAt(delm, name, -1));
		idx2 = matchStrPosAt(delm, name, index - 1) - delm_len;
		//printf("idx2 = %d\n", idx2);
		_newStrLen = idx2;
		//_newStrLen = idx2 - delm_len + 1;
		if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
			return NULL;
		(_new)[_newStrLen + 1] = '\0';
		for(i = 0; i <= _newStrLen; i++)
			(_new)[i] = name[i];
	}
	else if(index > 1 && index < idx+1){
		idx1 = matchStrPosAt(delm, name, index - 1) + 1;
		idx2 = matchStrPosAt(delm, name, index);
		_newStrLen = name_len - delm_len - idx2 + idx1 + idx + 2;
		if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
			return NULL;
		(_new)[_newStrLen + 1] = '\0';		
		for(i = 0; newpos <= _newStrLen; i++){
			if(i >= idx1 && idx2 >= i)
				continue;
			else
				(_new)[newpos++] = name[i];
		}
	}
	strcpy(name, _new);
	StrFree(_new); _new = NULL;
	return name;	
}

char *delete_val(char *name, char *value, char *delm)
{
	int i, c;
	char *tmp = NULL, *del = NULL;
	
	c = matchStrPosAt(delm, name, -1) + 1;
	if(c == 0) return "";
	for (i = 1; i <= c ; i++) {
		tmp = StrDup(name);
		del = StrDup(index_str(tmp, delm, i));
		if(!strcmp(del, value)){
			StrFree(tmp); tmp = NULL;
			printf("value %s is matched in %s\n", value, name);
			tmp = StrDup(delete_str(name, delm, i));
			strcpy(name, tmp);
			printf("delete '%s' which is spcified at position %d\n", value, i);
			break;
		}
	}
	StrFree(tmp); StrFree(del); tmp = del = NULL;
	return name;
}

#if 1
char *modify_str(char *name, char *value, char *delm, int index)
{
	int i, idx, idx1, idx2, newpos = 0;
	int name_len = strlen(name), delm_len = strlen(delm), vlen = strlen(value);
	int _newStrLen;
	char *_new;

	idx = matchStrPosAt(delm, name, -1);
	//printf("delete_str : idx = %d\n", idx);
	index = abs(index);
	if(idx <= 0)
		return value;
	else if(index == 0 || idx +1 < index)
		return name;
	else if(index == 1){
		//return strmhead(name, delm, 1);
		idx1 = matchStrPosAt(delm, name, index) - delm_len;
		//printf("%d\n", idx1);
		_newStrLen = name_len - idx1 + vlen + 1 + delm_len;
		if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
			return NULL;
		(_new)[_newStrLen + 1] = '\0';

		newpos = idx1;
		for(i = 0; i <= _newStrLen; i++){
			if(i < vlen)
				(_new)[i] = value[i];
			else
				(_new)[i] = name[++newpos];
		}
	}
	else if(index >= idx + 1){
		//return strcutail(name, delm, matchStrPosAt(delm, name, -1));
		idx2 = matchStrPosAt(delm, name, index);
		//printf("idx2 = %d\n", idx2);
		_newStrLen = idx2 + vlen + 2;
		//_newStrLen = idx2 - delm_len + 1;
		if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
			return NULL;
		(_new)[_newStrLen + 1] = '\0';

		newpos = 0;
		for(i = 0; i <= _newStrLen; i++){
			if(i <= idx2)
				(_new)[i] = name[i];
			else
				(_new)[i] = value[newpos++];
		}
	}
	else if(index > 1 && index < idx+1){
		idx1 = matchStrPosAt(delm, name, index - 1) + 1;
		idx2 = matchStrPosAt(delm, name, index) - delm_len + 1;
		_newStrLen = name_len + idx1 - idx2 + vlen + 1;

		if ((_new = (char *)malloc(sizeof(char*) * _newStrLen + 1)) == NULL)
			return NULL;
		(_new)[_newStrLen + 1] = '\0';

		newpos = 0;
		for(i = 0; i <= _newStrLen; i++){
			if(i < idx1)
				(_new)[i] = name[i];
			else if(i >= idx1 && (idx1 + vlen) > i)
				(_new)[i] = value[newpos++];
			else
				(_new)[i] = name[idx2++];
		}
	}
	//name = strdup(_new);
	strcpy(name, _new);
	//StrFree(_new); _new = NULL;
	return name;	
}
#else
char *modify_str(char *str, char *value, char *delm, int index)
{
#if 1
	char *tmp = StrDup(str);
	tmp = StrDup(delete_str(tmp, delm, index));
	//printf("modify_str : '%s'\n", tmp);
	tmp = StrDup(insert_str(tmp, value, delm, index));
	//printf("modify_str : '%s'\n", tmp);
	str = StrDup(tmp);
	StrFree(tmp);
#else
	char tmp[512], buf[512];

	strcpy(tmp, str);
	strcpy(buf, delete_str(tmp, delm, index));
	strcpy(tmp, insert_str(buf, value, delm, index));
	str = StrDup(tmp);
#endif
	return str;
}
#endif

char *str2digits(char *sval, char *delimiter, int length)
{
	int i, c = 0, find = 0;
	char *val = NULL, *tmp = NULL, *del = NULL;

	c = matchStrPosAt(delimiter, sval, -1) + 1;
	val = StrDup("");
	for(i = 1; i <= c; i++){
		del = StrDup(sval);
		tmp = StrDup(index_str(del, delimiter, i));
		if(strlen(tmp) == strspn(tmp, "0123456789")) {
			tmp = StrDup(insert_str(val, tmp, delimiter, i));
			val = StrDup(tmp);
			find++;
		}
		if(find == length) break;
	}
	sval = StrDup(val);
	StrFree(val); StrFree(tmp); StrFree(del);
	val = tmp = del = NULL;
	return sval;
}

int str2id(char *sval, char *delimiter)
{
	int i, c = 0;
	char *val = NULL, *tmp = NULL, *del = NULL;

	c = matchStrPosAt(delimiter, sval, -1) + 1;
	val = StrDup("");
	for(i = 1; i <= c; i++){
		del = StrDup(sval);
		tmp = index_str(del, delimiter, i);
		if(strlen(tmp) == strspn(tmp, "0123456789")) {
			tmp = insert_str(val, tmp, "", i);
			val = StrDup(tmp);
		}
	}
	//StrFree(val); StrFree(tmp); StrFree(del);
	//val = tmp = del = NULL;
	return atoi(val);
}

int val_exist(char *name, char *value, char *delm)
{
	int i, c, ret = 0;
	char *tmp, *id;
	
	c = matchStrPosAt(delm, name, -1) + 1;
	//if(c == 1) return ret;
	for (i = 1; i <= c ; i++) {
		tmp = StrDup(name);
		id = StrDup(index_str(tmp, delm, i));
		if(!strcmp(id, value)){
			ret = i;
			break;
		}
	}
	//StrFree(tmp);
	return ret;
}

char *get_one_line(char *s, int n, FILE *f)
{
	char *ptr = s;
	char buf[1];
	int bread;

 	if (n <= 0) return NULL;
	while (--n)
	{
		if(!((bread = fread(buf, sizeof(char), sizeof(char), f)) > 0))
		{
			if (ptr == s)  return NULL;
			break;
		}
	 
		if ((*ptr++ = *buf) == '\n') break;
	}
	 
	*ptr = '\0';
	return s;
}

int mac_validator(const char* value) {
	/* validate MAC format: XX:XX:XX:XX:XX:XX or XXXXXXXXXXXX
	 * XX is valid haxadecimal number (case insensitive)
	 */
	const char* q;
	int cnt = 5;
	if (value == NULL)
		return -9;
	q = value;
	for (; *q; q++) {
		if(strstr(q, ":")){
			if (*q == ':') {
				--cnt;
				if ((q - value) % 3 != 2) {
					return -1;
				}
			} 
		}
		else if (*q >= 'A' && *q <= 'F') {
		} else if (*q >= 'a' && *q <= 'f') {
		} else if (*q >= '0' && *q <= '9') {
		} else
			return -2;
	}
	if(strstr(q, ":"))
		if ((q - value) != 17)
			return -3;
	if(cnt == 0 || cnt == 5)
		return 0;
	else
		return -1;
}
