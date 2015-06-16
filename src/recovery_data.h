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

#include "buffer.h"         // for buffer_read
#include "record_data.h"    // for struct _Record
#include "show_data.h"      // for show_record function

void recovery_data() {
    int count = 1;
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
        printf("%d\t", count++);
        show_record(t);
        putchar('\n');
        t.attrNum = t.len = 0;
        t.aids.clear();
        t.offs.clear();
        // just control the output for readable!!
        // using the buffer's benifit to reduce calculate
        // 缓冲技术的使用,只在用户想要继续看数据的时候才继续读取数据
        if (count % 9 == 0) {
            char continue_read;
            printf("Press Enter to continue reading and 'q' to end reading!\n");
            while ((continue_read = getchar()) != '\n' && continue_read != 'q');
            if (continue_read == 'q') break;    // quit read data
            putchar('\n');
            putchar('\n');
        }
    }
    buffer_end();
}

#endif