#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "catalog.h"
#include "record_data.h"

#ifndef MEMPCPY_FROM_MEMCPY
#define MEMPCPY_FROM_MEMCPY
#define mempcpy(a, b, c) \
(memcpy((a), (b), (c)) != NULL? (a)+(c) : (a))
#endif

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

int extract_int(const char *src, char *dest, int &curp, int st) {
	int num = 0;
	int i = st;
	while (isblank(src[i]) || src[i] == ':') i++;
	while (isdigit(src[i])) {
		num = num * 10 + (src[i++] - '0');
	}
	dest = mempcpy(dest, &num, sizeof(int));
	curp += sizeof(int);
	if (src[i] == ',') i++;
	return i - st;
}

int extract_bool(const char* src, char *dest, int &curp, int st) {
	int i = st;
	char ch;
	char value[10];
	while (isblank(src[i]) || src[i] == ':') i++;	// skip blank and ':'
	if (src[i] == '\'' || src[i] == '\"') {
		ch = src[i++];
	}
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
	curp += sizeof(char);
	if (src[i] == ch) i++;
	if (src[i] == ',') i++;
	//if (src[i] == '}') i++;
	return i - st;
}

int extract_string(const char *src, char *dest, int &curp, int st) {
	int i = st;
	char ch;
	while (isblank(src[i]) || src[i] == ':') i++;
	if (src[i] == '\"' || src[i] == '\'') {
		ch = src[i++];
	}
	while (src[i] != ch && src[i] != '\0') {
		*dest++ = src[i++];
		curp++;
	}
	if (src[i] == ch) i++;
	if (src[i] == ',') i++;
	//if (src[i] == '}') i++;
	return i - st;
}

int extract_nested_arr(const char *src, char *dest, int &curp, int st) {
	int i = st;
	while (isblank(src[i]) || src[i] == ':') i++;
	if (src[i] == '[') i++;
	while (src[i] != ']') {
		*dest++ = src[i++];
		curp++;
	}
	if (src[i] == ']') i++;
	if (src[i] == ',') i++;
	//if (src[i] == '}') i++;
	return i - st;
}

// 这里实际上可以使用insert.h里面的changeToRecord进行递归使用
int extract_nested_obj(const char *src, char *dest, int &curp, int st) {
	Record tt;
	tt.attrNum = tt.len = 0;
	char key[100];
	KEY_TYPE type;
	Catalog *instance = Catalog::getCatalogInstance();
	int i = st;
	while (isblank(src[i]) || src[i] == ':') i++;
	if (src[i] == '{') i++;
	while (src[i] != '}') {
		if (src[i] == '\"') {
			i += extract_key(src, key, i);
			while (isblank(src[i]) || src[i] == ':') i++;	// skip blank and ':'
			if (src[i] == '\"') {			// get type
				type = NESTEDSTR;
			} else if (isdigit(src[i])) {
				type = NESTEDINT;
			} else {
				type = UNKNOWN;
			}
			int key_id = instance->create(key, type);
			tt.attrNum++;
			tt.aids.push_back(key_id);
			tt.offs.push_back(tt.len);
			if (type == NESTEDINT) {
				i += extract_int(src, tt.data+tt.len, tt.len, i);
			} else {
				i += extract_string(src, tt.data+tt.len, tt.len, i);
			}
		} else {
			i++;
		}
	}
	dest = mempcpy(dest, &(tt.attrNum), sizeof(tt.attrNum));
	curp += sizeof(tt.attrNum);
	int k;
	for (k = 0; k < tt.attrNum; k++) {
		dest = mempcpy(dest, &(tt.aids[k]), sizeof(int));
		curp += sizeof(int);
	}
	for (k = 0; k < tt.attrNum; k++) {
		dest = mempcpy(dest, &(tt.offs[k]), sizeof(int));
		curp += sizeof(int);
	}
	dest = mempcpy(dest, &(tt.len), sizeof(tt.len));
	curp += sizeof(tt.len);
	dest = mempcpy(dest, tt.data, tt.len);
	curp += tt.len;
	if (src[i] == '}') i++;
	if (src[i] == ',') i++;
	if (src[i] == '}') i++;
	return i - st;
}

#endif
