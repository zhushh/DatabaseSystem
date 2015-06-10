/*************************************************************************
 > File Name: string.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Wed 10 Jun 2015 02:58:55 PM CST
 ************************************************************************/

#include "String.h"
#include <iostream>
using std::cin;
using std::cout;
using std::istream;
using std::ostream;

#include <cstring>
#include <cstdlib>

String::String() {
    buf = NULL;
    length = 0;
}

String::String(const char *s) {
    if (NULL == s) {
        buf = NULL;
        length = 0;
    } else {
        length = strlen(s);
        buf = new char[length+1];
        int i = 0;
        for (i = 0; i < length; i++) {
            buf[i] = s[i];
        }
        buf[i] = '\0';
    }
}

String::String(const String& s) {
    if (s.length > 0) {
        length = s.length;
        buf = new char[length + 1];
        int i = 0;
        for (i = 0; i < length; i++) {
            buf[i] = s.buf[i];
        }
        buf[i] = '\0';
    } else {
        length = 0;
        buf = NULL;
    }
}

const String& String::operator=(const String &s) {
    if (buf != NULL) {
        delete []buf;
        length = 0;
    }
    if (s.length > 0) {
        length = s.length;
        buf = new char[length + 1];
        int i = 0;
        for (i = 0; i < length; i++) {
            buf[i] = s.buf[i];
        }
        buf[i] = '\0';
    } else {
        length = 0;
        buf = NULL;
    }
    return s;
}

const char* String::operator=(const char *s) {
    if (buf != NULL) {
        delete []buf;
        length = 0;
    }
    int len = strlen(s);
    if (len > 0) {
        length = len;
        buf = new char[length+1];
        int i = 0;
        for (i = 0; i < length; i++) {
            buf[i] = s[i];
        }
        buf[i] = '\0';
    } else {
        buf = NULL;
        length = 0;
    }
    return s;
}

String::~String() {
    if (buf != NULL) {
        delete []buf;
    }
    length = 0;
}

bool String::operator==(const String& s) {
    if (length != s.length) {
        return false;
    } else {
        for (int i = 0; i < length; i++) {
            if (buf[i] != s.buf[i]) {
                return false;
            }
        }
        return true;
    }
}

bool String::operator!=(const String &s) {
    if (*this == s) {
        return false;
    } else {
        return true;
    }
}
void String::append(const String &s) {
    if (s.length > 0) {
        int len = length + s.length;
        char *temp = new char[len + 1];
        int i = 0;
        for (i = 0; i < length; i++) {
            temp[i] = buf[i];
        }
        int j = 0;
        for (j = 0; j < s.length; j++, i++) {
            temp[i] = s.buf[j];
        }
        temp[i] = '\0';
        length = len;
        delete []buf;
        buf = temp;
    }
}
void String::append(const char *s) {
    int l = strlen(s);
    if (l > 0) {
        int len = length + l;
        char *temp = new char[len+1];
        int i = 0;
        for (i = 0; i < length; i++) {
            temp[i] = buf[i];
        }
        int j = 0;
        for (j = 0; j < l; j++, i++) {
            temp[i] = s[j];
        }
        temp[i] = '\0';
        length = len;
        delete []buf;
        buf = temp;
    }
}
void String::append(char ch) {
    int len = length + 1;
    char *temp = new char[len+1];
    int i = 0;
    for (i = 0; i < length; i++) {
        temp[i] = buf[i];
    }
    temp[i++] = ch;
    temp[i] = '\0';
    length = len;
    delete []buf;
    buf = temp;
}

char* String::c_string() const {
    return buf;
}

String String::operator+(const String& s) {
    String t;
    t.append(*this);
    t.append(s);
    return t;
}
String String::operator+(const char *s) {
    String t;
    t.append(*this);
    t.append(s);
    return t;
}

int String::size() const {
    return length;
}

void String::clear() {
    if (buf != NULL) {
        delete []buf;
    }
    length = 0;
}

ostream& operator<<(ostream& out, const String &s) {
    if (s.buf != NULL) {
        out << s.buf;
    }
    return out;
}

istream& operator>>(istream& in, String &s) {
    char ss[BUF_SIZE];
    in >> ss;
    s.append(ss);
    return in;
}