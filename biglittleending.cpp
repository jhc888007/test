#include <iostream>


using namespace std;

int main() {
    unsigned long a = 0xF1F2F3F4F5F6F7F8;
    unsigned char c;

    asm volatile (
        "movq %1, %%rax;"
        "movb %%al, %0;"
        : "+m" (c)
        : "m" (a)
        : "%rax"
        );

    cout << hex << (int)c << endl;

    return 0;
}
