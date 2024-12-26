#include "pos.h"
#include <stdlib.h>
#include <stdio.h>

/* Helper function for testing. Prints out the queue. */
void queue_show(posqueue *q){
	pq_entry *curr_entry = q -> head;
	for (int i = 0; i <= q -> len; i++){
		if (curr_entry == NULL){
			printf("|QUEUE ENDED\n");
		}
		else{
			printf("(%d, %d) -> ", curr_entry -> p.r, 
					       curr_entry -> p.c);
			curr_entry = curr_entry -> next;
		}
	}
}

/* Helper function, throws an error if a queue's length is zero */
void check_empty_queue(unsigned int len){
	if (len == 0){
		 fprintf(stderr, "trying to remove from an empty posqueue");
                exit(1);
	}
}

/* Helper function, returns one is the positions are equal and zero if the 
* positions are not */
unsigned int pos_cmp(pos p1, pos p2) {
	unsigned int equal = 1;
	if (p1.r - p2.r) {
		equal = 0;
	}
	if (p1.c - p2.c) {
		equal = 0;
	}
	return equal;
}

/* Doesn't verify if these coordinates are on the board */
pos make_pos(unsigned int r, unsigned int c){
	pos new_pos;
	new_pos.r = r;
	new_pos.c = c;
	return new_pos;
}

/* Allocate a new, empty queue */
posqueue* posqueue_new(){
	posqueue *new = (posqueue*)malloc(sizeof(posqueue*));
	if (new == NULL){
		fprintf(stderr, "posqueue_new: malloc error");
		exit(1);
	}
	new -> len = 0;
	new -> head = NULL;
	new -> tail = NULL;
	return new;
}

/* Add a position to the back of the queue */
void pos_enqueue(posqueue* q, pos p){
	pq_entry *new_entry = (pq_entry*)malloc(sizeof(pq_entry));
	if (new_entry == NULL){
        	fprintf(stderr, "pos_enqueue: malloc error");
        	exit(1);
    	}
	new_entry -> p = p;
	new_entry -> next = NULL;
	new_entry -> prev = (q -> tail);

	// Both head and tail points to this pos if its the first in the queue
	if (q -> len == 0){
		q -> head = new_entry;
	}

	// Update the old tail to point to the new tail
	if (q -> tail != NULL){
		q -> tail -> next = new_entry;
	}
	// Otherwise, only update the tail
	q -> tail = new_entry;
	q -> len += 1;
}

/*  Remove and return the element at the front of the queue.
 *  Raises an error if the queue is empty */
pos pos_dequeue(posqueue* q){
	check_empty_queue(q -> len);
	pos removed_pos = make_pos(q -> head -> p.r, q -> head -> p.c); 
	if (q -> len == 1){
	 	free(q -> head);	
		q -> head = NULL;
		q -> tail = NULL;
	}
	else {
		pq_entry* new_head = q -> head -> next;
        	free(q -> head);
        	q -> head = new_head;
		q -> head -> prev = NULL;
	}
	q -> len -= 1;
	return removed_pos;
}

/* Removes and returns the element at the back of the queue.
 * Raises an error if the queue is empty */
pos posqueue_remback(posqueue* q){
	check_empty_queue(q -> len);
	pos removed_pos = make_pos(q -> tail -> p.r, q -> tail -> p.c);
	
	if (q -> len == 1){
		free(q -> tail);
		q -> head = NULL;
		q -> tail = NULL;
	}
	else {
		pq_entry* new_tail = q -> tail -> prev;
		free(q -> tail);
		q -> tail = new_tail;
		q -> tail -> next = NULL;
	}
	q -> len -= 1;
	return removed_pos;
}

/* Deallocates the entire existing queue */
void posqueue_free(posqueue* q){
	while (q -> len){
		pos_dequeue(q);
	}
	free(q);
}
