#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

size_t binarySearch(struct kvpair *arr, Key val, size_t arrSize, int strict);
// Range scans?

int isFull(struct node *node)
{
    return node->size == M - 1;
}

int main(int argc, char *argv[])
{
    struct node root = { 0 };
    int choice = -1;
    while (choice) {
	displayTree(&root);
	printf("0. Exit\n");
	printf("1. insert\n");
	// TODO
	printf("> ");
	scanf("%d", &choice);
    }

}

/* Returns null if the key was not found. */
Value find(struct node *root, Key key)
{
    struct node *node = root;
    while (1) {
	if (node->isLeaf) {
	    break;
	}
	size_t idx = binarySearch(node->records, key, node->size, 0);
	node = node->children[idx];
    }

    size_t idx = binarySearch(node->records, key, node->size, 1);
    if (idx == -1) {
	return NULL;
    }
    return node->records[idx].val;
}

void insert(struct node *root, Key key, Value val)
{
    struct node *node = root;
    // Find the leaf to insert into
    while (1) {
        if (node->isLeaf) {
            break;
        }
        //
        size_t idx = binarySearch(node->records, key, node->size, 0);
        node = node->children[idx];
    }

    // if the node is full, split the node during insertion
    if (isFull(node)) {
        splitInsert(node, key, val);
        return;
    }
    // if the node is not full, insert into right spot.
    // find the right index
    int targetIdx = 0;
    while (targetIdx < M - 1 && node->records[targetIdx].key < key) {
        targetIdx++;
    }
    // shift everything from behind and insert the record.
    for (int i = node->size - 1; i > targetIdx; i--) {
        node->records[i] = node->records[i - 1];
    }
    node->records[targetIdx].key = key;
    node->records[targetIdx].val = val;
    node->size++;
}

// This implementation does splits naively on all full node insertions,
// without considering any rotations, etc. to improve the space utilization.
void splitInsert(struct node *node, Key key, Value val)
{
    const int midIdx = node->size / 2;
    Key midKey = node->records[midIdx].key;

    const int daughterSize = (node->size - 1) / 2;
    struct node newRightNode = {.size = daughterSize,.isLeaf =
	    node->isLeaf,.parent = node->parent
    };

    // Copy the second half into new node.
    struct kvpair *rightRecords = newRightNode.records;
    struct node **rightChildren = newRightNode.children;
    for (int srcIdx = midIdx + 1, tarIdx = 0; srcIdx < node->size; srcIdx++, tarIdx++) {
        rightRecords[tarIdx] = node->records[srcIdx];
        rightChildren[tarIdx] = node->children[srcIdx];
    }
    rightChildren[daughterSize] = node->children[node->size];	// copy the last child ptr


    // push the mid point up.
    // handle the case where parent is full recursively

    // make two new nodes (handle leaves properly)

    // update the child ptrs in parent properly
}

void displayTree(struct node *root)
{
    if (!root) {
        printf("Tree isn't initialized yet\n");
    }
    // TODO BFS
    displayNode(root);
    printf("\n");
}

void displayNode(struct node *node)
{
    printf("|");
    for (int i = 0; i < node->size; i++) {
	printf(" %d", node->records[i].key);
    }
    for (int i = node->size; i < M - 1; i++) {
	printf(" -");
    }
    printf(" |");
}

// If strict is false, then the position where the element should be will be
// returned, instead of -1 indicating it doesn't exist.
size_t binarySearch(struct kvpair *arr, Key val, size_t arrSize, int strict)
{
    size_t l = 0, r = arrSize - 1;
    while (l <= r) {
        size_t midIdx = l + ((r - l) / 2);

        Key key = arr[midIdx].key;
        if (key == val) {
            return midIdx;
        } else if (key < val) {
            l = midIdx + 1;
        } else {
            r = midIdx - 1;
        }
    }
    return strict ? -1 : l;
}
