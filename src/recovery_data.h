/*************************************************************************
 > File Name: recovery_data.h
 > Author: zhushh
 > Mail: 
 > Created Time: Fri 12 Jun 2015 11:18:32 PM CST
 ************************************************************************/

#ifndef _RECOVERY_DATA_H
#define _RECOVERY_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"     // for buffer_read
#include "record_data.h"     // for struct _Record
#include "catalog.h"    // for KEY_TYPE & Catalog_data info

static Catalog *recovery_ctlog = Catalog::getCatalogInstance();

void print_value(const char *src, KEY_TYPE type, int size) {
    if (type == INT) {
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
    } else if (type == STRING) {
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
    } else if (type == NESTEDOBJ) {
        putchar('{');
        for (int i = 0; i < size; i++) {
            putchar(src[i]);
        }
        putchar('}');
    }
}

void show_record(Record &t) {
    putchar('{');
    Catalog_data dat;
    for (int i = 0; i < t.attrNum; i++) {
        recovery_ctlog->find(t.aids[i], dat);
        printf("\"%s\": ", dat.key_name);
        int size;
        if(i < t.attrNum - 1) {
            size = t.offs[i+1] - t.offs[i];
            print_value(t.data + t.offs[i], dat.key_type, size);
            putchar(',');
            putchar(' ');
        } else {
            size = t.len - t.offs[i];
            print_value(t.data + t.offs[i], dat.key_type, size);
        }
    }
    printf("}\n");
}

void recovery_data() {
    buffer_start();
    Record t;
    while (buffer_read(&(t.attrNum), sizeof(t.attrNum)) && t.attrNum > 0) {
        int j, num;
        for (j = 0; j < t.attrNum; j++) {
            buffer_read(&num, sizeof(int));
            t.aids.push_back(num);
        }
        for (j = 0; j < t.attrNum; j++) {
            buffer_read(&num, sizeof(int));
            t.offs.push_back(num);
        }
        buffer_read(&(t.len), sizeof(t.len));
        buffer_read(t.data, t.len);
        show_record(t);
        t.attrNum = t.len = 0;
        t.aids.clear();
        t.offs.clear();
    }
    buffer_end();
}

#endif
