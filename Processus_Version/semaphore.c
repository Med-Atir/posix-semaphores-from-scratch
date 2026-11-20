#include "semaphore.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int emfiler(int pid,File f) {
    if (f->last >= MAX - 1)
        return 0;
    f->last += 1;
    f->pid[f->last] = pid;
    return 1;
}
int defiler(File f) {
    if (!est_vide(f)) {
        for (int i=0;i<f->last;i++)
            f->pid[i] = f->pid[i+1];
        f->last--;
       return 1;
    }
    return 0;
}
int est_vide(File f) {
    return f->last == -1;
}
int tab_vide(Buffer tab) {
    return tab->cmp == 0;
}
int get_first(File f) {
    if (est_vide(f))
        return -1;
    return f->pid[0];
}
int tab_plein(Buffer tab) {
    return tab->cmp >= MAX;
}
void V(int* s,File f) {
    (*s)++;
    if (*s <= 0) {
        int p = get_first(f);
        if (defiler(f)) {
            if (kill(p, SIGCONT) == -1) {
                perror("kill SIGCONT");// kill retourne -1 en cas error and 0 in succed alors je handle error
            }
            else {
                fprintf(stdout, "[Succes] pid %d: is deblocked\n",p);
            }
        }
        else {
            fprintf(stderr, "[Error]: no process are blocked\n");
        }
    }
}
void P(int* s,File f) {
    (*s)--;
    if (*s < 0) {
        int p = getpid();
        fprintf(stdout, "[Succes] pid %d: is blocked\n",p);
        if (emfiler(p,f)) {
            if (kill(p, SIGSTOP) == -1) {
                perror("kill SIGSTOP");// kill retourne -1 en cas error and 0 in succed alors je handle error
            }
        }
        else {
            fprintf(stderr, "[Error] pid %d: file is plein dude, go buy ram(you cant)\n", p);
        }
    }
}