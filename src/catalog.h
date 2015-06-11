/*************************************************************************
 > File Name: catalog.h
 > Author: zhushh
 > Mail: 
 > Created Time: Mon 08 Jun 2015 02:03:47 PM CST
 ************************************************************************/

#ifndef _CATALOG_H
#define _CATALOG_H

#include <cstring>
#include "vector.h"

// the KEY_TYPE is useful for catalog file
enum KEY_TYPE {
    INT, BOOL, STRING, NESTEDINT, NESTSTR, NESTEDOBJ, NESTEDARR
};

// define the Catalog_data
typedef struct _Catalog_data {
    int id;
    char key_name[50];
    KEY_TYPE key_type;
    int count;
    const _Catalog_data& operator=(const _Catalog_data &other) {
        id = other.id;
        strncpy(key_name, other.key_name, sizeof(key_name));
        key_type = other.key_type;
        count = other.count;
        return other;
    }
} Catalog_data;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName& ); \
TypeName& operator=(const TypeName&)

class Catalog {
    public:
        // 用于获取catalog对象的操作句柄
        static Catalog* getCatalogInstance(void);
        ~Catalog();

        // 若name & type 已经存在catalog当中，进行count++
        // 反之，新建一个数据，记录name & type并生成新的key_id
        // 返回值为key_id
        int create(const char *name, KEY_TYPE type);

        // 获取所有的数据
        void find(Vector<Catalog_data> &) const;
        // 获取key_name == name的所有数据
        void find(const char *name, Vector<Catalog_data> &) const;
        // 获取key_name == name && key_type == type的数据
        bool find(const char *name, KEY_TYPE type, Catalog_data &) const;
        // 获取key_id == id的数据
        bool find(int id, Catalog_data &) const;

        // 获取第一个数据
        bool findOne(Catalog_data &) const;
        // 获取第一个key_name == name的数据
        bool findOne(const char *name, Catalog_data &) const;

    private:
        // catalog的对象instance只允许一个，目的诗保持catalog数据的同步
        Catalog();      // 不允许外部创建对象实例
        DISALLOW_COPY_AND_ASSIGN(Catalog);      // 不允许拷贝

        bool readFromFile(const char *src_file);
        bool writeToFile(const char *src_file);
        static Catalog *instance_;      // 对象获取实例的唯一句柄
        static bool isDirty;            // 利用dirty标记catalog是否被修改过
        static int key_id_count;        // 标记需要新增key id的id
        Vector<Catalog_data> v;         // 用于存储catalog的数据
};

#endif