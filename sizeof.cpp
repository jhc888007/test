#include <iostream>

using namespace std;

class Base {
    bool a;
public:
    Base() { cout << "Base()" << endl; }
    ~Base() { cout << "~Base()" << endl; }
    virtual void f(int i) { cout << "Base::f(int)" << endl; }
    virtual void f(double d) { cout << "Base::f(double)" << endl; }
    virtual void g(int i) { cout << "Base::g(int)" << endl; }
    void g(double d) { cout << "Base::g(double)" << endl; }
};

class Derived: public Base {
public:
    Derived() { cout << "Derived()" << endl; }
    ~Derived() { cout << "~Deriverd()" << endl; }
    void f(int i) { cout << "Derived::f(int)" << endl; }
};

class Empty {
};

int main() {
    cout << sizeof(Base) << endl;
    cout << sizeof(Derived) << endl;
    cout << sizeof(Empty) << endl;
    return 0;
}
