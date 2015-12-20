// Code in this file is intended to allow maintenance of the date time clock

#ifndef _TIME_KEEPER_C_
#define _TIME_KEEPER_C_

#include <avr/interrupt.h>

volatile uint32_t uptime_s;         // Uptime in seconds (will roll-over after more than a century)

volatile uint16_t datetime_ms;      // Millisecond component of the datetime
volatile uint8_t datetime_s;        // Second component of the datetime
volatile uint8_t datetime_min;      // Minutes component of the datetime
volatile uint8_t datetime_hour;     // Hours component of the datetime
volatile uint8_t datetime_day;      // Day-of-the-month component of the datetime
volatile uint8_t datetime_month;    // Month component of the datetime
volatile uint16_t datetime_year;    // Year component of the datetime

// FIXME - write unit tests for these - check overflows happen correctly at e.g. the end of the day
// Helper macros to keep the code sane
uint8_t days_in_month(uint8_t month, uint8_t year){
  switch(month){
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      return 31;
    case 4: case 6: case 9: case 11:
      return 30;
    case 2:
      // Fucking February.
      if ( (year%4==0) && ( (year%100!=0) || (year%1000==0) ) ) return 29;
      else return 28;
    default:
      return 0; // Invalid month - panic.
  }
}
#define INC_MS()    { datetime_ms +=1;    if(datetime_ms > 999)   { datetime_ms = 0; INC_S(); } }
#define INC_S()     { datetime_s  +=1;    if(datetime_s  > 59)    { datetime_s  = 0; INC_MIN(); } }
#define INC_MIN()   { datetime_min +=1;   if(datetime_min > 59)   { datetime_min = 0; INC_HOUR(); } }
#define INC_HOUR()  { datetime_hour +=1;  if(datetime_hour > 23)  { datetime_hour = 0; INC_DAY(); } }
#define INC_DAY()   { datetime_day +=1;   if(datetime_day > days_in_month(datetime_month, datetime_year))  { datetime_day=1; INC_MONTH(); } }
#define INC_MONTH() { datetime_month +=1; if(datetime_month > 12) { datetime_month=1; INC_YEAR(); } }
#define INC_YEAR()  { datetime_year += 1; }  // I'll be long dead before this overflows

ISR(TIMER1_COMPA_vect)
{
    // This interrupt should be triggered every millisecond
    INC_MS();
}

void setup_datetime(uint16_t ms, uint8_t s, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint16_t year) {
    uptime_s = 0;

    datetime_ms    = ms;
    datetime_s     = min;
    datetime_min   = min;
    datetime_hour  = hour;
    datetime_day   = day;
    datetime_month = month;
    datetime_year  = year;
}

void setup_timer1(void){
  // FIXME - initialise timer 1, set it running, make it overflow when OCR1A is matched
}

void setup_datetime_default(void){
    setup_datetime(0, 0, 0, 0, 0, 0, 2015);
}

#endif
