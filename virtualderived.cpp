#include <iostream>

using namespace std;

#define X64
#ifdef X64
typedef unsigned long u_ptr;
#else
typedef unsigned int u_ptr;
#endif

typedef void (*pfunc)(void);

class A_father {
public:
    u_ptr v1;
    void f1(void) { cout << "A_father::f1()" << endl; }
};

class A_child: public A_father {
public:
    u_ptr v2;
    u_ptr v3;
    void f1(void) { cout << "A_child::f1()" << endl; }
    void f2(void) { cout << "A_child::f2()" << endl; }
};

void testA() {
    A_father a1;
    A_child a2;
    cout << "a1: " << sizeof(a1) << " a2: " << sizeof(a2) << endl; //大小即为所有变量大小总和

    a1.v1 = 11;
    a2.v1 = 21;
    a2.v2 = 22;

    //向下转换
    //A_child *pa3 = &a1; //编译不过，必须有虚函数
    //A_child *pa3 = dynamic_cast<A_child*>(&a1); //编译不过，必须有虚函数
    
    //向上转换
    A_father *pa4 = &a2; //没有多态
    cout << "a2.v1: " << a2.v1 << " pa4->v1: " << pa4->v1 << endl;
    cout << "a2.f1(): ";
    a2.f1();
    cout << "pa4->f1(): ";
    pa4->f1();

    return ;
}

class B_father {
public:
    u_ptr v1;
    u_ptr v2;
    void f1() { cout << "B_father::f1()" << endl; }
};

class B_child: public B_father {
public:
    u_ptr v1; //重复变量
    u_ptr v3;
    void f1() { cout << "B_child::f1()" << endl; }
};

void testB() {
    B_father b1;
    B_child b2;
    cout << "sizeof(b1): " << sizeof(b1) << " sizeof(b2): " << sizeof(b2) << endl; //包含重复变量

    //向上转换
    B_father *pb3 = &b2;
    b2.B_father::v1 = 11; //重复变量访问方法
    b2.v1 = 21;
    b2.v2 = 22;
    b2.v3 = 23;
    cout << "pb3->v1: " << pb3->v1 << " b2.v1: " << b2.v1 << " b2.B_father::v1: " << b2.B_father::v1 << endl;

    //内存布局
    cout << "&pb3->v1: " << &pb3->v1 << " &pb3->v2: " << &pb3->v2 << endl;
    cout << "&b2.B_father::v1: " << &b2.B_father::v1 << " &b2.v2: " << &b2.v2 << " &b2.v1: " << &b2.v1 << " &b2.v3: " << &b2.v3 << endl;

    return ;
}

class C_father {
public:
    u_ptr v1;
    virtual void f1() { cout << "C_father::f1()" << endl; }
};

class C_child: public C_father {
public:
    u_ptr v2;
    void f1() { cout << "C_child::f1()" << endl; }
};

void testC() {
    C_father c1;
    C_child c2;
    cout << "sizeof(c1): " << sizeof(c1) << " sizeof(c2): " << sizeof(c2) << endl;

    //向上转换
    c1.v1 = 11;
    c2.v1 = 21;
    c2.v2 = 22;
    C_father *pc3 = &c2;
    cout << "c2.f1(): ";
    c2.f1();
    cout << "pc3->f1(): ";
    pc3->f1(); //多态的体现

    //内存布局
    u_ptr **val = (u_ptr**)&c1;
    pfunc pf1 = (pfunc)(**val); //父类虚函数表第一位
    cout << "father: pf1: 0x" << hex << (u_ptr)pf1 << endl;
    cout << "father: pf1(): ";
    pf1();
    pfunc pf2 = (pfunc)(*((*val)+1)); //父类虚函数表第二位，后面这几位不知道是些什么东西
    cout << "father: pf2: 0x" << hex << (u_ptr)pf2 << endl;
    cout << "father: *pf2: 0x" << hex << *(u_ptr*)pf2 << endl;
    pfunc pf3 = (pfunc)(*((*val)+2)); //父类虚函数表第三位
    cout << "father: pf3: 0x" << hex << (u_ptr)pf3 << endl;
    pfunc pf4 = (pfunc)(*((*val)+3)); //父类虚函数表第四位
    cout << "father: pf4: 0x" << hex << (u_ptr)pf4 << endl;
    
    val = (u_ptr**)&c2;
    pf1 = (pfunc)(**val); //子类虚函数表第一位
    cout << "child: pf1: 0x" << hex << (u_ptr)pf1 << endl;
    cout << "child: pf1(): ";
    pf1();
    pf2 = (pfunc)(*((*val)+1)); //子类虚函数表第二位，为0
    cout << "child: pf2: 0x" << hex << (u_ptr)pf2 << endl;

    cout << "val: " << hex << (u_ptr)val << " &c2.v1: " << &c2.v1 << " &c2.v2: " << &c2.v2 << endl;
    cout << "*val: " << dec << *(u_ptr *)val << " *(val+1): " << *(u_ptr *)(val+1) 
        << " *(val+2): " << *(u_ptr *)(val+2) << endl;

    //向下转换
    //C_child *pc4 = &c1; //编译不通过
    
    //C_child *pc5 = dynamic_cast<C_child*>(&c1); //编译告警，并且不能真正使用指针
    //cout << "pc5->v1: " << pc5->v1 << endl; //如果使用，则崩溃

    //C_father *pc6 = &c1;
    //C_child *pc7 = dynamic_cast<C_child*>(pc6); //编译不会告警，但还是不能真正使用指针
    //cout << "pc7->v1: " << pc7->v1 << endl; //如果使用，则崩溃
    
    C_father *pc8 = &c2;
    C_child *pc9 = dynamic_cast<C_child*>(pc8);
    cout << "pc9->v1: " << pc9->v1 << " pc9->v2: " << pc9->v2 << endl; //实体是子类型，调用才不会发生崩溃
    cout << "pc9->f1(): ";
    pc9->f1();
}

class D_father {
public:
    u_ptr v1;
    virtual void f1() { cout << "D_father::f1()" << endl; }
    virtual void f2() { cout << "D_father::f2()" << endl; }
    void f3() { cout << "D_father::f3()" << endl; }
};

class D_child: public D_father {
public:
    u_ptr v2;
    virtual void f1() { cout << "D_child::f1()" << endl; }
    void f2() { cout << "D_child::f2()" << endl; }
    virtual void f3() { cout << "D_child::f3()" << endl; }
};

class D_grand: public D_child {
public:
    u_ptr v3;
    void f1() { cout << "D_grand::f1()" << endl; }
    void f2() { cout << "D_grand::f2()" << endl; }
    void f3() { cout << "D_grand::f3()" << endl; }
};

void testD() {
    D_father d1;
    D_child d2;
    D_grand d3;

    d1.v1 = 11;
    d2.v1 = 21;
    d2.v2 = 22;
    d3.v1 = 31;
    d3.v2 = 32;
    d3.v3 = 33;

    cout << "sizeof(d1): " << sizeof(d1) << " sizeof(d2): " << sizeof(d2) << " sizeof(d3): " << sizeof(d3) << endl;

    //向上转换
    D_father *pd4 = &d3;
    D_child *pd5 = &d3;
    cout << "pd4->f1()";
    pd4->f1();
    cout << "pd4->f2()";
    pd4->f2();
    cout << "pd4->f3()";
    pd4->f3();
    cout << "pd5->f1()";
    pd5->f1();
    cout << "pd5->f2()";
    pd5->f2();
    cout << "pd5->f3()";
    pd5->f3();
}

class E_father {
public:
    u_ptr v1;
    virtual void f1() { cout << "E_father::f1()" << endl; }
};

class E_child1: public E_father {
public:
    u_ptr v2;
    void f1() { cout << "E_child1::f1()" << endl; }
};

class E_child2: public E_father {
public:
    u_ptr v3;
    void f1() { cout << "E_child2::f1()" << endl; }
};

//class E_grand: public E_child1, E_child2 { //这样E_child2成了私有继承
class E_grand: public E_child1, public E_child2 {
public:
    u_ptr v4;
    void f1() { cout << "E_grand::f1()" << endl; }
};

void testE() {
    E_father e1;
    E_child1 e2;
    E_child2 e3;
    E_grand e4;

    e1.v1 = 11;
    e2.v1 = 21;
    e2.v2 = 22;
    e3.v1 = 31;
    e3.v3 = 33;
    //e4.v1 = 41; //编译不通过，歧义
    //e4.E_father::v1 = 41; //编译不通过，歧义
    e4.E_child1::v1 = 41;
    e4.E_child2::v1 = 411;
    e4.v2 = 42;
    e4.v3 = 43;
    e4.v4 = 44;

    cout << "sizeof(E_father): " << sizeof(E_father) << " sizeof(E_child1): " << sizeof(E_child1)
        << " sizeof(E_child2): " << sizeof(E_child2) << " sizeof(E_grand): " << sizeof(E_grand) << endl;

    //E_father *pe5 = &e4; //编译不通过，歧义
    E_father *pe5 = (E_child1 *)(&e4);
    E_father *pe6 = (E_child2 *)(&e4);
    E_child1 *pe7 = &e4;
    E_child2 *pe8 = &e4;
    cout << "&e4: " << &e4 << " pe5: " << pe5 << " pe6: " << pe6 << " pe7: " << pe7
        << " pe8: " << pe8 << endl;
    /* E_grand布局：
     * 虚函数表一
     * E_child1::v1
     * v2
     * 虚函数表二
     * E_child2::v1
     * v3
     * v4
     */

    cout << "pe5->f1(): ";
    pe5->f1();
    cout << "pe6->f1(): ";
    pe6->f1();
    cout << "pe7->f1(): ";
    pe7->f1();
    cout << "pe8->f1(): ";
    pe8->f1();
}

class F_father {
public:
    u_ptr v1;
    virtual void f1() { cout << "F_father::f1()" << endl; }
};

class F_child1: virtual public F_father {
public:
    u_ptr v2;
    void f1() { cout << "F_child1::f1()" << endl; }
};

class F_child2: virtual public F_father {
public:
    u_ptr v3;
    void f1() { cout << "F_child2::f1()" << endl; }
};

class F_grand: public F_child1, public F_child2 {
public:
    u_ptr v4;
    void f1() { cout << "F_grand::f1()" << endl; }
};

void testF() {
    F_father f1;
    F_child1 f2;
    F_child2 f3;
    F_grand f4;

    cout << "sizeof(f1): " << sizeof(f1) << " sizeof(f2): " << sizeof(f2) << " sizeof(f3): " << sizeof(f3)
        << " sizeof(f4): " << sizeof(f4) << endl;

    f1.v1 = 11;
    f2.v1 = 21;
    f2.v2 = 22;
    f3.v1 = 31;
    f3.v3 = 33;
    f4.v1 = 41;
    f4.v2 = 42;
    f4.v3 = 43;
    f4.v4 = 44;

    cout << "&f4.v1: " << &f4.v1 << " &f4.v2: " << &f4.v2 << " &f4.v3: " << &f4.v3 << " &f4.v4: " << &f4.v4 << endl;

    u_ptr **val = (u_ptr **)(&f4);
    cout << "*(u_ptr *)(val): 0x" << hex << *(u_ptr *)(val) 
        << " *(u_ptr *)(val+1): " << dec << *(u_ptr *)(val+1) 
        << " *(u_ptr *)(val+2): 0x" << *(u_ptr *)(val+2) 
        << " *(u_ptr *)(val+3): " << *(u_ptr *)(val+3) 
        << " *(u_ptr *)(val+4): " << *(u_ptr *)(val+4) 
        << " *(u_ptr *)(val+5): " << *(u_ptr *)(val+5) 
        << " *(u_ptr *)(val+6): " << *(u_ptr *)(val+6) 
        << endl;
    pfunc pf1 = (pfunc)(**val);
    cout << "pf1: 0x" << hex << (**val) << endl;
    cout << "pf1() ";
    pf1();
    pfunc pf2 = (pfunc)(**(val+2));
    cout << "pf2: 0x" << hex << (**(val+2)) << endl;
    cout << "pf2() ";
    pf2();
    /* pfunc *pf3 = (pfunc*)(**(val+5));  //这个指针不是函数指针，调用函数会崩溃
    cout << "pf3: 0x" << hex << (**(val+5)) << endl;
    cout << "pf3() ";
    pf3(); */
    /*cout << "&:0x" << hex << *val << " : " << dec << (**val) << endl;
    cout << "&:0x" << hex << *val+1 << " : " << dec << (*(*val+1)) << endl;
    cout << "&:0x" << hex << *val+2 << " : " << dec << (int)(*(*val+2)) << endl;
    cout << "&:0x" << hex << *val+3 << " : " << dec << (*(*val+3)) << endl;
    cout << "&:0x" << hex << *val+4 << " : " << dec << (*(*val+4)) << endl;
    cout << "&:0x" << hex << *val+5 << " : " << dec << (int)(*(*val+5)) << endl;
    cout << "&:0x" << hex << *val+6 << " : " << dec << (int)(*(*val+6)) << endl;
    cout << "&:0x" << hex << *val+7 << " : " << dec << (*(*val+7)) << endl;*/

    F_father *pf5 = &f4;
    F_child1 *pf6 = &f4;
    F_child2 *pf7 = &f4;
    cout << "&f4: " << &f4 << " pf5: " << pf5 << " pf6: " << pf6 << " pf7: " << pf7 << endl;
    pf6->f1();
    pf7->f1();
    pf5->f1();

    //向下转换
    F_grand *pf8 = dynamic_cast<F_grand *>(pf5);
    pf8->f1();
    F_child1 *pf9 = dynamic_cast<F_child1 *>(pf5);
    pf9->f1();
    F_child1 *pf10 = dynamic_cast<F_child1 *>(pf5);
    pf10->f1();
}

class G_father {
public:
    u_ptr v1;
};

class G_child1: virtual public G_father {
public:
    u_ptr v2;
};

class G_child2: virtual public G_father {  //虚继承会增加一个表，结构大小比变量总和增加一个指针的大小
public:
    u_ptr v3;
};

//class G_grand: virtual public G_child1, virtual public G_child2 { 
//如果添加virtual，则结构大小增加一个指针的大小，但是还是不能向下转换
class G_grand: public G_child1, public G_child2 {
public:
    u_ptr v4;
};

void testG() {
    G_father g1;
    G_child1 g2;
    G_child2 g3;
    G_grand g4;

    cout << dec << "sizeof(g1): " << sizeof(g1) << " sizeof(g2): " << sizeof(g2) << " sizeof(g3): " << sizeof(g3)
        << " sizeof(g4): " << sizeof(g4) << endl;

    g1.v1 = 11;
    g2.v1 = 21;
    g2.v2 = 22;
    g3.v1 = 31;
    g3.v3 = 33;
    g4.v1 = 41;
    g4.v2 = 42;
    g4.v3 = 43;
    g4.v4 = 44;

    u_ptr **val = (u_ptr **)(&g4);
    cout << "*(u_ptr *)(val): " << dec << *(u_ptr *)(val) 
        << " *(u_ptr *)(val+1): " << *(u_ptr *)(val+1) 
        << " *(u_ptr *)(val+2): " << *(u_ptr *)(val+2) 
        << " *(u_ptr *)(val+3): " << *(u_ptr *)(val+3) 
        << " *(u_ptr *)(val+4): " << *(u_ptr *)(val+4) 
        << " *(u_ptr *)(val+5): " << *(u_ptr *)(val+5) 
        << " *(u_ptr *)(val+6): " << *(u_ptr *)(val+6) 
        << endl;

    //向上转换
    G_father *pg5 = &g4;
    G_child1 *pg6 = &g4;
    G_child2 *pg7 = &g4;

    //向下转换
    //G_grand *pg8 = dynamic_cast<G_grand *>(pg5); //不能映射，因为基类没有虚表，没有偏移量记录
    //G_grand *pg9 = dynamic_cast<G_grand *>(pg6); //不能映射，因为基类没有虚表，没有偏移量记录
    //G_grand *pg10 = dynamic_cast<G_grand *>(pg7); //不能映射，因为基类没有虚表，没有偏移量记录

    //pg5 = &g3;
    //pg7 = dynamic_cast<G_child2 *>(pg5); //不能映射，虽然有虚表，但是貌似其中没有偏移量记录，只有加虚函数才可以实现向下转换

}

class H_father {
public:
    u_ptr v1;
    virtual void f1() { cout << "H_father::f1()" << endl; }
};

class H_child: public H_father {
public:
    u_ptr v2;
    void f1() { cout << "H_child::f1()" << endl; }
    virtual void f2() { cout << "H_child::f2()" << endl; }
};

class H_grand: public H_child {
public:
    u_ptr v3;
    void f1() { cout << "H_grand::f1()" << endl; }
    void f2() { cout << "H_grand::f2()" << endl; }
    virtual void f3() { cout << "H_child::f3()" << endl; }
};

void testH(void) {
    cout << "sizeof(H_father): " <<  sizeof(H_father) << " sizeof(H_child): " << sizeof(H_child) << " sizeof(H_grand): "
        << sizeof(H_grand) << endl;
}

class I_father {
public:
    u_ptr v1;
    virtual void f1() { cout << "I_father::f1()" << endl; }
};

class I_child: virtual public I_father {
public:
    u_ptr v2;
    void f1() { cout << "I_child::f1()" << endl; }
    virtual void f2() { cout << "I_child::f2()" << endl; }
};

class I_grand: virtual public I_child {
public:
    u_ptr v3;
    void f1() { cout << "I_grand::f1()" << endl; }
    void f2() { cout << "I_grand::f2()" << endl; }
    virtual void f3() { cout << "I_child::f3()" << endl; }
};

void testI(void) {
    cout << "sizeof(I_father): " <<  sizeof(I_father) << " sizeof(I_child): " << sizeof(I_child) << " sizeof(I_grand): "
        << sizeof(I_grand) << endl;
}

int main() {
    testA();
    cout << endl;
    testB();
    cout << endl;
    testC();
    cout << endl;
    testD();
    cout << endl;
    testE();
    cout << endl;
    testF();
    cout << endl;
    testG();
    cout << endl;
    testH();
    cout << endl;
    testI();
    cout << endl;
    return 0;
}
