#include <iostream>
#include <vector>

using namespace std;

/*
//原版
template <template <class ItemT1> class ContainerT, class ItemT2>
class Container {
public:
	virtual void push_back(ItemT2 i) = 0;
	virtual void pop_back(ItemT2 &i) = 0;
};

template <template <class ItemT1> class ContainerT, class ItemT2>
class SafeContainer: public Container<ContainerT, ItemT2> {
public:
	void push_back(ItemT2 i) {
		cout << "SafeContainer push_back" << endl;
	}
	void pop_back(ItemT2 &i) {
		cout << "SafeContainer pop_back" << endl;
	}
};

template <class ItemT>
class Vector: public vector<ItemT> {
public:
	void push_back(ItemT i) {
		cout << "Vector push_back" << endl;
	}
	void pop_back(ItemT &i) {
		cout << "Vector pop_back" << endl;
	}
};

typedef SafeContainer<Vector, int> VectorContainer;
*/

class Normal {
public:
	void push_back(int &i) {
		cout << "Normal push_back" << endl;
	}
};

template<class ItemT>
class Template {
public:
	void push_back(ItemT &i) {
		cout << "Template push_back" << endl;
	}
};

template< template<class ItemT1>class ContainerT, class ItemT>
class Container {
public:
	virtual void push_back(ItemT &i) {
		//普通类
		int j;
		Normal().push_back(j);
		//模板类
		Template<ItemT>().push_back(i);
		//输入模板类，某些编译器编译不过
		ContainerT<ItemT>().push_back(i);
		//输入模板类
		ContainerT<ItemT> c;
		c.push_back(i);

		cout << "Container push_back" << endl;
	}
};

template<class ItemT>
class Vector: public vector<ItemT> {
public:
	void push_back(ItemT &i) {
		cout << "Vector push back" << endl;
	}
};

//类型typedef，可以编译通过，但指定了第二个类型为int
typedef Container<Vector, int> VectorContainer;

//类型模板部分继承
template<class ItemT>
class VectorContainer2: public Container<Vector,ItemT> {
public:
	void push_back(ItemT &i) {
		Vector<ItemT>().push_back(i);
		cout << "Vector Container 2 push back" << endl;
	}
};

/*
编译不过，vector不能直接拿来使用
template<class ItemT>
class VectorContainer3: public Container<vector,ItemT> {
public:
	void push_back(ItemT &i) {
		Vector<ItemT>().push_back(i);
		cout << "Vector Container 3 push back" << endl;
	}
};
*/

//类型模板全部继承
class VectorContainer4: public Container<Vector,int> {
public:
	void push_back(int &i) {
		Vector<int>().push_back(i);
		cout << "Vector Container 4 push_back" << endl;
	}
};

int main() {
	VectorContainer v;
	int i;
	v.push_back(i);

	VectorContainer2<int> v2;
	int i2;
	v2.push_back(i2);

	VectorContainer4 v4;
	int i4;
	v4.push_back(i4);

	cout << "Hello World" << endl;
	return 0;
}
