/*
 * xutils.h
 *
 * Created by Jackie Xie on 2011-08-20.
 * Copyright 2011 Jackie Xie. All rights reserved.
 *
 */

#ifndef __X_UTILS_H__
#define __X_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <poll.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#if __linux__
#include <sys/types.h>
#include <sys/stat.h>
#endif

//#define ___DEBUG___

#define swap(a,b) {		\
		(a) += (b); 			\
		(b) = (a) - (b);		\
		(a) -= (b);				\
}

/* Define the basic constants used */
#define D_TRUE 1
#define D_FALSE 0
#define D_NULL ( ( void * ) 0 )
/* D_UNUSED is a useful macro to avoid warnings about unused parameters */
#define D_UNUSED(X) ( ( void ) (X) )

#if defined(WIN32) && !defined(__GNUC__) && !defined(__MWERKS__)
#define D_UNLINK(X) _unlink(X)
#else
#define D_UNLINK(X) unlink(X)
#endif

#if defined(__FreeBSD__) || defined(__APPLE__) || defined(MACOSX) || defined(darwin)
#define GETDELIM_CHUNKSIZE (64)
ssize_t getdelim(char **lineptr, size_t *n, int delim, FILE *stream);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
#endif

#define INVALID 	1
#define TOOSMALL 	2
#define TOOLARGE 	3

#define IP_QUAD(ip)  (ip)>>24,((ip)&0x00ff0000)>>16,((ip)&0x0000ff00)>>8,((ip)&0x000000ff)
/*
printf("My IP Address (0x%x) is %d.%d.%d.%d\n", my_ip, IP_QUAD(my_ip));
*/
static int  rgiState[57]; // leave this alone

/* The following typedefs allow portable use of basic data types */
typedef unsigned char D_BOOL;
typedef unsigned char D_U8;
typedef signed char D_I8;
typedef unsigned short D_U16;
typedef signed short D_I16;
typedef unsigned long D_U32;
typedef signed long D_I32;
#if defined(WIN32) && !defined(__GNUC__)
/* At least near and far are things of the past. */
typedef unsigned __int64 D_U64;
typedef signed __int64 D_I64;
#else
typedef unsigned long long D_U64;
typedef signed long long D_I64;
#endif
typedef char * D_VSTR;
typedef char D_VCHR;
typedef const char * D_CSTR;
typedef unsigned long D_SIZE;
typedef unsigned long D_ADDR;

typedef D_U32 D_TIME;

/** Defines a parameter. */
typedef struct OAMC_PARAM_TAG
{
    struct OAMC_PARAM_TAG *pNext;
    D_VSTR Name;
    D_U32 NameHash;
    D_VSTR StrValue;
    D_I32 Value;
} OAMC_PARAM;

#ifndef NVRAM
#define NVRAM
#endif
#ifdef NVRAM
#include <arpa/inet.h>

extern int nvram_invmatch(char *name, char *match);
extern int nvram_match(char *name, char *match);
extern int nvram_commit(void);
extern char *nvram_get(const char *name);
extern int nvram_set(const char *name, const char *val);
extern int nvram_unset(const char *s);

#endif

typedef enum {Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday} Days;
typedef enum {First, Second, Third, Fourth, Fifth} Weeks;
typedef enum {January = 1, February, March, April, May, June, July, August, September, October, November, December} Months; 	

typedef struct {
	int hour;
	int minute;
	int second;
} Time;

typedef struct {
	Time time;
	Days day;
	Weeks week;
	Months month;
	int year;
	time_t ptime;
} datetime;

long long strtonum(const char *, long long, long long, const char **);
char* itos (int);
int isLetters(const char *text);
time_t _time(time_t *);
time_t get_time(time_t *);
bool isLeapYear (int year);
bool IsLeapYear(int year)	;
Days get_day_code (int year);
int dayOfMonth(datetime dt);
int date2jds(char* datetime);
int jds2date(int days, char* datetime);
int jds2mds(int days, char* dst);
int days2date(int Days, char* dst);
int date2days(char* date);
time_t str2datetime(char *datetime);
int getTZ(char **tz);
int setTZ(char *timezone);
int setTZName(char *tzname);
int number_gen(void);
int number_range(int, int);
void init_gen( );
void random_string(char *string, const unsigned int length);
void Shell_Sort( int *, int);
int lowercase (char *sPtr);
int uppercase (char *sPtr);
int xtrim(char *);
char *ltrim(char *const s);
char *rtrim(char *const s);
char *trim(char *const s);
char *GetStrBetweenStr(char *, char *, char *);
char *StrDup(const char *s);
void StrFree(void *);
char *StrCat(char *, const char *);
int matchStrPosAt(const char *, char *, int);
char* replacestr(const char* s, char* in, char* out, int delimiters);
char* replaceall(const char * source, char * in, char * out, int delimiters, int max);
unsigned char* ConvertStringIntoByte(char *pszStr, unsigned char* pbyNum);
int xasprintf(char **bufp, const char *format, ...);
char *readin(FILE *in);
int copyFile(const char *Fin, const char *Fout);
int getFileSize(char* filename, long *filesize);
unsigned long iptoul(char *ip);
char * ultoip(unsigned long ul);
char *strcutail (char *str, const char *n, int pos);
char *strmhead (char *str, const char *n, int pos);
char *index_str (char *str, const char *n, int index);
char *insert_str(char *str, char *value, char *delm, int index);
char *delete_str(char *name, char *delm, int index);
char *delete_val(char *name, char *value, char *delm);
char *modify_str(char *str, char *value, char *delm, int index);
int val_exist(char *name, char *value, char *delm);
char *str2digits(char *sval, char *delimiter, int length);
int str2id(char *sval, char *delimiter);
char *get_one_line(char *s, int n, FILE *f);
char *cat_file(char *name);
int mac_validator(const char* value);
u_char parse_pfxlen(const char *, in_addr_t *);
in_addr_t	 parse_net(const char *);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
