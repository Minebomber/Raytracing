#include "hit_record.h"

void hit_record_set_normal(hit_record* rec, ray* r, vec3 n) {
	rec->front_face = vec3_dot(r->direction, n) < 0;
	rec->normal = rec->front_face ? n : vec3_inv(n);
}