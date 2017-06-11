#include <iostream>
#include <new>
#include <stdlib.h>

void MyOom() {
    std::cout << "Oom" << std::endl;
    exit(0);
}

int main() {
    std::set_new_handler(MyOom);
    int *verylarge = new int[0x1FFFFFFF];
    delete verylarge;
    return 0;
}
