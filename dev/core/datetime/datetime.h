/**
 * \file datetime.h
 *
 * \brief module for date and timing functions
 */

#ifndef DATETIME_H_FILE
#define DATETIME_H_FILE

/***************************************************************************************************
 * Includes
 **************************************************************************************************/

#include "std_types.h"

#include "datetime_common.h"

/***************************************************************************************************
 * Checks
 **************************************************************************************************/

// function for a fast (but accurate) comparison - faster than calculating the timestamp and
// comparing those values
std_return_type datetime_later_than(
    t_datetime *a,
    t_datetime *b,
    boolean *   a_later_than_b,
    boolean     ignore_ms);

boolean datetime_is_equal(t_datetime *a, t_datetime *b, boolean ignore_ms);

boolean datetime_date_is_valid(t_datetime_date *date);

boolean datetime_is_valid(t_datetime *datetime);

/***************************************************************************************************
 * Timestamp Conversions
 **************************************************************************************************/

std_return_type datetime_to_timestamp(t_datetime *datetime, uint32 *tstamp_utc, uint32 *tstamp_ms);

std_return_type datetime_from_timestamp(
    t_datetime *  datetime,
    const uint32 *tstamp_utc,
    const uint32 *tstamp_ms);

void datetime_get_defaults(t_datetime *datetime);

void datetime_get_maximum_default(t_datetime *datetime);

void datetime_get_minimum_default(t_datetime *datetime);

/***************************************************************************************************
 * Operations
 **************************************************************************************************/

std_return_type datetime_add_milliseconds(t_datetime *datetime, uint32 ms);

std_return_type datetime_add_seconds(t_datetime *datetime, uint32 seconds);

std_return_type datetime_add_minutes(t_datetime *datetime, uint32 minutes);

std_return_type datetime_add_hours(t_datetime *datetime, uint32 hours);

std_return_type datetime_sub_milliseconds(t_datetime *datetime, uint32 ms);

std_return_type datetime_sub_seconds(t_datetime *datetime, uint32 seconds);

std_return_type datetime_sub_minutes(t_datetime *datetime, uint32 minutes);

std_return_type datetime_sub_hours(t_datetime *datetime, uint32 hours);

#endif /* DATETIME_H_FILE */
