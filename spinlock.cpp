#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int share_mem[100];
pthread_spinlock_t lock;

void *thread_run(void *arg) {
    printf("Try Lock2\n");
    int ret = pthread_spin_lock(&lock);
    printf("Lock2:%d\n", ret);
    ret = pthread_spin_unlock(&lock);
    printf("Unlock2:%d\n", ret);
    return NULL;
}

int main(void) {
    pthread_t th;
    int ret;

    ret = pthread_spin_init(&lock, PTHREAD_PROCESS_SHARED);
    if (ret) {
        printf("Error2:%d\n", ret);
        exit(0);
    }

    printf("Try Lock1\n");
    ret = pthread_spin_lock(&lock);
    printf("Lock1:%d\n", ret);

    ret = pthread_create(&th, NULL, thread_run, NULL);
    if (ret) {
        printf("Error1:%d\n", ret);
        exit(0);
    }

    sleep(5);

    ret = pthread_spin_unlock(&lock);
    printf("Unlock1:%d\n", ret);

    pthread_join(th, NULL);
    pthread_spin_destroy(&lock);

    exit(0);
}

