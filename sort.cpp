#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

template <class T>
void print(T begin, T end) {
    for (T it = begin; it != end; it++) 
        cout << *it << ",";
    cout << endl;
}

struct Comp1 {
    bool operator() (int a, int b) {return a>b;}
}comp1;

struct Comp2 {
    bool operator() (const int &a, const int &b) {return a>b;}
}comp2;

bool comp3(const int &a, const int &b) {return a>b;}

struct Sample {
    int v;
    Sample(int val): v(val) {}
    bool operator< (const Sample &b) {return v>b.v;}
    friend bool comp4(const Sample &a, const Sample &b);
    friend ostream &operator<< (ostream &os, Sample &s) {os<<s.v;}
    struct Comp5;
    friend Comp5;
};

struct Comp5 {
    bool operator() (const Sample &a, const Sample &b) {return a.v>b.v;}
}comp5;

bool comp4(const Sample &a, const Sample &b) {return a.v>b.v;} //定义在类中就不行

int main() {
    vector<int> vInt1, vInt2, vInt3, vInt4, vInt5;
    vInt1.push_back(4);
    vInt1.push_back(6);
    vInt1.push_back(2);
    vInt1.push_back(8);
    vInt1.push_back(2);
    vInt1.push_back(1);
    vInt1.push_back(7);
    vInt1.push_back(5);
    vInt1.push_back(9);
    vInt2.insert(vInt2.end(), vInt1.begin(), vInt1.end());
    vInt3.insert(vInt3.end(), vInt1.begin(), vInt1.end());
    vInt4.insert(vInt4.end(), vInt1.begin(), vInt1.end());
    vInt5.insert(vInt5.end(), vInt1.begin(), vInt1.end());

    print<vector<int>::iterator>(vInt1.begin(), vInt1.end());

    sort(vInt1.begin(), vInt1.end());
    print<vector<int>::iterator>(vInt1.begin(), vInt1.end());

    sort(vInt2.begin(), vInt2.end(), comp1);
    print<vector<int>::iterator>(vInt2.begin(), vInt2.end());
    
    sort(vInt3.begin(), vInt3.end(), comp2);
    print<vector<int>::iterator>(vInt3.begin(), vInt3.end());

    sort(vInt4.begin(), vInt4.end(), comp3);
    print<vector<int>::iterator>(vInt4.begin(), vInt4.end());

    vector<Sample> vSample1, vSample2, vSample3;
    vSample1.push_back(Sample(4));
    vSample1.push_back(Sample(6));
    vSample1.push_back(Sample(2));
    vSample1.push_back(Sample(8));
    vSample1.push_back(Sample(2));
    vSample1.push_back(Sample(1));
    vSample1.push_back(Sample(7));
    vSample1.push_back(Sample(5));
    vSample1.push_back(Sample(9));
    vSample2.insert(vSample2.begin(), vSample1.begin(), vSample1.end());
    vSample3.insert(vSample3.begin(), vSample1.begin(), vSample1.end());

    print<vector<Sample>::iterator>(vSample1.begin(), vSample1.end());

    sort(vSample1.begin(), vSample1.end());
    print<vector<Sample>::iterator>(vSample1.begin(), vSample1.end());

    sort(vSample2.begin(), vSample2.end(), comp4);
    print<vector<Sample>::iterator>(vSample2.begin(), vSample2.end());

    list<Sample> lSample1, lSample2, lSample3;
    lSample1.push_back(Sample(4));
    lSample1.push_back(Sample(6));
    lSample1.push_back(Sample(2));
    lSample1.push_back(Sample(8));
    lSample1.push_back(Sample(2));
    lSample1.push_back(Sample(1));
    lSample1.push_back(Sample(7));
    lSample1.push_back(Sample(5));
    lSample1.push_back(Sample(9));
    lSample2.insert(lSample2.begin(), lSample1.begin(), lSample1.end());
    lSample3.insert(lSample3.begin(), lSample1.begin(), lSample1.end());

    print<list<Sample>::iterator>(lSample1.begin(), lSample1.end());

    lSample1.sort();
    print<list<Sample>::iterator>(lSample1.begin(), lSample1.end());

    lSample2.sort(comp4);
    print<list<Sample>::iterator>(lSample2.begin(), lSample2.end());

    lSample3.sort(comp5);
    print<list<Sample>::iterator>(lSample3.begin(), lSample3.end());

    return 0;
}
