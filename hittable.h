#pragma once

#include "hit_record.h"
#include <stdbool.h>

typedef enum {
	HITTABLE_SPHERE,
} HittableType;

typedef struct hittable {
	HittableType type;
	bool (*hit)(struct hittable*, ray*, double, double, hit_record*);
	union {
		struct {
			vec3 center;
			double radius;
			size_t material;
		} sphere;
	};
} hittable;

bool sphere_init(hittable* h, vec3 center, double radius, size_t material);
bool sphere_hit(hittable* h, ray* r, double t_min, double t_max, hit_record* rec);