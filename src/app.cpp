/*************************************************************************
 > File Name: app.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Mon 08 Jun 2015 02:51:53 PM CST
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "vector.h"
#include "check.h"
#include "find.h"
#include "catalog.h"
#include "insert.h"
#include "recovery_data.h"

// define bool for C language file
#ifndef __cplusplus
typedef enum bool {
    true = 1,
    false = 0
} bool;
#endif

const int NAMESIZE = 1024;

void showHelp() {
	printf("---------------------------------------------------------------\n");
	printf("\thelp          -- print manual.\n");
	printf("\t ?            -- print manual\n");
	printf("\texit          -- exit application\n");
	printf("\tcheck catalog -- print catalog file\n");
	printf("\tinsert file   -- load the `file' data into database system\n");
	printf("\tfind A = B    -- find the data whose key A has value as B\n");
	printf("\tshowJson      -- print all data with Json format.\n");
	printf("---------------------------------------------------------------\n");
}

void startShow() {
	printf("Welcome using this Application made by our team.\n");
	printf("The next just show the Application's manual.\n");
	printf("Best wish.\n\n");
	showHelp();
}

int main(int argc, char **argv) {
	char cmd[100];
	char filename[NAMESIZE];
	//char keyName[100];
	char keyValue[1024];
	Catalog *catalog = Catalog::getCatalogInstance();	// read/create catalog
	startShow();
	printf(">> ");
	while (scanf("%s", cmd) != EOF) {
		if (strncmp("?", cmd, sizeof("?")) == 0) {
			showHelp();
		} else if (strncmp("help", cmd, sizeof("help")) == 0) {
			showHelp();
		} else if (strncmp("exit", cmd, sizeof("exit")) == 0) {
			break;
		} else if (strncmp("insert", cmd, sizeof("insert")) == 0) {
			scanf("%s", filename);
			printf("Inserting %s ...\n", filename);
			insert(filename);
		} else if (strncmp("check", cmd, sizeof("check")) == 0) {
			scanf("%s", filename);
			showCatalog();
		} else if (strncmp("find", cmd, sizeof("find")) == 0) {
			int size;
			Vector<int> aids;
			if (find_get_arguments(aids, keyValue, size)) {
				find(aids, keyValue, size);
				putchar('\n');
			} else {
				printf("Invalid!\n");
			}
		} else if (strncmp("showJson", cmd, sizeof("json")) == 0) {
			recovery_data();	// just recovery all the data
		} else {
            system(cmd);	// try to call system command
        }
		printf(">> ");
	}
	delete catalog;		// write catalog to file catalog for next time using.
    return 0;
}
