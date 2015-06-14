/*************************************************************************
 > File Name: find.h
 > Author: zhushh
 > Mail: 
 > Created Time: Tue 02 Jun 2015 07:25:33 PM CST
 ************************************************************************/

#ifndef ZZ_FIND_H
#define ZZ_FIND_H

// #ifndef DEBUG
// #define DEBUG
// #endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"         // for vector
#include "catalog.h"        // for catalog
#include "buffer.h"         // for buffer_read
#include "record_data.h"    // for struct _Record
#include "show_data.h"      // for show_record function

static Catalog *find_ctlog = Catalog::getCatalogInstance();

bool find_binsearch_id(int id, Vector<Catalog_data> &v) {
    int st = 0, ed = v.size() - 1;
    while (st <= ed) {
        int mid = st + (ed - st)/2;
        if (v[mid].id == id) {
            return true;
        } else if (v[mid].id > id) {
            ed = mid - 1;
        } else {
            st = mid + 1;
        }
    }
    return false;
}

bool is_equal(const void *a, const void *b) {
    return true;
}

void find(const char *key, const char *value) {
#ifdef DEBUG
    int count = 1;
#endif
    Vector<Catalog_data> vv;
    // the find function will return the increasing catalog data sorted by id
    find_ctlog->find(key, vv);
    if (vv.size() == 0) {
        printf("NONE\n");
    } else {
        buffer_start();
        Record t;
        while (buffer_read(&(t.attrNum), sizeof(t.attrNum)) && t.attrNum > 0) {
            bool isFound = false;
            int j, num, index = 0;
            for (j = 0; j < t.attrNum; j++) {
                buffer_read(&num, sizeof(int));
                t.aids.push_back(num);
                if (find_binsearch_id(num, vv)) {
                    isFound = true;
                    index = j;
                }
            }
            for (j = 0; j < t.attrNum; j++) {
                buffer_read(&num, sizeof(int));
                t.offs.push_back(num);
            }
            buffer_read(&(t.len), sizeof(t.len));
            buffer_read(t.data, t.len);
            if (isFound) {
                if (is_equal(value, t.data+t.offs[index])) {
                    #ifdef DEBUG
                        printf("%d\t\n", count++);
                    #endif
                    show_record(t);
                }
            }
            t.attrNum = t.len = 0;
            t.aids.clear();
            t.offs.clear();
        }
        buffer_end();
    }
}

#endif
