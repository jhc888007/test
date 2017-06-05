#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main() {
    sem_t *sem;

    sem = (sem_t*)mmap(0, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0); //匿名信号量

    if (sem == (void*)-1) {
        exit(0);
    }

    /*
    sem_t _sem;
    sem = &_sem; //sem不在共享内存中，不能够正确生效
    */

    int ret;

    ret = sem_init(sem, 1, 1);

    pid_t pid;

    pid = fork();

    if (pid < 0) {
        printf("Fork error\n");
        exit(0);
    }

    if (pid == 0) {
        sem_wait(sem);
        printf("Child begin\n");
        sleep(5);
        sem_post(sem);
        printf("Child end\n");
        
        ret = sem_destroy(sem);
        printf("Sem destroy: %d\n", ret);

        exit(0);
    }

    sem_wait(sem);
    printf("Father begin\n");
    sleep(5);
    sem_post(sem);
    printf("Father end\n");

    sleep(1);
    printf("Another test prepare\n");
    sem_wait(sem);
    printf("Another test begin\n");
    sleep(3);
    sem_post(sem);
    printf("Another test end\n");

    wait(NULL);

    ret = sem_destroy(sem);
    printf("Sem destroy: %d\n", ret);

    return 0;
}

