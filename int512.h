#pragma once

typedef struct __int512 int512;

#define BIGINT_512_IN_32LONG 16 /*first bit in all int for sign*/ + 1
#define BIGINT_512_IN_16INT 32 + 1

// must be less than int bounders
#define COUNT_LEVEL BIGINT_512_IN_16INT

#define TOP_LEVEL -1 + COUNT_LEVEL


int512 * int512_new() ;

void int512_free (int512 * self) ;

int int512_at (int512 * self, int i) ;

void int512_set_at (int512 * self, int i, int value) ;

void int512_print (int512 * self) ;

void int512_sum_int_byLevel (int512 * self, int level, int addition) ;

void int512_sum_int (int512 * self, int addition) ;

void int512_difference_int (int512 * self, int subtrahend) ;

int int512_compare_int (int512 * self, int toCompare) ;

int int512_compare_int512 (int512 * first, int512 * second) ;

void int512_copy_int512 (int512 * to, int512 * from) ;

int512 * int512_copy_new (int512 * from) ;

void int512_sum_int512 (int512 * first, int512 * second) ;

int512 * int512_sum_new (int512 * first, int512 * second) ;

void int512_difference_int512 (int512 * first, int512 * second) ;

int512 * int512_subtrahend_int (int minued, int512 * subtrahend) ;

void int512_changeSign (int512 * self) ;

void int512_product_int_byLevel (int512 * self, int level, int multiplier) ;

int512 * int512_product_byLevel_new (int512 * self, int level, int multiplier) ;

int512 * int512_product_byLevel_new_clean (int512 * self, int level, int multiplier) ;

void int512_mult_int512 (int512 * base, int512 * multiplier) ;

void int512_set_value_fromLevel (int512 * self, int level, int value) ;

void int512_set_value_toLevel (int512 * self, int level, int value) ;

void int512_set_value (int512 * self, int value) ;

void int512_set_min (int512 * self) ;

void int512_set_max (int512 * self) ;

char * int512_toBase (int512 * self, unsigned base) ;

int512 * int512_fromBase (char * str, unsigned base) ;
