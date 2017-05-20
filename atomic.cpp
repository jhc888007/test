#include <atomic>
#include <iostream>

using namespace std;

struct I {
    atomic_int i;
    //atomic_long d;
};

int main() {
    atomic_int i = {0};
    atomic_int j(0);
    atomic_int k{0};
    //atomic_int i = 0; //错误，因为这个调用了拷贝构造函数，但拷贝构造函数被禁用了，第一条初始化调用的是list构造函数，不是拷贝构造函数
    i++;
    cout << i << " " << sizeof(i) << endl;
    cout << sizeof(I) << endl;
    return 0;
}
