#pragma once

#include "vec3.h"

typedef struct {
	vec3 origin;
	vec3 direction;
} ray;

vec3 ray_at(ray r, double t);
vec3 ray_p_at(ray* r, double t);