#include "raytrace.h"
#include "random.h"
#include <time.h>

bool raytrace_init(raytrace* rt, int w, int h) {
	if (!raytrace_resize(rt, w, h))
		return false;

	if (!vector_init(&rt->world, sizeof(hittable)) ||
		!vector_init(&rt->materials, sizeof(material)))
		return false;

	material m;
	hittable obj;

	srand(time(NULL));

	lambertian_init(&m, (vec3) { 0.5, 0.5, 0.5 }); // Ground
	vector_push_back(&rt->materials, &m);
	sphere_init(&obj, (vec3) { 0, -1000, 0 }, 1000, 0);
	vector_push_back(&rt->world, &obj);

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double mat = random();
			vec3 center = { a + 0.9 * random(), 0.2, b + 0.9 * random() };
			if (vec3_length(vec3_sub(center, (vec3) { 4, 0.2, 0 })) > 0.9) {
				if (mat < 0.8) {
					lambertian_init(&m, vec3_random());
				} else if (mat < 0.95) {
					metal_init(&m, vec3_randrange(0.5, 1), randrange(0, 0.5));
				} else {
					dielectric_init(&m, 1.5);
				}
				vector_push_back(&rt->materials, &m);
				sphere_init(&obj, center, 0.2, rt->materials.length - 1);
				vector_push_back(&rt->world, &obj);
			}
		}
	}
	
	dielectric_init(&m, 1.5);
	vector_push_back(&rt->materials, &m);
	sphere_init(&obj, (vec3) { 0, 1, 0 }, 1.0, rt->materials.length - 1);
	vector_push_back(&rt->world, &obj);

	lambertian_init(&m, (vec3) { 0.4, 0.2, 0.1 });
	vector_push_back(&rt->materials, &m);
	sphere_init(&obj, (vec3) { -4, 1, 0 }, 1.0, rt->materials.length - 1);
	vector_push_back(&rt->world, &obj);

	metal_init(&m, (vec3) { 0.7, 0.6, 0.5 }, 0.0);
	vector_push_back(&rt->materials, &m);
	sphere_init(&obj, (vec3) { 4, 1, 0 }, 1.0, rt->materials.length - 1);
	vector_push_back(&rt->world, &obj);

	vec3 from = { 13, 2, 3 };
	vec3 to = { 0, 0, 0 };
	vec3 up = { 0, 1, 0 };
	double focus = 10.0;
	double aperture = 0.1;
	camera_init(&rt->camera, 
		from, 
		to,
		up,
		20.0, 
		(double)w / (double)h,
		aperture,
		focus
	);

	return true;
}

bool raytrace_destroy(raytrace* rt) {
	if (!rt || !rt->pixels) return false;
	free(rt->pixels);
	rt->width = -1;
	rt->height = -1;
	vector_destroy(&rt->world);
	vector_destroy(&rt->materials);
	return true;
}

bool raytrace_resize(raytrace* rt, int w, int h) {
	if (!rt || w < 1 || h < 1) return false;
	void* pixels = realloc(rt->pixels, w * h * sizeof(COLORREF));
	if (pixels) {
		rt->width = w;
		rt->height = h;
		rt->pixels = pixels;
	}
	return true;
}

static double clamp(double v, double min, double max) {
	if (v < min) return min;
	if (v > max) return max;
	return v;
}

bool raytrace_set_pixel(raytrace* rt, int x, int y, vec3 color) {
	if (!rt || !rt->pixels || x < 0 || x >= rt->width || y < 0 || y >= rt->height)
		return false;
	double scale = 1.0 / rt->samples;
	color.x = sqrt(color.x * scale);
	color.y = sqrt(color.y * scale);
	color.z = sqrt(color.z * scale);

	if (color.x != color.x) color.x = 0;
	if (color.y != color.y) color.y = 0;
	if (color.z != color.z) color.z = 0;

	rt->pixels[y * rt->width + x] = RGB(
		(int)(256 * clamp(color.z, 0.0, 0.999)),
		(int)(256 * clamp(color.y, 0.0, 0.999)),
		(int)(256 * clamp(color.x, 0.0, 0.999))
	);
	return true;
}

void raytrace_paint(raytrace* rt, HDC hdc) {
	HBITMAP map = CreateBitmap(rt->width, rt->height, 1, 32, rt->pixels);
	HDC src = CreateCompatibleDC(hdc);
	SelectObject(src, map);
	BitBlt(hdc, 0, 0, rt->width, rt->height, src, 0, 0, SRCCOPY);
	DeleteObject(map);
	DeleteDC(src);
}

static bool cast_ray(vector* objects, ray* r, double t_min, double t_max, hit_record* rec) {
	hit_record temp;
	bool has_hit = false;
	double closest = t_max;
	for (size_t i = 0; i < objects->length; i++) {
		hittable* object = vector_at(objects, i);
		if (object->hit(object, r, t_min, closest, &temp)) {
			has_hit = true;
			closest = temp.t;
			*rec = temp;
		}
	}
	return has_hit;
}

static vec3 ray_color(ray r, vector* objects, vector* materials, int depth) {
	if (depth <= 0) return (vec3) { 0 };
	hit_record rec;
	if (cast_ray(objects, &r, 0.001, INFINITY, &rec)) {
		ray scattered;
		vec3 attenuation;
		material* mat = vector_at(materials, rec.material);
		if (mat->scatter(mat, &r, &rec, &attenuation, &scattered))
			return vec3_mul_vec3(ray_color(scattered, objects, materials, depth - 1), attenuation);
		return (vec3) { 0 };
	}
	vec3 unit = vec3_unit(r.direction);
	double t = 0.5 * (unit.y + 1.0);
	return vec3_add(
		vec3_mul((vec3) { .x = 1.0, .y = 1.0, .z = 1.0 }, 1.0 - t),
		vec3_mul((vec3) { .x = 0.5, .y = 0.7, .z = 1.0 }, t)
	);
}

static DWORD WINAPI raytrace_run_render(LPVOID* lpParam) {
	raytrace* rt = (raytrace*)lpParam;
	DWORD code = 0;
	while (rt->isRendering) {
		DWORD waitResult = WaitForSingleObject(rt->progressMutex, INFINITE);
		if (waitResult != WAIT_OBJECT_0)
			return FALSE;
		int work = rt->currentProgress++;
		ReleaseMutex(rt->progressMutex);
		if (work >= RENDER_ROWS * RENDER_COLS) {
			if (work >= RENDER_ROWS * RENDER_COLS + MAX_RENDER_THREADS - 1)
				rt->isRendering = false;
			return TRUE;
		}
		
		int row = work / RENDER_COLS;
		int col = work % RENDER_COLS;
		int width = rt->width / RENDER_COLS;
		int height = rt->height / RENDER_ROWS;

		for (int j = row * height; j < (row + 1) * height; j++) {
			for (int i = col * width; i < (col + 1) * width; i++) {
				vec3 color = { 0 };
				for (int s = 0; s < rt->samples; s++) {
					double u = (double)(i + random()) / (double)(rt->width - 1);
					double v = (double)(rt->height - 1 - j + random()) / (double)(rt->height - 1);
					vec3_ip_add(&color, ray_color(camera_ray(&rt->camera, u, v), &rt->world, &rt->materials, rt->depth));
				}
				raytrace_set_pixel(rt, i, j, color);
			}
		}
	}
	return TRUE;
}

bool raytrace_start_render(raytrace* rt) {
	if (!rt || !rt->width || !rt->height || !rt->pixels || rt->isRendering)
		return false;
	rt->isRendering = true;
	rt->currentProgress = 0;
	rt->progressMutex = CreateMutex(NULL, FALSE, NULL);
	if (!rt->progressMutex)
		return false;

	DWORD id;
	for (int i = 0; i < MAX_RENDER_THREADS; i++) {
		rt->threads[i] = CreateThread(
			NULL,
			0,
			raytrace_run_render,
			rt,
			0,
			&id
		);
		if (!rt->threads[i])
			return false;
	}

	return true;
}

bool raytrace_stop_render(raytrace* rt) {
	WaitForMultipleObjects(MAX_RENDER_THREADS, rt->threads, TRUE, INFINITE);
	for (int i = 0; i < MAX_RENDER_THREADS; i++)
		if (!CloseHandle(rt->threads[i])) return false;
		else rt->threads[i] = 0;
	if (!CloseHandle(rt->progressMutex))
		return false;
	return true;
}