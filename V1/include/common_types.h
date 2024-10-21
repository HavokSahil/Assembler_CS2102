#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H

#include <stdint.h>
#include <stddef.h>

#define TRUE 1
#define FALSE 0

typedef unsigned long ASize;
typedef unsigned int AAddr;
typedef unsigned char ABool;
typedef unsigned char AByte;
typedef signed long AInt;
typedef signed long AErr;
typedef unsigned char AType;
typedef char* AString;
typedef uint32_t AInt32;

/* Color Data Type for Red-Black Tree Node*/
typedef enum {
	RED,
	BLACK
} Color;

#endif
