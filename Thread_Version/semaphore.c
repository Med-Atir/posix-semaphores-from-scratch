#include "semaphore.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int emfiler(pthread_t pid,File f) {
    Cellule* p = (Cellule*)malloc(sizeof(Cellule));
    if (p == NULL)
        return 0;
    p->pid = pid;
    p->suivant = NULL;
    if (est_vide(f)) {
        f->first = p;
        f->last = p;
    }
    else {
        f->last->suivant = p;
        f->last = p;
    }
    return 1;
}
int defiler(File f) {
    if (!est_vide(f)) {
        Cellule* p = f->first;
        f->first = f->first->suivant;
        free(p);
        if (f->first == NULL)
            f->last = NULL;
        return 1;
    }
    return 0;
}
int est_vide(File f) {
    return f->first == NULL;
}
int tab_vide(Buffer tab) {
    return tab->cmp <= 0;
}
pthread_t get_first(File f) {
    if (est_vide(f))
        return 0;
    return f->first->pid;
}
int tab_plein(Buffer tab) {
    return tab->cmp >= MAX;
}
void V(int* s,File f) {
    (*s)++;
    if (*s <= 0) {
        pthread_t p = get_first(f);
        if (defiler(f)) {
            if (pthread_kill(p, SIGUSR1) !=0) {
                fprintf(stderr, "[Error] pid %lu: Cant pause processus\n", p);// pause retourne -1 en cas error and 0 in succed alors je handle error
            }
            else {
                fprintf(stdout, "[Succes] pid %lu: is deblocked\n",p);
            }
        }
        else {
            fprintf(stderr, "[Error]: no process are blocked\n");
        }
    }
}
void P(int* s,File f,sigset_t* sig) {
    (*s)--;
    int set;
    if (*s < 0) {
        pthread_t p = pthread_self();
        if (emfiler(p,f)) {
            if (sigwait(sig,&set) != 0) {
                fprintf(stderr, "[Error] pid %lu: Cant pause processus\n", p);// pause retourne -1 en cas error and 0 in succed alors je handle error
            }
            else {
                fprintf(stdout, "[Succes] pid %lu: is blocked(actually by now it has already woke up haha but imagine we still in 1960's (Dijkstra time)\n",p);
            }
        }
        else {
            fprintf(stderr, "[Error] pid %lu: file is plein dude, go buy ram(you cant in todays economie)\n", p);
        }
    }
}