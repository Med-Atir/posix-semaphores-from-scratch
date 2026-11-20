#include "p1.h"

void produire(Buffer b) {
    if (!tab_plein(b)) {
        b->element[b->cmp] = b->cmp;
        b->cmp++;
    }
}
void *production(void*arg) {
    ThreadArgs *data = (ThreadArgs *)arg;
    while (1) {
        P(&(data->s->vide),data->file,data->set);
        produire(data->buffer);
        V(&(data->s->plein),data->file);
    }
    return NULL;
}