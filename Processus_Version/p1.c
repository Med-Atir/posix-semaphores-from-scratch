#include "p1.h"

void produire(Buffer b) {
    if (!tab_plein(b)) {
        b->element[b->cmp] = b->cmp;
        b->cmp++;
    }
}
void production(Buffer b,File f,Semaphore s) {
    while (1) {
        P(&(s->plein),f);
        produire(b);
        V(&(s->vide),f);
    }
}