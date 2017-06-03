#include <set>
#include <iostream>
#include <algorithm>

using namespace std;

#define DUMP(m) cout<<#m<<"'s element: "; for_each(m.cbegin(),m.cend(),[](int v)->void{cout<<" "<<v;}); cout<<endl

int main() {
    multiset<int> m1 = {1,1,2,3,4,4,5,5,5,6};
    DUMP(m1);
    for (auto it = m1.begin(); it != m1.end(); it++) {
        if (*it == 4) {
            m1.erase(it);
            break;
        }
    }
    DUMP(m1);
    for (auto it = m1.begin(); it != m1.end(); it++) {
        if (*it == 4) {
            m1.erase(it);
            break;
        }
    }
    DUMP(m1);
    for (auto it = m1.begin(); it != m1.end(); it++) {
        if (*it == 5) {
            m1.erase(*it);
            break;
        }
    }
    DUMP(m1);

    return 0;
}
