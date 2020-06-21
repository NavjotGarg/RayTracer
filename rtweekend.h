#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <cstdlib>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.14159f;

// Utility Functions

inline float degrees_to_radians(float degrees) {
	return degrees * pi / 180;
}

inline float random_float() {
	// returns a random real in [0,1)
	return rand() / (RAND_MAX + 1.0f);
}

inline float random_float(float min, float max) {
	// returns a random real in [0,1)
	return min + (max-min)*random_float();
}

inline float clamp(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

// Common Headers

#include "ray.h"
#include "vec3.h"

#endif