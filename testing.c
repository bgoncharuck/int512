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
	int512_sum_long (a, 1);
	int512_print (a);
	printf ("\n\n");
	int512_sum_long (b, -1);
	int512_print (b);


	return EXIT_SUCCESS;
}
