#include <stdlib.h>

#include "datetime.h"
#include "std_types.h"

/***************************************************************************************************
 * Definitions
 **************************************************************************************************/

#define _SECS_PER_MINUTE 60UL
#define _SECS_PER_HOUR   3600UL
#define _SECS_PER_DAY    86400UL

/***************************************************************************************************
 * Data
 **************************************************************************************************/

static const uint8 _datetime_days_in_month[2][12] = {
    {31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U},
    {31U, 29U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U}};

/***************************************************************************************************
 * Internal Functions
 **************************************************************************************************/

static inline uint8 _is_leap_year(uint16 year);

static inline uint16 _days_in_year(uint16 year);

static void _add_year(t_datetime *datetime);

static void _add_month(t_datetime *datetime);

static void _add_day(t_datetime *datetime);

static void _add_hour(t_datetime *datetime);

static void _add_minute(t_datetime *datetime);

static void _add_second(t_datetime *datetime);

static void _sub_year(t_datetime *datetime);

static void _sub_month(t_datetime *datetime);

static void _sub_day(t_datetime *datetime);

static void _sub_hour(t_datetime *datetime);

static void _sub_minute(t_datetime *datetime);

static void _sub_second(t_datetime *datetime);

/***************************************************************************************************
 * Date and Alarm
 **************************************************************************************************/

void datetime_get_defaults(t_datetime *datetime)
{
    if (datetime == NULL_PTR)
    {
        return;
    }

    datetime->ms          = 0U;
    datetime->date.year   = 2018;
    datetime->date.month  = 5;
    datetime->date.day    = 22;
    datetime->date.hour   = 11;
    datetime->date.minute = 48;
    datetime->date.second = 0;
}

void datetime_get_minimum_default(t_datetime *datetime)
{
    if (datetime == NULL_PTR)
    {
        return;
    }

    datetime->date.year   = DATETIME_EPOCH_YEAR;
    datetime->date.month  = 1U;
    datetime->date.day    = 1U;
    datetime->date.hour   = 0U;
    datetime->date.minute = 0U;
    datetime->date.second = 0U;
    datetime->ms          = 0U;
}

void datetime_get_maximum_default(t_datetime *datetime)
{
    if (datetime == NULL_PTR)
    {
        return;
    }

    datetime->ms          = 999U;
    datetime->date.year   = DATETIME_MAX_YEAR;
    datetime->date.month  = 12U;
    datetime->date.day    = 31U;
    datetime->date.hour   = 23U;
    datetime->date.minute = 59U;
    datetime->date.second = 59U;
}

boolean datetime_is_equal(t_datetime *a, t_datetime *b, boolean ignore_ms)
{
    boolean ret = TRUE;

    if ((a == NULL_PTR) || (b == NULL_PTR))
    {
        ret = FALSE;
    }

    if (ret == TRUE)
    {
        ret = ((!ignore_ms) && (a->ms != b->ms)) ? FALSE : ret;
        ret = ((ret == TRUE) && (a->date.second != b->date.second)) ? FALSE : ret;
        ret = ((ret == TRUE) && (a->date.minute != b->date.minute)) ? FALSE : ret;
        ret = ((ret == TRUE) && (a->date.hour != b->date.hour)) ? FALSE : ret;
        ret = ((ret == TRUE) && (a->date.day != b->date.day)) ? FALSE : ret;
        ret = ((ret == TRUE) && (a->date.month != b->date.month)) ? FALSE : ret;
        ret = ((ret == TRUE) && (a->date.year != b->date.year)) ? FALSE : ret;
    }

    return ret;
}

std_return_type datetime_later_than(
    t_datetime *a,
    t_datetime *b,
    boolean *   a_later_than_b,
    boolean     ignore_ms)
{
    boolean ret = STD_OK;

    if ((a == NULL_PTR) || (b == NULL_PTR) || (a_later_than_b == NULL_PTR))
    {
        // not performing datetime_is_valid checks (!)
        ret = STD_NOT_OK;
    }

    if (ret == STD_OK)
    {
        *a_later_than_b = FALSE;

        if (a->date.year > b->date.year)
        {
            *a_later_than_b = TRUE;
            return ret;
        }
        else
        {
            if (a->date.year != b->date.year)
            {
                return ret;
            }
        }
        if (a->date.month > b->date.month)
        {
            *a_later_than_b = TRUE;
            return ret;
        }
        else
        {
            if (a->date.month != b->date.month)
            {
                return ret;
            }
        }
        if (a->date.day > b->date.day)
        {
            *a_later_than_b = TRUE;
            return ret;
        }
        else
        {
            if (a->date.day != b->date.day)
            {
                return ret;
            }
        }
        if (a->date.hour > b->date.hour)
        {
            *a_later_than_b = TRUE;
            return ret;
        }
        else
        {
            if (a->date.hour != b->date.hour)
            {
                return ret;
            }
        }
        if (a->date.minute > b->date.minute)
        {
            *a_later_than_b = TRUE;
            return ret;
        }
        else
        {
            if (a->date.minute != b->date.minute)
            {
                return ret;
            }
        }
        if (a->date.second > b->date.second)
        {
            *a_later_than_b = TRUE;
            return ret;
        }
        else
        {
            if (a->date.second != b->date.second)
            {
                return ret;
            }
        }

        if (!ignore_ms)
        {
            if (a->ms > b->ms)
            {
                *a_later_than_b = TRUE;
                return ret;
            }
            else
            {
                if (a->ms != b->ms)
                {
                    return ret;
                }
            }
        }
    }

    return ret;
}

/***************************************************************************************************
 * UTC conversion / calendar / validation functions
 **************************************************************************************************/

static inline uint8 _is_leap_year(uint16 year)
{
    // precise until about 2106
    // if( !((year) % 4U) && (((year) % 100U) || !((year) % 400U)) )
    // precise enough - good until 2105
    if (!(year % 4) && !(year == 2100U))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static inline uint16 _days_in_year(uint16 year)
{
    if (_is_leap_year(year))
    {
        return DATETIME_DAYS_IN_LEAP_YEAR;
    }
    else
    {
        return DATETIME_DAYS_IN_NORMAL_YEAR;
    }
}

boolean datetime_date_is_valid(t_datetime_date *date)
{
    boolean is_valid = TRUE;

    if (date == NULL_PTR)
    {
        // returning immediately due to pointer checks
        return FALSE;
    }

    // Make sure time is valid
    if (is_valid && ((date->second >= 60) || (date->minute >= 60) || (date->hour >= 24)))
    {
        is_valid = FALSE;
    }

    // Make sure month and date is valid. The rtc_calendar type uses '1' for the first
    // day of the month as well as '1' for january (does not match ATMEL calendar type).
    if (is_valid
        && ((date->month > 12) || (date->month < 1) || (date->day > 31) || (date->day < 1)))
    {
        is_valid = FALSE;
    }

    // Make sure days in month are not more than it should be
    if (is_valid
        && (date->day > _datetime_days_in_month[_is_leap_year(date->year)][date->month - 1]))
    {
        is_valid = FALSE;
    }

    // Make sure year is not earlier than 1970 and before 2106
    if (is_valid && ((date->year < DATETIME_EPOCH_YEAR) || (date->year >= 2106)))
    {
        is_valid = FALSE;
    }

    return is_valid;
}

boolean datetime_is_valid(t_datetime *datetime)
{
    boolean is_valid = TRUE;
    // boolean date_later_than_max = FALSE;
    // boolean min_later_than_date = FALSE;

    if (datetime == NULL_PTR)
    {
        // returning immediately due to pointer checks
        return FALSE;
    }

    if (is_valid && (datetime->ms >= 1000))
    {
        is_valid = FALSE;
    }

    if (is_valid && !datetime_date_is_valid(&datetime->date))
    {
        is_valid = FALSE;
    }

    return is_valid;
}

std_return_type datetime_to_timestamp(t_datetime *datetime, uint32 *tstamp_utc, uint32 *tstamp_ms)
{
    std_return_type ret = STD_OK;

    uint8  date_month = 0;
    uint16 date_year  = 0;

    uint16 years_l = 0;
    uint16 years_n = 0;

    if ((datetime == NULL_PTR) || (tstamp_utc == NULL_PTR))
    {
        ret = STD_NOT_OK;
    }

    if (ret == STD_OK)
    {
        ret = datetime_is_valid(datetime) ? STD_OK : STD_NOT_OK;
    }

    if (ret == STD_OK)
    {
        *tstamp_utc = 0;
        date_month  = datetime->date.month;
        date_year   = datetime->date.year;

        // Add number of seconds elapsed in current month
        *tstamp_utc += datetime->date.second + ((datetime->date.day - 1) * DATETIME_SECS_PER_DAY)
            + (datetime->date.hour * DATETIME_SECS_PER_HOUR)
            + (datetime->date.minute * DATETIME_SECS_PER_MINUTE);

        while (date_month > 1)
        {
            date_month--;
            // Add number of seconds in months of current year
            *tstamp_utc += _datetime_days_in_month[_is_leap_year(date_year)][date_month - 1]
                * DATETIME_SECS_PER_DAY;
        }

        // this thing is at least 48 loops
        // while (date_year > DATETIME_EPOCH_YEAR) {
        //     date_year--;
        //     // Add number of seconds in all years since epoch year
        //     *tstamp_utc += _days_in_year (date_year) * DATETIME_SECS_PER_DAY;
        // }

        // faster version using multiplication assuming % 4 holds for leap years (until 2105)
        years_l = 0;
        years_l += (date_year > DATETIME_EPOCH_LEAP_YEAR)
            ? (date_year - DATETIME_EPOCH_LEAP_YEAR) >> 2
            : 0;
        years_l += (date_year > DATETIME_EPOCH_LEAP_YEAR) ? 1 : 0;
        // we're not allowed to count the current year if it is a leap year
        years_l -= (_is_leap_year(date_year) && (date_year != DATETIME_EPOCH_LEAP_YEAR)) ? 1 : 0;
        // and the year 2100 is not a leap year and thus does not fulfill the %4 rule
        years_l -= (date_year >= 2100U) ? 1 : 0;

        years_n = date_year - DATETIME_EPOCH_YEAR - years_l;

        *tstamp_utc += years_n * DATETIME_SECS_PER_DAY * DATETIME_DAYS_IN_NORMAL_YEAR;
        *tstamp_utc += years_l * DATETIME_SECS_PER_DAY * DATETIME_DAYS_IN_LEAP_YEAR;

        // copy milliseconds into separate parameter if available
        if (tstamp_ms != NULL_PTR)
        {
            *tstamp_ms = datetime->ms;
        }
    }

    return ret;
}

std_return_type datetime_from_timestamp(
    t_datetime *  datetime,
    const uint32 *tstamp_utc,
    const uint32 *tstamp_ms)
{
    std_return_type ret = STD_OK;
    uint32          day_number;
    uint32          day_clock;
    uint16          days_in_year;

    if ((datetime == NULL_PTR) || (tstamp_utc == NULL_PTR))
    {
        ret = STD_NOT_OK;
    }

    if (ret == STD_OK)
    {
        datetime->date.year  = DATETIME_EPOCH_YEAR;
        datetime->date.month = 0;

        day_clock  = *tstamp_utc % DATETIME_SECS_PER_DAY;
        day_number = *tstamp_utc / DATETIME_SECS_PER_DAY;

        datetime->date.second = (day_clock % DATETIME_SECS_PER_MINUTE);
        datetime->date.minute = (day_clock % DATETIME_SECS_PER_HOUR) / DATETIME_SECS_PER_MINUTE;
        datetime->date.hour   = (day_clock / DATETIME_SECS_PER_HOUR);
        // datetime->date.dayofweek = ((day_number + 4) % 7) + 1;

        // keep adding years until we run out of days (can't be modulo due to leap-years)
        days_in_year = _days_in_year(datetime->date.year);
        while (day_number >= days_in_year)
        {
            day_number -= days_in_year;
            datetime->date.year++;
            days_in_year = _days_in_year(datetime->date.year);
        }

        // already set by last while-loop :
        // days_in_year = _days_in_year (datetime->date.year);

        // keep on adding months - since they don't have the same number of days we need to loop
        while (day_number
               >= _datetime_days_in_month[_is_leap_year(datetime->date.year)][datetime->date.month])
        {
            day_number -= _datetime_days_in_month[_is_leap_year(datetime->date.year)]
                                                 [datetime->date.month];
            datetime->date.month += 1;
        }

        // add to day and month since they start at 1
        datetime->date.day = day_number + 1;
        datetime->date.month += 1;

        if (tstamp_ms != NULL_PTR)
        {
            datetime->ms = *tstamp_ms;
        }
    }

    return ret;
}

/***************************************************************************************************
 * Operations
 **************************************************************************************************/

static void _add_year(t_datetime *datetime)
{
    if (datetime->date.year < 2105)
    {
        datetime->date.year += 1;
    }
    // else return STD_OVFL
}

static void _add_month(t_datetime *datetime)
{
    if (++datetime->date.month > 12)
    {
        datetime->date.month = 1;
        _add_year(datetime);
    }
}

static void _add_day(t_datetime *datetime)
{
    if (++datetime->date.day
        > _datetime_days_in_month[_is_leap_year(datetime->date.year)][datetime->date.month - 1])
    {
        datetime->date.day = 1;
        _add_month(datetime);
    }
}

static void _add_hour(t_datetime *datetime)
{
    if (++datetime->date.hour >= 24)
    {
        datetime->date.hour = 0;
        _add_day(datetime);
    }
}

static void _add_minute(t_datetime *datetime)
{
    if (++datetime->date.minute >= 60)
    {
        datetime->date.minute = 0;
        _add_hour(datetime);
    }
}

static void _add_second(t_datetime *datetime)
{
    if (++datetime->date.second >= DATETIME_SECS_PER_MINUTE)
    {
        datetime->date.second = 0;
        _add_minute(datetime);
    }
}

std_return_type datetime_add_milliseconds(t_datetime *datetime, uint32 ms)
{
    std_return_type ret    = STD_OK;
    uint32          ovfl   = 0;
    uint32          acc_ms = 0;

    // performs NULL_PTR check
    ret = datetime_is_valid(datetime) ? STD_OK : STD_NOT_OK;

    if (ret == STD_OK)
    {
        ovfl   = ms;
        acc_ms = (uint32) datetime->ms;

        while (ovfl > 0)
        {
            ovfl = 0;

            // handle true 32 bit overflow
            if (ms > 0xFFFFFFFFUL - datetime->ms)
            {
                // use calculation instead of real 32 bit overflow due to uint32_t size:
                // on 64 bit platforms uint32_t is only _at_least_ 32 bit size (typically 64 bit)
                ovfl = ms - (0xFFFFFFFFUL - datetime->ms) - 1U;
                // ovfl = (uint32) datetime->ms + ms;
            }

            if (ovfl > 0)
            {
                acc_ms = 0xFFFFFFFFUL;
            }
            else
            {
                acc_ms = acc_ms + ms;
            }

            while (acc_ms >= 1000U)
            {
                _add_second(datetime);
                acc_ms -= 1000U;
            }
            ms = ovfl;
        }

        datetime->ms = (uint16) acc_ms;
    }
    return ret;
}

std_return_type datetime_add_seconds(t_datetime *datetime, uint32 seconds)
{
    std_return_type ret = STD_OK;
    uint32          i   = 0;
    uint32          secs_in_current_month;
    uint32          days_in_current_month;
    boolean         is_leap_year = FALSE;

    uint32 seconds_to_day = 0;

    // performs NULL_PTR check
    ret = datetime_is_valid(datetime) ? STD_OK : STD_NOT_OK;

    // early return in case of zero update
    if (seconds == 0)
    {
        return ret;
    }

    if (ret == STD_OK)
    {
        seconds_to_day = 0;

        // fast-lane for larger calculations > avoid terribly long looping
        if (seconds > _SECS_PER_DAY)
        {
            seconds_to_day += (60 - datetime->date.second) * 1;
            _add_minute(datetime);
            seconds_to_day += (60 - datetime->date.minute) * DATETIME_SECS_PER_MINUTE;
            _add_hour(datetime);
            seconds_to_day += (24 - datetime->date.hour) * DATETIME_SECS_PER_HOUR;
            // _add_day (datetime);

            datetime->date.second = 0;
            datetime->date.minute = 0;
            datetime->date.hour   = 0;
            seconds -= seconds_to_day;

            is_leap_year          = _is_leap_year(datetime->date.year);
            days_in_current_month = _datetime_days_in_month[is_leap_year][datetime->date.month - 1];
            days_in_current_month -= datetime->date.day;
            secs_in_current_month = (uint32) days_in_current_month * _SECS_PER_DAY;

            while (seconds > secs_in_current_month)
            {
                seconds -= secs_in_current_month;
                _add_month(datetime);
                datetime->date.day = 1;
                is_leap_year       = _is_leap_year(datetime->date.year);
                secs_in_current_month
                    = (uint32) _datetime_days_in_month[is_leap_year][datetime->date.month - 1]
                    * _SECS_PER_DAY;
            }
        }

        while (seconds > _SECS_PER_DAY)
        {
            _add_day(datetime);
            seconds -= _SECS_PER_DAY;
        }

        while (seconds > _SECS_PER_HOUR)
        {
            _add_hour(datetime);
            seconds -= _SECS_PER_HOUR;
        }

        while (seconds > _SECS_PER_DAY)
        {
            _add_hour(datetime);
            seconds -= _SECS_PER_DAY;
        }

        for (i = 0; i < seconds; i++)
        {
            _add_second(datetime);
        }
    }
    return ret;
}

std_return_type datetime_add_minutes(t_datetime *datetime, uint32 minutes)
{
    std_return_type ret = STD_OK;
    uint32          i   = 0;

    // performs NULL_PTR check
    ret = datetime_is_valid(datetime) ? STD_OK : STD_NOT_OK;

    if (ret == STD_OK)
    {
        for (i = 0; i < minutes; i++)
        {
            _add_minute(datetime);
        }
    }
    return ret;
}

std_return_type datetime_add_hours(t_datetime *datetime, uint32 hours)
{
    std_return_type ret = STD_OK;
    uint32          i   = 0;

    // performs NULL_PTR check
    ret = datetime_is_valid(datetime) ? STD_OK : STD_NOT_OK;

    if (ret == STD_OK)
    {
        for (i = 0; i < hours; i++)
        {
            _add_hour(datetime);
        }
    }
    return ret;
}

static void _sub_year(t_datetime *datetime)
{
    if (--datetime->date.year >= 2105)
    {
        datetime->date.year = 0;
    }
}

static void _sub_month(t_datetime *datetime)
{
    if (--datetime->date.month == 0)
    {
        _sub_year(datetime);
        datetime->date.month = 12U;
    }
}

static void _sub_day(t_datetime *datetime)
{
    if (--datetime->date.day == 0)
    {
        _sub_month(datetime);
        datetime->date.day
            = _datetime_days_in_month[_is_leap_year(datetime->date.year)][datetime->date.month - 1];
    }
}

static void _sub_hour(t_datetime *datetime)
{
    if (--datetime->date.hour >= 24)
    {
        datetime->date.hour = 23;
        _sub_day(datetime);
    }
}

static void _sub_minute(t_datetime *datetime)
{
    if (--datetime->date.minute >= 60)
    {
        datetime->date.minute = 59U;
        _sub_hour(datetime);
    }
}

static void _sub_second(t_datetime *datetime)
{
    if (--datetime->date.second >= DATETIME_SECS_PER_MINUTE)
    {
        datetime->date.second = DATETIME_SECS_PER_MINUTE - 1;
        _sub_minute(datetime);
    }
}

std_return_type datetime_sub_milliseconds(t_datetime *datetime, uint32 ms)
{
    std_return_type ret = STD_OK;

    // performs NULL_PTR check
    ret = datetime_is_valid(datetime) ? STD_OK : STD_NOT_OK;

    if (ret == STD_OK)
    {
        while (ms >= 1000U)
        {
            _sub_second(datetime);
            ms -= 1000U;
        }

        if (ms > datetime->ms)
        {
            _sub_second(datetime);
            datetime->ms = 1000 - (ms - datetime->ms);
        }
        else
        {
            datetime->ms -= ms;
        }
    }
    return ret;
}

std_return_type datetime_sub_seconds(t_datetime *datetime, uint32 seconds)
{
    std_return_type ret = STD_OK;
    uint32          i   = 0;
    uint32          secs_in_current_month;
    boolean         is_leap_year = FALSE;

    // performs NULL_PTR check
    ret = datetime_is_valid(datetime) ? STD_OK : STD_NOT_OK;

    // early return in case of zero update
    if (seconds == 0)
    {
        return ret;
    }

    if (ret == STD_OK)
    {
        secs_in_current_month = datetime->date.day * _SECS_PER_DAY;

        while (seconds > secs_in_current_month)
        {
            seconds -= secs_in_current_month;
            _sub_month(datetime);
            is_leap_year          = _is_leap_year(datetime->date.year);
            datetime->date.day    = _datetime_days_in_month[is_leap_year][datetime->date.month - 1];
            secs_in_current_month = (uint32) datetime->date.day * _SECS_PER_DAY;
        }

        while (seconds > _SECS_PER_DAY)
        {
            _sub_day(datetime);
            seconds -= _SECS_PER_DAY;
        }

        while (seconds > _SECS_PER_HOUR)
        {
            _sub_hour(datetime);
            seconds -= _SECS_PER_HOUR;
        }

        while (seconds > _SECS_PER_DAY)
        {
            _sub_hour(datetime);
            seconds -= _SECS_PER_DAY;
        }

        for (i = 0; i < seconds; i++)
        {
            _sub_second(datetime);
        }
    }
    return ret;
}

std_return_type datetime_sub_minutes(t_datetime *datetime, uint32 minutes)
{
    std_return_type ret = STD_OK;
    uint32          i   = 0;

    // performs NULL_PTR check
    ret = datetime_is_valid(datetime) ? STD_OK : STD_NOT_OK;

    if (ret == STD_OK)
    {
        for (i = 0; i < minutes; i++)
        {
            _sub_minute(datetime);
        }
    }
    return ret;
}

std_return_type datetime_sub_hours(t_datetime *datetime, uint32 hours)
{
    std_return_type ret = STD_OK;
    uint32          i   = 0;

    // performs NULL_PTR check
    ret = datetime_is_valid(datetime) ? STD_OK : STD_NOT_OK;

    if (ret == STD_OK)
    {
        for (i = 0; i < hours; i++)
        {
            _sub_hour(datetime);
        }
    }
    return ret;
}
