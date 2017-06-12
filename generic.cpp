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
    f = func1<float>(i); //ʹ��ʱ�����ػ������������Ļ���������ʲô���壿ֱ�ӵ���func0�������ˣ�
    return 0;
}

