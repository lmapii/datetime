/**
 * \file test_datetime_api.c
 */

#include <stdio.h>
#include "unity.h"

// #define TST_PRINT_ON

#include "std_types.h"
#include "util.h"

#include "datetime.h"

void setUp (void)
{
}

void tearDown (void)
{
}

void test_datetime_x_is_valid (void)
{
    // t_datetime date = t_datetime_init_zero;
    TEST_ASSERT_EQUAL (FALSE, datetime_date_is_valid (NULL_PTR));
    TEST_ASSERT_EQUAL (FALSE, datetime_is_valid (NULL_PTR));
}

void test_datetime_from_timestamp (void)
{
    t_datetime date = t_datetime_init_zero;
    uint32 tstamp_utc, tstamp_ms;

    TEST_ASSERT_EQUAL (STD_NOT_OK, datetime_from_timestamp (NULL_PTR, &tstamp_utc, &tstamp_ms));
    TEST_ASSERT_EQUAL (STD_NOT_OK, datetime_from_timestamp (&date, NULL_PTR, &tstamp_ms));
    TEST_ASSERT_EQUAL (STD_NOT_OK, datetime_from_timestamp (NULL_PTR, NULL_PTR, &tstamp_ms));
}

void test_datetime_to_timestamp (void)
{
    t_datetime date = t_datetime_init_zero;
    uint32 tstamp_utc, tstamp_ms;

    TEST_ASSERT_EQUAL (STD_NOT_OK, datetime_to_timestamp (NULL_PTR, &tstamp_utc, &tstamp_ms));
    TEST_ASSERT_EQUAL (STD_NOT_OK, datetime_to_timestamp (&date, NULL_PTR, &tstamp_ms));
    TEST_ASSERT_EQUAL (STD_NOT_OK, datetime_to_timestamp (NULL_PTR, NULL_PTR, &tstamp_ms));
}
