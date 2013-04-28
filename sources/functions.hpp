#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <stdlib.h>

template <typename T> T min(T a, T b) { return a < b ? a : b; }
template <typename T> T max(T a, T b) { return a > b ? a : b; }

inline float frand() {
	return rand() / (float)RAND_MAX;
}

inline float frand(float a, float b) {
	return frand() * (b - a) + a;
}

#endif
