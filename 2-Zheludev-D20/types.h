#pragma once

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef char boolean;
typedef unsigned int uint;

typedef struct {
	uint size;
	boolean isUsed;
	boolean isTouched;
} plentyElem_t;

typedef struct {
	uint size;
	int index;
} stackElem_t;

typedef struct {
	int cur;
	stackElem_t* arr;
} stack_t;
