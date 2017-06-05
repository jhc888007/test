#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <error.h>

#define SIZE 1024

int main() {
    int shmid;
    char *shmaddr;
    struct shmid_ds buf;

    int flag = 0;
    int pid;

    shmid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | 0600);
    if (shmid < 0) {
        printf("Get shmid error\n");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        printf("Get fork error\n");
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    if (pid == 0) {
        printf("Child begin\n");

        shmaddr = (char *)shmat(shmid, NULL, 0);
        if (shmaddr == (void *)(-1)) {
            printf("Child mat addr error\n");
            exit(1);
        }
        int count = 10;
        while(count--) {
            memcpy(shmaddr, &count, sizeof(int));
            sleep(5);
        }
        shmdt(shmaddr);
        return 0;
    }

    sleep(1);
    printf("Father begin\n");

    /*flag = shmctl(shmid, IPC_STAT, &buf);
    if (flag == -1) {
        printf("Get ctl error\n");
        exit(1);
    }*/

    shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (void *)(-1)) {
        printf("Father mat addr error\n");
        exit(1);
    }

    int count = 10;
    while(count--) {
        int tmp = 0;
        memcpy(&tmp, shmaddr,  sizeof(int));
        printf("recv: %d\n", tmp);
        sleep(5);
    }
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
