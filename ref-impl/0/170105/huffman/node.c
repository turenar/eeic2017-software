#include "node.h"
#include <stdlib.h>
#include <stdlib.h>

Node* node_new(int symbol, unsigned int count) {
	Node* ret = (Node*) calloc(1, sizeof(Node));
	ret->symbol = symbol;
	ret->count = count;
	ret->left =NULL;
	ret->right = NULL;
	return ret;
}

void node_free(Node* n) {
	if (n != NULL) {
		node_free(n->left);
		node_free(n->right);
		free(n);
	}
}
