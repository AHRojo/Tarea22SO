#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

FILE *fp;

int main(){
   // sem_t *mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   // sem_t *lectores = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //sem_init(mutex, 0, 1);
   // sem_init(lectores, 0, 1);
    sem_t *mutex = sem_open("pmutex", O_CREAT | O_EXCL, 0644, 1);
    sem_unlink("pmutex");

    sem_t *lectores = sem_open("plectores", O_CREAT | O_EXCL, 0644, 1);
    sem_unlink("plectores");

    char text[200];
    int lector = fork();
    int i = 0;

    if(lector == 0){ //Lector
        while (i < 3){
            sem_wait(lectores);
            sem_wait(mutex);
            //zona critica
            printf("Lectores Leyendo\n");
            fp = fopen("texto.txt", "r");
            while((fgets(text, sizeof(text), fp) != NULL)){
                printf("%s", text);
            }
            fclose(fp);
            printf("\n");
            sem_post(mutex);
            sem_post(lectores);
            i++;
        }
    }

    else{   //Escritor
        while(i < 3){
            sem_wait(mutex);
            sem_wait(lectores);
            if(i == 0){
                printf("Escritor Escribe\n");
            }
            fp = fopen("texto.txt", "a");
            printf("Poema: ");
            fgets(text, sizeof(text), stdin);
            fputs(text, fp);
            fclose(fp);
            sem_post(lectores);
            sem_post(mutex);
            i++;
        }
    }
    if(lector != 0){
        wait(NULL);
    }

    return 0;
}