#include "p2.h"

void consommer(Buffer b) {
    if (!tab_vide(b)) {
        b->cmp--;
        b->element[b->cmp] = -1;
    }
}
void *consommation(void*arg) {
    ThreadArgs *data = (ThreadArgs *)arg;
    while (1) {
        P(&(data->s->plein),data->file,data->set);
        consommer(data->buffer);
        V(&(data->s->vide),data->file);
    }
    return NULL;
}