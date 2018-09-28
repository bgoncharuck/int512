#include "int512.h"

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>



int main (void) {

	int512 * a = int512_new();
	int512 * b = int512_new();
	int512 * c = int512_new();
	int512 * d = int512_new();
	int512 * e = int512_new();
	int512 * z = int512_new();
	int512 * x = int512_new();

	int512_set_max (a);
	int512_set_min (b);

	int512_print (a);
	printf ("\n\n");
	int512_print (b);
	printf ("\n\n");
	int512_sum_int (a, 2);
	int512_print (a);
	printf ("\n\n");
	int512_sum_int (b, -2);
	int512_print (b);

	printf ("\n\nSUMn\n");
	int512_sum_int (c, INT_MAX);
	int512_print (c);
	int512_sum_int (c, INT_MAX);
	int512_print (c);

	int512_sum_int (d, 42);
	int512_set_value (e, 2);
	int512_set_value (x, 2);

	printf ("\n\nMultn\n");
	int512_product_int_byLevel (x, TOP_LEVEL, -42);
	int512_print(x);
	int512_mult_int512 (e, d);
	int512_print(e);

	int512_free (a);
	int512_free (b);
	int512_free (c);
	int512_free (d);
	int512_free (e);

	return EXIT_SUCCESS;
}
