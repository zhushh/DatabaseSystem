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
        t.attrNum = t.len = 0;
        t.aids.clear();
        t.offs.clear();
    }
    buffer_end();
}

#endif