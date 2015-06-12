#ifndef RECORD_H
#define RECORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "vector.h"
#include "catalog.h"

#ifndef MEMPCPY_FROM_MEMCPY
#define MEMPCPY_FROM_MEMCPY
#define mempcpy(a, b, c) \
(memcpy((a), (b), (c)) != NULL? (a)+(c) : (a))
#endif

#define RECORD_DATA_MAXSIZE 1024

typedef struct _Record {
	int attrNum;
	Vector<int> aids;
	Vector<int> offs;
	int len;
	char data[RECORD_DATA_MAXSIZE];	// a record data buffer
} Record;

// 读取一个key name
// 返回读取的个数
int extract_key(const char *src, char *key, int st) {
	int i = st;
	while (isblank(src[i])) i++;
	if (src[i] == '\"') i++;
	while (src[i] != '\"' && src[i] != '\0') {
		*key++ = src[i++];
	}
	*key = '\0';
	if (src[i] == '\"') i++;
	return i - st;
}

// get the type
int extract_type(const char *src, KEY_TYPE &type, int st) {
	int i = st;
	while (isblank(src[i]) || src[i] == ':') i++;	// skip blank and ':'
	if (src[i] == '\"') {
		type = STRING;
	} else if (src[i] == '{') {
		type = NESTEDOBJ;
	} else if (src[i] == '[') {
		type = NESTEDARR;
	} else if (isdigit(src[i])) {
		type = INT;
	} else {
		type = BOOL;
	}
	return i - st;
}

int extract_int(const char *src, char *dest, int st) {
	int num = 0;
	int i = st;
	while (!isdigit(src[i]) && src[i] != '\0') i++;
	while (isdigit(src[i])) {
		num = num * 10 + (src[i++] - '0');
	}
	dest = mempcpy(dest, &num, sizeof(int));
	*dest = '\0';
	if (src[i] == ',') i++;
	return i - st;
}

int extract_bool(const char* src, char *dest, int st) {
	int i = st;
	char value[10];
	while (isblank(src[i]) || src[i] == ':') i++;	// skip blank and ':'
	int j = 0;
	while (isalpha(src[i])) {
		value[j++] = src[i++];
	}
	value[j] = '\0';
	if (strncmp("true", value, sizeof("true")) == 0) {
		char boolean = 'T';
		dest = mempcpy(dest, &boolean, sizeof(char));
	} else {
		char boolean = 'F';
		dest = mempcpy(dest, &boolean, sizeof(char));
	}
	*dest = '\0';
	if (src[i] == ',') i++;
	if (src[i] == '}') i++;
	return i - st;
}

int extract_string(const char *src, char *dest, int st) {
	int i = st;
	while (isblank(src[i]) || src[i] == ':') i++;
	if (src[i] == '\"') i++;
	while (src[i] != '\"' && src[i] != '\0') {
		*dest++ = src[i++];
	}
	*dest = '\0';
	if (src[i] == '\"') i++;
	if (src[i] == ',') i++;
	if (src[i] == '}') i++;
	return i - st;
}

int extract_nested_arr(const char *src, char *dest, int st) {
	int i = st;
	while (isblank(src[i]) || src[i] == ':') i++;
	if (src[i] != '[') i++;
	while (src[i] != ']') {
		*dest++ = src[i++];
	}
	*dest = '\0';
	if (src[i] == ']') i++;
	if (src[i] == ',') i++;
	if (src[i] == '}') i++;
	return i - st;
}

int extract_nested_obj(const char *src, char *dest, int st) {
	int i = st;
	while (isblank(src[i]) || src[i] == ':') i++;
	if (src[i] != '{') i++;
	while (src[i] != '}') {
		*dest++ = src[i++];
	}
	*dest = '\0';
	if (src[i] == '}') i++;
	if (src[i] == ',') i++;
	if (src[i] == '}') i++;
	return i - st;
}

#endif