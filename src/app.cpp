/*************************************************************************
 > File Name: app.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Mon 08 Jun 2015 02:51:53 PM CST
 ************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "vector.h"
#include "check.h"
#include "find.h"
#include "catalog.h"
#include "catalog.cpp"

// define bool for C language file
#ifndef __cplusplus
typedef enum bool {
    true = 1,
    false = 0
} bool;
#endif

const int NAMESIZE = 1024;

void showHelp() {
	printf("Welcome using this Application made by our team.\n");
	printf("The next just show the Application's manual.\n");
	printf("Best wish.\n\n");
	printf("---------------------------------------------------------------\n");
	printf("\thelp          -- print manual.\n");
	printf("\t ?            -- print manual\n");
	printf("\texit          -- exit application\n");
	printf("\tcheck catalog -- print catalog file\n");
	printf("\tinsert file   -- load the `file' data into database system\n");
	printf("\tfind A = B    -- find the data whose key A has value as B\n");
	printf("---------------------------------------------------------------\n");
}

bool read_key_name_and_value(char *name, char *value);

int main() {
	char cmd[50];
	char filename[NAMESIZE];
	char keyName[100];
	char keyValue[100];
	showHelp();
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
			// to-do
		} else if (strncmp("check", cmd, sizeof("check")) == 0) {
			char ch;
			while ((ch = getchar()) != '\n') ;
			showCatalog();
		} else if (strncmp("find", cmd, sizeof("find")) == 0) {
			if (!read_key_name_and_value(keyName, keyValue)) {
				printf("Invalid command!\n");
			} else {
				// to-do
				printf("keyName == %s\n", keyName);
				printf("keyValue == %s\n", keyValue);
			}
		}
		printf(">> ");
	}
    return 0;
}

bool read_key_name_and_value(char *name, char *value) {
	char ch;
	while (isblank((ch = getchar())) || ch == '\"');	// skip blank & '"'

	// check valid and read key name
	if (ch == '=') {
		while (ch != '\n') ch = getchar();	// clear buffer
		return false;
	} else {
		*name++ = ch;
		while (!isblank((ch = getchar())) && ch != '\"' && ch != '=') {
			*name++ = ch;
		}
		*name = '\0';

		// find the sign of equal and skip it
		if (ch == '\"') {
			while ((ch = getchar()) != '=' && ch != '\n');
		} else if (ch != '=') {
			while ((ch = getchar()) != '=' && ch != '\n');
		}
		// check valid and start read key value
		if (ch != '=') {
			while (ch != '\n') ch = getchar();	// clear buffer
			return false;
		} else {
			while (isblank(ch = getchar()) || ch == '\"');
			if (ch == '\n') {
				while (ch != '\n') ch = getchar();	// clear buffer
				return false;
			} else {
				*value++ = ch;
				while (!isblank((ch = getchar())) && ch != '\"' && ch != '\n') {
					*value++ = ch;
				}
				*value = '\0';
			}
		}
	}
	while (ch != '\n') ch = getchar();	// clear buffer
	return true;
}
