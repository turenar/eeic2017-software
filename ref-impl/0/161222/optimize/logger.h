#pragma once

#ifndef __attribute_format__
#ifdef __GNUC__
#define __attribute_format__(type, string_index, first_to_check) __attribute__((format(type, string_index, first_to_check)))
#else
#define __attribute_format__(a,b,c) /**/
#endif
#endif

void log_debug(const char* file, int line, const char* fmt, ...) __attribute_format__(printf, 3, 4);
void log_info(const char* file, int line, const char* fmt, ...) __attribute_format__(printf, 3, 4);
void log_warn(const char* file, int line, const char* fmt, ...) __attribute_format__(printf, 3, 4);
void log_error(const char* file, int line, const char* fmt, ...) __attribute_format__(printf, 3, 4);

/** @param level error|warn|info|debug */
#define LOG(level, ...) (log_ ## level (__FILE__, __LINE__, __VA_ARGS__))
