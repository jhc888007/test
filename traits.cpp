#include <iostream>


class type1 {};
class type2 {};
class type3 {};

template <class T>
class traits {
public:
    typedef type1 mktype;
};

//泛型偏特化
template <class T>
class traits<T*> {
public:
    typedef type2 mktype;
};

//泛型偏特化
template <>
class traits<float> {
public:
    typedef type3 mktype;
};

//哑元入参负责区分调用函数
template <class T>
void func1(T t, type1) {
    std::cout << "func1_type1" << " " << t << std::endl;
}

//哑元入参负责区分调用函数
template <class T>
void func1(T t, type2) {
    std::cout << "func1_type2" << " " << *t << std::endl;
}

//哑元入参负责区分调用函数
template <class T>
void func1(T t, type3) {
    std::cout << "func1_type3" << " " << t << std::endl;
}

//根据泛化类型里的typedef来调用相应的函数
template <class T>
void func2(T t) {
    func1(t, typename traits<T>::mktype());
}

int main() {
    int i = 0;
    func2(i);
    func2(&i);
    float f = 0;
    func2(f);

    return 0;
}

