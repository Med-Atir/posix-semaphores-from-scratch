#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "p1.h"
#include "p2.h"
#include "semaphore.h"


int main() {
    int i;
    pthread_t t1,t2;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGUSR1);
    Semphore s = (etat*)malloc(sizeof(etat));
    File file = (File)malloc(sizeof(Maillon));
    Buffer buffer = (Buffer)malloc(sizeof(tab));
    pthread_sigmask(SIG_BLOCK,&set,NULL);
    if (s != NULL && file != NULL && buffer != NULL) {
        s->plein = 0;
        s->vide = 1;
        file->first = NULL;
        file->last = NULL;
        buffer->cmp = 0;
        for (i=0;i<MAX;i++)
            buffer->element[i] = -1;
    }
    else {
        fprintf(stderr, "[Error]: memory is plein dude, go ask Sam to lend you some ram\n");
        return -1;
    }
    ThreadArgs args = { buffer, file,s, &set };
    int th1 = pthread_create(&t1, NULL, production, &args);
    int th2 = pthread_create(&t2, NULL, consommation, &args);
    if (th1 == 0 && th2 == 0) {
        fprintf(stdout,"The childs has been created susccesfuly now they will alternes with each other\n");
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        fprintf(stdout,"They are working....");
    }
    else {
        fprintf(stderr,"Something happened and couldnt create them, I am not sorry....\n");
    }
    return 0;
}