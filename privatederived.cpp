#include <iostream>

using namespace std;

class Base {
public:
    Base() {}
    ~Base() {}
    Base(const Base &b) {}
    Base & operator= (const Base &b) {}
    void * operator new (size_t size) {}
};

//私有继承并不能阻止派生类使用相应方法
class Derived: private Base{
    int a;
public:
    Base & operator= (const Base &b) {
        cout << "=1" << endl;
    }
    Derived & operator= (const Derived &d) {
        cout << "=2" << endl;
    }
    void func() {}
};

int main() {
    Base c, d;
    Derived a, b;
    a = b;
    b = c;
    return 0;
}
