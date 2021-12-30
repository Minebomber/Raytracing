#pragma once

#include "vec3.h"

double random();
vec3 vec3_random();
double randrange(double min, double max);
vec3 vec3_randrange(double min, double max);
vec3 random_unit_sphere();
vec3 random_unit_vector();
vec3 random_hemisphere(vec3 normal);
vec3 random_unit_disk();