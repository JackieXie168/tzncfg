/*
 * tznconfig.c
 *
 * Created by Jackie Xie on 2011-08-18.
 * Copyright 2011 Jackie Xie. All rights reserved.
 *
 */

#include <errno.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <xutils/xutils.h>

int main(int argc, char *argv[])
{
	struct timeval tv;
	char help_msg[1024] = "Usage: tzconfig [-s <timezone>] [-x <timezone name is encoded according to IEEE 1003.1 (POSIX)>] [-d yyyy-mm-dd] [-j <days of this year>] [-m <[J]<days of this year>] [-i] [-h]\n\n";
	char *tz, *tzname, *date, dst[64], *tmp;
	int c, days;

	strcat(help_msg, "tzconfig command summary\n");
	strcat(help_msg, "\ttzconfig is a function to setup/get timezone infomation.\n");
	strcat(help_msg, "\t-s：setup datetime for specified timezone.\n");
	strcat(help_msg, "\t\t<timezone>(input format)：'<+ | ->xx:xx' (x=[0~9]).\n");
	strcat(help_msg, "\t-x：setup datetime for specified timezone name.\n");
	strcat(help_msg, "\t\t<timezone name>(input format)：'std offset dst [offset],start[/time],end[/time]' .\n");
	strcat(help_msg, "\t-d：To convert date to days.\n");
	strcat(help_msg, "\t-j：To convert days to date.\n");
	strcat(help_msg, "\t\t leap year : 0 ~ 365 \n");
	strcat(help_msg, "\t\t otherwise : 1 ~ 365 \n");
	strcat(help_msg, "\t-m：To convert 'Jn' to 'Mm.w.d' format. \n");
	strcat(help_msg, "\t\t where 'Jn' is specifies the Julian day, with n between 1 and 365. February 29 is never counted, even in leap years. \n");
	strcat(help_msg, "\t\t where 'n' is the days of this year which means the Julian day, with n between 0 and 365. February 29 is counted in leap years. \n");
	strcat(help_msg, "\t-i：To get the local timezone information.\n");
	strcat(help_msg, "\t-h：To show this help message.\n");

	if(argc <= 1 || ((isgraph(*argv[1]) || ispunct(*argv[1])) && *argv[1]!='-'))
		fprintf(stderr, "%s", help_msg);

	while ((c = getopt(argc, argv, "S:X:s:x:D:d:J:j:M:m:I0:i0:h0:H0:?0:0:-")) != -1){
		switch (c) {
			case 'S':
			case 's':
				tz = optarg;
				setTZ(tz);
				break;
			case 'X':
			case 'x':
				tzname = optarg;
				setTZName(tzname);
				break;
			case 'D':
			case 'd':
				date = optarg;
				days = date2days(date);
				printf("%d\n", days);
				break;
			case 'J':
			case 'j':
				days = atoi(optarg);
				date = StrDup("yyyy-mm-dd");
				days2date(days, date);
				printf("%s\n", date);
				StrFree(date);
				break;
			case 'I':
			case 'i':
				getTZ(&tz);
				printf("本地時區是：UTC%s\n", tz);
				break;
			case 'M':
			case 'm':
				tmp = StrDup(optarg);
				if(strstr(tmp, "J")){
					days = atoi(strmhead(tmp, "J", -1));
					if(days != 59){
						days = days2date(days, dst);
						jds2mds(days, dst);
					}
					else
						printf("Invalid !!!\n");
				}
				else{
					days = atoi(optarg);
					/* step 1 : to find the date by days */
					days = days2date(days, dst);
					/* step 2 : to find the result by Julian day & date */
					jds2mds(days, dst);
				}
				printf("%s\n", dst);
				StrFree(tmp);
				break;
			case 'H':
			case 'h':
			case '?':
			default:
				fprintf(stderr, "%s", help_msg);
				exit(0);
				break;
		}
	}

	return 0;
}
