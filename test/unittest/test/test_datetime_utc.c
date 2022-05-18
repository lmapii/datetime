/**
 * \file test_datetime_utc.c
 */

#include <stdio.h>
#include <string.h>

#include "unity.h"

#define TST_PRINT_ON

#include "std_types.h"
#include "util.h"

#include "datetime.h"

static void _tst_date_reset (t_datetime * datetime)
{
    t_datetime date = t_datetime_init_zero;
    memcpy ((void *) datetime, (void *) &date, sizeof (t_datetime));
}

typedef struct
{
    t_datetime date;
    uint32 tstamp;
} t_test_stamps;

static t_test_stamps _test_stamps [] =
{
    // basic tstamp checks for years and leap-years (10 tests)
      { .date = { .date = { .year = 1970 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =           0 }
    , { .date = { .date = { .year = 1970 ,.month = 12 ,.day = 31 ,.hour = 23 ,.minute = 59 ,.second = 59 } ,.ms = 999 }, .tstamp =    31535999 }
    , { .date = { .date = { .year = 1971 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =    31536000 }
    , { .date = { .date = { .year = 1972 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =    63072000 }
    , { .date = { .date = { .year = 1973 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =    94694400 }
    , { .date = { .date = { .year = 1974 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =   126230400 }
    , { .date = { .date = { .year = 1976 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =   189302400 }
    , { .date = { .date = { .year = 2000 ,.month =  1 ,.day =  1 ,.hour = 23 ,.minute = 59 ,.second = 59 } ,.ms = 999 }, .tstamp =   946771199 }
    , { .date = { .date = { .year = 2000 ,.month = 12 ,.day = 31 ,.hour = 23 ,.minute = 59 ,.second = 59 } ,.ms = 999 }, .tstamp =   978307199 }
    , { .date = { .date = { .year = 2016 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =  1451606400 }
    , { .date = { .date = { .year = 2100 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =  4102444800 }

    // basic checks that m/d, h/m/s are counted correctly
    , { .date = { .date = { .year = 2021 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =  1609459200 }
    , { .date = { .date = { .year = 2021 ,.month =  2 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =  1612137600 }
    , { .date = { .date = { .year = 2021 ,.month =  1 ,.day =  2 ,.hour =  0 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =  1609545600 }
    , { .date = { .date = { .year = 2021 ,.month =  1 ,.day =  1 ,.hour =  1 ,.minute =  0 ,.second =  0 } ,.ms =   0 }, .tstamp =  1609462800 }
    , { .date = { .date = { .year = 2021 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  1 ,.second =  0 } ,.ms =   0 }, .tstamp =  1609459260 }
    , { .date = { .date = { .year = 2021 ,.month =  1 ,.day =  1 ,.hour =  0 ,.minute =  0 ,.second =  1 } ,.ms =   0 }, .tstamp =  1609459201 }

    // check february in non- and leap-years. but well, should already work since they are counted in leap-years
    , { .date = { .date = { .year = 2064 ,.month =  2 ,.day = 23 ,.hour = 11 ,.minute = 22 ,.second = 33 } ,.ms =   0 }, .tstamp =  2970991353 }
    , { .date = { .date = { .year = 2101 ,.month =  2 ,.day = 11 ,.hour = 22 ,.minute = 55 ,.second = 44 } ,.ms =   0 }, .tstamp =  4137605744 }

    // some more timestamps
    , { .date = { .date = { .year = 1982 ,.month =  9 ,.day = 30 ,.hour = 12 ,.minute = 15 ,.second = 12 } ,.ms =   0 }, .tstamp =   402236112 }
    , { .date = { .date = { .year = 1988 ,.month =  5 ,.day = 29 ,.hour = 12 ,.minute =  6 ,.second =  6 } ,.ms =   0 }, .tstamp =   580910766 }
    , { .date = { .date = { .year = 1990 ,.month =  5 ,.day = 29 ,.hour = 12 ,.minute =  6 ,.second =  6 } ,.ms =   0 }, .tstamp =   643982766 }
    , { .date = { .date = { .year = 1997 ,.month = 12 ,.day = 31 ,.hour = 23 ,.minute = 14 ,.second = 43 } ,.ms =   0 }, .tstamp =   883610083 }
    , { .date = { .date = { .year = 2011 ,.month =  7 ,.day = 14 ,.hour = 11 ,.minute = 14 ,.second = 43 } ,.ms =   0 }, .tstamp =  1310642083 }
    , { .date = { .date = { .year = 2024 ,.month =  3 ,.day =  3 ,.hour =  3 ,.minute =  3 ,.second =  3 } ,.ms =   0 }, .tstamp =  1709434983 }
    , { .date = { .date = { .year = 2037 ,.month =  7 ,.day =  7 ,.hour =  7 ,.minute =  7 ,.second =  7 } ,.ms =   0 }, .tstamp =  2130563227 }
    , { .date = { .date = { .year = 2043 ,.month =  4 ,.day =  4 ,.hour =  4 ,.minute =  4 ,.second =  4 } ,.ms =   0 }, .tstamp =  2311733044 }
    , { .date = { .date = { .year = 2050 ,.month =  5 ,.day =  5 ,.hour =  5 ,.minute =  5 ,.second =  5 } ,.ms =   0 }, .tstamp =  2535339905 }
    , { .date = { .date = { .year = 2069 ,.month =  6 ,.day =  6 ,.hour =  6 ,.minute =  6 ,.second =  6 } ,.ms =   0 }, .tstamp =  3137724366 }
    , { .date = { .date = { .year = 2077 ,.month =  1 ,.day =  1 ,.hour =  1 ,.minute =  1 ,.second =  1 } ,.ms =   0 }, .tstamp =  3376688461 }
    , { .date = { .date = { .year = 2086 ,.month =  8 ,.day =  8 ,.hour =  8 ,.minute =  8 ,.second =  8 } ,.ms =   0 }, .tstamp =  3679632488 }
    , { .date = { .date = { .year = 2099 ,.month =  9 ,.day =  9 ,.hour =  9 ,.minute =  9 ,.second =  9 } ,.ms =   0 }, .tstamp =  4092628149 }
    , { .date = { .date = { .year = 2105 ,.month =  5 ,.day =  5 ,.hour =  5 ,.minute =  5 ,.second =  5 } ,.ms =   0 }, .tstamp =  4270943105 }
};

static void _test_init (void)
{
    // datetime_init_ExpectAndReturn (NULL_PTR, STD_OK);
    // TEST_ASSERT_EQUAL (STD_OK, datetime_init (NULL_PTR));
}

void setUp (void)
{

}

void tearDown (void)
{
}

void test_to_tstamp (void)
{
    uint8 i = 0;
    uint32 tstamp = 123;
    uint32 ms = 0;

    _test_init ();

    // TEST_ASSERT_EQUAL (STD_OK, datetime_to_timestamp (&(_test_stamps [11].date), &tstamp, NULL_PTR));
    // TEST_ASSERT_EQUAL (_test_stamps [11].tstamp, tstamp);

    for (i = 0; i < sizeof (_test_stamps) / sizeof (_test_stamps [0]); i++)
    {
        TEST_ASSERT_EQUAL (STD_OK, datetime_to_timestamp (&(_test_stamps [i].date), &tstamp, &ms));
        TEST_ASSERT_EQUAL (_test_stamps [i].tstamp, tstamp);
        TEST_ASSERT_EQUAL (_test_stamps [i].date.ms, ms);

        TEST_ASSERT_EQUAL (STD_OK, datetime_to_timestamp (&(_test_stamps [i].date), &tstamp, NULL_PTR));
        TEST_ASSERT_EQUAL (_test_stamps [i].tstamp, tstamp);
    }

    // datetime_deinit_Expect ();
    // datetime_deinit ();
}

void test_from_tstamp (void)
{
    uint8 i = 0;
    t_datetime date = t_datetime_init_zero;
    uint32 tstamp_ms;

    _test_init ();

    // TEST_ASSERT_EQUAL (STD_OK, datetime_to_timestamp (&(_test_stamps [11].date), &tstamp, NULL_PTR));
    // TEST_ASSERT_EQUAL (_test_stamps [11].tstamp, tstamp);

    for (i = 0; i < sizeof (_test_stamps) / sizeof (_test_stamps [0]); i++)
    {
        _tst_date_reset (&date);
        // using .ms as tstamp_ms (!)
        tstamp_ms = _test_stamps [i].date.ms;

        // abusing date.ms as timestamp for the date
        TEST_ASSERT_EQUAL (STD_OK, datetime_from_timestamp (&date, &(_test_stamps [i].tstamp), &tstamp_ms));

        // TEST_ASSERT_EQUAL_MEMORY (&(_test_stamps [i].date), &date, sizeof (t_datetime));
        TEST_ASSERT_EQUAL (_test_stamps [i].date.date.second    , date.date.second);
        TEST_ASSERT_EQUAL (_test_stamps [i].date.date.minute    , date.date.minute);
        TEST_ASSERT_EQUAL (_test_stamps [i].date.date.hour      , date.date.hour);
        TEST_ASSERT_EQUAL (_test_stamps [i].date.date.day       , date.date.day);
        TEST_ASSERT_EQUAL (_test_stamps [i].date.date.month     , date.date.month);
        TEST_ASSERT_EQUAL (_test_stamps [i].date.date.year      , date.date.year);
        TEST_ASSERT_EQUAL (_test_stamps [i].date.ms             , date.ms);
    }

    // datetime_deinit_Expect ();
    // datetime_deinit ();
}
