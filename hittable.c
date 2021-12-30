#include "hittable.h"

bool sphere_init(hittable* h, vec3 center, double radius, size_t material) {
	if (!h) return false;
	h->type = HITTABLE_SPHERE;
	h->hit = sphere_hit;
	h->sphere.center = center;
	h->sphere.radius = radius;
	h->sphere.material = material;
	return true;
} 

bool sphere_hit(hittable* h, ray* r, double t_min, double t_max, hit_record* rec) {
	vec3 oc = vec3_p_sub_p(&r->origin, &h->sphere.center);
	double a = vec3_p_length2(&r->direction);
	double half_b = vec3_p_dot(&r->direction, oc);
	double c = vec3_p_length2(&oc) - h->sphere.radius * h->sphere.radius;
	double disc = half_b * half_b - a * c;
	if (disc < 0)
		return false;
	double sqrtd = sqrt(disc);
	double root = (-half_b - sqrt(disc)) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}
	rec->t = root;
	rec->point = ray_p_at(r, root);
	hit_record_set_normal(rec, r, vec3_div(vec3_sub(rec->point, h->sphere.center), h->sphere.radius));
	rec->material = h->sphere.material;
	return true;
}