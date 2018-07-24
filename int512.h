typedef struct __int512 int512;

#define BIGINT_512_IN_32LONG 16

// must be less than int bounders
#define COUNT_LEVEL BIGINT_512_IN_32LONG

int512 * int512_new() ;

void int512_free (int512 * self) ;

long int512_at (int512 * self, int i) ;

void int512_setat (int512 * self, int i, long val) ;

void int512_print (int512 * self) ;

void int512_sum_long_byLevel (int512 * self, int level, long addition) ;

void int512_sum_long (int512 * self, long addition) ;

void int512_difference_long (int512 * self, long subtrahend) ;

int int512_compare_long (int512 * self, long toCompare) ;

int int512_compare_int512 (int512 * first, int512 * second) ;

void int512_sum_int512 (int512 * first, int512 * second) ;
