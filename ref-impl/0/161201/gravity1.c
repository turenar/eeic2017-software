#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 170
#define HEIGHT 50

const double G = 1.0;  // gravity constant

struct star {
	double m;   // mass
	double x;   // position_x
	double y;
	double vx;  // velocity_x
	double vy;
};

struct star stars[] = {
		{1.0, 10.0,  10.0,  0.0, 0},
		{0.2, -10.0, -10.0, 0.2, 0},
		{0.1, 0.0,   5.0,   0,   -0.2}
};

const int nstars = sizeof(stars) / sizeof(struct star);

void plot_stars(FILE* fp, const double t) {
	int i;
	char space[WIDTH][HEIGHT];

	memset(space, ' ', sizeof(space));
	for (i = 0; i < nstars; i++) {
		const int x = (int) (WIDTH / 2 + stars[i].x);
		const int y = (int) (HEIGHT / 2 + stars[i].y);
		if (x < 0 || x >= WIDTH) {
			continue;
		}
		if (y < 0 || y >= HEIGHT) {
			continue;
		}
		char c = 'o';
		if (stars[i].m >= 1.0) {
			c = 'O';
		}
		space[x][y] = c;
	}

	int x, y;
	fprintf(fp, "----------\n");
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			fputc(space[x][y], fp);
		}
		fputc('\n', fp);
	}
	fflush(fp);

	printf("t = %5.1f", t);
	for (i = 0; i < nstars; i++) {
		printf(", stars[%d] = (%7.2f,%7.2f)", i, stars[i].x, stars[i].y);
	}
	printf("\n");
}

void update_velocities(const double dt) {
	for (int i = 0; i < nstars; i++) {
		double ax = 0;
		double ay = 0;

		for (int j = 0; j < nstars; ++j) {
			if (i != j) {
				double dx = stars[j].x - stars[i].x;
				double dy = stars[j].y - stars[i].y;
				double r = sqrt(dx * dx + dy * dy);
				ax += G * stars[j].m * dx / fabs(r * r * r);
				ay += G * stars[j].m * dy / fabs(r * r * r);
			}

		}

		stars[i].vx += ax * dt;
		stars[i].vy += ay * dt;
	}
}

void update_positions(const double dt) {
	int i;
	for (i = 0; i < nstars; i++) {
		stars[i].x += stars[i].vx * dt;
		stars[i].y += stars[i].vy * dt;
	}
}

int main(int argc, char** argv) {
	const char* filename = "space.txt";
	FILE* fp;
	if ((fp = fopen(filename, "a")) == NULL) {
		fprintf(stderr, "error: cannot open %s.\n", filename);
		return 1;
	}

	double dt;
	if (argc < 2) {
		dt = 1.;
	} else {
		char* endptr;
		dt = strtod(argv[1], &endptr);
		if (*endptr != '\0') {
			fprintf(stderr, "invalid double?: %s\n", argv[1]);
			return 1;
		}
	}
	const double stop_time = 1000;

	int i;
	double t;
	for (i = 0, t = 0; t <= stop_time; i++, t += dt) {
		update_velocities(dt);
		update_positions(dt);
		if (i % 10 == 0) {
			plot_stars(fp, t);
			usleep(200 * 1000);
		}
	}

	fclose(fp);

	return 0;
}
