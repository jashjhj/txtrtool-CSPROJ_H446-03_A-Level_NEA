

#include "txtrtool_maths.h"

#include <math.h>

bool txmaths::is_pow_2(float x) { // checks if is a power of 2 using its mantissa
	int exponent = 0;
	float mantissa = frexp(x, &exponent); //takes the mantissa, tacks it onto new float with exp 0.
	return mantissa == 0.5f ? true : false;
}

