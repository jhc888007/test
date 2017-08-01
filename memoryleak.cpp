#include <iostream>

using namespace std;

class A
{
public:
	void Func1 (void) { cout << "A" << endl; }
	void Func2 (void) { a = 1; cout << "B" << endl; }
private:
	int a;
};

int main(void)
{
	A *p;
	int i = 0;
	//cin >> i; //If not committed, will core dump. If committed, maybe not core dump
	if (i > 10)
	{
		A a;
		p = &a;
	}
	p->Func1(); //Will not core dump
	p->Func2(); //Maybe core dump
	return 0;
}
