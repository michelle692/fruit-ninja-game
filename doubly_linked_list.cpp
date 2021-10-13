//=================================================================
// Implementation for DLL module.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

LLNode* create_llnode(void* data) {

    // Your code here
    LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
  
    return newNode; // replace this
}

DLinkedList* create_dlinkedlist(void) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}

int getSize(DLinkedList* dLinkedList){

  // Your code here

  return dLinkedList->size;
}

LLNode* getHead(DLinkedList* dLinkedList){

  // Your code here

  return dLinkedList->head; // replace this
}

LLNode* getTail(DLinkedList* dLinkedList){

  // Your code here

  return dLinkedList->tail; // replace this
}

LLNode* getNext(LLNode* node){

  // Your code here

  return node->next; // replace this
}

LLNode* getPrev(LLNode* node){

  // Your code here

  return node->prev; // replace this
}

void* getData(LLNode* node){

  // Your code here

  return node->data; // replace this
}

void insertAfter(DLinkedList* dLinkedList, LLNode* prev_node, void* data){
  if (prev_node == NULL) {
    printf("the given previous node cannot be NULL");
    return;
  }

  // Your code here
  LLNode* tempStart = dLinkedList->head;
  while(tempStart != NULL) {
    if(tempStart == prev_node) {
      LLNode* nNode = create_llnode(data);
      nNode->prev = tempStart;
      nNode->next = tempStart->next;
      tempStart->next = nNode;
      if(nNode->next != NULL) 
        nNode->next->prev = nNode;
      dLinkedList->size++;
      if(tempStart == dLinkedList->tail) {
        dLinkedList->tail = nNode;
      }
      return;
    }
    tempStart = tempStart->next;
  }
  return;
}

void insertBefore(DLinkedList* dLinkedList, LLNode* next_node, void* data){
  if (next_node == NULL) {
    printf("the given next node cannot be NULL");
    return;
  }

  // Your code here
  LLNode* tempStart = dLinkedList->tail;
  while(tempStart != NULL) {
    if(tempStart == next_node) {
      LLNode* pNode = create_llnode(data);
      pNode->next = tempStart;
      pNode->prev = tempStart->prev;
      tempStart->prev = pNode;
      if(pNode->prev != NULL) 
        pNode->prev->next = pNode;
      dLinkedList->size++;
      if(tempStart == dLinkedList->head) {
        dLinkedList->head = pNode;
      }
      return;
    }
    tempStart = tempStart->prev;
  }
  return;
}

void insertHead(DLinkedList* dLinkedList, void* data){
  if(dLinkedList->head == NULL){
    LLNode* newNode = create_llnode(data);
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
    dLinkedList->size++; 
  } else {
    insertBefore(dLinkedList, dLinkedList->head, data);
  }
}

void insertTail(DLinkedList* dLinkedList, void* data){

  // Your code here
  if(dLinkedList->tail == NULL) {
    LLNode* newNode = create_llnode(data);
    dLinkedList->head = newNode;
    dLinkedList->tail = newNode;
    dLinkedList->size++;
  } else {
    insertAfter(dLinkedList, dLinkedList->tail, data);
  }

}

void deleteNode(DLinkedList* dLinkedList, LLNode* Node){

  // Your code here
  LLNode* tempStart = dLinkedList->head;
  while(tempStart != NULL) {
    if(tempStart == Node) {
      if(tempStart->next != NULL)
        tempStart->next->prev = tempStart->prev;
      if(tempStart->prev != NULL)
        tempStart->prev->next = tempStart->next;
      if(tempStart == dLinkedList->head)
        dLinkedList->head = tempStart->next;
      if(tempStart == dLinkedList->tail)
        dLinkedList->tail = tempStart->prev;
      free(tempStart);
      dLinkedList->size--;
      return;
    }
    tempStart = tempStart->next;
  }
}

void destroyList(DLinkedList* dLinkedList){

  // Your code here
  LLNode* nextN = dLinkedList->head;
  LLNode* freeN = nextN;
  while(nextN != NULL) {
    freeN = nextN;
    nextN = nextN->next;
    free(freeN);
  }
  free(dLinkedList);
}

void reverse(DLinkedList* dLinkedList)
{

  // Your code here
  LLNode* curr = dLinkedList->head;
  LLNode* nex;
  while(curr != NULL) {
    nex = curr->next;
    curr->next = curr->prev;
    curr->prev = nex;
    curr = curr->prev;
  }
  LLNode* prevHead = dLinkedList->head;
  dLinkedList->head = dLinkedList->tail;
  dLinkedList->tail = prevHead;
}
