#pragma once

#include <stdbool.h>
#include "optimizer.h"

bool read_csv(const char* filename, char col_delimiter, bool has_header, optimizer_info*);
