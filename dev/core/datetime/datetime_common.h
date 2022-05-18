/**
 * \file datetime_common.h
 *
 * \brief Common definitions for datetime and delegate
 */

#ifndef DATETIME_COMMON_H_FILE
#define DATETIME_COMMON_H_FILE

/***************************************************************************************************
 * Includes
 **************************************************************************************************/

#include "std_types.h"

/***************************************************************************************************
 * Definitions
 **************************************************************************************************/

#define DATETIME_EPOCH_YEAR      1970UL // unix epoch year
#define DATETIME_EPOCH_LEAP_YEAR 1972UL // first leap year
#define DATETIME_MAX_YEAR        2105UL // maximum supported year

#define DATETIME_DAYS_IN_NORMAL_YEAR (365U)
#define DATETIME_DAYS_IN_LEAP_YEAR   ((DATETIME_DAYS_IN_NORMAL_YEAR) + 1U)

#define DATETIME_SECS_PER_DAY    86400UL
#define DATETIME_SECS_PER_HOUR   3600UL
#define DATETIME_SECS_PER_MINUTE 60UL

#define DATETIME_MS_PER_SEC  (1000UL)
#define DATETIME_MS_PER_MIN  (60000UL)
#define DATETIME_MS_PER_HOUR (3600000UL)

#define DATETIME_INVALID_UTC_TSTAMP (0xFFFFFFFF)

typedef struct
{
    uint8 second; /**< [0 .. 59] Seconds */
    uint8 minute; /**< [0 .. 59] Minutes */
    uint8 hour;   /**< [0 .. 23] Hours  */

    uint8  day;   /**< [1 .. 31] Day in Month */
    uint8  month; /**< [1 .. 12] Month in Year */
    uint16 year;  /**< Year (typically 1970+) */

} t_datetime_date;

typedef struct
{
    t_datetime_date date;
    uint16          ms;
} t_datetime;

typedef void (*t_datetime_alarm)(t_datetime *date);

#define t_datetime_date_init_zero                                            \
    {                                                                        \
        .second = 0, .minute = 0, .hour = 0, .day = 0, .month = 0, .year = 0 \
    }

#define t_datetime_init_zero                       \
    {                                              \
        .date = t_datetime_date_init_zero, .ms = 0 \
    }

#endif /* DATETIME_COMMON_H_FILE */
