//=================================================================
// Header file for DLL module.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

// A linked list node structure.
typedef struct llnode_t {
    void* data;
    struct llnode_t* prev;
    struct llnode_t* next;
}LLNode;

/// The structure to store the information of a doubly linked list
typedef struct dlinkedlist_t {
    struct llnode_t* head;
    struct llnode_t* tail;
    int size;
} DLinkedList;

// Used for testing.
struct LLItem {};
void testDLL(void);

/**
 * create_llnode
 *
 * Creates a node by allocating memory for it on the heap,
 * and initializing its previous and next pointers to NULL and its data pointer to the input
 * data pointer
 *
 * @param data A void pointer to data the user is adding to the doublely linked list.
 * @return A pointer to the linked list node
 */
LLNode* create_llnode(void* data);

/**
 * create_dlinkedlist
 *
 * Creates a doubly linked list by allocating memory for it on the heap. Initialize the size to zero,
 * as well as head and tail pointers to NULL
 *
 * @return A pointer to an empty dlinkedlist
 */
DLinkedList* create_dlinkedlist(void);


/**
 * insertHead
 *
 * Create a new LLNode with the given data and insert it at the head of the doubly linked list.
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @param data A void pointer to data the user is adding to the doubly linked list.
 * 
 */
void insertHead(DLinkedList* dLinkedList, void* data);

/**
 * deleteNode
 *
 * Delete the node pointed to by Node (splice it out).  Update head/tail if necessary.
 * Assumes Node is not NULL.
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @param Node A pointer to a linked list node.
 */
void deleteNode(DLinkedList* dLinkedList, LLNode* Node);

/**
 * insertAfter
 *
 * Insert data after the prev_node. Update head/tail if necessary.
 * Assumes prev_node is not NULL.
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @param pre_node A pointer to a linked list node.
 * @param data A void pointer to data.
 */
void insertAfter(DLinkedList* dLinkedList, LLNode* prev_node, void* data);

/**
 * insertBefore
 *
 * Insert data before the prev_node (reference node). Update head/tail if necessary.
 * Assumes prev_node is not NULL.
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @param pre_node A pointer to a linked list node.
 * @param data A void pointer to data.
 */
void insertBefore(DLinkedList* dLinkedList, LLNode* prev_node, void* data);

/**
 * insertTail
 *
 * Create a new LLNode with the given data and insert it at the tail of the doubly linked list.
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @param data A void pointer to data the user is adding to the doubly linked list.
 * 
 */
void insertTail(DLinkedList* dLinkedList, void* data);

/**
 * reverse
 *
 * Reverse the order of the doubly linked list. Update head/tail if necessary.
 *
 * @param dLinkedList A pointer to the doubly linked list
 * 
 */
void reverse(DLinkedList* dLinkedList);


/**
 * destroyList
 *
 * Destroy the doubly linked list. Everything in the linked list including list structure,
 * nodes and data are all freed from the heap
 *
 * @param dLinkedList A pointer to the doubly linked list
 *
 */
void destroyList(DLinkedList* dLinkedList);

/**
 * getSize
 *
 * Return the size of the doubly linked list
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @return  the size
 */
int getSize(DLinkedList* dLinkedList);
LLNode* getHead(DLinkedList* dLinkedList);
LLNode* getNext(LLNode* node);
void* getData(LLNode* node);
#endif

