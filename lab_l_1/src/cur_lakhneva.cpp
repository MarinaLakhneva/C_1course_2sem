# include "labengine.h"
#include <stdio.h>
#include <math.h>

void Curve(int xa, int ya, int xb, int yb, int n) {
	if (n == 0) {
		LabDrawLine(xa, ya, xb, ya);
		LabDrawLine(xb, ya, xb, yb);
	}
	else {
		int xc = (xa + xb) / 2;
		int yc = (yb + ya) / 2;
		Curve(xa, ya, xc, yc, n - 1);
		Curve(xc, yc, xb, yb, n - 1);
	}
}

void CochCurve(int xa, int ya, int xb, int yb, int n) {
	if (n == 0) {
		LabDrawLine(xa, ya, xb, yb);
		LabDrawFlush();
	}
	else {
		int xc = xa + (xb - xa) / 3;
		int yc = yb + 2 * (ya - yb) / 3;
		int xd = xa + (xb - xa) * 2 / 3;
		int yd = yb + (ya - yb) / 3;
		int xm = (xa + xb) / 2;
		int ym = (yb + ya) / 2;
		int xp = xm - yd + yc;
		int yp = ym + xd - xc;
		CochCurve(xa, ya, xc, yc, n - 1);
		CochCurve(xc, yc, xp, yp, n - 1);
		CochCurve(xp, yp, xd, yd, n - 1);
		CochCurve(xd, yd, xb, yb, n - 1);
		/*LabDrawPoint(xa, ya);
		LabDrawPoint(xb, yb);
		LabDrawPoint(xc, yc);
		LabDrawPoint(xd, yd);
		LabDrawPoint(xm, ym);
		LabDrawPoint(xp, yp);*/
	}
}

void CochCurveTriangle(int xa, int ya, int xb, int yb, int n) {
	int ab = (int)pow((int)pow((xb - xa), 2) + (int)pow((ya - yb), 2), 0.5);
	int am = (int)(ab * 0.5);
	int bm = (int)sqrt((int)pow(ab, 2) - (int)pow(am, 2));
	int xt = xa + 2 * am;
	int yt = yb + bm;

	CochCurve(xb, yb, xa, ya, n);
	CochCurve(xa, ya, xt, yt, n);
	CochCurve(xt, yt, xb, yb, n);
}

void CochInteractive(int xa, int ya, int xb, int yb, int n) {
	int tap = LabInputKeyReady();
	while (1) {
		if (tap == LABKEY_ESC || n < 1) {
			break;
		}
		if (tap == LABKEY_UP) {
			n++;
			LabClear();
		}
		else if (tap == LABKEY_DOWN) {
			n--;
			LabClear();
		}
		CochCurveTriangle(xa, ya, xb, yb, n);
		tap = LabInputKey();
	}
}

int main(void) {
	if (LabInit()) {  // Инициализация
	
		int h = LabGetHeight();
		int w = LabGetWidth();
		int xa = 100;
		int ya = 300;
		int xb = 300;
		int yb = 100;
		CochInteractive(xa, ya, xb, yb, 2);

		LabInputKey();

		LabTerm();      // Завершение работы
	}
	return 0;
}
