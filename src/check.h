/*************************************************************************
 > File Name: check.h
 > Author: zhushh
 > Mail: 
 > Created Time: Tue 02 Jun 2015 07:24:59 PM CST
 ************************************************************************/

#ifndef _CHECK_H
#define _CHECK_H

#include "catalog.h"
#include "catalog.cpp"
#include "vector.h"

#include <stdio.h>

char types[][30] = {
	{"INT"}, {"BOOL"}, {"STRING"}, {"NESTEDINT"},
	{"NESTEDSTR"}, {"NESTEDOBJ"}, {"NESTEDARR"}
};

char* output_type(KEY_TYPE type) {
	if (type == INT) {
		return types[0];
	} else if (type == BOOL) {
		return types[1];
	} else if (type == STRING) {
		return types[2];
	} else if (type == NESTEDINT) {
		return types[3];
	} else if (type == NESTSTR) {
		return types[4];
	} else if (type == NESTEDOBJ) {
		return types[5];
	} else if (type == NESTEDARR) {
		return types[6];
	}
}

void showCatalog() {
	Catalog *instance = Catalog::getCatalogInstance();
	Vector<Catalog_data> v;
	instance->find(v);

	printf("%10s|%20s|%12s|%10s\n", "ID", "KEY_NAME", "KEY_TYPE", "COUNT");
	int i = 55;
	while (i-- > 0) putchar('-');
	putchar('\n');
	int size = v.size();
	for (i = 0; i < size; i++) {
		printf("%10d", v[i].id);
		printf("%20s", v[i].key_name);
		printf("%12s", output_type(v[i].key_type));
		printf("%10d\n", v[i].count);
	}
	printf("\n");
}

#endif
