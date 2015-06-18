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
#include "buffer.h"         // for buffer function
#include "record_data.h"    // for struct _Record
#include "extract.h"        // for extract data
#include "show_record.h"    // for show_record function
#include "check.h"          // for debug to output type name

// declare function
char find_get_key(char *);
bool find_get_value_and_type(char *, KEY_TYPE &, int &);
// get the arguments for finding
bool find_get_arguments(Vector<int> &, char *, int &);
// compare the bytes to check if they is equal
bool find_compare(const void *, const void *, int);
// utility function supporting finding nested_obj's key
void find(const Vector<int> &, const void *, int);

// start definition
char find_get_key(char *key) {
    char ch;
    // read key name
    while (isblank((ch = getchar())));  // skip blank 
    if (ch == '\"') {
        while ((ch = getchar()) != '\"' && ch != '\n' && ch != '.') {
            *key++ = ch;
        }
        *key = '\0';
    } else if (ch == '\'') {
        while ((ch = getchar()) != '\'' && ch != '\n' && ch != '.') {
            *key++ = ch;
        }
        *key = '\0';
    } else if (ch != '\n') {
        *key++ = ch;
        while (!isblank((ch = getchar())) && ch != '=' && ch != '\n' && ch != '.') {
            *key++ = ch;
        }
        *key = '\0';
    }
    return ch;
}

bool find_get_value_and_type(char *value, KEY_TYPE &type, int &size) {
    char ch;
    char src[2048];
    size = 0;
    while (isblank(ch = getchar()));
    if (ch != '\n') {
        int i = 0;
        src[i++] = ch;
        while (!isblank((ch = getchar())) && ch != '\n') src[i++] = ch;
        src[i] = '\0';
        if (src[0] == '{') {
            type = NESTEDOBJ;
            extract_nested_obj(src, value, size, 0);
        } else if (src[0] == '[') {
            src[i++] = ch;
            while ((ch = getchar()) != '\n') {
                src[i++] = ch;
            }
            src[i] = '\0';
            type = NESTEDARR;
            extract_nested_arr(src, value, size, 0);
            value[size] = '\0';
        } else if (src[0] == '\"' || src[0] == '\'') {
            if (strncmp("\"true\"", src, sizeof("\"true\"")) == 0) {
                type = BOOL;
                extract_bool(src, value, size, 0);
            } else if (strncmp("\"false\"", src, sizeof("\"false\"")) == 0) {
                type = BOOL;
                extract_bool(src, value, size, 0);
            } else {
                type = STRING;
                extract_string(src, value, size, 0);
            }
        } else if (isdigit(src[0])) {
            type = INT;
            extract_int(src, value, size, 0);
        } else if (src[0] == 't' || src[0] == 'f') {
            type = BOOL;
            extract_bool(src, value, size, 0);
        } else {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool find_get_arguments(Vector<int> &aids, char *value, int &size) {
    Catalog *catalog = Catalog::getCatalogInstance();   // read/create catalog
    Catalog_data dat;
    KEY_TYPE type;
    char ch;
    char key[100];
    // get key name
    while ((ch = find_get_key(key)) == '.') {
        if (catalog->find(key, NESTEDOBJ, dat)) {
            aids.push_back(dat.id);
        } else {
            return false;
        }
    }
    if (ch == '\n') {
        printf("Please input key value\n");
        return false;
    }
    if (ch != '=') while (isblank(ch = getchar()));
    if (ch != '=') {
        while ((ch = getchar()) != '\n');
        return false;
    }
    if (find_get_value_and_type(value, type, size)) {
        // reset type
        if (aids.size() > 0 && type == INT) {
            type = NESTEDINT;
        } else if (aids.size() > 0 && type == STRING) {
            type = NESTEDSTR;
        }
        // check exist
        if (catalog->find(key, type, dat)) {
            aids.push_back(dat.id);
        } else {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool find_compare(const void *src, const void *dest, int size) {
    char *a = (char*)src;
    char *b = (char*)dest;
    while (size-- > 0) {
        if (*a++ != *b++) {
            return false;
        }
    }
    return true;
}

void find(const Vector<int> &ids, const void *value, int size) {
    int count = 1;
    buffer_start();
    Record t;
    while (buffer_read(&(t.attrNum), sizeof(t.attrNum)) && t.attrNum > 0) {
        int num, j, index = 0;
        bool isFound = false;
        // read attr ids and check exist
        for (j = 0; j < t.attrNum; j++) {
            buffer_read(&num, sizeof(num));
            t.aids.push_back(num);
            if (ids[0] == num) {
                isFound = true;
                index = j;
            }
        }
        // read offsets
        for (j = 0; j < t.attrNum; j++) {
            buffer_read(&num, sizeof(num));
            t.offs.push_back(num);
        }
        buffer_read(&(t.len), sizeof(t.len));
        buffer_read(t.data, t.len);
        if (isFound) {
            // buffer_read(t.data, t.len);
            char *curptr = t.data + t.offs[index];
            int idsize = ids.size();
            // nested search id
            for (int k = 1; isFound && k < idsize; k++) {
                int attrN;
                char *ptr = curptr;
                isFound = false;
                memcpy(&attrN, curptr, sizeof(attrN));
                curptr += sizeof(attrN);
                // check that sub_obj's aid
                for (int l = 0; l < attrN; l++) {
                    memcpy(&num, curptr, sizeof(num));
                    curptr += sizeof(num);
                    if (num == ids[k]) {
                        // read len and move curptr
                        // store data len in num
                        // store index
                        if (k < idsize - 1) {
                            memcpy(&num, ptr + 2*4*(attrN+1) - 4, sizeof(num));
                            curptr = ptr + 2*4*(attrN+1) + num;
                        } else {
                            memcpy(&num, ptr + 4*(attrN+1+l), sizeof(num));
                            curptr = ptr + 2*4*(attrN+1) + num;
                        }
                        isFound = true;
                        index = l;
                        break;
                    }
                }
            }
            if (isFound && find_compare(value, curptr, size)) {
                printf("%d\t", count++);
                show_record(t);
                putchar('\n');
                // just control the output for readable!!
                // using the buffer's benifit to reduce calculate
                // 缓冲技术的使用,只在用户想要继续看数据的时候才继续读取数据
                // if (count % 9 == 0) {
                //     char continue_read;
                //     printf("Press Enter to continue reading and 'q' to end reading!\n");
                //     while ((continue_read = getchar()) != '\n' && continue_read != 'q');
                //     if (continue_read == 'q') break;    // quit read data
                //     putchar('\n');
                //     putchar('\n');
                // }
            }
        }
        // else if (buffer_skip(t.len) < 0) {
        //    fprintf(stderr, "Error, find, call buffer_skip fail!\n");
        //    exit(1);
        //}
        t.attrNum = t.len = 0;
        t.aids.clear();
        t.offs.clear();
    }
    buffer_end();
}

#endif
