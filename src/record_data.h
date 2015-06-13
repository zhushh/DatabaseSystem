#ifndef RECORD_DATA_H
#define RECORD_DATA_H

#include "vector.h"

#define RECORD_DATA_MAXSIZE 4096

typedef struct _Record {
	int attrNum;
	Vector<int> aids;
	Vector<int> offs;
	int len;
	char data[RECORD_DATA_MAXSIZE];	// a record data buffer
} Record;

#endif