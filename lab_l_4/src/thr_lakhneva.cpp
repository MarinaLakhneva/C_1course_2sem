#include "labengine.h"
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define A (M_PI / 180.0)
#include <Windows.h>

typedef struct {
	double x;
	double y;
}point_t;

typedef struct {
	double x;
	double y;
}vec_t;

typedef struct {
	point_t a;
	point_t b;
}rect_t;

point_t Transform(point_t p, rect_t const* from, rect_t const* to) {
	point_t m;
	m.x = (p.x - from->a.x) * (to->b.x - to->a.x) / (from->b.x - from->a.x) + to->a.x;
	m.y = (p.y - from->a.y) * (to->b.y - to->a.y) / (from->b.y - from->a.y) + to->a.y;
	return m;
}

void DrawAxes(rect_t const* math, rect_t const* screen) {
	point_t p;
	p.x = 0;
	p.y = 0;

	point_t c = Transform(p, math, screen);
	LabDrawLine(screen->a.x, c.y, screen->b.x, c.y);
	LabDrawLine(c.x, screen->a.y, c.x, screen->b.y);
}

void DrawAnalyticalPath(rect_t const* math, rect_t const* screen,
	vec_t r0, vec_t v0, vec_t a0, double dt) {
	point_t r1 = { 0,0 }, r2;

	double t0 = 0;
	double tmax = 2 * v0.y / a0.y;

	tmax = tmax > 0 ? tmax : -tmax;

	r1 = Transform(r1, math, screen);
	for (t0; t0 <= tmax; t0 += dt) {
		r2.x = r0.x + v0.x * t0 + 0.5 * a0.x * t0 * t0;
		r2.y = r0.y + v0.y * t0 + 0.5 * a0.y * t0 * t0;
		r2 = Transform(r2, math, screen);

		LabDrawLine(r1.x, r1.y, r2.x, r2.y);
		r1.x = r2.x;
		r1.y = r2.y;
		LabDrawFlush();
	}
}

void DrawEulerPath(rect_t const* math, rect_t const* screen, vec_t r0, vec_t v0, vec_t a0, double dt) {
	point_t r1 = { 0,0 }, r2;
	vec_t v = { v0.x,v0.y };

	r2.x = v0.x * dt;
	r2.y = v0.y * dt;

	double dvx = a0.x * dt;
	double dvy = a0.y * dt;

	while (r2.y >= 0) {
		r1 = Transform(r1, math, screen);
		r2 = Transform(r2, math, screen);

		LabDrawLine(r1.x, r1.y, r2.x, r2.y);
		r2 = Transform(r2, screen, math);

		r1.x = r2.x;
		r1.y = r2.y;

		v.x += dvx;
		v.y += dvy;

		r2.x = r1.x + v.x * dt;
		r2.y = r1.y + v.y * dt;
		LabDrawFlush();
	}
}

void SimulateEulerPath(rect_t const* math, rect_t const* screen,
	vec_t r0, vec_t v0, vec_t a0) {
	vec_t v = v0;

	double dt = 0;

	LARGE_INTEGER start, end, frequency;
	QueryPerformanceFrequency(&frequency);
	point_t r1 = { 0,0 }, r2;

	r2.x = v.x * dt;
	r2.y = v.y * dt;
	while (r2.y >= 0) {
		double dvx = a0.x * dt;
		double dvy = a0.y * dt;

		QueryPerformanceCounter(&start);
		r1 = Transform(r1, math, screen);
		r2 = Transform(r2, math, screen);

		LabDrawLine(r1.x, r1.y, r2.x, r2.y);
		r2 = Transform(r2, screen, math);
		r1.x = r2.x;
		r1.y = r2.y;

		v.x += dvx;
		v.y += dvy;

		r2.x += v.x * dt;
		r2.y += v.y * dt;
		LabDrawFlush();
		QueryPerformanceCounter(&end);
		dt = (end.QuadPart - start.QuadPart) / (double)frequency.QuadPart;
	}
}

int main(void) {
	if (LabInit()) {
		int h = LabGetHeight();
		int w = LabGetWidth();

		rect_t screen;
		screen.a.x = 0;
		screen.a.y = 0;
		screen.b.x = w;
		screen.b.y = h;

		rect_t math;
		math.a.x = -3;
		math.a.y = 28;
		math.b.x = 41;
		math.b.y = -5;

		LabSetColor(LABCOLOR_BROWN);
		DrawAxes(&math, &screen);

		vec_t a0 = { 0, -9.8 };
		vec_t v0 = { 20 * cos(60 * A), 20 * sin(60 * A) };
		vec_t r0 = { 0,0 };

		LabSetColor(LABCOLOR_DARK_GREEN);
		DrawAnalyticalPath(&math, &screen, r0, v0, a0, 1);
		DrawAnalyticalPath(&math, &screen, r0, v0, a0, 0.5);
		LabSetColor(LABCOLOR_RED);
		DrawEulerPath(&math, &screen, r0, v0, a0, 1);
		DrawEulerPath(&math, &screen, r0, v0, a0, 0.5);
		LabSetColor(LABCOLOR_YELLOW);
		SimulateEulerPath(&math, &screen, r0, v0, a0);

		LabInputKey();
		LabTerm();   
	}
	return 0;
}
