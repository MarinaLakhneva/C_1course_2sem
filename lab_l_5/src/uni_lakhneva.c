# pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>


typedef enum {
	CIRCLE,
	RECTANGLE,
	POLYGON
}form_t;

typedef enum {
	RED,
	GREEN,
	BLUE
}color_t;

typedef struct {
	float radius;
}circle_t;

typedef struct {
	float width;
	float height;
}rectangle_t;

typedef struct {
	int n;
	float side_size;
}polygon_t;

typedef struct {
	form_t form;
	color_t color;
	union {
		circle_t circle;
		rectangle_t rectangle;
		polygon_t polygon;
	}as;
}figure_t;

int form_search(figure_t* figures, int len, form_t form) {
	int form_counter = 0;
	for (int i = 0; i < len; i++) {
		if (figures[i].form == form)
			form_counter++;
	}
	return form_counter;
}

int color_search(figure_t* figures, int len, color_t color) {
	int color_counter = 0;
	for (int i = 0; i < len; i++) {
		if (figures[i].color == color)
			color_counter++;
	}
	return color_counter;
}

float circle_square(float radius) {
	return M_PI * radius * radius;
}

float rectangle_square(float a, float b) {
	return a * b;
}

float polygon_square(int n, float size_side) {
	return n * size_side * size_side / (4 * tan(M_PI / n));
}

float sum_of_squares(figure_t* figures, int len) {
	float s = 0;
	for (int i = 0; i < len; i++) {
		if (figures[i].form == CIRCLE) {
			s += circle_square(figures[i].as.circle.radius);
		}
		if (figures[i].form == RECTANGLE) {
			s += rectangle_square(figures[i].as.rectangle.width, figures[i].as.rectangle.height);
		}
		if (figures[i].form == POLYGON) {
			s += polygon_square(figures[i].as.polygon.n, figures[i].as.polygon.side_size);
		}
	}
	return s;
}

int main(void) {
	FILE* bin = fopen("C:\\Users\\Marina\\source\\repos\\UNI_Lakhneva\Debug.bin", "rb");
	fseek(bin, 0, SEEK_END);

	figure_t* fig1;
	int k = ftell(bin);
	int m = sizeof(figure_t);
	int len = ftell(bin) / sizeof(figure_t);
	printf("%i\n", len);

	fseek(bin, 0, SEEK_SET);
	figure_t* figures = (figure_t*)(malloc(len * sizeof(figure_t)));
	if (figures == NULL) {
		printf("the memory could not be allocated");
		return 0;
	}

	fread(figures, sizeof(figure_t), len, bin);
	int red, green, blue;
	int circle, rectangle, polygon;
	float square;

	red = color_search(figures, len, RED);
	green = color_search(figures, len, GREEN);
	blue = color_search(figures, len, BLUE);

	circle = form_search(figures, len, CIRCLE);
	rectangle = form_search(figures, len, RECTANGLE);
	polygon = form_search(figures, len, POLYGON);

	printf("red = %i, green = %i, blue = %i\n", red, green, blue);
	printf("circles = %i, rectangles = %i, polygons = %i\n", circle, rectangle, polygon);
	printf("%.2f", sum_of_squares(figures, len));

	fclose(bin);
	free(figures);
	return 0;
}