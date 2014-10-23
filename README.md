tznconfig
=========

Setup system date time by local time zone name



☆ Introduction

	# Setup system date time by local time zone name.

	Introduction
		A utility to setup system time zone.


	Details

		In POSIX systems, a user can specify the time zone by means of the TZ environment variable. For information about how to set environment variables, see section Environment Variables. The functions for accessing the time zone are declared in `time.h'.

		You should not normally need to set TZ. If the system is configured properly, the default time zone will be correct. You might set TZ if you are using a computer over a network from a different time zone, and would like times reported to you in the time zone local to you, rather than what is local to the computer.

		In POSIX.1 systems the value of the TZ variable can be in one of three formats. With the GNU C library, the most common format is the last one, which can specify a selection from a large database of time zone information for many regions of the world. The first two formats are used to describe the time zone information directly, which is both more cumbersome and less precise. But the POSIX.1 standard only specifies the details of the first two formats, so it is good to be familiar with them in case you come across a POSIX.1 system that doesn't support a time zone information database.

		The first format is used when there is no Daylight Saving Time (or summer time) in the local time zone:

			std offset

		The std string specifies the name of the time zone. It must be three or more characters long and must not contain a leading colon, embedded digits, commas, nor plus and minus signs. There is no space character separating the time zone name from the offset, so these restrictions are necessary to parse the specification correctly.

		The offset specifies the time value you must add to the local time to get a Coordinated Universal Time value. It has syntax like [+|-]hh[:mm[:ss]]. This is positive if the local time zone is west of the Prime Meridian and negative if it is east. The hour must be between 0 and 23, and the minute and seconds between 0 and 59.

		The minutes ( mm) and seconds ( ss) are optional. The hour ( hh) shall be required and may be a single digit. The offset following std shall be required. If no offset follows dst, the alternative time is assumed to be one hour ahead of standard time. One or more digits may be used; the value is always interpreted as a decimal number. The hour shall be between zero and 24, and the minutes (and seconds)-if present-between zero and 59. The result of using values outside of this range is unspecified. If preceded by a '-', the timezone shall be east of the Prime Meridian; otherwise, it shall be west (which may be indicated by an optional preceding '+' ).

		For example, here is how we would specify Eastern Standard Time, but without any Daylight Saving Time alternative:

		EST+5：The second format is used when there is Daylight Saving Time:

		std offset dst offset?,start[/time],end[/time]

		The initial std and offset specify the standard time zone, as described above. The dst string and offset specify the name and offset for the corresponding Daylight Saving Time zone; if the offset is omitted, it defaults to one hour ahead of standard time.

		The remainder of the specification describes when Daylight Saving Time is in effect. The start field is when Daylight Saving Time goes into effect and the end field is when the change is made back to standard time. The following formats are recognized for these fields:

			"FST+3FDT02:00,J60/00,J304/02"

		This specification describes a fictitious zone that lies three hours east of GMT. It makes a two hour shift forward for daylight savings which begins on March 1st at midnight, and ends on October 31st at 2am. The 'J' designation in the start/end specs signifies that counting starts at one and February 29th is never counted.

			"FST+3FDT,59,304"

		This specification is significant because of the '59'. The lack of 'J' for the start and end dates, indicates that the Julian day-count begins at zero and ends at 365. If you do the math, you'll see that allows for a total of 366 days. This is fine in leap years, but in non-leap years '59' (Feb-29) does not exist.

		Jn：This specifies the Julian day, with n between 1 and 365. February 29 is never counted, even in leap years.

		n：This specifies the Julian day, with n between 0 and 365. February 29 is counted in leap years.

		Mm.w.d：This specifies day d of week w of month m. The day d must be between 0 (Sunday) and 6. The week w must be between 1 and 5; week 1 is the first week in which day d occurs, and week 5 specifies the last d day in the month. The month m should be between 1 and 12.

		The time fields specify when, in the local time currently in effect, the change to the other time occurs. If omitted, the default is 02:00:00.

		For example, here is how you would specify the Eastern time zone in the United States, including the appropriate Daylight Saving Time and its dates of applicability. The normal offset from UTC is 5 hours; since this is west of the prime meridian, the sign is positive. Summer time begins on the first Sunday in April at 2:00am, and ends on the last Sunday in October at 2:00am.

			EST+5EDT,M4.1.0/2,M10.5.0/2


	Feature

		．Setup system datetime by local time zone name which is encoded according to IEEE 1003.1 (POSIX) in C.
		．This utility could be compile and run in Mac OS X/Linux.



☆ HowTo 
 
	#A Guide for building the application.

	Building/Compiling Procedure
		$ git clone https://github.com/JackieXie168/tznconfig.git
		$ cd tznconfig
		$ ./autogen.sh && make

	Install the application
		$ sudo make install

	How to run the application

	on Mac OS X
		$ xnvram import nvram/conf/timezone-2013.conf
		$ sudo tznconfig -x BRT3BRST,M10.3.0/0,M2.3.0/0
		$ sudo tznconfig -x `xnvram get Pacific/Marquesas`

	on Linux
		$ nvram import nvram/conf/timezone-2013.conf
		$ sudo tznconfig -x BRT3BRST,M10.3.0/0,M2.3.0/0
		$ sudo tznconfig -x `nvram get Pacific/Marquesas`

