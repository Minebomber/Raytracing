#include "ray.h"

vec3 ray_at(ray r, double t) {
	return vec3_add(r.origin, vec3_mul(r.direction, t));
}

vec3 ray_p_at(ray* r, double t) {
	return vec3_add(r->origin, vec3_mul(r->direction, t));
}