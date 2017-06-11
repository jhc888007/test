#include <iostream>
#include <stdlib.h>
#include <new>
#include <stddef.h>
#include <limits.h>

template<class T>
T *func(int size, T*) {
    T *tmp = (T *)malloc(size);
    return tmp;
}

int main() {
    float *f = func<float>(10, (float *)0);
    std::cout << size_type(1);
    return 0;
}
