#include <math.h>
#include "labengine.h"
#include <math.h>

#define MAX_DISTANCE    2.0
#define MAX_ITERATIONS  1023

typedef struct {
	double x, y;
} point_t;

typedef struct {
	point_t a, b;
} rect_t;

typedef struct {
	rect_t screen;
	rect_t math;
} viewport_t;

typedef struct {
  unsigned char r, g, b;
} color_t;

static color_t s_palette[] = {
  {0x00, 0x00, 0xFF},
  {0x00, 0xFF, 0xFF},
  {0xFF, 0xFF, 0x00},
  {0xFF, 0x00, 0x00},
  {0xFF, 0xFF, 0x00},
  {0x00, 0xFF, 0xFF},
  {0x00, 0x00, 0xFF},
};

const int MAX_COLORS = sizeof(s_palette) / sizeof(s_palette[0]);


point_t Transform(point_t p, rect_t const* from, rect_t const* to) {
	point_t res = {
	  to->a.x + (p.x - from->a.x) * (to->b.x - to->a.x) / (from->b.x - from->a.x),
	  to->a.y + (p.y - from->a.y) * (to->b.y - to->a.y) / (from->b.y - from->a.y)
	};
	return res;
}

void DrawAxes(viewport_t const* view) {
	point_t a = view->screen.a;
	point_t b = view->screen.b;
	point_t zero = { 0.0, 0.0 };

	zero = Transform(zero, &view->math, &view->screen);

	LabDrawRectangle((int)a.x, (int)a.y, (int)b.x + 1, (int)b.y + 1);

	if (zero.x >= a.x && zero.x <= b.x)
		LabDrawLine((int)zero.x, (int)a.y, (int)zero.x, (int)b.y + 1);
	if (zero.y >= a.y && zero.y <= b.y)
		LabDrawLine((int)a.x, (int)zero.y, (int)b.x + 1, (int)zero.y);
}

labbool_t IsInsideDisk(point_t p, point_t arg) {
	return p.x * p.x + p.y * p.y < 1.0;
}

int IsOutsideJulia(point_t p, point_t arg) {
	int i;
	double t;

	point_t c = arg;
	point_t z = p;


	for (i = 0; i < MAX_ITERATIONS; i++) {
		if (z.x * z.x + z.y * z.y > MAX_DISTANCE * MAX_DISTANCE) {
			return MAX_ITERATIONS - i;
		}
		t = 2 * z.x * z.y + c.y;
		z.x = z.x * z.x - z.y * z.y + c.x;
		z.y = t;
	}
	return -1;
}

int IsOutsideMan(point_t p, point_t arg) {
	int i;
	double t;

	point_t c = p;
	point_t z;

	z.x = 0;
	z.y = 0;

	for (i = 0; i < MAX_ITERATIONS; i++) {
		if (z.x * z.x + z.y * z.y > MAX_DISTANCE * MAX_DISTANCE)
			return MAX_ITERATIONS - i;
		
		t = 2 * z.x * z.y + c.y;
		z.x = z.x * z.x - z.y * z.y + c.x;
		z.y = t;
	}
	return -1;
}

void DrawSet(viewport_t const* view, labbool_t(*isInside)(point_t, point_t), point_t arg) {
	double x, y;
	point_t p;

	for (x = view->screen.a.x; x <= view->screen.b.x; x++) {
		for (y = view->screen.a.y; y <= view->screen.b.y; y++) {
			p.x = x;
			p.y = y;
			p = Transform(p, &view->screen, &view->math);
			if (isInside(p, arg) > 0) {
				double t = (double)isInside(p, arg)/(double)(MAX_ITERATIONS + 1);
				t = t * t * t;

				double K = t * (MAX_COLORS - 1);
				double alpha = K - (int)K;

				int r = (1 - alpha)*s_palette[(int)K].r+alpha* s_palette[(int)K+1].r;
				int g = (1 - alpha)*s_palette[(int)K].g + alpha * s_palette[(int)K+1].g;
				int b = (1 - alpha)*s_palette[(int)K].b + alpha * s_palette[(int)K+1].b;

				LabSetColorRGB(r,g,b);
				LabDrawPoint((int)x, (int)y);
			}
		}
	}
}

void Run(void)
{
	int width = LabGetWidth();
	int height = LabGetHeight();
	viewport_t view[2] = { {
	  {0.0, 0.0, (double)(width / 2 - 1), (double)(height - 1)},
	  {-2.0, 3.0, 2.0, -3.0},
	}, {
	  {(double)(width / 2), 0.0, (double)(width - 1), (double)(height - 1)},
	  {-0.7454356, 0.113019, -0.7454215, 0.1129986},
	} };

	point_t c = { -0.835, 0.2321 };
	double alpha = 0.0, r = 0.32;

	while (!LabInputKeyReady()) {
		DrawAxes(&view[0]);
		DrawSet(&view[0], IsOutsideJulia, c);

		DrawAxes(&view[1]);
		DrawSet(&view[1], IsOutsideMan, c);

		LabDrawFlush();
		LabDelay(20);
	}
	LabInputKey();
}

int main(void) {
	if (LabInit()) {
		Run();
		LabTerm();
	}
	return 0;
}
