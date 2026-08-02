#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void) {

    /*
    two main types for date in c
    - time_t:   a real type capable of holding a time. 
                So by the spec, this could be a floating type or integer type. 
                In POSIX (Unix-likes), it’s an integer. This holds calendar time. 
                Which you can think of as UTC time.
    
    - strct tm: holds the components of a calendar time. This is a broken-down time, i.e. the components of the time, 
                like hour, minute, second, day, month, year, etc.
    */

    /*
    struct tm {
    int tm_sec;    // seconds after the minute -- [0, 60]
    int tm_min;    // minutes after the hour -- [0, 59]
    int tm_hour;   // hours since midnight -- [0, 23]
    int tm_mday;   // day of the month -- [1, 31]
    int tm_mon;    // months since January -- [0, 11]
    int tm_year;   // years since 1900
    int tm_wday;   // days since Sunday -- [0, 6]
    int tm_yday;   // days since January 1 -- [0, 365]
    int tm_isdst;  // Daylight Saving Time flag
};
    */

    // current time
    time_t now;
    now = time(NULL);
    time(&now);

    printf("%s", ctime(&now));

    // string in following format
    // Sun Feb 28 18:47:25 2021

    // you can also convert time_t to struct tm

    printf("Local: %s", asctime(localtime(&now)));
    printf("  UTC: %s", asctime(gmtime(&now)));


    {
        
    // initially in local time
    struct tm some_time = {
        .tm_year = 82,
        .tm_mon = 3,
        .tm_mday = 12,
        .tm_hour = 12,
        .tm_min = 0,
        .tm_sec = 4,
        .tm_isdst = -1};

        time_t some_time_epoch;

        some_time_epoch = mktime(&some_time);

        printf("%s", ctime(&some_time_epoch));
        printf("Is DST: %d\n", some_time.tm_isdst);
    }

    {
        time_t now = time(NULL);
        struct tm *local = localtime(&now);
        struct tm *utc = gmtime(&now);

        printf("Local time: %s", ctime(&now)); // local time with time_t
        printf("Local time: %s", asctime(local)); // local time with struct tm (pritns the same as utc for me)
        printf("UTC:      : %s", asctime(utc)); // UTC with a struct tm


    }

    {
        char s[128];
        time_t now = time(NULL);

        // %c: print date as per current locale
        strftime(s, sizeof s, "%c", localtime(&now));
        puts(s);

        // %A: full weekday name
        // %B: full month name
        // %d: day of the month

        strftime(s, sizeof s, "%A, %B, %d", localtime(&now));
        puts(s);

        // %I: hour (12 hour clock)
        // %M: minute
        // %S: second
        // %p: AM or PM
        strftime(s, sizeof s, "It's %I:%M:%S %p", localtime(&now));
        puts(s);   // It's 10:29:00 PM

        // %F: ISO 8601 yyyy-mm-dd
        // %T: ISO 8601 hh:mm:ss
        // %z: ISO 8601 time zone offset
        strftime(s, sizeof s, "ISO 8601: %FT%T%z", localtime(&now));
        puts(s);   // ISO 8601: 2021-02-28T22:29:00-0800
    }

    // if you wanna get more resolution
    {
        /*
          struct timespec {
            time_t tv_sec;   // Seconds
            long   tv_nsec;  // Nanoseconds (billionths of a second)
        };
        */
        
        
        struct timespec ts;
        timespec_get(&ts, TIME_UTC);

        printf("%ld s, %ld ns\n", ts.tv_sec, ts.tv_nsec);

        double float_time = ts.tv_sec + ts.tv_nsec/1000000000.0;
        printf("%f seconds since epoch\n", float_time);

    }

    // diff between 2 dates
    {
         struct tm time_a = {
        .tm_year=82,   // years since 1900
        .tm_mon=3,     // months since January -- [0, 11]
        .tm_mday=12,   // day of the month -- [1, 31]
        .tm_hour=4,    // hours since midnight -- [0, 23]
        .tm_min=00,    // minutes after the hour -- [0, 59]
        .tm_sec=04,    // seconds after the minute -- [0, 60]
        .tm_isdst=-1,  // Daylight Saving Time flag
    };

    struct tm time_b = {
        .tm_year=120,  // years since 1900
        .tm_mon=10,    // months since January -- [0, 11]
        .tm_mday=15,   // day of the month -- [1, 31]
        .tm_hour=16,   // hours since midnight -- [0, 23]
        .tm_min=27,    // minutes after the hour -- [0, 59]
        .tm_sec=00,    // seconds after the minute -- [0, 60]
        .tm_isdst=-1,  // Daylight Saving Time flag
    };

    time_t cal_a = mktime(&time_a);
    time_t cal_b = mktime(&time_b);

    double diff = difftime(cal_b,cal_a);

    double years = diff / 60 /60 /24 /365.2425; // close enough

    printf("%f seconds (%f years) between events\n", diff, years);
    }


    return 0;

}