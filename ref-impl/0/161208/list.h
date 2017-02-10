#pragma once

#include <stddef.h>

typedef struct node {
	char* str;
	struct node* next;
	struct node* prev;
} Node;

struct linked_list {
	Node* first;
	Node* last;
	size_t len;
};

struct linked_list_iterator {
	struct linked_list* list;
	struct node* prev; // prev
	struct node* pointer; // now
	struct node* next; // next
};

typedef struct linked_list LinkedList;
typedef struct linked_list_iterator LinkedListIterator;

LinkedList* list_new();
void list_free();

void list_push_front(LinkedList*, const char*);
void list_push_back(LinkedList*, const char*);

void list_pop_front(LinkedList*);
void list_pop_back(LinkedList*);

void list_remove_all(LinkedList*);

size_t list_get_count(LinkedList*);

Node* list_get_back_node(LinkedList*);
// iterator作ったけどあまり意味なかった
void list_iterator(LinkedList*, LinkedListIterator*);
const char* list_itr_next(LinkedListIterator*);
int list_itr_has_next(LinkedListIterator*);
void list_itr_remove(LinkedListIterator*);
