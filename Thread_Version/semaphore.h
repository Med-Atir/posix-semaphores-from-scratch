#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
#define MAX 10
#include <pthread.h>
#include <signal.h>
typedef int data;
typedef struct {
    int plein;
    int vide;
}etat;
typedef etat* Semphore;
typedef struct {
    data element[MAX] ;
    int cmp;
}tab;
typedef tab* Buffer;
typedef struct cellule {
    pthread_t pid;
    struct cellule* suivant;
}Cellule;
typedef struct {
    Cellule* first;
    Cellule* last;
}Maillon;
typedef Maillon* File;
typedef struct {
    Buffer buffer;
    File file;
    Semphore s;
    sigset_t *set;
} ThreadArgs;
int emfiler(pthread_t pid,File f);
int defiler(File f);
pthread_t get_first(File f);
int est_vide(File f);
int tab_vide(Buffer tab);
int tab_plein(Buffer tab);
void V(int* s,File f);
void P(int* s,File f,sigset_t*);

#endif