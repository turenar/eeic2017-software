#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 50

#ifndef PORCELAIN_MODE
#define PORCELAIN_MODE 0
#endif

/*
 * 3次元出力に対応しました
 *
 */
const double G = 1.0;  // gravity constant

struct star {
	double m;   // mass
	double x;   // position_x
	double y;
	double z;
	double vx;  // velocity_x
	double vy;
	double vz;
};

struct star stars[] = {
#if 0
{2.0, 0, 0, 0, 0.0, 0, 0},
{0.5, -20.0, 0, 0, 0, 0.2, 0},
{0.5003, 20.0, 0, 0, 0, -0.2, 0},
{0.5002, 0, -20.0, 0, -0.2, 0, 0},
{0.5001, 0, 20.0, 0, 0.2, 0, 0},
{0.1001, 50, 0, 0, -1.1, 0.1, 0},
{0.1001, 30, 0, 0, 1, 0.1, 0}
#else
		{4.0, 0, 0, 0, 0, 0, 0},
		{0.5, 20.0, 0, 0, 0, 0.2, 0.2},
		{0.51, -20.0, 0, 0, 0, -0.2, -0.2},
		{0.51, 0, 20.0, 20.0, -0.2, 0, 0},
		{0.5, 0, -20.0, -20.0, 0.2, 0, 0}
#endif
};

const int nstars = sizeof(stars) / sizeof(struct star);

void plot_stars(FILE* fp, const double t) {
	int i;
	char space[WIDTH * 2 + 1][HEIGHT];

	memset(space, ' ', sizeof(space));
	for (int x = 0; x < WIDTH; ++x) {
		space[x][HEIGHT / 2] = '.';
		space[WIDTH + x + 1][HEIGHT / 2] = '.';
	}
	space[WIDTH - 1][HEIGHT / 2 - 1] = 'X';
	space[WIDTH * 2][HEIGHT / 2 - 1] = 'X';
	space[WIDTH / 2 + 1][0] = 'Y';
	space[WIDTH / 2 * 3 + 1][0] = 'Z';
	for (int y = 0; y < HEIGHT; ++y) {
		space[WIDTH / 2][y] = ':';
		space[WIDTH][y] = '|';
		space[3 * WIDTH / 2][y] = ':';
	}

	for (i = 0; i < nstars; i++) {
		if (stars[i].m <= 0) {
			continue;
		}
		const int x = (int) (WIDTH / 2 + stars[i].x);
		const int y = (int) (HEIGHT / 2 + stars[i].y);
		const int z = (int) (HEIGHT / 2 + stars[i].z);
		if (x < 0 || x >= WIDTH) {
			continue;
		}
		char c = (char) ('a' + i);
		if (stars[i].m >= 1.0) {
			c = (char) ('A' + i);
		}
		if (y >= 0 && y < HEIGHT) {
			space[x][y] = c;
		}
		if (z >= 0 && z < HEIGHT) {
			space[WIDTH + 1 + x][z] = c;
		}
	}

	int x, y;
	fprintf(fp, "----------\n");
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH * 2 + 1; x++) {
			char c = space[x][y];
#ifdef _DEBUG
			if ('a' <= c && c <= 'z') {
				fprintf(fp, "\033[3%dm%c\033[0m", (c - 'a') % 6 + 1, c);
				continue;
			} else if ('A' <= c && c <= 'Z') {
				fprintf(fp, "\033[1;3%dm%c\033[0m", (c - 'A') % 6 + 1, c);
				continue;
			}
#endif
			fputc(c, fp);
		}
		fputc('\n', fp);
	}
	fflush(fp);

#if !PORCELAIN_MODE
	printf("t = %5.1f", t);
	for (i = 0; i < nstars; i++) {
		printf(", stars[%d] = (%7.2f,%7.2f,%7.2f)", i, stars[i].x, stars[i].y, stars[i].z);
	}
#else
	printf("%f", t);
	for (i = 0; i < nstars; ++i) {
		if (stars[i].m <= 0) {
			printf("\t\t");
		} else {
			printf("\t%f\t%f", stars[i].x, stars[i].y);
		}
	}
#endif
	printf("\n");
}

void update_velocities(const double dt) {
	for (int i = 0; i < nstars; i++) {
		if (stars[i].m <= 0) {
			continue;
		}
		double ax = 0;
		double ay = 0;
		double az = 0;

		for (int j = 0; j < nstars; ++j) {
			if (i != j && stars[j].m > 0) {
				double dx = stars[j].x - stars[i].x;
				double dy = stars[j].y - stars[i].y;
				double dz = stars[j].z - stars[i].z;
				double r = sqrt(dx * dx + dy * dy + dz * dz);
				ax += G * stars[j].m * dx / fabs(r * r * r);
				ay += G * stars[j].m * dy / fabs(r * r * r);
				az += G * stars[j].m * dz / fabs(r * r * r);
			}
		}

		stars[i].vx += ax * dt;
		stars[i].vy += ay * dt;
		stars[i].vz += az * dt;
	}
}

void update_positions(const double dt) {
	int i;
	for (i = 0; i < nstars; i++) {
		stars[i].x += stars[i].vx * dt;
		stars[i].y += stars[i].vy * dt;
		stars[i].z += stars[i].vz * dt;
	}
}

void try_fusion() {
	for (int i = 0; i < nstars; i++) {
		if (stars[i].m <= 0) {
			continue;
		}
		for (int j = i + 1; j < nstars; j++) {
			if (stars[j].m <= 0) {
				continue;
			}
			const double dx = stars[i].x - stars[j].x;
			const double dy = stars[i].y - stars[j].y;
			const double dz = stars[i].z - stars[j].z;
			if (dx * dx + dy * dy + dz * dz < 0.5) {
				double new_m = stars[i].m + stars[j].m;
				double centroid_x = (stars[i].m * stars[i].x + stars[j].m * stars[j].x) / new_m;
				double centroid_y = (stars[i].m * stars[i].y + stars[j].m * stars[j].y) / new_m;
				double centroid_z = (stars[i].m * stars[i].z + stars[j].m * stars[j].z) / new_m;
				double momentum_x = stars[i].m * stars[i].vx + stars[j].m * stars[j].vx;
				double momentum_y = stars[i].m * stars[i].vy + stars[j].m * stars[j].vy;
				double momentum_z = stars[i].m * stars[i].vz + stars[j].m * stars[j].vz;
				double new_vx = momentum_x / new_m;
				double new_vy = momentum_y / new_m;
				double new_vz = momentum_z / new_m;
#ifdef _DEBUG
				fprintf(stderr, "fusion %d->%d\n", i, j);
				fprintf(stderr, "a: m=%f,x=%f,y=%f,z=%f,vx=%f,vy=%f,vz=%f\n",
				        stars[i].m, stars[i].x, stars[i].y, stars[i].z, stars[i].vx, stars[i].vy, stars[i].vz);
				fprintf(stderr, "b: m=%f,x=%f,y=%f,z=%f,vx=%f,vy=%f,vz=%f\n",
				        stars[j].m, stars[j].x, stars[j].y, stars[j].z, stars[j].vx, stars[j].vy, stars[j].vz);
#endif
				stars[i].m = new_m;
				stars[i].vx = new_vx;
				stars[i].vy = new_vy;
				stars[i].vz = new_vz;
				stars[i].x = centroid_x;
				stars[i].y = centroid_y;
				stars[i].z = centroid_z;

				stars[j].m = 0;
				stars[j].vx = 0;
				stars[j].vy = 0;
				stars[j].vz = 0;
				stars[j].x = 0;
				stars[j].y = 0;
				stars[j].z = 0;
				fprintf(stderr, "A: m=%f,x=%f,y=%f,z=%f,vx=%f,vy=%f,vz=%f\n",
				        stars[i].m, stars[i].x, stars[i].y, stars[i].z, stars[i].vx, stars[i].vy, stars[i].vz);
				fprintf(stderr, "B: m=%f,x=%f,y=%f,z=%f,vx=%f,vy=%f,vz=%f\n",
				        stars[j].m, stars[j].x, stars[j].y, stars[j].z, stars[j].vx, stars[j].vy, stars[j].vz);
			}
		}
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
		dt = 0.1;
	} else {
		char* endptr;
		dt = strtod(argv[1], &endptr);
		if (*endptr != '\0') {
			fprintf(stderr, "invalid double?: %s\n", argv[1]);
			return 1;
		}
	}
	const double stop_time = 10000;

	int i;
	double t;
	for (i = 0, t = 0; t <= stop_time; i++, t += dt) {
		update_velocities(dt);
		update_positions(dt);
		try_fusion();
		if (i % 100 == 0) {
			plot_stars(fp, t);
#if !PORCELAIN_MODE
			usleep(200 * 1000);
#endif
		}
	}

	fclose(fp);

	return 0;
}
