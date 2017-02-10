#include "drawer.h"
#include <stdlib.h>
#include <assert.h>
#include "painter.h"
#include "util.h"


static inline void swap_int(int*, int*);

static void swap_int(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}


void draw_line(const int x0, const int y0, const int x1, const int y1) {
	int i;
	const int n = max(abs(x1 - x0), abs(y1 - y0));
	for (i = 0; i <= n; i++) {
		const int x = x0 + i * (x1 - x0) / n;
		const int y = y0 + i * (y1 - y0) / n;
		set_canvas_color_safe(x, y, fg_color);
	}
}

void draw_circle(const int x0, const int y0, const int r) {
	assert(r >= 0);

	int r2 = r * r;
	int dy = r;
	for (int dx = 0; dx <= r && dy >= 0;) {
		if (dx * dx + dy * dy >= r2) {
			set_canvas_color_safe(x0 + dx, y0 + dy, fg_color);
			set_canvas_color_safe(x0 + dx, y0 - dy, fg_color);
			set_canvas_color_safe(x0 - dx, y0 + dy, fg_color);
			set_canvas_color_safe(x0 - dx, y0 - dy, fg_color);
		}
		int dx_1 = dx + 1;
		int dy_1 = dy - 1;
		if (dx * dx + dy_1 * dy_1 >= r2) {
			--dy;
		} else if (dx_1 * dx_1 + dy_1 * dy_1 >= r2) {
			++dx;
			--dy;
		} else {
			++dx;
		}
	}
	if (fill) {
		for (int dx = 0; dx <= r; ++dx) {
			for (dy = 0; dy <= r; ++dy) {
				if (dx * dx + dy * dy < r2) {
					set_canvas_color_safe(x0 + dx, y0 + dy, bg_color);
					set_canvas_color_safe(x0 + dx, y0 - dy, bg_color);
					set_canvas_color_safe(x0 - dx, y0 + dy, bg_color);
					set_canvas_color_safe(x0 - dx, y0 - dy, bg_color);
				}
			}
		}
	}
}

void draw_rect(int x0, int y0, int x1, int y1) {
	if (x0 > x1) {
		swap_int(&x0, &x1);
	}
	if (y0 > y1) {
		swap_int(&y0, &y1);
	}
	int x_max = min(x1, width - 1);
	for (int x = x0; x <= x_max; ++x) {
		set_canvas_color_safe(x, y0, fg_color);
		set_canvas_color_safe(x, y1, fg_color);
	}
	int y_max = min(y1, height - 1);
	for (int y = y0; y <= y_max; ++y) {
		set_canvas_color_safe(x0, y, fg_color);
		set_canvas_color_safe(x1, y, fg_color);
	}
	if (fill) {
		for (int x = x0 + 1; x < x_max; ++x) {
			for (int y = y0 + 1; y < y_max; ++y) {
				canvas[y][x] = bg_color;
			}
		}
	}
}
