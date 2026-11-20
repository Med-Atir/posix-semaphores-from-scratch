#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "p1.h"
#include "p2.h"
#include "semaphore.h"


int main() {
    int i;
    // File file;
    // tab tableau;
    // Buffer buffer = &tableau;
    File file = mmap(NULL, sizeof(Cellule), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    Buffer buffer = mmap(NULL, sizeof(tab), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    Semaphore s = mmap(NULL, sizeof(sys), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (file == MAP_FAILED || buffer == MAP_FAILED || s == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    file->last = -1;
    buffer->cmp = 0;
    s->plein =1;
    s->vide =0;
    for (i=0;i<MAX;i++)
        buffer->element[i] = -1;
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("FORK: ");//perror give the exact error; ici je verifie si vraiment on a creer le fils
    }
    else if (pid1 == 0) {
        production(buffer,file,s);
        fprintf(stdout,"The child has been created susccesfuly now it will alterne with his bro\n");
        exit(EXIT_SUCCESS);
    }
    else {
        pid_t pid2 = fork();
        if (pid2 < 0) {
            perror("FORK: ");//perror give the exact error; ici je verifie si vraiment on a creer le fils
        }
        else if (pid2 == 0) {
            consommation(buffer,file,s);
            fprintf(stdout,"The child has been created susccesfuly now it will alterne with his bro\n");

            exit(EXIT_SUCCESS);
        }
        else {
            fprintf(stdout,"My sons are working......\n");
            wait(NULL);
            wait(NULL);
        }
    }
    munmap(file, sizeof(Cellule));
    munmap(buffer, sizeof(tab));
    munmap(s, sizeof(sys));
    return 0;
}