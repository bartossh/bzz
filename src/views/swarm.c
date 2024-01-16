/// Copyright (c) 2024 Bartosz Lenart

#include "views.h"

BzzSwarm bzzSwarmNew(void)
{
    BzzSwarm b = {
        .buffer = {0},
        .swarm_size = 0
    };

    return b;
}


bool bzzSwarmAppend(BzzSwarm *s, BzzAnimated b)
{
    if (!s || s->swarm_size == MAX_SWARN_SIZE) {
        return false;
    }
    
    s->buffer[s->swarm_size++] = b;

    return true;
}


int bzzSwarmGetSize(BzzSwarm *s)
{
    if (!s) {
        return -1;
    }

    return s->swarm_size;
}

BzzAnimated *bzzSwarmAt(BzzSwarm *s, int idx)
{
    if (!s || idx > s->swarm_size-1) {
        return NULL;
    }
    
    return &s->buffer[idx];
}


bool bzzRemoveAt(BzzSwarm *s, int idx)
{
    if (!s || idx > s->swarm_size-1) {
        return false;
    }

    int j = 0;
    for (int i = 0; i < s->swarm_size-1;) {
        if (i == idx) {
            j++;
        }
        s->buffer[i++] = s->buffer[j++];
    }
    s->swarm_size--;

    return true;
}
