#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "attribute.h"

typedef struct file_chunk {
	unsigned char* buf;
	size_t len;
} FileChunk;

struct file_buffer;
typedef struct file_buffer FileBuffer;

FileBuffer* buf_new() __attribute_malloc__;
bool buf_fully_read(FileBuffer*, FILE*);
size_t buf_get_chunk_count(const FileBuffer*);
void buf_rewind(FileBuffer*);
bool buf_next_chunk(FileBuffer*, FileChunk**);
void buf_free(FileBuffer*);
