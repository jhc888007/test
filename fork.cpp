#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> //fork() sleep()
#include <stdlib.h> //exit()

int main() {
    pid_t pchild1, pchild2;

    pchild1 = fork();

    if (pchild1 < 0) {
        std::cout << "Fork error" << std::endl;
        exit(0);
    }
    
    if (pchild1 == 0) {
        std::cout << "Child1 begin" << std::endl;
        sleep(3);
        std::cout << "Child1 end" << std::endl;
        exit(0);
    } 

    pchild2 = fork();

    if (pchild2 < 0) {
        std::cout << "Fork error" << std::endl;
        exit(0);
    }

    if (pchild2 == 0) {
        std::cout << "Child2 begin" << std::endl;
        sleep(6);
        std::cout << "Child2 end" << std::endl;
        exit(0);
    }

    std::cout << "Child1: " << pchild1 << " Child2: " << pchild2 << std::endl;

    /*
    pid_t pret;
    int status;
    pret = wait(&status);  //阻塞等待一个子进程，另一个子进程在主进程结束之前成为僵尸进程
    std::cout << "Child: " << pret << std::endl;
    */

    /*
    pid_t pret = 0;
    int status;
    while (pret != -1) {
        pret = wait(&status);  //如果没有其他子进程则返回-1
        std::cout << "Child: " << pret << std::endl;
    }
    */

    /*
    void handler(int signo);
    signal(SIGCHLD, handler);
    */

    pid_t pret;
    int status;
    int count = 2;
    while (pret!=-1) { //count) { //可以判断count或返回值为-1
        pret = waitpid(-1, &status, WNOHANG);
        if (pret == 0) {
            sleep(1);
            continue;
        } else if (pret == pchild1)
            count--;
        else if (pret == pchild2)
            count--;
        else
            std::cout << "Unknown status: " << pret << std::endl;
    }

    sleep(20);
    std::cout << "Father end" << std::endl;
    exit(0);
}

void handler(int signo) {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) ; //此处必须有循环，因为SIGNAL不排队，可能同时有多个SIGNAL
}
