#include "vec3.h"

vec3 vec3_inv(vec3 v) {
	return (vec3){
		.x = -v.x,
		.y = -v.y,
		.z = -v.z
	};
}

void vec3_ip_inv(vec3* v) {
	v->x = -v->x;
	v->y = -v->y;
	v->z = -v->z;
}

vec3 vec3_add(vec3 u, vec3 v) {
	return (vec3){
		.x = u.x + v.x,
		.y = u.y + v.y,
		.z = u.z + v.z
	};
}

void vec3_ip_add(vec3* u, vec3 v) {
	u->x += v.x;
	u->y += v.y;
	u->z += v.z;
}

vec3 vec3_va_add(vec3 u, int n, ...) {
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
		vec3_ip_add(&u, va_arg(args, vec3));
	va_end(args);
	return u;
}

void vec3_ip_va_add(vec3* u, int n, ...) {
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
		vec3_ip_add(u, va_arg(args, vec3));
	va_end(args);
}

vec3 vec3_sub(vec3 u, vec3 v) {
	return (vec3) {
		.x = u.x - v.x,
		.y = u.y - v.y,
		.z = u.z - v.z
	};
}

void vec3_ip_sub(vec3* u, vec3 v) {
	u->x -= v.x;
	u->y -= v.y;
	u->z -= v.z;
}

vec3 vec3_va_sub(vec3 u, int n, ...) {
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
		vec3_ip_sub(&u, va_arg(args, vec3));
	va_end(args);
	return u;
}

void vec3_ip_va_sub(vec3* u, int n, ...) {
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++)
		vec3_ip_sub(u, va_arg(args, vec3));
	va_end(args);
}

vec3 vec3_p_sub_p(vec3* u, vec3* v) {
	return (vec3) {
		.x = u->x - v->x,
		.y = u->y - v->y,
		.z = u->z - v->z
	};
}

vec3 vec3_mul(vec3 v, double c) {
	return (vec3) {
		.x = v.x * c,
		.y = v.y * c,
		.z = v.z * c
	};
}

void vec3_ip_mul(vec3* v, double c) {
	v->x *= c;
	v->y *= c;
	v->z *= c;
}

vec3 vec3_mul_vec3(vec3 u, vec3 v) {
	return (vec3) {
		.x = u.x * v.x,
		.y = u.y * v.y,
		.z = u.z * v.z
	};
}

vec3 vec3_div(vec3 v, double c) {
	return (vec3) {
		.x = v.x / c,
		.y = v.y / c,
		.z = v.z / c
	};
}

void vec3_ip_div(vec3* v, double c) {
	v->x /= c;
	v->y /= c;
	v->z /= c;
}

double vec3_length(vec3 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

double vec3_length2(vec3 v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

double vec3_p_length2(vec3* v) {
	return v->x * v->x + v->y * v->y + v->z * v->z;
}

vec3 vec3_unit(vec3 v) {
	return vec3_div(v, vec3_length(v));
}

void vec3_ip_unit(vec3* v) {
	vec3_ip_div(v, vec3_length(*v));
}

double vec3_dot(vec3 u, vec3 v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

double vec3_p_dot(vec3* u, vec3 v) {
	return u->x * v.x + u->y * v.y + u->z * v.z;
}

vec3 vec3_cross(vec3 u, vec3 v) {
	return (vec3) {
		.x = u.y * v.z - u.z * v.y,
		.y = u.z * v.x - u.x * v.z,
		.z = u.x * v.y - u.y * v.x
	};
}

bool vec3_near_zero(vec3 v) {
	return (fabs(v.x) < 1e-8) && (fabs(v.y) < 1e-8) && (fabs(v.z) < 1e-8);
}

vec3 vec3_reflect(vec3 v, vec3 n) {
	return vec3_sub(v, vec3_mul(n, 2.0 * vec3_dot(v, n)));
}

vec3 vec3_refract(vec3 v, vec3 n, double c) {
	double cost = fmin(vec3_dot(vec3_inv(v), n), 1.0);
	vec3 a = vec3_mul(vec3_add(v, vec3_mul(n, cost)), c);
	vec3 b = vec3_mul(n, -sqrt(fabs(1.0 - vec3_length2(a))));
	return vec3_add(a, b);
}