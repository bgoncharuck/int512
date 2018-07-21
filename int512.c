#include "int512.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// you can change this adding abort() or else
#define throw(MSG) fprintf(stderr, "%s\n",MSG)

#define TOP_LEVEL -1 + COUNT_LEVEL

struct __int512 {
	long at[COUNT_LEVEL];
};


int512 * int512_new() {

	int512 * self = malloc (sizeof(int512));
	if (self == NULL) abort();

	int size = TOP_LEVEL;

	while (1 + size)
		self->at[size--] = 0;


	return self;
}


void int512_free (int512 * self) {

	if (self == NULL) {
		throw("null pointer in int512_free()");
		return;
	}

	free(self);
}


long long int512_at (int512 * self, int i) {

	if (self == NULL) {
		throw("null pointer in int512_at()");
		return 0;
	}


	if (i >= COUNT_LEVEL || i < 0) {
		throw("out of bounders in int512_at()");
		return 0;
	}

	return self->at[i];
}

static void int512_sum_long_posTop_negSum_negAddition \
	(int512 * self, int previousLevel, long addition) {

	if (self->at[previousLevel-1] > 0) {

		self->at[previousLevel-1]--;

		for (int i = previousLevel; i < TOP_LEVEL; i++)
			self->at[i] = LONG_MAX;

		self->at[TOP_LEVEL] = LONG_MAX + addition;
	}

	else if (self->at[previousLevel-1] == 0) {

		if (previousLevel != 1)
			int512_sum_long_posTop_negSum_negAddition (self, previousLevel-1, addition);

		else
			self->at[TOP_LEVEL] = addition;
	}


}

static void int512_sum_long_posTop_negSum_posAddition \
	(int512 * self, int previousLevel, long addition) {

	if (self->at[previousLevel-1] < LONG_MAX) {

		self->at[previousLevel-1]++;

		for (int i = previousLevel; i < TOP_LEVEL; i++)
			self->at[i] = 0;

		self->at[TOP_LEVEL] = -1 * addition;

		return;
	}

	else {
		if (previousLevel != 1)
			int512_sum_long_posTop_negSum_posAddition (self, previousLevel-1, addition);

		else {
			for (int i = previousLevel-1; i < COUNT_LEVEL; i++)
				self->at[i] = LONG_MIN;

			self->at[TOP_LEVEL] = LONG_MIN - addition;

			return;
		}
	}
}

void int512_sum_long (int512 * self, long addition) {

	if (self == NULL) {
		throw("null pointer in int512_sum_long()");
		return;
	}

	if (addition == 0)
		return;


	else if (self->at[TOP_LEVEL] >= 0) {

		if ((self->at[TOP_LEVEL] + addition) > 0) {
			self->at[TOP_LEVEL] += addition;
			return;
		}

		else if ((self->at[TOP_LEVEL] + addition) < 0) {

			if (addition < 0) {

				addition += self->at[TOP_LEVEL];

				int512_sum_long_posTop_negSum_negAddition (self, TOP_LEVEL, addition);
			}

			else {

				addition = LONG_MAX - self->at[TOP_LEVEL] - addition;

				int512_sum_long_posTop_negSum_posAddition (self, TOP_LEVEL, addition);
			}

		}

		else {
			if (addition < 0)
				self->at[TOP_LEVEL] = 0;

			else {
				int512_sum_long (self, addition-1);
				int512_sum_long (self, 1);
				// @TODO change to lower level solution
			}
		}

	}

}
