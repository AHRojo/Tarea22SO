#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include  <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <semaphore.h>
#include <pthread.h>

int main(){
    sem_t *mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *escritor = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *en_espera = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(mutex, 0, 1);
    sem_init(escritor, 0, 1);
    int writer = fork();
    int i = 0;
    if(writer == 0){
        while (i < 3){
            printf("HIJO\n");
            sem_wait(escritor);
            if ((*en_espera) == 0){
                sem_wait(mutex);
                getchar();
                printf("HIJO QLIAO\n");
                sem_post(mutex);
            }
            sem_post(escritor);
            i++;
        }
    }
    else{
        while (i < 3){
            sem_wait(escritor);
            (*en_espera)++;
            sem_wait(mutex);
            getchar();
            printf("SOY EL PADRE\n");
            sem_post(mutex);
            sem_post(escritor);
            i++;
        }
    }

    munmap(mutex, sizeof(sem_t));
    munmap(escritor, sizeof(sem_t));
    munmap(en_espera, sizeof(int));
    return 0;
}
