#pragma once

#include "ray.h"

typedef struct hit_record {
	vec3 point;
	vec3 normal;
	size_t material;
	double t;
	bool front_face;
} hit_record;

void hit_record_set_normal(hit_record* rec, ray* r, vec3 n);