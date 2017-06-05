#include <iostream>

using namespace std;

class Base {
public:
    void func1() {cout << "func1()" << endl;}
    virtual void func2() {cout << "func2()" << endl;}
    void func3() {cout << "func3()" << endl;}
    virtual void func4() {cout << "func4()" << endl;}
};

class Derived: public Base {
public:
    void func1(int x) {
        func2();
        func3();
    }
    void func4(int x) {
        Base::func1();
        Base::func4();
    }
};

int main() {
    Derived a;
    return 0;
}



