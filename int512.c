#include "int512.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// you can change this by adding abort() or else
#define throw(MSG) fprintf(stderr, "%s\n",MSG)

#define TOP_LEVEL -1 + COUNT_LEVEL

// @INT512
struct __int512 {
	int at[COUNT_LEVEL];
};

int512 * int512_new() {

	int512 * self = malloc (sizeof(int512));
	if (self == NULL) abort();

	int512_set_value (self, 0);

	return self;
}

void int512_free (int512 * self) {

	if (self == NULL) {
		throw("null pointer in int512_free()");
		return;
	}

	free(self);
}

int int512_at (int512 * self, int i) {

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

void int512_set_at (int512 * self, int i, int value) {

	if (self == NULL) {
		throw("null pointer in int512_setat()");
		return;
	}


	if (i >= COUNT_LEVEL || i < 0) {
		throw("out of bounders in int512_setat()");
		return;
	}

	self->at[i] = value;
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

static void int512_sum_int_posTop_negSum_negAddition \
	(int512 * self, int previousLevel, int fromLevel, int addition) {

	// _0_1_0_0_-2_4_7
	if (self->at[previousLevel-1] > 0) {

		// _0_0_0_0_-2_4_7 reduce current level by one
		self->at[previousLevel-1]--;

		// _0_0_9_9_-2_4_7 change all levels till fromLevel to MAX val
		for (int i = previousLevel; i < fromLevel; i++)
			self->at[i] = INT_MAX;

		// _0_0_9_9_8_4_7
		self->at[fromLevel] = INT_MAX + addition + 1;
	}

	else if (self->at[previousLevel-1] == 0) {

		if (previousLevel != 1)
			int512_sum_int_posTop_negSum_negAddition (self, previousLevel-1, fromLevel, addition);

		else {
			// _0_0_0_0_-2_4_7 all lower levels are zeroes, so we just paste addition in current level
			self->at[fromLevel] = addition;

			for (int i = TOP_LEVEL; i > fromLevel; i--)

				if (self->at[i] > 0) { // if there any positive numbers in higher levels we add it
					// _0_0_0_0_-1_-5_-3
					//
					//  |4|5|6| levels
					// |-2|4|7| values
					//
					// level += -1 if > 0 + MIN value
					// then we increase lower level by one
					// and do so for each level from top till fromLevel
					//
					// 1. |6| = 7 -1 -9 = -3; |5|++ = 5
					// |-2|5|-3|
					// 2. |5| = 5 -1 -9 = -5; |4|++ = -1
					// |-1|-5|-3|
					for (int j = i; j > fromLevel; j--) {
						self->at[j] += \
						((self->at[j]>0) ? -1 : 0) + INT_MIN;

						self->at[j-1]++;
					}
					return;
				}
		}
	}
}

static void int512_sum_int_posTop_negSum_posAddition \
	(int512 * self, int previousLevel, int fromLevel, int addition) {

	// 0_3_9_9_-2_4_5
	if (self->at[previousLevel-1] < INT_MAX) {

		// 0_4_9_9_-2_4_5 increase current level by one
		self->at[previousLevel-1]++;

		// 0_4_0_0_-2_4_5 change all levels till fromLevel to zero
		for (int i = previousLevel; i < fromLevel; i++)
			self->at[i] = 0;

		// 0_4_0_0_2_4_5
		self->at[fromLevel] = -1 * addition;
	}

	else {
		if (previousLevel != 1)
			int512_sum_int_posTop_negSum_posAddition (self, previousLevel-1, fromLevel, addition);

		else {
			// 9_9_9_9_-2_4_5 now we must create our own "out of bounders"

			// -9_-9_-9_-9_-2_4_5
			for (int i = 0; i < fromLevel; i++)
				self->at[i] = INT_MIN;

			// -9_-9_-9_-9_2_4_5
			self->at[fromLevel] = -1 * addition;

			// -9_-9_-9_-9_-7_-5_-4
			for (int i = fromLevel; i <= TOP_LEVEL; i++)
				self->at[i] += INT_MIN;
		}
	}
}

static void int512_sum_int_negTop_posSum_posAddition \
	(int512 * self, int previousLevel, int fromLevel, int addition) {

	// 0_-3_0_0_3_-3_-4
	if (self->at[previousLevel-1] < 0) {

		// 0_-2_0_0_3_-3_-4
		self->at[previousLevel-1]++;

		// 0_-2_-9_-9_3_-3_-4
		for (int i = previousLevel; i < fromLevel; i++)
			self->at[i] = INT_MIN;

		// 0_-2_-9_-9_-6_-3_-4
		self->at[fromLevel] = INT_MIN + addition;
	}

	else {
		if (previousLevel != 1)
			int512_sum_int_negTop_posSum_posAddition (self, previousLevel-1, fromLevel, addition);

		else {
			// 0_0_0_3_-3_-4 all lower levels are zeroes, so we just paste addition in current level
			self->at[fromLevel] = addition;

			for (int i = TOP_LEVEL; i > fromLevel; i--)

				if (self->at[i] < 0) {

					// _0_0_0_2_6_6
					//
					// |3|4 |5 |
					// |3|-3|-4|
					//
					// level += 1 if < 0 + MAX values
					// then we decreace lower level by one
					// and do so for each level from top till fromLevel
					//
					// 1. |5| = -4 + 1 + 9 = 6; |4|-- = -4;
					// |3|-4|6|
					// 2. |4| = -4 + 1 + 9 = 6; |3|-- = 2;
					// |2|6|6|
					for (int j = i; j > fromLevel; j--) {
						self->at[j] += \
						((self->at[j]<0) ? 1 : 0) + INT_MAX;

						self->at[j-1]--;
					}
					return;
				}
		}
	}
}

static void int512_sum_int_negTop_posSum_negAddition \
	(int512 * self, int previousLevel, int fromLevel, int addition) {

	// _0_-2_-9_-9_3_-4_-2
	if (self->at[previousLevel-1] > INT_MIN) {

		// _0_-3_-9_-9_3_-4_-2
		self->at[previousLevel-1]--;

		// _0_-3_0_0_3_-4_-2
		for (int i = previousLevel; i < fromLevel; i++)
			self->at[i] = 0;

		// _0_-3_0_0_-3_-4_-2
		self->at[fromLevel] = -1 * addition;
	}

	else {
		if (previousLevel != 1)
			int512_sum_int_negTop_posSum_negAddition (self, previousLevel-1, fromLevel, addition);

		else {
			// _-9_-9_-9_3_-4_-2 let's create our own "out of bounders"

			// _9_9_9_3_-4_-2
			for (int i = 0; i < fromLevel; i++)
				self->at[i] = INT_MAX;

			// _9_9_9_-3_-4_-2
			self->at[fromLevel] = -1 * addition;

			// _9_9_9_6_5_7
			for (int i = fromLevel; i <= TOP_LEVEL; i++)
				self->at[i] += INT_MAX;
		}
	}
}

void int512_sum_int_byLevel (int512 * self, int level, int addition) {

	if (self == NULL) {
		throw("null pointer in int512_sum_int()");
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

				int512_sum_int_posTop_negSum_negAddition (self, level, level, addition);
			}

			else {

				addition = INT_MAX - self->at[level] - addition;

				int512_sum_int_posTop_negSum_posAddition (self, level, level, addition);
			}

		}

		else {
			if (addition < 0)
				self->at[level] = 0;

			else {
				int512_sum_int_byLevel (self, level, addition-1);
				int512_sum_int_byLevel (self, level, 1);
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

				int512_sum_int_negTop_posSum_posAddition (self, level, level, addition);
			}

			else {
				addition = INT_MIN - self->at[level] - addition;

				int512_sum_int_negTop_posSum_negAddition (self, level, level, addition);
			}
		}

		else {
			if (addition > 0)
				self->at[level] = 0;

			else {
				int512_sum_int_byLevel (self, level, addition+1);
				int512_sum_int_byLevel (self, level, -1);
			}
		}
	}
}

void int512_sum_int (int512 * self, int addition) {

	int512_sum_int_byLevel (self, TOP_LEVEL, addition);
}

void int512_difference_int (int512 * self, int subtrahend) {

	if (self == NULL) {
		throw("null pointer in int512_difference_int()");
		return;
	}

	if (subtrahend == 0) return;

	// example in ascii char : -128 * (-1) != 128, because 127 is max
	// solution is	x * (-1) == x - x*2 == 0 - x
	int addition = 0; addition -= subtrahend;

	int512_sum_int (self, addition);
}

int int512_compare_int (int512 * self, int toCompare) {

	if (self == NULL) {
		throw("null pointer in int512_compare_int()");
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

void int512_copy_int512 (int512 * to, int512 * from) {

	if (to == NULL || from == NULL) {
		throw("null pointer in int512_copy_int512()");
		return;
	}

	for (int i = 0; i < COUNT_LEVEL; i++)
		to->at[i] = from->at[i];
}

int512 * int512_copy_new (int512 * from) {

	if (from == NULL) {
		throw("null pointer in int512_copy_new()");
		return NULL;
	}

	int512 * to = int512_new();

	int512_copy_int512 (to, from);

	return to;
}

void int512_sum_int512 (int512 * first, int512 * second) {

	if (first == NULL || second == NULL) {
		throw("null pointer in int512_sum_int512()");
		return;
	}

	for (int i = TOP_LEVEL; i >= 0; i--)
		int512_sum_int_byLevel (first, i, second->at[i]);
}

int512 * int512_sum_new (int512 * first, int512 * second) {

	if (first == NULL || second == NULL) {
		throw("null pointer in int512_sum_new()");
		return NULL;
	}

	int512 * self = int512_new();

	int512_sum_int512 (self, first);
	int512_sum_int512 (self, second);

	return self;
}

void int512_difference_int512 (int512 * first, int512 * second) {

	if (first == NULL || second == NULL) {
		throw("null pointer in int512_difference_int512()");
		return;
	}

	// see difference for int
	int temporary = 0;

	for (int i = TOP_LEVEL; i >= 0; i--) {
		temporary -= second->at[i];
		int512_sum_int_byLevel (first, i, temporary);
		temporary = 0;
	}
}

int512 * int512_subtrahend_int (int minued, int512 * subtrahend) {

	if (subtrahend == NULL) {
		throw("null pointer in int512_subtrahend_int()");
		return NULL;
	}

	int512 * self = int512_new();
	int512_sum_int (self, minued);
	int512_difference_int512 (self, subtrahend);

	return self;
}

/*
void int512_leveledProduct_inTwoLong (long * resulth, long * resultl, int a, int b) {
  __asm__(
    "imull %3 \n\t"
    :"=a" (*resultl), "=d" (*resulth)
    :"a" (a), "rm" (b) :
    );
}
*/

void int512_leveledProduct_inTwoLong (long * resulth, long * resultl, int a, int b) {
	long temp = a;
	temp *= b;
	*resultl =  temp & INT_MAX;
	*resulth =  temp >> (sizeof(INT_MAX) * 8 - 1);
}

static void int512_product_int_operation \
	(int512 * self, int previousLevel, int fromLevel, int multiplier) {

	long lvlOut0 = 0, // level
	lvlOut1 = 0, // level
	lvlOut2 = 0; // addition

	for (int i = fromLevel; i > 0; i--) {
		lvlOut0 = 0; lvlOut1 = 0;
		int512_leveledProduct_inTwoLong (&lvlOut0, &lvlOut1, self->at[i], multiplier);

		if (lvlOut1 > INT_MAX)
			while (lvlOut1 > INT_MAX) {
				lvlOut1 -= INT_MAX;
				lvlOut0++;
			}

		if ((lvlOut1 + lvlOut2) > INT_MAX)
			while ((lvlOut1 + lvlOut2) > INT_MAX) {
				lvlOut2 -= INT_MAX;
				lvlOut0++;
			}


		self->at[i] = lvlOut1 + lvlOut2;

		lvlOut2 = lvlOut0;
	}

	// If i == 0 && lvlOut2 != 0 we must create out of borders
	if (lvlOut2 != 0) {

		for (int i = 0; i <= TOP_LEVEL; i++)
			self->at[i] *= -1;

		self->at[0] += lvlOut2;
	}
}

void int512_product_int_byLevel (int512 * self, int level, int multiplier) {

	if (self == NULL) {
		throw("null pointer in int512_product_int()");
		return;
	}

	if (multiplier == 0) {
		int512_set_value_fromLevel (self, level, 0);
		return;
	}

	else if (int512_nullEntrance (self, level) == -1)
		return;

	else if (self->at[level] > 0) {
		if (multiplier > 0) {


		}
		else {


		}
	}

	else if (self->at[level] < 0) {
		if (multiplier > 0) {


		}
		else {


		}
	}
}

void int512_product_int512 (int512 * base, int512 * multiplier) {

	// i will use int512 as temp values array (not in usual way)
	// int512 * sumArray[fromLevel + 1];
	// for (int i = 0; i <= fromLevel; i++)
		// sumArray[i] = int512_new();
}

void int512_set_value_fromLevel (int512 * self, int level, int value) {

	if (self == NULL) {
		throw("null pointer in int512_set_value_fromLevel()");
		return;
	}

	for (int i = level; i >= 0; i--)
		self->at[i] = value;
}

void int512_set_value (int512 * self, int value) {

	int512_set_value_fromLevel (self, TOP_LEVEL, value);
}

void int512_set_min (int512 * self) {

	int512_set_value (self, INT_MIN);
}

void int512_set_max (int512 * self) {

	int512_set_value (self, INT_MAX);
}

int int512_fromLevel_value (int512 * self, int fromLevel, int value) {

	if (self == NULL) {
		throw("null pointer in int512_fromLevel_value()");
		return;
	}

	if (value != 0) {
		for (int i = fromLevel; i >= 0; i--)

			if (self->at[i] == 0) {
				if (i == fromLevel) return 0;

				else if (self->at[i+1] == 0 && self->at[i] != 0)
					return 0;
			}

			else if (self->at[i] != value)
				return 0;
	}
	else {
		for (int i = fromLevel; i >= 0; i--)
			if (self->at[i] != 0)
				return 0;
	}

	return 1;
}

int int512_nullEntrance (int512 * self, int fromLevel) {

	if (self == NULL) {
		throw("null pointer in int512_nullEntrance()");
		return -42;
	}

	for (int i = fromLevel; i >= 0; i--)
		if (self->at[i] != 0)
			return i;

	return -1;
}

char * int512_toBase (int512 * self, unsigned base) {

	return NULL;
}

int512 * int512_fromBase (char * str, unsigned base) {

	return NULL;
}
