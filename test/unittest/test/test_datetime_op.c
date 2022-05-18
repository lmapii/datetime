/**
 * \file test_datetime_op.c
 */

#include <stdio.h>
#include "unity.h"

// #define TST_PRINT_ON

#include "std_types.h"
#include "util.h"

#include "datetime.h"

typedef struct
{
    uint32 second;
    uint32 minute;
    uint32 hour;

    // uint32 day;
    // uint32 month;
    // uint32 year;

} t_test_datetime_a;

typedef struct
{
    t_test_datetime_a date;
    uint32 ms;
} t_test_datetime_mod;

typedef struct
{
    t_datetime ini;
    t_test_datetime_mod mod;
    t_datetime exp;
    boolean do_add;

} t_test_datetime;

static t_test_datetime _tst_data [] =
{
    {     .ini = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =     0 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =     0 } ,.do_add = TRUE  }

    // test basic adding and subtracting [ms]

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =    33 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   300 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =   100 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   433 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =   334 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    11 } ,.ms =   999 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =   777 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    13 } ,.ms =   110 } ,.do_add = TRUE }

    // test basic adding and subtracting [s]

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =    12 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    24 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =    12 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =     0 } ,.ms =   333 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =    60 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    23 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =    60 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    21 ,.second =    12 } ,.ms =   333 } ,.do_add = FALSE }


    // test basic adding and subtracting [m]

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =    10 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    32 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =    12 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    10 ,.second =    12 } ,.ms =   333 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =    60 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    16 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =    23 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    14 ,.minute =    59 ,.second =    12 } ,.ms =   333 } ,.do_add = FALSE }

    // test basic adding and subtracting [h]

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =     8 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    23 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =    12 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =     3 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =    10 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  3 ,.hour =     1 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  7 ,.day =  2 ,.hour =    15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =    16 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2018 ,.month =  7 ,.day =  1 ,.hour =    23 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = FALSE }

    // test basic adding and subtracting [h > days], non- / leap year

    ,{    .ini = { .date = { .year = 2000 ,.month =  2 ,.day = 28 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =        24 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2000 ,.month =  2 ,.day = 29 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2000 ,.month =  1 ,.day =  1 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =  366 * 24 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2001 ,.month =  1 ,.day =  1 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 1972 ,.month =  1 ,.day =  1 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =  366 * 24 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 1973 ,.month =  1 ,.day =  1 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2100 ,.month =  1 ,.day =  1 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =  366 * 24 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2101 ,.month =  1 ,.day =  2 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 1971 ,.month =  1 ,.day =  1 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =  366 * 24 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 1972 ,.month =  1 ,.day =  2 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2000 ,.month = 12 ,.day = 31 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =  366 * 24 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 1999 ,.month = 12 ,.day = 31 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 1972 ,.month = 12 ,.day = 31 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =  366 * 24 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 1971 ,.month = 12 ,.day = 31 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 2100 ,.month = 12 ,.day = 31 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =  366 * 24 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 2099 ,.month = 12 ,.day = 30 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 1971 ,.month = 12 ,.day = 31 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 }
        , .mod = { .date = {                                       .hour =  366 * 24 ,.minute =     0 ,.second =     0 } ,.ms =     0 }
        , .exp = { .date = { .year = 1970 ,.month = 12 ,.day = 30 ,.hour =        15 ,.minute =    22 ,.second =    12 } ,.ms =   333 } ,.do_add = FALSE }

    // test handling of leap years (more and mixed)

    ,{    .ini = { .date = { .year = 1972 ,.month = 12 ,.day =  1 ,.hour =        11 ,.minute =    11 ,.second =     0 } ,.ms =   111 }
        , .mod = { .date = {                                       .hour =  365 * 24 ,.minute =     5 ,.second =    11 } ,.ms =  1111 }
        , .exp = { .date = { .year = 1973 ,.month = 12 ,.day =  1 ,.hour =        11 ,.minute =    16 ,.second =    12 } ,.ms =   222 } ,.do_add = TRUE  }

    ,{    .ini = { .date = { .year = 1972 ,.month =  2 ,.day =  1 ,.hour =        11 ,.minute =    11 ,.second =     0 } ,.ms =   111 }
        , .mod = { .date = {                                       .hour =  365 * 24 ,.minute =     5 ,.second =    11 } ,.ms =  1111 }
        , .exp = { .date = { .year = 1973 ,.month =  1 ,.day = 31 ,.hour =        11 ,.minute =    16 ,.second =    12 } ,.ms =   222 } ,.do_add = TRUE  }

    ,{    .ini = { .date = { .year = 1972 ,.month = 12 ,.day =  1 ,.hour =        11 ,.minute =    11 ,.second =    12 } ,.ms =   111 }
        , .mod = { .date = {                                       .hour =  365 * 24 ,.minute =     5 ,.second =    10 } ,.ms =  1111 }
        , .exp = { .date = { .year = 1971 ,.month = 12 ,.day =  2 ,.hour =        11 ,.minute =     6 ,.second =     1 } ,.ms =     0 } ,.do_add = FALSE  }

    // large operation - ms

    ,{    .ini = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =            0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =   0xFFFFFFFF }
        , .exp = { .date = { .year = 1970 ,.month =  2 ,.day = 19 ,.hour =    17 ,.minute =     2 ,.second =    47 } ,.ms =          295 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =          100 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =   0xFFFFFFFF }
        , .exp = { .date = { .year = 1970 ,.month =  2 ,.day = 19 ,.hour =    17 ,.minute =     2 ,.second =    47 } ,.ms =          394 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =          100 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =         2000 }
        , .exp = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     2 } ,.ms =          100 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 1971 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =          100 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =         2000 }
        , .exp = { .date = { .year = 1970 ,.month = 12 ,.day = 31 ,.hour =    23 ,.minute =    59 ,.second =    58 } ,.ms =          100 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 1970 ,.month =  2 ,.day = 19 ,.hour =    17 ,.minute =     2 ,.second =    47 } ,.ms =          295 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =   0xFFFFFFFF }
        , .exp = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =     0 } ,.ms =            0 } ,.do_add = FALSE }

    // large operation - s (tests shortcut secs/month)

    ,{    .ini = { .date = { .year = 1971 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =        0 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =  4294967 } ,.ms = 0 }
        , .exp = { .date = { .year = 1971 ,.month =  2 ,.day = 19 ,.hour =    17 ,.minute =     2 ,.second =       47 } ,.ms = 0 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 1971 ,.month =  2 ,.day = 19 ,.hour =    17 ,.minute =     2 ,.second =       47 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =  4294967 } ,.ms = 0 }
        , .exp = { .date = { .year = 1971 ,.month =  1 ,.day =  1 ,.hour =     0 ,.minute =     0 ,.second =        0 } ,.ms = 0 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  5 ,.day = 22 ,.hour =    11 ,.minute =    49 ,.second =        15 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =  26099250 } ,.ms = 0 }
        , .exp = { .date = { .year = 2019 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2019 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =  26099250 } ,.ms = 0 }
        , .exp = { .date = { .year = 2018 ,.month =  5 ,.day = 22 ,.hour =    11 ,.minute =    49 ,.second =        15 } ,.ms = 0 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 1972 ,.month = 12 ,.day =  1 ,.hour =    11 ,.minute =    11 ,.second =        12 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =  31536310 } ,.ms = 0 }
        , .exp = { .date = { .year = 1971 ,.month = 12 ,.day =  2 ,.hour =    11 ,.minute =     6 ,.second =         2 } ,.ms = 0 } ,.do_add = FALSE }

    ,{    .ini = { .date = { .year = 1972 ,.month =  2 ,.day =  1 ,.hour =    11 ,.minute =    11 ,.second =         0 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =  31536312 } ,.ms = 0 }
        , .exp = { .date = { .year = 1973 ,.month =  1 ,.day = 31 ,.hour =    11 ,.minute =    16 ,.second =        12 } ,.ms = 0 } ,.do_add = TRUE  }

    // large operation : 2 years, 69 days, 13 hours, 44 minutes, 33 seconds
    // 2*365*24*60*60 + 69*24*60*60 + 13*60*60 + 44*60 + 33 = 69083073 ( + tstamp of below date is 1426858605)

    ,{    .ini = { .date = { .year = 2015 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =  69083073 } ,.ms = 0 }
        , .exp = { .date = { .year = 2017 ,.month =  5 ,.day = 28 ,.hour =     3 ,.minute =    21 ,.second =        18 } ,.ms = 0 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2017 ,.month =  5 ,.day = 28 ,.hour =     3 ,.minute =    21 ,.second =        18 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second =  69083073 } ,.ms = 0 }
        , .exp = { .date = { .year = 2015 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 } ,.do_add = FALSE }

    // large operation : 3 years, 69 days, 13 hours, 44 minutes, 33 seconds
    // 3*365*24*60*60 + 69*24*60*60 + 13*60*60 + 44*60 + 33 = 100619073 ( + tstamp of below date is 1426858605)

    ,{    .ini = { .date = { .year = 2015 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second = 100619073 } ,.ms = 0 }
        , .exp = { .date = { .year = 2018 ,.month =  5 ,.day = 28 ,.hour =     3 ,.minute =    21 ,.second =        18 } ,.ms = 0 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2018 ,.month =  5 ,.day = 28 ,.hour =     3 ,.minute =    21 ,.second =        18 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second = 100619073 } ,.ms = 0 }
        , .exp = { .date = { .year = 2015 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 } ,.do_add = FALSE }

    // large operation : 4 years, 69 days, 13 hours, 44 minutes, 33 seconds
    // 4*365*24*60*60 + 69*24*60*60 + 13*60*60 + 44*60 + 33 = 132155073 ( + tstamp of below date is 1426858605)

    ,{    .ini = { .date = { .year = 2015 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second = 132155073 } ,.ms = 0 }
        , .exp = { .date = { .year = 2019 ,.month =  5 ,.day = 28 ,.hour =     3 ,.minute =    21 ,.second =        18 } ,.ms = 0 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2019 ,.month =  5 ,.day = 28 ,.hour =     3 ,.minute =    21 ,.second =        18 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second = 132155073 } ,.ms = 0 }
        , .exp = { .date = { .year = 2015 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 } ,.do_add = FALSE }

    // large operation : 4 years, 69 days, 13 hours, 44 minutes, 33 seconds
    // 5*365*24*60*60 + 69*24*60*60 + 13*60*60 + 44*60 + 33 = 163691073 ( + tstamp of below date is 1426858605)

    ,{    .ini = { .date = { .year = 2015 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second = 163691073 } ,.ms = 0 }
        , .exp = { .date = { .year = 2020 ,.month =  5 ,.day = 27 ,.hour =     3 ,.minute =    21 ,.second =        18 } ,.ms = 0 } ,.do_add = TRUE }

    ,{    .ini = { .date = { .year = 2020 ,.month =  5 ,.day = 27 ,.hour =     3 ,.minute =    21 ,.second =        18 } ,.ms = 0 }
        , .mod = { .date = {                                       .hour =     0 ,.minute =     0 ,.second = 163691073 } ,.ms = 0 }
        , .exp = { .date = { .year = 2015 ,.month =  3 ,.day = 20 ,.hour =    13 ,.minute =    36 ,.second =        45 } ,.ms = 0 } ,.do_add = FALSE }

};

void setUp (void)
{
}

void tearDown (void)
{
}

void test_dummy (void)
{
    t_datetime datetime = t_datetime_init_zero;
    uint32 i = 0;

    for (i = 0; i < sizeof (_tst_data) / sizeof (_tst_data [0]); i++)
    {
        memcpy ((void *) &datetime, (void *) &_tst_data [i].ini, sizeof (t_datetime));

        if (_tst_data [i].do_add)
        {
            TEST_ASSERT_EQUAL( STD_OK, datetime_add_milliseconds  (&datetime, _tst_data [i].mod.ms));
            TEST_ASSERT_EQUAL( STD_OK, datetime_add_seconds       (&datetime, _tst_data [i].mod.date.second));
            TEST_ASSERT_EQUAL( STD_OK, datetime_add_minutes       (&datetime, _tst_data [i].mod.date.minute));
            TEST_ASSERT_EQUAL( STD_OK, datetime_add_hours         (&datetime, _tst_data [i].mod.date.hour));
        }
        else
        {
            TEST_ASSERT_EQUAL( STD_OK, datetime_sub_milliseconds  (&datetime, _tst_data [i].mod.ms));
            TEST_ASSERT_EQUAL( STD_OK, datetime_sub_seconds       (&datetime, _tst_data [i].mod.date.second));
            TEST_ASSERT_EQUAL( STD_OK, datetime_sub_minutes       (&datetime, _tst_data [i].mod.date.minute));
            TEST_ASSERT_EQUAL( STD_OK, datetime_sub_hours         (&datetime, _tst_data [i].mod.date.hour));
        }

        // TEST_ASSERT_EQUAL_MEMORY (&_tst_data [i].exp, &datetime, sizeof (t_datetime));
        TEST_ASSERT_EQUAL (_tst_data [i].exp.date.year      , datetime.date.year);
        TEST_ASSERT_EQUAL (_tst_data [i].exp.date.month     , datetime.date.month);
        TEST_ASSERT_EQUAL (_tst_data [i].exp.date.day       , datetime.date.day);
        TEST_ASSERT_EQUAL (_tst_data [i].exp.date.hour      , datetime.date.hour);
        TEST_ASSERT_EQUAL (_tst_data [i].exp.date.minute    , datetime.date.minute);
        TEST_ASSERT_EQUAL (_tst_data [i].exp.date.second    , datetime.date.second);
        TEST_ASSERT_EQUAL (_tst_data [i].exp.ms             , datetime.ms);
    }
}
