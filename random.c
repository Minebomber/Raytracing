#include "random.h"
#include <stdlib.h>

double random() {
	return rand() / (RAND_MAX + 1.0);
}

vec3 vec3_random() {
	return (vec3) { random(), random(), random() };
}

double randrange(double min, double max) {
	return min + (max - min) * random();
}

vec3 vec3_randrange(double min, double max) {
	return (vec3) { randrange(min, max), randrange(min, max), randrange(min, max) };
}

vec3 random_unit_sphere() {
	while (true) {
		vec3 v = vec3_randrange(-1, 1);
		if (vec3_length2(v) < 1) return v;
	}
	return (vec3) { 0 };
}

vec3 random_unit_vector() {
	return vec3_unit(random_unit_sphere());
}

vec3 random_hemisphere(vec3 normal) {
	vec3 unit = random_unit_sphere();
	if (vec3_dot(unit, normal) > 0.0)
		return unit;
	return vec3_inv(unit);
}

vec3 random_unit_disk() {
	while (true) {
		vec3 p = { randrange(-1, 1), randrange(-1, 1), 0 };
		if (vec3_length2(p) >= 1) continue;
		return p;
	}
}