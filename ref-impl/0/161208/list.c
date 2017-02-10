#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

#define MAXLEN 1024

static Node* list_node_new(const char*);
static void list_node_free(Node*);

static Node* list_node_new(const char* str) {
	Node* p = (Node*) calloc(1, sizeof(Node));
	char* s = (char*) malloc(strlen(str) + 1);
	strcpy(s, str);
	p->str = s;
	return p;
}

static void list_node_free(Node* n) {
	free(n->str);
	free(n);
}

LinkedList* list_new() {
	LinkedList* ret = (LinkedList*) malloc(sizeof(LinkedList));
	memset(ret, 0, sizeof(LinkedList));
	return ret;
}

void list_push_front(LinkedList* list, const char* str) {
	// Create a new element
	Node* p = list_node_new(str);
	p->next = list->first;
	if (list->first) {
		list->first->prev = p;
	} else {
		list->last = p;
	}
	list->first = p;
	list->len++;
}

void list_push_back(LinkedList* list, const char* str) {
	// Create a new element
	Node* p = list_node_new(str);
	p->prev = list->last;
	if (list->last) {
		list->last->next = p;
	} else {
		list->first = p;
	}
	list->last = p;
	list->len++;
}

void list_pop_front(LinkedList* list) {
	assert(list->first != NULL); // Don't call pop_front() when the list is empty
	Node* p = list->first;
	list->first = p->next;
	if (!p->next) {
		list->last = NULL;
	}

	free(p->str);
	free(p);
	list->len--;
}

void list_pop_back(LinkedList* list) {
	assert(list->last != NULL); // Don't call pop_back() when the list is empty
	Node* p = list->last;
	list->last = p->prev;
	if (!p->prev) {
		list->first = NULL;
	}

	free(p->str);
	free(p);
	list->len--;
}

void list_remove_all(LinkedList* list) {
	Node* p = list->first;
	while (p != NULL) {
		Node* next = p->next;
		list_node_free(p);
		p = next;
	}
	list->len = 0;
}

size_t list_get_count(LinkedList* list) {
	return list->len;
}

void list_iterator(LinkedList* list, LinkedListIterator* it) {
	it->list = list;
	it->pointer = NULL;
	it->next = list->first;
}

const char* list_itr_next(LinkedListIterator* it) {
	assert(it->next);
	Node* p = it->next;
	it->next = p->next;
	return p->str;
}

int list_itr_has_next(LinkedListIterator* it) {
	return it->next != NULL;
}

void list_itr_remove(LinkedListIterator* it) {
	Node* p = it->pointer;
	if (p->prev) {
		p->prev->next = p->next;
	}
	if (p->next) {
		p->next->prev = p->prev;
	}
	list_node_free(p);
	it->pointer = NULL;
	it->list->len--;
}

Node* list_get_back_node(LinkedList* list) {
	return list->last;
}
