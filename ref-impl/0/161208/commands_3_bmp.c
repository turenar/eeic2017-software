#include "commands.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "util.h"

#define BITMAP_FILE_HEADER_REAL_SIZE 14
struct bitmap_info_header {
	uint32_t header_size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t compression;
	uint32_t image_data_size;
	int32_t x_pix_per_meter;
	int32_t y_pix_per_meter;
	uint32_t used_palette_size;
	uint32_t important_color_count;
};

struct bitmap_file_header {
	char file_type[2];
	uint32_t file_size;
	uint32_t reserved;
	uint32_t offset_bytes;
	struct bitmap_info_header info;
};

static void write_bitmap_header(struct bitmap_file_header* header, FILE* fp);

static void write_bitmap_header(struct bitmap_file_header* header, FILE* fp) {
	// little-endian
	fwrite(header, 2, 1, fp);
	// offsetof(,file_size) should be 4 but bmp must be packed
	void* remain_header = ((char*) header) + offsetof(struct bitmap_file_header, file_size);
	fwrite(remain_header, 4, 3, fp);
	fwrite(&header->info, header->info.header_size, 1, fp);
}

CommandReturn cmd_bmp(char* filename) {
	if (filename[0] == '\0') {
		fprintf(stderr, "usage: bmp <filename>\n");
		return COMMAND_ERROR;
	}

	struct bitmap_file_header header;
	memset(&header, 0, sizeof(header));
	header.file_type[0] = 'B';
	header.file_type[1] = 'M';
	header.info.header_size = sizeof(header.info);
	header.offset_bytes = BITMAP_FILE_HEADER_REAL_SIZE + header.info.header_size;
	header.info.width = width;
	header.info.height = height;
	header.info.planes = 1;
	header.info.bit_count = 32; // 行間paddingがめんどくさい
	header.info.compression = 0;
	header.info.image_data_size = (uint32_t) (4 * width * height);
	header.info.x_pix_per_meter = (int32_t) (12 * INCH_PER_METER);
	header.info.y_pix_per_meter = (int32_t) (12 * INCH_PER_METER);
	header.info.used_palette_size = 0;
	header.info.important_color_count = 0;
	header.file_size = header.offset_bytes + header.info.image_data_size;

	FILE* fp = fopen(filename, "w");
	if (!fp) {
		perror(filename);
		return COMMAND_ERROR;
	}

	write_bitmap_header(&header, fp);
	for (int y = height - 1; y >= 0; --y) {
		uint32_t* col_p = canvas[y];
		for (int x = 0; x < width; ++x) {
			uint32_t c = *col_p++;
			uint8_t bgra[4];
			bgra[0] = get_b(c);
			bgra[1] = get_g(c);
			bgra[2] = get_r(c);
			bgra[3] = 0u;
			fwrite(bgra, 1, 4, fp);
		}
	}
	fclose(fp);
	printf("bmp file written: %s\n", filename);
	return COMMAND_OK;
}
