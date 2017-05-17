#include <iostream>
#include <set>

struct int_cmp {
    bool operator() (const int &a, const int &b) const {
        return (a&(~1)) < (b&(~1));
    }
};

struct int_ {
    int a;
    int b;
    int_(int i, int j) {a = i; b = j;}
    bool operator< (const struct int_ &i) const {
        return b < i.b;
    }
};

int main(void) {
    std::set<int, int_cmp> set1;
    std::set<int_> set2;

    set1.insert(0);
    set1.insert(1);
    set1.insert(2);
    set1.insert(3);

    for (std::set<int, int_cmp>::iterator it = set1.begin(); it != set1.end(); it++) {
        std::cout << *it << std::endl;
    }
    //output 0, 2

    set2.insert(int_(0, 1));
    set2.insert(int_(1, 1));
    set2.insert(int_(2, 1));
    set2.insert(int_(2, 2));

    for (std::set<int_>::iterator it = set2.begin(); it != set2.end(); it++) {
        std::cout << it->a << "," << it->b << std::endl;
    }
    //output 0,1 2,2


    return 0;
}


