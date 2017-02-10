#include "file_buffer.h"
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

#define BUFSIZE 65536

struct file_buffer {
	FileChunk* chunks;
	unsigned int allocated_chunks;
	unsigned int chunk_count;
	int iterator_pos;
};

typedef struct file_buffer FileBuffer;

static void extend_chunk_array(FileBuffer* buf);

FileBuffer* buf_new() {
	FileBuffer* buf = (FileBuffer*) malloc(sizeof(FileBuffer));
	buf->allocated_chunks = 16;
	buf->chunk_count = 0;
	buf->chunks = (FileChunk*) malloc(buf->allocated_chunks * sizeof(FileChunk));
	buf->iterator_pos = -1;
	return buf;
}

bool buf_fully_read(FileBuffer* buf, FILE* fp) {
	unsigned char tmp[BUFSIZE];
	FileChunk* chunk_p = buf->chunks;
	unsigned int count = 0;
	while (true) {
		size_t loaded_len = fread(tmp, 1, sizeof(tmp), fp);
		LOG(debug, "loaded len = %zu", loaded_len);
		if (loaded_len <= 0) {
			break;
		}
		if (count >= buf->allocated_chunks) {
			extend_chunk_array(buf);
			chunk_p = &buf->chunks[count];
		}
		chunk_p->buf = (unsigned char*) malloc(loaded_len);
		memcpy(chunk_p->buf, tmp, loaded_len);
		chunk_p->len = loaded_len;
		chunk_p++;
		count++;
	}
	buf->chunk_count = count;
	return true;
}

size_t buf_get_chunk_count(const FileBuffer* buf) {
	return buf->chunk_count;
}

void buf_rewind(FileBuffer* buf) {
	buf->iterator_pos = -1;
}

bool buf_next_chunk(FileBuffer* buf, FileChunk** chunk) {
	buf->iterator_pos++;
	if ((unsigned int) buf->iterator_pos >= buf->chunk_count) {
		return false;
	}
	*chunk = &buf->chunks[buf->iterator_pos];
	return true;
}

void buf_free(FileBuffer* buf) {
	FileChunk* chunk = buf->chunks;
	for (unsigned int i = 0; i < buf->chunk_count; ++i) {
		free((chunk++)->buf);
	}
	free(buf->chunks);
	free(buf);
}

static void extend_chunk_array(FileBuffer* buf) {
	assert(buf->iterator_pos == -1);
	unsigned int new_length = buf->allocated_chunks * 2;
	FileChunk* new_chunks = (FileChunk*) realloc(buf->chunks, sizeof(FileChunk) * new_length);
	assert(new_chunks);
	buf->chunks = new_chunks;
	buf->allocated_chunks = new_length;
}
