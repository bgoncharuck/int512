#pragma once

typedef struct __int512 int512;

#define BIGINT_512_IN_32LONG 16 /*first bit in all long for sign*/ + 1

// must be less than int bounders
#define COUNT_LEVEL BIGINT_512_IN_32LONG

int512 * int512_new() ;

void int512_free (int512 * self) ;

long int512_at (int512 * self, int i) ;

void int512_set_at (int512 * self, int i, long value) ;

void int512_print (int512 * self) ;

void int512_sum_long_byLevel (int512 * self, int level, long addition) ;

void int512_sum_long (int512 * self, long addition) ;

void int512_difference_long (int512 * self, long subtrahend) ;

int int512_compare_long (int512 * self, long toCompare) ;

int int512_compare_int512 (int512 * first, int512 * second) ;

void int512_copy_int512 (int512 * to, int512 * from) ;

int512 * int512_copy_new (int512 * from) ;

void int512_sum_int512 (int512 * first, int512 * second) ;

int512 * int512_sum_new (int512 * first, int512 * second) ;

void int512_difference_int512 (int512 * first, int512 * second) ;

int512 * int512_subtrahend_long (long minued, int512 * subtrahend) ;

void int512_set_min (int512 * self) ;

void int512_set_max (int512 * self) ;

void int512_set_value_fromLevel (int512 * self, int level, long value) ;

void int512_set_value (int512 * self, long value) ;

int int512_fromLevel_value (int512 * self, int level, long value) ;

int int512_nullEntrance (int512 * self, int fromLevel) {

char * int512_toBase (int512 * self, unsigned base);

int512 * int512_fromBase (char * str, unsigned base);
