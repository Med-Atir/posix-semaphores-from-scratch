#include "p2.h"

void consommer(Buffer b) {
    if (!tab_vide(b)) {
        b->cmp--;
        b->element[b->cmp] = -1;
    }
}
void consommation(Buffer b,File f,Semaphore s) {
    while (1) {
        P(&(s->vide),f);
        consommer(b);
        V(&(s->plein),f);
    }
}