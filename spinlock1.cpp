#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//汇编实现
class SpinLock {
public:
    int lock;
public:
    SpinLock () {
        lock = 0;
        //Lock();
    }
    ~SpinLock () {
        //Unlock();
    }
    void Lock(void) {
        int tmp;
        __asm__ __volatile__(
            "L1:;"
            "cmp $1, %0;"
            "je L1;"
            "L2:;"
            "movl $1, %1;"
            "xchg %1, %0;"
            "cmp $1, %1;"
            "je L1;"
            : "+m" (lock), "+r" (tmp)
            );
    }
    void Unlock(void) {
        int tmp;
        __asm__ __volatile__(
            "movl $0, %1;"
            "xchg %1, %0;"
            : "+m" (lock), "+r" (tmp)
            );
    }
};

void *thread_run(void *arg) {
    SpinLock *pl = (SpinLock*)arg;
    printf("Try Lock 2\n");
    pl->Lock();
    printf("Lock 2\n");
    pl->Unlock();
    printf("Unlock 2\n");
    return NULL;
}

int main() {
    SpinLock l;
    pthread_t th;
    int ret;

    printf("Try Lock 1\n");
    l.Lock();
    printf("Lock 1\n");

    ret = pthread_create(&th, NULL, thread_run, &l);
    if (ret) {
        printf("Error1:%d\n", ret);
        exit(0);
    }

    sleep(5);

    l.Unlock();
    printf("Unlock 1\n");

    ret = pthread_join(th, NULL);
    if (ret) {
        printf("Error2:%d\n", ret);
        exit(0);
    }

    exit(0);
}
