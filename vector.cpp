#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Struct {
    int _a;
public:
    Struct(int a): _a(a) { cout << "Struct(" << _a << ")" << endl; }
    Struct(const Struct &s) { _a = s._a; cout << "Struct(" << _a << ")Copy" << endl; }
    ~Struct() { cout << "~Struct(" << _a << ")" << endl; }
    Struct &operator= (const Struct &s) { _a = s._a; cout << "Struct(" << _a << ")=" << endl; }
    bool operator== (const Struct &s) const { return _a == s._a; }
    int getVal() const { return _a; } 
};

#define DUMP(v1) cout<<#v1<<".size(): "<<v1.size()<<" "<<#v1<<".capacity(): "<<v1.capacity()<<endl
//FOR1会进行复制构造，然后马上析构
#define FOR1(v1) cout<<#v1<<"'s elements:"; for(auto val:v1) cout<<" "<<val.getVal(); cout<<endl
//FOR2不会进行复制构造
#define FOR2(v1) cout<<#v1<<"'s elements:"; for_each(v1.cbegin(),v1.cend(),\
    [](const Struct &val)->void{cout<<" "<<val.getVal();}); cout<<endl

template<class T>
void swap(vector<T> &v) {
    vector<T> v_;
    v_.swap(v);
}

int main() {
    vector<int> v1;
    DUMP(v1);
    v1.push_back(1);
    DUMP(v1);
    v1.push_back(2);
    DUMP(v1);
    v1.push_back(3);
    DUMP(v1);
    v1.push_back(4);
    DUMP(v1);
    v1.push_back(5);
    DUMP(v1);
    v1.clear();
    DUMP(v1);
    swap<int>(v1);
    DUMP(v1);

    //C++11
    vector<int> v2 = {1,2,3,4,5};
    DUMP(v2);
    v2.shrink_to_fit();
    DUMP(v2);
    v2.clear();
    DUMP(v2);
    v2.shrink_to_fit();
    DUMP(v2);

    vector<Struct> v3 = {Struct(1), Struct(2), Struct(3), Struct(4), Struct(5)};
    DUMP(v3);
    v3.clear();
    DUMP(v3);
    swap<Struct>(v3);
    DUMP(v3);

    vector<Struct> v4 = {Struct(1), Struct(2), Struct(3), Struct(4), Struct(5), Struct(6), Struct(7), Struct(8), Struct(9)};
    DUMP(v4); FOR1(v4); FOR2(v4);
    for (auto it = v4.begin(); it != v4.end();) {
        if (it->getVal() == 5)
            it = v4.erase(it);
        else
            it++;
    }
    DUMP(v4); FOR2(v4);
    auto it = find(v4.begin(), v4.end(), Struct(3));
    if (it != v4.end())
        v4.erase(it);
    DUMP(v4); FOR2(v4);
    auto rit = find(v4.rbegin(), v4.rend(), Struct(7));
    if (rit != v4.rend())
        v4.erase(--rit.base());
    DUMP(v4); FOR2(v4);
    v4.clear();
    DUMP(v4); FOR2(v4);

    vector<vector<Struct> > v5;
    vector<Struct> v6 = {Struct(1), Struct(2)};
    DUMP(v6);
    vector<Struct> v7 = {Struct(3), Struct(4)};
    DUMP(v7);
    v5.push_back(v6);
    DUMP(v5);
    v5.push_back(v7);
    DUMP(v5);
    v5.erase(v5.begin());
    DUMP(v5);

    return 0;
}

