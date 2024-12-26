#ifndef POS_H
#define POS_H

/* Uses zero based indexing to represent a position on the game board */
struct pos {
    unsigned int r, c;
};

typedef struct pos pos;


typedef struct pq_entry pq_entry;

struct pq_entry {
    pos p;
    pq_entry *next, *prev;
};


struct posqueue {
    pq_entry *head, *tail;
    unsigned int len;
};

typedef struct posqueue posqueue;

/* Helper function for testing. Prints out the queue. */
void queue_show(posqueue *q);

/* Helper function. Returns one is the positions are equal.
* Returns zero if the positions are not */
unsigned int pos_cmp(pos p1, pos p2);

/* Doesn't verify if these coordinates are on the board */
pos make_pos(unsigned int r, unsigned int c);

/* Allocate a new, empty queue */
posqueue* posqueue_new();

/* Add a position to the back of the queue */
void pos_enqueue(posqueue* q, pos p);

/*  Remove and return the element at the front of the queue. 
 *  Raises an error if the queue is empty */ 
pos pos_dequeue(posqueue* q);

/* Removes and returns the element at the back of the queue. 
 * Raises an error if the queue is empty */
pos posqueue_remback(posqueue* q);

/* Deallocates the entire existing queue */
void posqueue_free(posqueue* q);

#endif /* POS_H */