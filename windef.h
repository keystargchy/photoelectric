#ifndef _WINDEF_H_
#define _WINDEF_H_

typedef unsigned char BOOL;

#ifndef TRUE
#define TRUE 	1
#endif

#ifndef FALSE
#define FALSE 	0
#endif


/* Word control code */
#define WD_NONE()           "\033[0m"     /* Close all property */
#define WD_HIGHLIGHT()      "\033[1m"     /* High light */
#define WD_UNDERLINE()      "\033[4m"     /* Under line */
#define WD_FLICKER()        "\033[5m"     /* Flicker */
#define WD_RVSPRN()         "\033[7m"     /* Reverse display */
#define WD_FADE()           "\033[8m"     /* Fade */

/* words colour */
#define FG_BLACK()          "\033[30m"    /* Black */
#define FG_RED()            "\033[31m"    /* Red */
#define FG_GREEN()          "\033[32m"    /* Green */
#define FG_YELLOW()         "\033[33m"    /* Yellow */
#define FG_BLUE()           "\033[34m"    /* Blue */
#define FG_PURPLE()         "\033[35m"    /* Purple */
#define FG_DGREEN()         "\033[36m"    /* Dark Green */
#define FG_WHITE()          "\033[37m"    /* White */

#define Err_log(fmt, arg...)    do{\
		printf(FG_RED()"%s: %d "fmt, __FUNCTION__, __LINE__, ##arg);    \
		printf(WD_NONE());			   \
	}while(0)

#endif
