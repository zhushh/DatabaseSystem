#ifndef INSERT_H
#define INSERT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "buffer.h"
#include "record.h"
#include "catalog.h"

#define STR_MAXSIZE 4096
// get catalog instance handler
static Catalog *insert_ctlog = Catalog::getCatalogInstance();

// 将一条记录转换成Record格式
void changeToRecord(const char *src, Record &t) {
	char key[100];
	KEY_TYPE type;
	int i = 0;
	while (src[i] != '\0') {
		if (src[i] == '\"') {
			i += extract_key(src, key, i);
			i += extract_type(src, type, i);
			int key_id = insert_ctlog->create(key, type);	// create new catalog data
			t.attrNum++;
			t.aids.push_back(key_id);
			t.offs.push_back(t.len);
			// extract data
			if (type == INT) {
				i += extract_int(src, t.data+t.len, i);
			} else if (type == BOOL) {
				i += extract_bool(src, t.data+t.len, i);
			} else if (type == STRING) {
				i += extract_string(src, t.data+t.len, i);
			} else if (type == NESTEDARR) {
				i += extract_nested_arr(src, t.data+t.len, i);
			} else {		// type is NESTED_OBJ
				i += extract_nested_obj(src, t.data+t.len, i);
			}
			t.len = strlen(t.data);
		} else {	// remember increase the i if nothing to do
			i++;
		}
	}
}

// 把一条Record格式的数据写入到buffer中
// buffer会自动以8k大小写成一个页面
void writeRecordToBuffer(Record &t) {
	buffer_write(&(t.attrNum), sizeof(t.attrNum));
	int size = t.aids.size();
	for (int i = 0; i < size; i++) {
		buffer_write(&(t.aids[i]), sizeof(t.aids[i]));
	}
	size = t.offs.size();
	for (int i = 0; i < size; i++) {
		buffer_write(&(t.offs[i]), sizeof(t.offs[i]));
	}
	buffer_write(&(t.len), sizeof(t.len));
	buffer_write(t.data, t.len);
}

bool insert(const char *filename) {
	FILE *fp;
	if ((fp = fopen(filename, "r+")) == NULL) {
		fprintf(stderr, "%s can't open!\n", filename);
		return false;
	}
	char src[STR_MAXSIZE];
	while (fgets(src, sizeof(src), fp) != NULL) {
		Record t;
		t.attrNum = t.len = 0;
		changeToRecord(src, t);
		writeRecordToBuffer(t);
	}
    buffer_flush();     // write the data in last page
	return true;
}

#endif