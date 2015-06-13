/*************************************************************************
 > File Name: find.h
 > Author: zhushh
 > Mail: 
 > Created Time: Tue 02 Jun 2015 07:25:33 PM CST
 ************************************************************************/

#ifndef _FIND_H
#define _FIND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"         // for vector
#include "catalog.h"        // for catalog
#include "buffer.h"         // for buffer_read
#include "record_data.h"    // for struct _Record

static Catalog *find_ctlog = Catalog::getCatalogInstance();

void find(const char *key, const char *value) {
    Vector<Catalog_data> t;
    find_ctlog->find(key, t);
}

#endif
