#include "labengine.h"
#include<math.h>

#define N 100
#define R 2

typedef struct {
	double x;
	double y;
}point_t;

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

labbool_t IsInsideDisk(point_t p) { 
	if (p.x * p.x + p.y * p.y <= 1) {
		return LAB_TRUE;
	}
	return LAB_FALSE;
}

void DrawDisk(rect_t const* math, rect_t const* screen) {
	point_t p, p1;
	for (int i = 0; i <= screen->b.x; i++) {
		for (int j = 0; j <= screen->b.y; j++) {
			p.x = i;
			p.y = j;

			p1 = Transform(p, screen, math);
			if (IsInsideDisk(p1)) {
				LabDrawPoint(p.x, p.y);
			}
		}
	}
}

labbool_t IsInsideJulia(point_t p, point_t c) {
	for (int i = 0; i < N; i++) {
		double x = p.x * p.x - p.y * p.y;
		double y = 2 * p.x * p.y;

		p.x = x + c.x;
		p.y = y + c.y;
		if (p.x * p.x + p.y * p.y > 4) {
			return LAB_FALSE;
		}
	}
	return LAB_TRUE;
}

void DrawJulia(rect_t const* math, rect_t const* screen, point_t c) {
	point_t p, p1;
	for (int i = 0; i <= screen->b.x; i++) {
		for (int j = 0; j <= screen->b.y; j++) {
			p.x = i;
			p.y = j;
			p1 = Transform(p, screen, math);
			if (IsInsideJulia(p1, c)) {
				LabDrawPoint(p.x, p.y);
			}
		}
	}
}

int main(void) {
	if (LabInit()) {
		int h = LabGetHeight();
		int w = LabGetWidth();

		rect_t screen;
		screen.a.x = 0;
		screen.a.y = 0;
		screen.b.x = w / 2;
		screen.b.y = h;

		rect_t math;
		math.a.x = -2;
		math.a.y = 3;
		math.b.x = 2;
		math.b.y = -3;

		DrawAxes(&math, &screen);
		DrawDisk(&math, &screen);

		screen.a.x = w / 2;
		screen.a.y = 0;
		screen.b.x = w;
		screen.b.y = h;

		DrawAxes(&math, &screen);

		double a = 0;
		double r = 0.32;
		while (!(LabInputKeyReady())) {
			point_t c;
			c.x = r * sin(a);
			c.y = r - r * cos(a);
			DrawJulia(&math, &screen, c);
			LabDrawFlush();
			a += 0.5;
			LabClear();
			DrawAxes(&math, &screen);
		}
		LabInputKey();
		LabTerm();
	}
	return 0;
}