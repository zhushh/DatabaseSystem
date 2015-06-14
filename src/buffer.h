/*************************************************************************
 > File Name: buffer.h
 > Author: zhushh
 > Mail: 
 > Created Time: Fri 12 Jun 2015 08:37:03 PM CST
 ************************************************************************/
#define DEBUG

#ifndef ZZ_BUFFER_H
#define ZZ_BUFFER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef MEMPCPY_FROM_MEMCPY
#define MEMPCPY_FROM_MEMCPY
#define mempcpy(a, b, c) \
(memcpy((a), (b), (c)) != NULL? (a)+(c) : (a))
#endif

#define PAGE_SIZE 1024*8

static char source_file[] = "serialization.data";
static char buffer[PAGE_SIZE+1];
static char *bufptr = buffer;
static char *bufend = buffer + PAGE_SIZE;
static bool isReading = false;
static long current_page = -1;
static long page_counts = 0;
static FILE *fp = NULL;

// declare function
void buffer_start();
void buffer_end();
void buffer_flush();
void buffer_write(const void*, int);
bool buffer_read_page(int);
bool buffer_read(void *, int);

// when using buffer, the buffer_start function must be called firstly.
// And when finished using buffer, should call the buffer_end function.
void buffer_start() {
    if (fp == NULL) {
        if ((fp = fopen(source_file, "rb+")) == NULL) {
            // fprintf(stderr, "Error, buffer_start, while open!\n");
            // exit(1);
            // if no exist source file, then create one
            if ((fp = fopen(source_file, "wb+")) == NULL) {
                fprintf(stderr, "Error, buffer_start, while open!\n");
                exit(1);
            }
        }
        isReading = false;
        bufptr = buffer;
        fseek(fp, 0L, SEEK_END);
        long pos = ftell(fp);
        if (pos == -1L) {
            fprintf(stderr, "Error, buffer_read_page, while get position!\n");
            exit(1);
        } else {
            page_counts = pos / PAGE_SIZE + (pos % PAGE_SIZE == 0 ? 0 : 1);
        }
    }
}

void buffer_end() {
    if (fp != NULL) {
        if (!isReading) {
            buffer_flush();
        }
        fclose(fp);
        fp = NULL;
    }
}

void buffer_flush() {
    if (isReading) {
        return ;
    } else if (bufptr != buffer) {
        if (bufptr - buffer != PAGE_SIZE) {     // clear buffer
            int clear_size = PAGE_SIZE - (bufptr - buffer);
            int i;
            for (i = 0; i < clear_size; i++) {
                bufptr[i] ^= bufptr[i];
            }
        }
        fseek(fp, PAGE_SIZE - (bufend - buffer), SEEK_END);
        if (fwrite(buffer, PAGE_SIZE, 1, fp) != 1) {
            fprintf(stderr, "Error, buffer_flush, while writing!\n");
            exit(1);
        }
        bufend = bufptr;
        bufptr = buffer;
    }
}

void buffer_write(const void *src, int size) {
    if (isReading) {
        isReading = false;
        bufptr = buffer;
    }
    char *ptr = (char*)src;
    while (size + (bufptr - buffer) >= PAGE_SIZE) {
        int len = PAGE_SIZE - (bufptr - buffer);
        bufptr = mempcpy(bufptr, ptr, len);
        buffer_flush();
        ptr += len;
        size -= len;
    }
    if (size > 0) {
        bufptr = mempcpy(bufptr, ptr, size);
    }
}

// page number start from 0
bool buffer_read_page(long pnum) {
    if (!isReading) {
        buffer_flush();
        isReading = true;
    }
    fseek(fp, 0L, SEEK_END);
    long pos = ftell(fp);
    if (pos == -1L) {
        fprintf(stderr, "Error, buffer_read_page, while get position!\n");
        exit(1);
    }
    page_counts = pos / PAGE_SIZE + (pos % PAGE_SIZE == 0 ? 0 : 1);
    if (pnum < 0 || pnum > page_counts) {
        return false;
    } else {
        fseek(fp, (pnum*PAGE_SIZE), SEEK_SET);
        if (fread(buffer, PAGE_SIZE, 1, fp) != 1) {
            if (!feof(fp)) {
                fprintf(stderr, "Error, buffer_read_page, while reading!\n");
                exit(1);
            }
        }
        current_page = pnum;    // set current page as pnum
        bufptr = buffer;
        return true;
    }
}

bool buffer_read(void *dest, int size) {
    if (!isReading) {
        buffer_flush();
        isReading = true;
        current_page = 0;
        buffer_read_page(current_page);
    }
    char *ptr = (char*)dest;
    while (size + (bufptr - buffer) >= PAGE_SIZE) {
        int len = PAGE_SIZE - (bufptr - buffer);
        ptr = mempcpy(ptr, bufptr, size);
        size -= len;
        bufptr += len;
        if (!buffer_read_page(++current_page)) {
            return false;
        }
    }
//    if (current_page == page_counts) {
//        if (size > 0 && bufend - bufptr > size) {
//            memcpy(ptr, bufptr, size);
//            bufptr += size;
//        } else if (size > 0) {
//            return false;
//        }
//    } else 
    if (size > 0) {
        memcpy(ptr, bufptr, size);
        bufptr += size;
    }
    return true;
}

#endif
