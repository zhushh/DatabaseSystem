#ifndef ZZ_SHOW_DATA_H
#define ZZ_SHOW_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "record_data.h"    // for struct _Record
#include "catalog.h"    	// for KEY_TYPE & Catalog_data info

//declare function
void print_value(char *, KEY_TYPE, int);
void show_record(Record &);
void show_nested_obj(char *src);

void print_value(char *src, KEY_TYPE type, int size) {
    if (type == INT || type == NESTEDINT) {
        int num;
        memcpy(&num, src, size);
        printf("%d", num);
    } else if (type == BOOL) {
        char boolean;
        memcpy(&boolean, src, size);
        if (boolean == 'T' || boolean == 't') {
            printf("true");
        } else {
            printf("false");
        }
    } else if (type == STRING || type == NESTEDSTR) {
        putchar('\"');
        for (int i = 0; i < size; i++) {
            putchar(src[i]);
        }
        putchar('\"');
    } else if (type == NESTEDARR) {
        putchar('[');
        for (int i = 0; i < size; i++) {
            putchar(src[i]);
        }
        putchar(']');
    }
}

void show_record(Record &t) {
    Catalog *instance = Catalog::getCatalogInstance();
    putchar('{');
    int i;
    Catalog_data dat;
    for (i = 0; i < t.attrNum; i++) {
        if (!instance->find(t.aids[i], dat)) {
            continue;   // skip the wrong case
        }
        printf("\"%s\": ", dat.key_name);
        int size;
        if (dat.key_type != NESTEDOBJ) {
            if (i < t.attrNum - 1) {
                size = t.offs[i+1] - t.offs[i];
                print_value(t.data + t.offs[i], dat.key_type, size);
                putchar(',');
                putchar(' ');
            } else {
                size = t.len - t.offs[i];
                print_value(t.data + t.offs[i], dat.key_type, size);
            }
        } else {
            Record tt;
            char *src = t.data + t.offs[i];
            int num, k;
            memcpy(&(tt.attrNum), src, sizeof(int));
            src += sizeof(int);
            for (k = 0; k < tt.attrNum; k++) {
                memcpy(&num, src, sizeof(int));
                tt.aids.push_back(num);
                src += sizeof(int);
            }
            for (k = 0; k < tt.attrNum; k++) {
                memcpy(&num, src, sizeof(int));
                tt.offs.push_back(num);
                src += sizeof(int);
            }
            memcpy(&(tt.len), src, sizeof(int));
            src += sizeof(int);
            memcpy(tt.data, src, tt.len);
            show_record(tt);
            if (i < t.attrNum - 1) {
                putchar(',');
                putchar(' ');
            }
        }
    }
    printf("}");
}

#endif