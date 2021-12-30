#pragma once

#include "framework.h"
#include "camera.h"
#include "vector.h"
#include "hittable.h"
#include "material.h"

#define MAX_RENDER_THREADS 4
#define RENDER_ROWS 12
#define RENDER_COLS 16

typedef struct {
	int width;
	int height;
	COLORREF* pixels;
	int samples;
	int depth;
	camera camera;
	vector world;
	vector materials;
	bool isRendering;
	HANDLE threads[MAX_RENDER_THREADS];
	HANDLE progressMutex;
	int currentProgress;
} raytrace;

bool raytrace_init(raytrace* rt, int w, int h);
bool raytrace_destroy(raytrace* rt);
bool raytrace_resize(raytrace* rt, int w, int h);
bool raytrace_set_pixel(raytrace* rt, int x, int y, vec3 color);
void raytrace_paint(raytrace* rt, HDC hdc);
bool raytrace_start_render(raytrace* rt);
bool raytrace_stop_render(raytrace* rt);