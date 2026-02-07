#ifndef RBTREE_H
#define RBTREE_H

#include <stddef.h>
#include <stdbool.h>

/* Color enumeration for red-black tree nodes */
typedef enum {
    RED,
    BLACK
} NodeColor;

/* Red-black tree node structure */
typedef struct RBNode {
    int key;
    void *data;
    NodeColor color;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
} RBNode;

/* Red-black tree structure */
typedef struct RBTree {
    RBNode *root;
    RBNode *nil;  /* Sentinel node for NULL pointers */
    size_t size;
} RBTree;

/* Tree operations */
RBTree *rbtree_create(void);
void rbtree_destroy(RBTree *tree);
bool rbtree_insert(RBTree *tree, int key, void *data);
bool rbtree_delete(RBTree *tree, int key);
void *rbtree_search(RBTree *tree, int key);
bool rbtree_contains(RBTree *tree, int key);
size_t rbtree_size(RBTree *tree);

/* Traversal operations */
void rbtree_inorder(RBTree *tree, void (*callback)(int key, void *data));
void rbtree_preorder(RBTree *tree, void (*callback)(int key, void *data));
void rbtree_postorder(RBTree *tree, void (*callback)(int key, void *data));

/* Helper functions for internal use */
RBNode *rbtree_minimum(RBTree *tree, RBNode *node);
RBNode *rbtree_maximum(RBTree *tree, RBNode *node);

#endif /* RBTREE_H */
