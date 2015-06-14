/*************************************************************************
 > File Name: buffer.h
 > Author: zhushh
 > Mail: 
 > Created Time: Fri 12 Jun 2015 08:37:03 PM CST
 ************************************************************************/
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

const int PAGE_SIZE = 1024*8;

static char source_file[] = "serialization.data";
static char buffer[PAGE_SIZE+1];
static char *bufptr = buffer;
static char *bufend = buffer + PAGE_SIZE;
static bool isReading = false;
static int current_page_num = -1;
static FILE *fp = NULL;

// declare function
void buffer_start();
void buffer_end();
void buffer_save();
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
            // if no exist source file, then create a new one
            if ((fp = fopen(source_file, "wb+")) == NULL) {
                fprintf(stderr, "Error, buffer_start, while open!\n");
                exit(1);
            } else {
                isReading = false;
                bufptr = buffer;
                bufend = buffer + PAGE_SIZE;
                return;
            }
        }
        isReading = false;
        bufptr = buffer;
        // get bufend position
        fseek(fp, -(PAGE_SIZE), SEEK_END);
        if (fread(buffer, PAGE_SIZE, 1, fp) != 1) {
            if (!feof(fp)) {
                fprintf(stderr, "Error, buffer_start, while reading\n");
                exit(1);
            }
        }
        bufend = buffer + PAGE_SIZE - 1;
        while ((*bufend) == 0 && bufend - buffer > 0) bufend--;
        if (bufend - buffer > 0) ++bufend;
    }
}

void buffer_end() {
    if (fp != NULL) {
        if (!isReading) {
            buffer_save();
        }
        fclose(fp);
        fp = NULL;
    }
}

void buffer_save() {
    if (isReading) {
        return ;
    } else if (bufptr != buffer) {
    	int clear_size = PAGE_SIZE - (bufptr - buffer);
    	while (clear_size-- > 0) {
    		bufptr[clear_size] ^= bufptr[clear_size];
    	}
    	fseek(fp, ((bufend - buffer) - PAGE_SIZE), SEEK_END);
        if (fwrite(buffer, PAGE_SIZE, 1, fp) != 1) {
            fprintf(stderr, "Error, buffer_save, while writing!\n");
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
        buffer_save();
        ptr += len;
        size -= len;
    }
    if (size > 0) {
        bufptr = mempcpy(bufptr, ptr, size);
    }
}

bool buffer_read_page(int pnum) {
    if (!isReading) {
        buffer_save();      // keep data consistent
        isReading = true;
    }
    fseek(fp, 0L, SEEK_END);
    long pos = ftell(fp);
    if (pos == -1L) {
        fprintf(stderr, "Error, buffer_read_page, while getting position!\n");
        exit(1);
    }
    int pages = pos/(PAGE_SIZE) + (pos%(PAGE_SIZE) == 0? 0 : 1);
    if (pnum < 0 || pnum > pages) {
        return false;
    } else {
        fseek(fp, (pnum*(PAGE_SIZE)), SEEK_SET);
        if (fread(buffer, PAGE_SIZE, 1, fp) != 1) {
            if (!feof(fp)) {
                fprintf(stderr, "Error, buffer_read_page, while reading!\n");
                exit(1);
            }
        }
        current_page_num = pnum;
        bufptr = buffer;
        return true;
    }
}

bool buffer_read(void *dest, int size) {
    if (!isReading) {
        buffer_save();
        isReading = true;
        buffer_read_page(0);     // start reading from the 0th page
    }
    char *ptr = (char*)dest;
    while (size + (bufptr - buffer) >= PAGE_SIZE) {
        int len = PAGE_SIZE - (bufptr - buffer);
        ptr = mempcpy(ptr, bufptr, len);
        size -= len;
        bufptr += len;
        if (!buffer_read_page(++current_page_num)) {
            return false;
        }
    }
    if (size > 0) {
        memcpy(ptr, bufptr, size);
        bufptr += size;
    }
    return true;
}

#endif
