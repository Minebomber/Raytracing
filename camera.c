#include "camera.h"
#include "random.h"

void camera_init(camera* c, vec3 pos, vec3 to, vec3 up, double vfov, double aspect, double aperture, double focus) {
	double t = vfov * M_PI / 180.0;
	double h = tan(t / 2);
	double height = 2.0 * h;
	double width = aspect * height;

	c->w = vec3_unit(vec3_sub(pos, to));
	c->u = vec3_unit(vec3_cross(up, c->w));
	c->v = vec3_cross(c->w, c->u);

	c->origin = pos;
	c->horizontal = vec3_mul(c->u, focus * width);
	c->vertical = vec3_mul(c->v, focus * height);
	c->lower_left = vec3_va_sub(c->origin, 3,
		vec3_div(c->horizontal, 2),
		vec3_div(c->vertical, 2),
		vec3_mul(c->w, focus)
	);
	c->lens = aperture / 2.0;
}

ray camera_ray(camera* c, double s, double t) {
	vec3 rd = vec3_mul(random_unit_disk(), c->lens);
	vec3 offset = vec3_add(
		vec3_mul(c->u, rd.x),
		vec3_mul(c->v, rd.y)
	);
	return (ray) {
		.origin = vec3_add(c->origin, offset), 
		.direction = vec3_va_add(c->lower_left, 4,
			vec3_mul(c->horizontal, s),
			vec3_mul(c->vertical, t),
			vec3_inv(c->origin),
			vec3_inv(offset)
		)
	};
}