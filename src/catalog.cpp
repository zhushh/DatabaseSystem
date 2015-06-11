/*************************************************************************
 > File Name: catalog.c
 > Author: zhushh
 > Mail: 
 > Created Time: Mon 08 Jun 2015 02:03:58 PM CST
 ************************************************************************/

#ifndef CATALOG_CPP
#define CATALOG_CPP

#include <stdio.h>
#include <string.h>
#include "catalog.h"

#ifndef CATALOG_DEFAULT_FILE
#define CATALOG_DEFAULT_FILE
const char* CATALOG_FILE = "catalog";
#endif

Catalog* Catalog::instance_ = NULL;
bool Catalog::isDirty = false;
int Catalog::key_id_count = 0;

Catalog* Catalog::getCatalogInstance(void) {
    if (NULL == instance_) {
        instance_ = new Catalog();
    }
    return instance_;
}
Catalog::~Catalog() {
    if (instance_ != NULL && isDirty) {		// 若catalog被修改过则重新写
        writeToFile(CATALOG_FILE);
        instance_ = NULL;
    }
}

// 若name & type 已经存在catalog当中，进行count++
// 反之，新建一个数据，记录name & type并生成新的key_id
// 返回值为key_id
int Catalog::create(const char *name, KEY_TYPE type) {
	isDirty = true;		// set dirty
	int size = v.size();
	for (int i = 0; i < size; i++) {
		if (v[i].key_type == type) {
			if (strncmp(v[i].key_name, name, sizeof(v[i].key_name)) == 0) {
				v[i].count++;
				return v[i].id;
			}
		}
	}
	Catalog_data dat;
	dat.id = key_id_count;
	strncpy(dat.key_name, name, sizeof(dat.key_name));
	dat.key_type = type;
	dat.count = 1;
	v.push_back(dat);
	return key_id_count++;
}

// 返回所有的数据
void Catalog::find(Vector<Catalog_data> &vv) const {
	vv = v;
}

// 返回key_name == name的所有数据
void Catalog::find(const char *name, Vector<Catalog_data> &t) const {
	int size = v.size();
	for (int i = 0; i < size; i++) {
		if (strncmp(name, v[i].key_name, sizeof(v[i].key_name)) == 0) {
			t.push_back(v[i]);
		}
	}
}

// 返回key_name == name && key_type == type的数据
bool Catalog::find(const char *name, KEY_TYPE type, Catalog_data &dat) const {
	int size = v.size();
	for (int i = 0; i < size; i++) {
		if (v[i].key_type == type) {
			if (strncmp(v[i].key_name, name, sizeof(v[i].key_name)) == 0) {
				dat = v[i];
				return true;
			}
		}
	}
	return false;
}

// 返回key_id == id的数据
// binary search key id in catalog
bool Catalog::find(int id, Catalog_data &dat) const {
	int st = 0, ed = v.size() - 1;
	while (st <= ed) {
		int mid = (st + ed) / 2;
		if (v[mid].id == id) {
			dat = v[mid];
			return true;
		} else if (v[mid].id > id) {
			ed = mid - 1;
		} else {
			st = mid + 1;
		}
	}
	return false;
}

// 返回第一个数据
bool Catalog::findOne(Catalog_data &dat) const {
	if (v.size() == 0) {
		return false;
	} else {
		dat = v[0];
		return true;
	}
}
// 返回第一个key_name == name的数据
bool Catalog::findOne(const char *name, Catalog_data &dat) const {
	int size = v.size();
	for (int i = 0; i < size; i++) {
		if (strncmp(v[i].key_name, name, sizeof(v[i].key_name)) == 0) {
			dat = v[i];
			return true;
		}
	}
	return false;
}

// 构造函数为私有，只允许构造一个实例对象，并通过instance_指针访问
Catalog::Catalog() {
	key_id_count = 0;
	isDirty = false;
	readFromFile(CATALOG_FILE);
}

bool Catalog::readFromFile(const char *src_file) {
	FILE *fp;
	if ((fp = fopen(src_file, "rw+")) == NULL) {
		//fprintf(stderr, "%s can't open!\n", src_file);
		return false;
	} else {
		Catalog_data dat;
		while (fread(&dat, sizeof(dat), 1, fp)) {
			v.push_back(dat);
			key_id_count++;
		}
		fclose(fp);
		return true;
	}
}

bool Catalog::writeToFile(const char *src_file) {
	FILE *fp;
	if ((fp = fopen(src_file, "w+")) == NULL) {
		fprintf(stderr, "%s can't open!\n", src_file);
		return false;
	} else {
		int size = v.size();
		for (int i = 0; i < size; i++) {
			fwrite(&(v[i]), sizeof(v[i]), 1, fp);
		}
		fclose(fp);
		return true;
	}
	return true;
}

#endif