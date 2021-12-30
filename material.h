#pragma once

#include "hit_record.h"
#include <stdbool.h>

typedef enum MaterialType {
	MATERIAL_LAMBERTIAN,
	MATERIAL_METAL,
	MATERIAL_DIELECTRIC
} MaterialType;

typedef struct material {
	MaterialType type;
	bool (*scatter)(struct material*, ray*, hit_record*, vec3*, ray*);
	union {
		struct {
			vec3 albedo;
		} lambertian;
		struct {
			vec3 albedo;
			double roughness;
		} metal;
		struct {
			double index;
		} dielectric;
	};
} material;

bool lambertian_init(material* m, vec3 albedo);
bool lambertian_scatter(material* m, ray* r, hit_record* rec, vec3* attenuation, ray* scattered);

bool metal_init(material* m, vec3 albedo, double roughness);
bool metal_scatter(material* m, ray* r, hit_record* rec, vec3* attenuation, ray* scattered);

bool dielectric_init(material* m, double index);
bool dielectric_scatter(material* m, ray* r, hit_record* rec, vec3* attenuation, ray* scattered);