/*************************************************************************
 > File Name: string.h
 > Author: zhushh
 > Mail: 
 > Created Time: Wed 10 Jun 2015 02:53:06 PM CST
 ************************************************************************/

// 注意下面的 #ifndef 和 #define语句，
// 若写成 #ifndef _STRING_H 和 #define _STRING_H
// 会导致编译错误，因为#include <string.h>文件内部有这两个语句
// 所以在实现.cpp文件的时候会导致string.h文件没有被包含进来
#ifndef ZZ_STRING_H
#define ZZ_STRING_H

#include <iostream>
using std::ostream;
using std::istream;

#define BUF_SIZE 1024

class String {
    public:
        String();
        explicit String(const char *);
        String(const String&);
        ~String();

        void append(const String&);
        void append(const char *);
        void append(char);
        char* c_string() const;

        const String& operator=(const String&);
        const char* operator=(const char *);
        bool operator==(const String&);
        bool operator!=(const String&);
        String operator+(const String&);
        String operator+(const char *);

        int size() const;
        void clear();
        friend ostream& operator<<(ostream& out, const String &);
        friend istream& operator>>(istream& in, String &);
    private:
        char *buf;
        int length;
};

#endif
