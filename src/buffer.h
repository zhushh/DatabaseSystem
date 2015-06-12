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

#define PAGE_SIZE 1024*8

static int page_num = 0;    // for naming the data file
static int current_open_page = -1;  // -1 means no file open
static char page_name[30];
static char page[PAGE_SIZE+1];      // buffer page
static char *buf_ptr = page;        // buffer pointer

// only buffer can write the buffer data to file
static void store_page() {
    FILE *fp;
    sprintf(page_name, "page_%d", page_num);
    if ((fp = fopen(page_name, "wb+")) == NULL) {
        fprintf(stderr, "%s can't open!\n", page_name);
        exit(1);
    } else {
        fwrite(page, PAGE_SIZE, 1, fp);
        page_num++;
        buf_ptr = page;
        fclose(fp);
    }
}

void buffer_write(const void *src, int size) {
    // to-do when buffer is opened for reading
    if (-1 != current_open_page) {
        current_open_page = -1;
        buf_ptr = page;
        // 如果有记录上次未写满的页面的信息记录时，
        // 可以在这里设置从最近一次写的最后一个页面的断点处继续写入
    }
    char *ptr = (char *)src;
    while (buf_ptr - page + size > PAGE_SIZE) {
        int len = PAGE_SIZE - (buf_ptr - page);
        buf_ptr = mempcpy(buf_ptr, ptr, len);
        store_page();
        ptr += len;
        size -= len;
    }
    if (size > 0) {
        buf_ptr = mempcpy(buf_ptr, ptr, size);
    }
}

// 把未写入页面的数据写入页面文件
void buffer_flush() {
    if (buf_ptr != page && current_open_page == -1) {
        FILE *fp;
        sprintf(page_name, "page_%d", page_num);
        if ((fp = fopen(page_name, "wb+")) == NULL) {
            fprintf(stderr, "%s can't open!\n", page_name);
            exit(1);
        } else {
            fwrite(page, buf_ptr - page, 1, fp);
            page_num++;
            buf_ptr = page;
            fclose(fp);
        }
    }
}

bool read_page(int pnum) {
    // to-do when buffer is opened for writing
    if (-1 == current_open_page) {
        buffer_flush();
    }
    // only permit to read the existent file
    if (pnum < 0 || pnum >= page_num) {
        return false;
    }
    FILE *fp;
    sprintf(page_name, "page_%d", pnum);
    if ((fp = fopen(page_name, "rb+")) == NULL) {
        fprintf(stderr, "%s can't open!\n", page_name);
        return false;
    } else {
        fread(page, PAGE_SIZE, 1, fp);
        buf_ptr = page;
        fclose(fp);
    }
    return true;
}

bool buffer_read(void *ptr, int size) {
    if (-1 == current_open_page) {
        buffer_flush();     // 把还没写入页面的数据写入页面
        current_open_page = 0;
        if (!read_page(current_open_page)) {
            return false;
        }
    }
    char *dest = (char*)ptr;
    while (size + buf_ptr - page > PAGE_SIZE) {
        int len = PAGE_SIZE - (buf_ptr - page);
        dest = mempcpy(dest, buf_ptr, len);
        size -= len;
        current_open_page++;
        if (!read_page(current_open_page)) {
            return false;
        }
    }
    if (size > 0) {
        dest = mempcpy(dest, buf_ptr, size);
        buf_ptr += size;
    }
    return true;
}

int page_count() {
    return page_num;
}

#endif
