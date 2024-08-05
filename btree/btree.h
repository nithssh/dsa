// Minimum 2 children, max 4. Keys bounds always one lesser.
#define M 4

// It does not seem to be possible to make this type generic
// without adding pointer indirection, and I'm not going to metaprogram this either.
#define Key int
#define Value void *

// This implementation is closer to the original Bayer description,
// where internal nodes also point to data/records.
struct kvpair {
    Key key;
    Value *val;
};

struct node {
    int size;
    // This can be determined without this flag by checking the children
    // arrary, but this is my personal tradeoff
    int isLeaf;
    struct node *parent;
    struct node *children[M];
    struct kvpair records[M - 1];
};

Value find(struct node *root, Key key);
void insert(struct node *root, Key key, Value val);
void del(struct node *root, Key key);
void update(struct node *root, Key key, Value val);
void inorder(struct node *root);
void splitInsert(struct node *root, Key key, Value val);

void displayTree(struct node *root);
void displayNode(struct node *root);
