/*************************************************************************
 > File Name: vector.h
 > Author: zhushh
 > Mail: 
 > Created Time: Sun 03 Jun 2015 03:48:17 PM CST
 ************************************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#define SIZE 1

template<typename T>
class Vector {
    public:
    	explicit Vector(int s = SIZE);
    	~Vector();
    	Vector(const Vector<T>& v);
    	int size() const;
    	bool empty() const;
    	void clear();
    	void push_back(const T &ele);
    	void pop_back();

    	T& operator[](int index) const;
    	const Vector<T>& operator =(const Vector<T>& v);
    private:
    	int maxSize;
    	int _size;
    	T *data;
};

#include <stdio.h>

template <typename T>
Vector < T >::Vector(int s) {
    maxSize = s;
    _size = 0;
    data = new T[maxSize];
}

template <typename T>
Vector < T >::~Vector() {
    if (data != NULL) {
        delete []data;
        data = NULL;
    }
    _size = 0;
    maxSize = 0;
}

template <typename T>
Vector < T >::Vector(const Vector<T>& v) {
    maxSize = v.maxSize;
    _size = v._size;
    data = new T[maxSize];
    for (int i = 0; i < _size; ++i) {
        data[i] = v.data[i];
    }
}

template <typename T>
int Vector < T >::size() const {
    return _size;
}

template <typename T>
bool Vector < T >::empty() const {
    if (_size > 0) {
        return false;
    } else {
        return true;
    }
}

template <typename T>
void Vector < T >::clear() {
    _size = 0;  // 不需要删除data内存,方便下次重新使用这个vector
}

template <typename T>
void Vector < T >::push_back(const T &ele) {
    if (_size == maxSize) {
        T *temp = new T[2*maxSize];
        for (int i = 0; i < _size; ++i) {
            // 如果是类的拷贝,请在类的实现该类时增加赋值运算符的重载,进行深拷贝
            *(temp + i) = *(data + i);
        }
        delete []data;
        maxSize = 2*maxSize;
        data = temp;
    }
    data[_size++] = ele;
}

template <typename T>
void Vector < T >::pop_back() {
    if (_size <= 0) {
        return ;
    } else {
        _size -= 1;
    }
}

template <typename T>
T& Vector < T >::operator[](int index) const {    // 返回值使用引用是为了实现可以作为左值
    if (index < 0 || index >= _size) {      // 如果越界,默认返回第一个元素
        return *data;
    } else {
        return *(data + index);
    }
}

template <typename T>
const Vector<T>& Vector < T >::operator =(const Vector<T>& v) {
    _size = 0;
    for (int i = 0; i < v._size; i++) {
        push_back(v.data[i]);
    }
    return v;
}

#endif
