#pragma once

#include <stdarg.h>
#include <stdbool.h>
#define _USE_MATH_DEFINES
#include <math.h>

typedef struct {
	double x, y, z;
} vec3;

vec3 vec3_inv(vec3 v);
void vec3_ip_inv(vec3* v);

vec3 vec3_add(vec3 u, vec3 v);
void vec3_ip_add(vec3* u, vec3 v);
vec3 vec3_va_add(vec3 u, int n, ...);
void vec3_ip_va_add(vec3* u, int n, ...);

vec3 vec3_sub(vec3 u, vec3 v);
void vec3_ip_sub(vec3* u, vec3 v);
vec3 vec3_va_sub(vec3 u, int n, ...);
void vec3_ip_va_sub(vec3* u, int n, ...);
vec3 vec3_p_sub_p(vec3* u, vec3* v);

vec3 vec3_mul(vec3 v, double c);
void vec3_ip_mul(vec3* v, double c);
vec3 vec3_mul_vec3(vec3 u, vec3 v);

vec3 vec3_div(vec3 v, double c);
void vec3_ip_div(vec3* v, double c);

double vec3_length(vec3 v);
double vec3_length2(vec3 v);
double vec3_p_length2(vec3* v);

vec3 vec3_unit(vec3 v);
void vec3_ip_unit(vec3* v);

double vec3_dot(vec3 u, vec3 v);
double vec3_p_dot(vec3* u, vec3 v);
vec3 vec3_cross(vec3 u, vec3 v);

bool vec3_near_zero(vec3 v);

vec3 vec3_reflect(vec3 v, vec3 n);
vec3 vec3_refract(vec3 v, vec3 n, double c);