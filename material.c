#include "material.h"
#include "hittable.h"
#include "random.h"

bool lambertian_init(material* m, vec3 albedo) {
	if (!m) return false;
	m->type = MATERIAL_LAMBERTIAN;
	m->scatter = lambertian_scatter;
	m->lambertian.albedo = albedo;
	return true;
}

bool lambertian_scatter(material* m, ray* r, hit_record* rec, vec3* attenuation, ray* scattered) {
	vec3 scatter_dir = vec3_add(rec->normal, random_unit_vector());
	if (vec3_near_zero(scatter_dir))
		scatter_dir = rec->normal;
	*attenuation = m->lambertian.albedo;
	*scattered = (ray){ .origin = rec->point, .direction = scatter_dir };
	return true;
}

bool metal_init(material* m, vec3 albedo, double roughness) {
	if (!m) return false;
	m->type = MATERIAL_METAL;
	m->scatter = metal_scatter;
	m->metal.albedo = albedo;
	m->metal.roughness = (roughness < 1) ? roughness : 1;
	return true;
}

bool metal_scatter(material* m, ray* r, struct hit_record* rec, vec3* attenuation, ray* scattered) {
	vec3 reflected = vec3_reflect(vec3_unit(r->direction), rec->normal);
	*attenuation = m->metal.albedo;
	*scattered = (ray) {
		.origin = rec->point,
		.direction = vec3_add(reflected, vec3_mul(random_unit_sphere(), m->metal.roughness))
	};
	return (vec3_dot(scattered->direction, rec->normal) > 0.0);
}

bool dielectric_init(material* m, double index) {
	if (!m) return false;
	m->type = MATERIAL_DIELECTRIC;
	m->scatter = dielectric_scatter;
	m->dielectric.index = index;
	return true;
}

static double reflectance(double cost, double index) {
	double r0 = (1 - index) / (1 + index);
	r0 *= r0;
	return r0 + (1 - r0) * pow((1 - cost), 5);
}

bool dielectric_scatter(material* m, ray* r, hit_record* rec, vec3* attenuation, ray* scattered) {
	*attenuation = (vec3){ 1.0, 1.0, 1.0 };
	scattered->origin = rec->point;
	double ratio = rec->front_face ? (1.0 / m->dielectric.index) : m->dielectric.index;
	vec3 unit = vec3_unit(r->direction);
	double cost = fmin(vec3_dot(vec3_inv(unit), rec->normal), 1.0);
	double sint = sqrt(1.0 - cost * cost);
	if (ratio * sint > 1.0 || reflectance(cost, ratio) > random())
		scattered->direction = vec3_reflect(unit, rec->normal);
	else
		scattered->direction = vec3_refract(unit, rec->normal, ratio);
	return true;
}