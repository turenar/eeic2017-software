#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "logger.h"
#include "optimizer.h"
#include "reader.h"

static void show_usage(char** argv);

int main(const int argc, char** argv) {
	struct option longopts[] = {
			{"alpha",      required_argument, NULL, 'a'},
			{"has-header", no_argument,       NULL, 'H'},
			{"help",       no_argument,       NULL, 'h'},
			{NULL, 0,                         NULL, 0}
	};
	double alpha = 0.01;
	bool has_header = false;

	int opt;
	int longindex;
	while ((opt = getopt_long(argc, argv, "a:Hh", longopts, &longindex)) != -1) {
		switch (opt) {
			case 'a': {
				char* endptr;
				alpha = strtod(optarg, &endptr);
				if (*endptr != '\0') {
					LOG(error, "alpha is not numeric");
					show_usage(argv);
					return 1;
				}
				break;
			}
			case 'H':
				has_header = true;
				break;
			case 'h':
				show_usage(argv);
				return 0;
			default:
				show_usage(argv);
				return 1;
		}
	}

	if (optind >= argc) {
		LOG(error, "missing filename");
		show_usage(argv);
		return 1;
	}

	optimizer_info info = {0};
	const char* filename = argv[optind];
	if (!read_csv(filename, '\t', has_header, &info)) {
		return 1;
	}
	int result;
	if (!regress_linear(&info)) {
		result = 1;
		goto free_data;
	}
	if (optimize_gradient_descent(alpha, &info) < 0) {
		result = 1;
		goto free_data;
	}
	info.f_show_result(&info);
	result = 0;
free_data:
	free_optimizer(&info);
	return result;
}

static void show_usage(char** argv) {
	fprintf(stderr, "usage: %s [--alpha <alpha>] [--has-header] <filename>\n", argv[0]);
}
