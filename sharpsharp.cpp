#include <stdio.h>

#define a 4

#define fun1(s) fun2(s)
#define fun2(s) #s

void nfun(void) {}
void mfun(void) {}

struct T {
    const char *name;
    void (*fun)(void);
};

#define T(NAME) {#NAME,NAME##fun}

T ts[] = {
    T(n),
    T(m),
};

#define P1(s,...) printf(s, ##__VA_ARGS__)
#define P2(s,a...) printf(s, ##a)


int main(void) {
    printf("%s\n", fun1(a)); //"4"
    printf("%s\n", fun2(a)); //"a"
    int b = 5;
    printf("%s\n", fun1(b)); //"b"
    printf("%s\n", fun2(b)); //"b"
    P1("P1 Test, %d, %d\n", 1, 2);
    P2("P2 Test, %d, %d\n", 1, 2);
    return 0;
}

