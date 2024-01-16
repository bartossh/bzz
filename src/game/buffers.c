/// Copyright (c) 2024 Bartosz Lenart

#include "game.h"

// ------------- Swarm -----------------

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


bool bzzSwarmRemoveAt(BzzSwarm *s, int idx)
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

// ------------- Stationaries -----------------

BzzStationaries bzzStationariesNew(void)
{
    BzzStationaries b = {
        .buffer = {0},
        .st_size = 0
    };

    return b;
}


bool bzzStationariesAppend(BzzStationaries *s, BzzStationary b)
{
    if (!s || s->st_size == MAX_STATIONARIES_SIZE) {
        return false;
    }
    
    s->buffer[s->st_size++] = b;

    return true;
}


int bzzStationariesGetSize(BzzStationaries *s)
{
    if (!s) {
        return -1;
    }

    return s->st_size;
}

BzzStationary *bzzStationariesAt(BzzStationaries *s, int idx)
{
    if (!s || idx > s->st_size-1) {
        return NULL;
    }
    
    return &s->buffer[idx];
}


bool bzzStationariesRemoveAt(BzzStationaries *s, int idx)
{
    if (!s || idx > s->st_size-1) {
        return false;
    }

    int j = 0;
    for (int i = 0; i < s->st_size-1;) {
        if (i == idx) {
            j++;
        }
        s->buffer[i++] = s->buffer[j++];
    }
    s->st_size--;

    return true;
}


