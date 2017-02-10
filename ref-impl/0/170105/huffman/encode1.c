#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define NSYMBOLS (256+1)

static unsigned int symbol_count[NSYMBOLS];

typedef struct node {
	int symbol;
	unsigned int count;
	struct node* left;
	struct node* right;
} Node;

static unsigned char get_readable_char(int symbol);

static void count_symbols(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "error: cannot open %s\n", filename);
		exit(1);
	}

	for (int i = 0; i < NSYMBOLS; i++) {
		symbol_count[i] = 0;
	}

	int c;
	for (;;) {
		c = fgetc(fp);
		if (c == EOF) {
			break;
		}
		symbol_count[c]++;
	}

	symbol_count[NSYMBOLS - 1]++; // End of File

	fclose(fp);
}

static Node* pop_min(int* n, Node* nodep[]) {
	// Find the node with the smallest count
	int j = 0;
	for (int i = 0; i < *n; i++) {
		if (nodep[i]->count < nodep[j]->count) {
			j = i;
		}
	}

	Node* node_min = nodep[j];

	// Remove the node pointer from nodep[]
	for (int i = j; i < (*n) - 1; i++) {
		nodep[i] = nodep[i + 1];
	}
	(*n)--;

	return node_min;
}

static Node* build_tree() {
	int n = 0;
	Node* nodep[NSYMBOLS];

	for (int i = 0; i < NSYMBOLS; i++) {
		if (symbol_count[i] == 0) {
			continue;
		}
		nodep[n] = malloc(sizeof(Node));
		nodep[n]->symbol = i;
		nodep[n]->count = symbol_count[i];
		nodep[n]->left = NULL;
		nodep[n]->right = NULL;
		n++;
	}

	while (n >= 2) {
		Node* node1 = pop_min(&n, nodep);
		Node* node2 = pop_min(&n, nodep);
		Node* parent = malloc(sizeof(Node));
		parent->symbol = -1;
		parent->count = node1->count + node2->count;
		parent->left = node1;
		parent->right = node2;
		nodep[n] = parent;
		n++;
	}

	return nodep[0];
}

// Perform depth-first traversal of the tree
static size_t traverse_tree(const int depth, const Node* np, char* const buf, char* lastptr) {
	assert(depth < NSYMBOLS);

	if (np->left == NULL) {
		*lastptr = '\0';
		if (np->symbol == (NSYMBOLS - 1)) {
			printf("EOF  : [%4u] %s\n", np->count, buf);
		} else {
			printf("%02x(%c): [%4u] %s\n", np->symbol, get_readable_char(np->symbol), np->count, buf);
		}
		return np->count;
	} else {
		size_t len = np->count;
		*lastptr = '0';
		len += traverse_tree(depth + 1, np->left, buf, lastptr + 1);
		*lastptr = '1';
		len += traverse_tree(depth + 1, np->right, buf, lastptr + 1);
		return len;
	}
}

static unsigned char get_readable_char(int symbol) {
	return (unsigned char) ((0x20 <= symbol && symbol <= 0x7e) ? symbol : '.');
}

void node_free(Node* n) {
	if (n != NULL) {
		node_free(n->left);
		node_free(n->right);
		free(n);
	}
}

int encode(const char* filename) {
	count_symbols(filename);
	Node* root = build_tree();
	char buf[256];
	printf("compressed length = %zu bits\n", traverse_tree(0, root, buf, buf));
	node_free(root);
	return 1;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "error: please specify the file.\n");
		exit(1);
	}

	encode(argv[1]);

	return 0;
}
