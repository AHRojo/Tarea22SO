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

int main(){
    //sem_t *mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *lectores = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //char yanine[500];
   // int en_espera = 0;
    //sem_init(mutex, 0, 1);
    //sem_init(lectores, 0, 1);
    lectores = sem_open("plectores", O_CREAT | O_EXCL, 0644, -1);
    sem_unlink("plectores");
    int writer = fork();
    int i = 0;
    if(writer == 0){ //LECTORES
        printf("perritos\n");
       // FILE *fp;
        while(i < 3){
            sem_wait(lectores);
            printf("perros\n");
            //fp = fopen("texto.txt", "r");
            //while(fgets(yanine, sizeof(yanine), fp) != NULL){
               // printf("%s", yanine);
          // }
           // fclose(fp);
            sem_post(lectores);
            i++;
        }
    }
    else{   //ESCRITOR
       // FILE *fp;
        while(i < 3){
            //sem_wait(mutex);
           // en_espera++;
           // if(en_espera > 0){
            sem_wait(lectores);
            //}
            //fp = fopen("texto.txt", "a");
            //fgets(yanine, sizeof(yanine), stdin);
            //fputs(yanine, fp);
            //fclose(fp);
            printf("escribi\n");
            //en_espera--;
           // if(en_espera == 0){
            sem_post(lectores);
         //   }
           // sem_post(mutex);
            i++;
        }
    }
    if(writer != 0){
        wait(NULL);
    }
 //   munmap(mutex, sizeof(sem_t));
    munmap(lectores, sizeof(sem_t));
    return 0;
}