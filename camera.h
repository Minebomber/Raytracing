#pragma once

#include "ray.h"

typedef struct {
	vec3 origin;
	vec3 lower_left;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lens;
} camera;

void camera_init(camera* c, vec3 pos, vec3 to, vec3 up, double vfov, double aspect, double aperture, double focus);
ray camera_ray(camera* c, double u, double v);