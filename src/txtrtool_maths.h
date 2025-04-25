#pragma once

//#include "txtrtool_maths.cpp"


namespace txmaths {
	bool is_pow_2(float x); //uses mantissa to check if it is a power of 2.



	int minmax(int lower, int a, int upper); // limits centre value between either edge.
	float minmax(float lower, float a, float upper);
}

