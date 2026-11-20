#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
#define MAX 10
typedef int data;
typedef struct {
    data element[MAX] ;
    int cmp;
}tab;
typedef struct {
    int plein;
    int vide;
}sys;
typedef sys* Semaphore;
typedef tab* Buffer;
typedef struct cellule {
    int pid[MAX];
    int last;
}Cellule;
typedef Cellule* File;
int emfiler(int pid,File f);
int defiler(File f);
int get_first(File f);
int est_vide(File f);
int tab_vide(Buffer tab);
int tab_plein(Buffer tab);
void V(int* s,File f);
void P(int* s,File f);

#endif