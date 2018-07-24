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

	for (int i = 0; i < COUNT_LEVEL; i++)
		int512_setat (a, i, LONG_MAX);

	for (int i = 0; i < COUNT_LEVEL; i++)
		int512_setat (b, i, LONG_MIN);

	int512_print (a);
	printf ("\n\n");
	int512_print (b);
	printf ("\n\n");
	int512_sum_long (a, 2);
	int512_print (a);
	printf ("\n\n");
	int512_sum_long (b, -2);
	int512_print (b);

	printf ("\n\nLOLKEK-LOLKEK\n\n");
	int512_sum_long (c, LONG_MAX);
	int512_print (c);
	int512_sum_long (c, LONG_MAX);
	int512_print (c);


	int512_free (a);
	int512_free (b);
	int512_free (c);
	int512_free (d);
	int512_free (e);

	return EXIT_SUCCESS;
}
