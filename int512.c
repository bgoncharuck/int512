#include "int512.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// you can change this by adding abort() or else
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

long int512_at (int512 * self, int i) {

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

void int512_setat (int512 * self, int i, long val) {

	if (self == NULL) {
		throw("null pointer in int512_setat()");
		return;
	}


	if (i >= COUNT_LEVEL || i < 0) {
		throw("out of bounders in int512_setat()");
		return;
	}

	self->at[i] = val;
}

void int512_print (int512 * self) {

	if (self == NULL) {
		throw("null pointer in int512_print()");
		return;
	}

	for (int i = 0; i <= TOP_LEVEL; i++)
		printf ("_%ld", self->at[i]);

	printf("\n");
}

static void int512_sum_long_posTop_negSum_negAddition \
	(int512 * self, int previousLevel, int topLevel, long addition) {

	// example
	// self->at[0]--;
	// self->at[1] = LONG_MAX;
	// self->at[2] = LONG_MAX;
	// self->at[TOP] = LONG_MAX + addition;

	if (self->at[previousLevel-1] > 0) {

		self->at[previousLevel-1]--;

		for (int i = previousLevel; i < topLevel; i++)
			self->at[i] = LONG_MAX;

		self->at[topLevel] = LONG_MAX + addition;
	}

	// example
	// if self->at[5] == 0
	// 	if self->at[4] == 0....
	// 		if self->at[topLevel] = addition;

	else if (self->at[previousLevel-1] == 0) {

		if (previousLevel != 1)
			int512_sum_long_posTop_negSum_negAddition (self, previousLevel-1, topLevel, addition);

		else
			self->at[topLevel] = addition;
	}
}

static void int512_sum_long_posTop_negSum_posAddition \
	(int512 * self, int previousLevel, int topLevel, long addition) {

	// example
	// self->at[0]++;
	// self->at[1] = 0;
	// self->at[2] = 0;
	// self->at[TOP] = -1 * addition;

	if (self->at[previousLevel-1] < LONG_MAX) {

		self->at[previousLevel-1]++;

		for (int i = previousLevel; i < topLevel; i++)
			self->at[i] = 0;

		self->at[topLevel] = -1 * addition;
	}

	// example
	// if self->at[5] == LONG_MAX
	// 	if self->at[4] == LONG_MAX....
	// 		if self->at[topLevel] = LONG_MIN - addition;

	else {
		if (previousLevel != 1)
			int512_sum_long_posTop_negSum_posAddition (self, previousLevel-1, topLevel, addition);

		else {
			for (int i = previousLevel-1; i < topLevel; i++)
				self->at[i] = LONG_MIN;

			self->at[topLevel] = LONG_MIN - addition;
		}
	}
}

static void int512_sum_long_negTop_posSum_posAddition \
	(int512 * self, int previousLevel, int topLevel, long addition) {

	// see posTop examples

	if (self->at[previousLevel-1] == 0) {

		if (previousLevel != 1)
			int512_sum_long_negTop_posSum_posAddition (self, previousLevel-1, topLevel, addition);

		else
			self->at[topLevel] = addition;
	}

	else if (self->at[previousLevel-1] < 0) {

		self->at[previousLevel-1]++;

		for (int i = previousLevel; i < topLevel; i++)
			self->at[i] = LONG_MIN;

		self->at[topLevel] = LONG_MIN + addition;
	}
}

static void int512_sum_long_negTop_posSum_negAddition \
	(int512 * self, int previousLevel, int topLevel, long addition) {

	// see posTop examples

	if (self->at[previousLevel-1] > LONG_MIN) {

		self->at[previousLevel-1]--;

		for (int i = previousLevel; i < topLevel; i++)
			self->at[i] = 0;

		self->at[topLevel] = LONG_MIN + addition;
	}

	else {
		if (previousLevel != 1)
			int512_sum_long_negTop_posSum_negAddition (self, previousLevel-1, topLevel, addition);

		else {
			for (int i = previousLevel-1; i < topLevel; i++)
				self->at[i] = LONG_MAX;

			self->at[topLevel] = LONG_MAX + addition;
		}
	}
}

void int512_sum_long_byLevel (int512 * self, int level, long addition) {

	if (self == NULL) {
		throw("null pointer in int512_sum_long()");
		return;
	}
	//
	if (addition == 0) return;
	//
	//
	else if (self->at[level] >= 0) {

		if ((self->at[level] + addition) > 0) {
			self->at[level] += addition;
			return;
		}

		else if ((self->at[level] + addition) < 0) {

			if (addition < 0) {

				addition += self->at[level];

				int512_sum_long_posTop_negSum_negAddition (self, level, level, addition);
			}

			else {

				addition = LONG_MAX - self->at[level] - addition;

				int512_sum_long_posTop_negSum_posAddition (self, level, level, addition);
			}

		}

		else {
			if (addition < 0)
				self->at[level] = 0;

			else {
				int512_sum_long_byLevel (self, level, addition-1);
				int512_sum_long_byLevel (self, level, 1);
			}
		}

	}
	//
	//
	else if (self->at[level] < 0) {

		if ((self->at[level] + addition) < 0) {
			self->at[level] += addition;
			return;
		}

		else if ((self->at[level] + addition) > 0) {

			if (addition > 0) {

				addition += self->at[level];

				int512_sum_long_negTop_posSum_posAddition (self, level, level, addition);
			}

			else {

				addition = LONG_MIN + self->at[level] + addition;

				int512_sum_long_negTop_posSum_negAddition (self, level, level, addition);
			}

		}

		else {
			if (addition > 0)
				self->at[level] = 0;

			else {
				int512_sum_long_byLevel (self, level, addition+1);
				int512_sum_long_byLevel (self, level, -1);
			}
		}
	}

}

void int512_sum_long (int512 * self, long addition) {

	int512_sum_long_byLevel (self, TOP_LEVEL, addition);
}

void int512_difference_long (int512 * self, long subtrahend) {

	if (self == NULL) {
		throw("null pointer in int512_difference_long()");
		return;
	}

	if (subtrahend == 0) return;

	// example in ascii char : -128 * (-1) != 128, because 127 is max
	// solution is	x * (-1) == x - x*2 == 0 - x
	long addition = 0; addition -= subtrahend;

	int512_sum_long (self, addition);
}

int int512_compare_long (int512 * self, long toCompare) {

	if (self == NULL) {
		throw("null pointer in int512_compare_long()");
		return 0;
	}

	for (int i = 0; i < TOP_LEVEL; i++)
		if (self->at[i] != 0)
			return (self->at[i] > 0) ? 1 : -1;

	if (self->at[TOP_LEVEL] > toCompare)
		return 1;

	else if (self->at[TOP_LEVEL] < toCompare)
		return -1;

	return 0;
}

int int512_compare_int512 (int512 * first, int512 * second) {

	if (first == NULL || second == NULL) {
		throw("null pointer in int512_compare_int512()");
		return 0;
	}

	for (int i = 0; i < COUNT_LEVEL; i++)
		if (first->at[i] != 0 && second->at[i] == 0)
			return (first->at[i] > 0) ? 1 : -1;

		else if (second->at[i] != 0 && first->at[i] == 0)
			return (second->at[i] > 0) ? -1 : 1;

		else if (first->at[i] != 0 && second->at[i] != 0)
			return (first->at[i] > second->at[i]) ? 1 : -1;

	return 0;
}

void int512_sum_int512 (int512 * first, int512 * second) {

	// @NOW_WE_HER
}
