// Implementation of the FriendBook ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fb.h"
#include "List.h"
#include "Map.h"
#include "Queue.h"

#define DEFAULT_CAPACITY 1 // DO NOT change this line

struct adjNode {
    int v;
    struct adjNode *next;
};

// DO NOT modify this struct
struct fb {
    int numPeople;
    int capacity;

    char **names; // the id of a person is simply the index
                  // that contains their name in this array
    
    Map nameToId; // maps names to ids

    struct adjNode **adj; // adjacency lists, kept in increasing order
};

static void increaseCapacity(Fb fb);
static char *myStrdup(char *s);
static int nameToId(Fb fb, char *name);

static struct adjNode *adjListInsert(struct adjNode *l, int v);
static struct adjNode *newAdjNode(int v);
static bool inAdjList(struct adjNode *l, int v);
static void freeAdjList(struct adjNode *l);

////////////////////////////////////////////////////////////////////////

// Creates a new instance of FriendBook
Fb FbNew(void) {
    Fb fb = malloc(sizeof(*fb));
    if (fb == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    fb->numPeople = 0;
    fb->capacity = DEFAULT_CAPACITY;
    
    fb->names = calloc(fb->capacity, sizeof(char *));
    if (fb->names == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    fb->nameToId = MapNew();

    fb->adj = calloc(fb->capacity, sizeof(struct adjNode *));
    if (fb->adj == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return fb;
}

void FbFree(Fb fb) {
    for (int i = 0; i < fb->capacity; i++) {
        freeAdjList(fb->adj[i]);
    }
    free(fb->adj);

    MapFree(fb->nameToId);

    for (int i = 0; i < fb->numPeople; i++) {
        free(fb->names[i]);
    }
    free(fb->names);
    
    free(fb);
}

int FbNumPeople(Fb fb) {
    return fb->numPeople;
}

bool FbAddPerson(Fb fb, char *name) {
    if (fb->numPeople == fb->capacity) {
        increaseCapacity(fb);
    }

    if (!MapContains(fb->nameToId, name)) {
        int id = fb->numPeople++;
        fb->names[id] = myStrdup(name);
        MapSet(fb->nameToId, name, id);
        return true;
    } else {
        return false;
    }
}

static void increaseCapacity(Fb fb) {
    int newCapacity = fb->capacity * 2;
    
    fb->names = realloc(fb->names, newCapacity * sizeof(char *));
    if (fb->names == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = fb->capacity; i < newCapacity; i++) {
        fb->names[i] = NULL;
    }
    
    fb->adj = realloc(fb->adj, newCapacity * sizeof(struct adjNode));
    if (fb->adj == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = fb->capacity; i < newCapacity; i++) {
        fb->adj[i] = NULL;
    }
    
    fb->capacity = newCapacity;
}

bool FbHasPerson(Fb fb, char *name) {
    return MapContains(fb->nameToId, name);
}

List FbGetPeople(Fb fb) {
    List l = ListNew();
    for (int id = 0; id < fb->numPeople; id++) {
        ListAppend(l, fb->names[id]);
    }
    return l;
}

bool FbFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    assert(id1 != id2);

    if (!inAdjList(fb->adj[id1], id2)) {
        fb->adj[id1] = adjListInsert(fb->adj[id1], id2);
        fb->adj[id2] = adjListInsert(fb->adj[id2], id1);
        return true;
    } else {
        return false;
    }
}

bool FbIsFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    return inAdjList(fb->adj[id1], id2);
}

////////////////////////////////////////////////////////////////////////
// Your tasks

int FbNumFriends(Fb fb, char *name) {
    // TODO: Complete this function
    int id = nameToId(fb, name);
    int numFriends = 0;
    struct adjNode *curr = fb->adj[id];
    while (curr != NULL) {
        numFriends++;
        curr = curr->next;
    }
    return numFriends;
}

bool FbUnfriend(Fb fb, char *name1, char *name2) {
    // First check if they are friends
    if (FbIsFriend(fb, name1, name2) == false) {
        return false;
    }

    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);

    // Unfriend user1 from user2
    struct adjNode *user1 = fb->adj[id1];
    struct adjNode *prev1 = NULL;
    while (user1 != NULL) {
        if (user1->v == id2) {
            if (prev1 == NULL) {
                fb->adj[id1] = user1->next;
            } else {
                prev1->next = user1->next;
            }
            free(user1);
            break;
        }
        prev1 = user1;
        user1 = user1->next;
    }

    // Unfriend user2 from user1
    struct adjNode *user2 = fb->adj[id2];
    struct adjNode *prev2 = NULL;
    while (user2 != NULL) {
        if (user2->v == id1) {
            if (prev2 == NULL) {
                fb->adj[id2] = user2->next;
            } else {
                prev2->next = user2->next;
            }
            free(user2);
            break;
        }
        prev2 = user2;
        user2 = user2->next;
    }

    return true;
}

List FbMutualFriends(Fb fb, char *name1, char *name2) {
    // TODO: Complete this function

    List l = ListNew();
    
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);

    struct adjNode *user1 = fb->adj[id1];
    while (user1 != NULL) {
        struct adjNode *user2 = fb->adj[id2];
        while (user2 != NULL) {
            if (user1->v == user2->v) {
                ListAppend(l, fb->names[user1->v]);
            }
            user2 = user2->next;
        }
        user1 = user1->next;
    }

    return l;
}

int FbFriendRecs1(Fb fb, char *name, struct recommendation recs[]) {
    // TODO: Complete this function
    return 0;
}

////////////////////////////////////////////////////////////////////////
// Optional task

List FbFriendRecs2(Fb fb, char *name) {
    // TODO: Complete this function
    List l = ListNew();
    return l;
}

////////////////////////////////////////////////////////////////////////
// Helper Functions

static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

// Converts a name to an ID. Raises an error if the name doesn't exist.
static int nameToId(Fb fb, char *name) {
    if (!MapContains(fb->nameToId, name)) {
        fprintf(stderr, "error: person '%s' does not exist!\n", name);
        exit(EXIT_FAILURE);
    }
    return MapGet(fb->nameToId, name);
}

static struct adjNode *adjListInsert(struct adjNode *l, int v) {
    if (l == NULL || v < l->v) {
        struct adjNode *new = newAdjNode(v);
        new->next = l;
        return new;
    } else if (v == l->v) {
        return l;
    } else {
        l->next = adjListInsert(l->next, v);
        return l;
    }
}

static struct adjNode *newAdjNode(int v) {
    struct adjNode *n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    n->v = v;
    n->next = NULL;
    return n;
}

static bool inAdjList(struct adjNode *l, int v) {
    for (struct adjNode *n = l; n != NULL && n->v <= v; n = n->next) {
        if (n->v == v) {
            return true;
        }
    }
    return false;
}

static void freeAdjList(struct adjNode *l) {
    struct adjNode *n = l;
    while (n != NULL) {
        struct adjNode *temp = n;
        n = n->next;
        free(temp);
    }
}

