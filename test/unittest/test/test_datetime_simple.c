/**
 * \file test_datetime_simple.c
 */

#include <stdio.h>
#include "unity.h"

// #define TST_PRINT_ON

#include "std_types.h"
#include "util.h"

#include "datetime.h"

static t_datetime _tst_date = t_datetime_init_zero;

void setUp (void)
{
}

void tearDown (void)
{
}

void test_get_defaults (void)
{
    t_datetime date = t_datetime_init_zero;

    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));
    datetime_get_defaults (&date);
    TEST_ASSERT_EQUAL (TRUE, datetime_is_valid (&date));
    datetime_get_defaults (NULL_PTR);
}

void test_equal (void)
{
    t_datetime date_a = t_datetime_init_zero;
    t_datetime date_b = t_datetime_init_zero;

    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (NULL_PTR, NULL_PTR, FALSE));
    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (&date_a, NULL_PTR, FALSE));
    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (NULL_PTR, &date_b, FALSE));

    datetime_get_defaults (&date_a);
    datetime_get_defaults (&date_b);

    TEST_ASSERT_EQUAL (TRUE, datetime_is_equal (&date_a, &date_b, FALSE));

    date_a.ms = date_b.ms + 1;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (&date_a, &date_b, FALSE));
    TEST_ASSERT_EQUAL (TRUE, datetime_is_equal (&date_a, &date_b, TRUE));
    date_a.ms = date_b.ms;

    date_a.date.second = date_b.date.second + 1;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (&date_a, &date_b, FALSE));
    date_a.date.second = date_b.date.second;

    date_a.date.minute = date_b.date.minute + 1;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (&date_a, &date_b, FALSE));
    date_a.date.minute = date_b.date.minute;

    date_a.date.hour = date_b.date.hour + 1;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (&date_a, &date_b, FALSE));
    date_a.date.hour = date_b.date.hour;

    date_a.date.day = date_b.date.day + 1;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (&date_a, &date_b, FALSE));
    date_a.date.day = date_b.date.day;

    date_a.date.month = date_b.date.month + 1;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (&date_a, &date_b, FALSE));
    date_a.date.month = date_b.date.month;

    date_a.date.year = date_b.date.year + 1;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_equal (&date_a, &date_b, FALSE));
    date_a.date.year = date_b.date.year;
}

void test_later (void)
{
    t_datetime date_a = t_datetime_init_zero;
    t_datetime date_b = t_datetime_init_zero;
    boolean a_later_than_b = FALSE;

    // date validity is not checked
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (FALSE, a_later_than_b);

    datetime_get_defaults (&date_a);
    datetime_get_defaults (&date_b);

    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (FALSE, a_later_than_b);

    TEST_ASSERT_EQUAL (STD_NOT_OK, datetime_later_than (NULL_PTR, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (STD_NOT_OK, datetime_later_than (&date_a, NULL_PTR, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (STD_NOT_OK, datetime_later_than (&date_a, &date_b, NULL_PTR, FALSE));

    date_a.date.second = date_b.date.second + 1;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (TRUE, a_later_than_b);
    date_a.date.second = date_b.date.second;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (FALSE, a_later_than_b);

    date_a.date.minute = date_b.date.minute + 1;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (TRUE, a_later_than_b);
    date_a.date.minute = date_b.date.minute;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (FALSE, a_later_than_b);

    date_a.date.hour = date_b.date.hour + 1;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (TRUE, a_later_than_b);
    date_a.date.hour = date_b.date.hour;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (FALSE, a_later_than_b);

    date_a.date.day = date_b.date.day + 1;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (TRUE, a_later_than_b);
    date_a.date.day = date_b.date.day;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (FALSE, a_later_than_b);

    date_a.date.month = date_b.date.month + 1;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (TRUE, a_later_than_b);
    date_a.date.month = date_b.date.month;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (FALSE, a_later_than_b);

    date_a.date.year = date_b.date.year + 1;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (TRUE, a_later_than_b);
    date_a.date.year = date_b.date.year;
    TEST_ASSERT_EQUAL (STD_OK, datetime_later_than (&date_a, &date_b, &a_later_than_b, FALSE));
    TEST_ASSERT_EQUAL (FALSE, a_later_than_b);
}

void test_valid_0 (void)
{
    t_datetime date = t_datetime_init_zero;

    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (NULL_PTR));
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    TEST_ASSERT_EQUAL (TRUE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.ms = 1234;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.second = 60;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.minute = 60;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.hour = 24;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.day = 32;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.day = 0;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.month = 0;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.month = 13;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.year = 2107;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.year = 2000;
    date.date.month = 2;
    date.date.day = 30;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));
    date.date.day = 29;
    TEST_ASSERT_EQUAL (TRUE, datetime_is_valid (&date));

    datetime_get_defaults (&date);
    date.date.year = 2001;
    date.date.month = 2;
    date.date.day = 29;
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));
    date.date.day = 28;
    TEST_ASSERT_EQUAL (TRUE, datetime_is_valid (&date));
}

static boolean _datetime_match (t_datetime * d0, t_datetime * d1)
{
    return ( \
        (d0->ms          == d1->ms)          && \
        (d0->date.year   == d1->date.year)   && \
        (d0->date.month  == d1->date.month)  && \
        (d0->date.day    == d1->date.day)    && \
        (d0->date.hour   == d1->date.hour)   && \
        (d0->date.minute == d1->date.minute) &&
        (d0->date.second == d1->date.second));
}

void test_get_max (void)
{
    t_datetime date = t_datetime_init_zero;

    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_maximum_default (NULL_PTR);
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_maximum_default (&date);
    TEST_ASSERT_EQUAL (TRUE, datetime_is_valid (&date));

    datetime_get_defaults (&_tst_date);

    _tst_date.ms           = 123U;
    _tst_date.date.year    = 2020;
    _tst_date.date.month   = 2;
    _tst_date.date.day     = 3;
    _tst_date.date.hour    = 4;
    _tst_date.date.minute  = 5;
    _tst_date.date.second  = 6;

    datetime_get_minimum_default (&date);
    TEST_ASSERT_FALSE(_datetime_match (&_tst_date, &date));
}

void test_get_min (void)
{
    t_datetime date = t_datetime_init_zero;

    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_minimum_default (NULL_PTR);
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (&date));

    datetime_get_minimum_default (&date);
    TEST_ASSERT_EQUAL (TRUE, datetime_is_valid (&date));

    datetime_get_defaults (&_tst_date);

    _tst_date.ms           = 123U;
    _tst_date.date.year    = 2020;
    _tst_date.date.month   = 2;
    _tst_date.date.day     = 3;
    _tst_date.date.hour    = 4;
    _tst_date.date.minute  = 5;
    _tst_date.date.second  = 6;

    datetime_get_minimum_default (&date);
    TEST_ASSERT_FALSE(_datetime_match (&_tst_date, &date));

    TEST_ASSERT_EQUAL (1, date.date.month);
    TEST_ASSERT_EQUAL (1, date.date.day);
    TEST_ASSERT_EQUAL (0, date.date.hour);
    TEST_ASSERT_EQUAL (0, date.date.minute);
    TEST_ASSERT_EQUAL (0, date.date.second);
}
