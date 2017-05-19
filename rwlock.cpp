#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class RWLockv {
public:
    virtual void RLock(int i) {cout << "RWLockv.RLock" << endl;}
    virtual void RUnlock(int i) {cout << "RWLockv.RUnlock" << endl;}
    virtual void WLock(int i) {cout << "RWLockv.WLock" << endl;}
    virtual void WUnlock(int i) {cout << "RWLockv.WUnlock" << endl;}
};

class MLock {
protected:
    pthread_mutex_t l;
public:
    MLock() { pthread_mutex_init(&l, NULL); }
    inline void Lock() {
        int ret = pthread_mutex_lock(&l);
        if (ret != 0)
            cout << "MLock.Lock error " << ret << endl;
    }
    inline void Unlock() {
        int ret = pthread_mutex_unlock(&l);
        if (ret != 0)
            cout << "MLock.Unlock error " << ret << endl;
    }
};

class MCond: public MLock {
    pthread_cond_t c;
public:
    MCond() { pthread_mutex_init(&l, NULL); pthread_cond_init(&c, NULL); }
    inline void Wait() {
        int ret = pthread_cond_wait(&c, &l);
        if (ret != 0)
            cout << "MCond.Wait error " << ret << endl;
    }
    inline void Broadcast() {
        int ret = pthread_cond_broadcast(&c);
        if (ret != 0)
            cout << "MCond.Broadcast error " << ret << endl;
    }
};

//使用互斥锁实现读写锁例一
//读过程相互不互斥，写过程相互互斥，读写进程相互互斥
//可以把所有读过程看作是连续的一个过程，此过程与写过程等同
//readerl是变量锁，用来修改变量
//wl是过程锁，用来锁住过程（或为写过程，或为连续的一系列读过程）
//由于读过程不互斥，如果读过程一直来，那么读过程一直连续，则写过程无法触发
class RWLock1: public RWLockv {
    MLock readerl;
    MLock wl;
    int readers;
public:
    RWLock1() { readers = 0; }
    void RLock(int i) {
        readerl.Lock();
        if (readers == 0)
            wl.Lock();
        readers++;
        readerl.Unlock();
        cout << "Lock1.RLock " << i  << endl;
    }
    void RUnlock(int i) {
        readerl.Lock();
        readers--;
        if (readers == 0)
            wl.Unlock();
        readerl.Unlock();
        cout << "Lock1.RUnlock " << i << endl;
    }
    void WLock(int i) {
        wl.Lock();
        cout << "Lock1.WLock " << i  << endl;
    }
    void WUnlock(int i) {
        wl.Unlock();
        cout << "Lock1.WUnlock " << i  << endl;
    }
};

//使用互斥锁实现读写锁例二
//读过程相互不互斥，写过程相互互斥，读写进程相互互斥
//自然想到增加一个变量writers和相应的变量锁，来记录当前等待的写过程
//读过程发起时查看此变量，如果大于0则while循环
//从读优先变成了写优先，只要有写过程读过程就不能触发
//但有形成了类似自旋锁的运行方式，循环判断writers
//如果在循环中加入过程锁wl.Lock()，则读过程在写过程或其他读过程完成后才能继续
//因为读过程分不清现在是读过程还是写过程在占用wl，这样成了单线程运行
class RWLock2: public RWLockv {
    MLock readerl;
    MLock wl;
    MLock writerl;
    int readers;
    int writers;
public:
    RWLock2() {readers = 0; writers = 0;}
    void RLock(int i) {
        readerl.Lock();
        writerl.Lock();
        cout << "RWLock2.RLock " << i << " writers " << writers << endl;
        while (writers > 0) {
            writerl.Unlock();
            readerl.Unlock();
            readerl.Lock();
            writerl.Lock();
        }
        writerl.Unlock();
        if (readers == 0)
            wl.Lock();
        readers++;
        readerl.Unlock();
        cout << "RWLock2.RLock " << i << " readers " << readers << endl;
    }
    void RUnlock(int i) {
        readerl.Lock();
        readers--;
        if (readers == 0)
            wl.Unlock();
        readerl.Unlock();
        cout << "RWLock2.RUnlock " << i << endl;
    }
    void WLock(int i) {
        writerl.Lock();
        writers++;
        cout << "RWLock2.WLock " << i << " writers " << writers << endl;
        writerl.Unlock();
        wl.Lock();
        writerl.Lock();
        writers--;
        writerl.Unlock();
        cout << "RWLock2.WLock " << i  << endl;
    }
    void WUnlock(int i) {
        wl.Unlock();
        cout << "RWLock2.WUnlock " << i  << endl;
    }
};

//使用互斥锁实现读写锁例三
//读过程相互不互斥，写过程相互互斥，读写进程相互互斥
//自然又想到需要增加一个锁来分别标记读过程和写过程
//这样可以阻塞住读过程试图反复读取writers
//还是写优先的，因为只要有写过程在等待writers就一直不为0
class RWLock3: public RWLockv {
    MLock rl;
    MLock readerl;
    MLock wl;
    MLock writerl;
    int readers;
    int writers;
public:
    RWLock3() {readers = 0; writers = 0;}
    void RLock(int i) {
        readerl.Lock();
        writerl.Lock();
        cout << "RWLock3.RLock " << i << " writers " << writers << endl;
        while (writers > 0) {
            writerl.Unlock();
            readerl.Unlock();
            wl.Lock();
            wl.Unlock();
            readerl.Lock();
            writerl.Lock();
        }
        writerl.Unlock();
        if (readers == 0)
            rl.Lock();
        readers++;
        readerl.Unlock();
        cout << "RWLock3.RLock " << i << " readers " << readers << endl;
    }
    void RUnlock(int i) {
        readerl.Lock();
        readers--;
        if (readers == 0)
            rl.Unlock();
        readerl.Unlock();
        cout << "RWLock3.RUnlock " << i << endl;
    }
    void WLock(int i) {
        writerl.Lock();
        writers++;
        cout << "RWLock3.WLock " << i << " writers " << writers << endl;
        writerl.Unlock();
        rl.Lock();
        wl.Lock();
        rl.Unlock();
        writerl.Lock();
        writers--;
        writerl.Unlock();
        cout << "RWLock3.WLock " << i  << endl;
    }
    void WUnlock(int i) {
        wl.Unlock();
        cout << "RWLock3.WUnlock " << i  << endl;
    }
};

//使用互斥锁实现读写锁例四
//读过程相互不互斥，写过程相互互斥，读写进程相互互斥
//也可以使用条件变量，用readers记录当前读过程数量，用writers记录当前写过程数量
//用条件变量和其中的锁来保护记录变量
//如果用waiters记录当前等待的写过程数据，则可以实现写优先
//如果不用waiters，则只能实现读优先
class RWLock4: public RWLockv {
    MCond c; //include a mutex lock and a cond
    int readers;
    int writers;
    int waiters;
public:
    RWLock4() {readers = 0; writers = 0; waiters = 0;}
    void RLock(int i) {
        c.Lock();
        while (writers > 0 || waiters > 0)
            c.Wait();
        readers++;
        c.Unlock();
        cout << "RWLock4.RLock " << i << endl;
    }
    void RUnlock(int i) {
        c.Lock();
        readers--;
        if (readers == 0)
            c.Broadcast();
        cout << "RWLock4.RUnlock " << i << endl;
        c.Unlock();
    }
    void WLock(int i) {
        c.Lock();
        waiters++;
        while (writers > 0 || readers > 0) {
            cout << "RWlock4.WLock " << i << " writers:" << writers << " readers:" << readers << endl;
            c.Wait();
        }
        waiters--;
        writers++;
        cout << "RWlock4.WLock " << i << endl;
    }
    void WUnlock(int i) {
        writers--;
        cout << "RWlock4.WUnlock " << i << endl;
        c.Broadcast();
        c.Unlock();
    }
};

RWLock1 rwlock1;
RWLock2 rwlock2;
RWLock3 rwlock3;
RWLock4 rwlock4;
RWLockv *v = &rwlock4;

void *read_thread(void *args) {
    int i = *(int *)args;
    sleep(i);
    v->RLock(i);
    sleep(8);
    v->RUnlock(i);
}

void *write_thread(void *args) {
    int i = *(int *)args;
    sleep(i);
    v->WLock(i);
    sleep(3);
    v->WUnlock(i);
}

int main(int argc, char *argv[]) {
    pthread_t p[15];
    int read_mark[10] = {0,1,2,3,4,5,6,7,8,9};
    int write_mark[5] = {0,1,2,3,4};

    for (int i = 0; i < 5; i++) {
        pthread_create(&p[i], NULL, read_thread, &read_mark[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_create(&p[i+5], NULL, write_thread, &write_mark[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_create(&p[i+10], NULL, read_thread, &read_mark[i+5]);
    }

    for (int i = 0; i < 15; i++) {
        pthread_join(p[i], NULL);
    }
    exit(0);
}


