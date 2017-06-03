#include <functional>
#include <stdio.h>

int main() {
    std::function<int(int)> f1([](int a)->int{return a;});
    std::function<int(int)> f2;
    std::function<int(double)> f3;
    std::function<double(double)> f4;
    f2 = [](int a)->int{return a;};
    f3 = [](int a)->int{return a;};
    f4 = [](int a)->int{return a;};
    printf("%d\n", f1(2));
    printf("%d\n", f2(2));
    printf("%d\n", f3(2));
    printf("%d\n", f4(2));

    return 0;
}
