/*************************************************************************
 > File Name: check.h
 > Author: zhushh
 > Mail: 
 > Created Time: Tue 02 Jun 2015 07:24:59 PM CST
 ************************************************************************/

#ifndef _CHECK_H
#define _CHECK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "catalog.h"
#include "vector.h"

char types[][30] = {
	{"INT"}, {"BOOL"}, {"STRING"}, {"NESTEDINT"},
	{"NESTEDSTR"}, {"NESTEDOBJ"}, {"NESTEDARR"}, {"UNKNOWN"}
};

char* check_get_type_name(KEY_TYPE type) {
	if (type == INT) {
		return types[0];
	} else if (type == BOOL) {
		return types[1];
	} else if (type == STRING) {
		return types[2];
	} else if (type == NESTEDINT) {
		return types[3];
	} else if (type == NESTEDSTR) {
		return types[4];
	} else if (type == NESTEDOBJ) {
		return types[5];
	} else if (type == NESTEDARR) {
		return types[6];
	} else {
		return types[7];
	}
}

void showCatalog() {
	Catalog *instance = Catalog::getCatalogInstance();
	Vector<Catalog_data> v;
	instance->find(v);

	printf("%10s|%20s|%12s|%10s\n", "ID", "KEY_NAME", "KEY_TYPE", "COUNT");
	int k = 55;
	while (k-- > 0) putchar('-');
	putchar('\n');
	int size = v.size();
	int i;
	for (i = 0; i < size; i++) {
		printf("%10d", v[i].id);
		printf("%20s", v[i].key_name);
		printf("%12s", check_get_type_name(v[i].key_type));
		printf("%10d\n", v[i].count);
		if ((i+1) % 36 == 0) {
			char continue_read;
			printf("Press Enter to continue reading and 'q' to end reading!\n");
			while ((continue_read = getchar()) != '\n' && continue_read != 'q');
			if (continue_read == 'q') break;    // quit read data
			putchar('\n');
			printf("%10s|%20s|%12s|%10s\n", "ID", "KEY_NAME", "KEY_TYPE", "COUNT");
			int k = 55;
			while (k-- > 0) putchar('-');
			putchar('\n');
		}
	}
	if (size == 0) {
		printf("NONE\n");
	}
	printf("\n");
}

#endif
