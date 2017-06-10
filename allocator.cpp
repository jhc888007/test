#include <iostream>
#include <stdlib.h>

template<class T>
T *func(int size, T*) {
    T *tmp = (T *)malloc(size);
    return tmp;
}

int main() {
    float *f = func<float>(10, (float *)0);
    return 0;
}
