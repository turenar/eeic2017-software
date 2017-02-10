#pragma once

#ifdef __GNUC__
// for __attribute_malloc__
#include <sys/cdefs.h>
#endif
#ifndef __attribute_malloc__
#define __attribute_malloc__ /**/
#endif

#ifndef __attribute_format__
#ifdef __GNUC__
#define __attribute_format__(type, string_index, first_to_check) __attribute__((format(type, string_index, first_to_check)))
#else
#define __attribute_format__(a,b,c) /**/
#endif
#endif
