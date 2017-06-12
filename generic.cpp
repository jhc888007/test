#include <iostream>

using namespace std;

template<class T>
T func0(int i) {
    T t;
    cout << "T" << endl;
    return t;
}

template<>
int func0<int>(int i) {
    cout << "int" << endl;
    return 0;
}

template<>
float func0<float>(int i) {
    cout << "float" << endl;
    return 0.0;
}

template<class T>
T func1(int i) {
    return func0<T>(i);
}

int main() {
    int i;
    float f;
    i = func1<int>(i);
    f = func1<float>(i); //使用时必须特化，但是这样的话包裹还有什么意义？直接调用func0不就完了？
    return 0;
}

