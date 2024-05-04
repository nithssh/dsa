#include <stdio.h>

// Minimum 2 children, max 4. Keys bounds always one lesser.
#define M 4

typedef int Key;
typedef void* Value;

// This implementation is closer to the original Bayer description,
// where internal nodes also point to data/records.
typedef struct {
    Key key;
    Value val;
} KeyValuePair;

struct node;
typedef struct node BTreeNode;
struct node {
    int size;
    // This can be determined without this flag by checking the children 
    // arrary, but this is a better tradeoff.
    int isLeaf; 
    BTreeNode *parent;
    BTreeNode *children[M];
    KeyValuePair records[M-1];
};

Value find(BTreeNode*, Key);
void insert(BTreeNode*, Key, Value);
void delete(BTreeNode*, Key);
void update(BTreeNode*, Key, Value);
void inorder(BTreeNode*);
void splitInsert(BTreeNode*, Key, Value);
// Range scans?

int isFull(BTreeNode* node) {
   return node->size == M-1;
} 

int main(int argc, char* argv[]) {
    BTreeNode root = {0};

}

/* Returns null if the key was not found. */
Value find(BTreeNode* root, Key key) {
    BTreeNode* node = root;
    while (1) {
       if (node->isLeaf) {
        break;
       } 
       // TODO binary search
       // find the path to take.
       int i = 0;
       KeyValuePair record = node->records[i]; 
       while (record.key < key && i <  node->size) {
            record = node->records[++i];
       }
       node = node->children[i];
    }

    int i = 0;
    // TODO binary search
    // while (node->records[i].key != key && i < node->size) i++;
    while (i < node->size) {
        if (node->records[i].key == key) {
            return node->records[i].val;
        } else {
            i++;
        }
    }
    if (i == node->size) {
        return NULL;
    }
}

void insert(BTreeNode* root, Key key, Value val) {
    BTreeNode* node = root;
    // Find the leaf to insert into
    while (1) {
       if (node->isLeaf) {
            break;
       } 
       // TODO binary search
       // find the path to take.
       int i = 0;
       KeyValuePair record = node->records[i]; 
       while (record.key < key && i <  node->size) {
            record = node->records[++i];
       }
       node = node->children[i];
    }

    // if the node is full, split the node during insertion
    if (isFull(node)) {
        splitInsert(node, key, val);
        return;
    }

    // if the node is not full, insert into right spot.
    // find the right index
    int targetIdx = 0;
    while (targetIdx < M-1 && node->records[targetIdx].key < key) {
        targetIdx++;
    }
    // shift everything from behind and insert the record.
    for (int i = node->size-1; i > targetIdx; i--) {
        node->records[i] = node->records[i-1];
    }
    node->records[targetIdx].key = key;
    node->records[targetIdx].val = val;
    node->size++;
}

// This implementation does splits naively on all full node insertions,
// without considering any rotations, etc. to improve the space utilization.
void splitInsert(BTreeNode* node, Key key, Value val) {
    const int midIdx = node->size / 2;
    Key midKey = node->records[midIdx].key;

    // Copy the second half into new node.
    KeyValuePair rightRecords[M-1] = {0};
    void* rightChildren[M] = {0};
    for (int srcIdx = midIdx + 1, tarIdx = 0; srcIdx < node->size; srcIdx++, tarIdx++) {
        rightRecords[tarIdx] = node->records[srcIdx];
        rightChildren[tarIdx] = node->children[srcIdx];
    }
    const int daughterSize = (node->size - 1) / 2;
    rightChildren[daughterSize] = node->children[node->size]; // copy the last child ptr

    BTreeNode newRightNode = {daughterSize, node->isLeaf, node->parent, rightRecords, rightChildren};

    // push the mid point up.
    // handle the case where parent is full recursively

    // make two new nodes (handle leaves properly)

    // update the child ptrs in parent properly
}
