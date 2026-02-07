#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

/* Create a new node */
static RBNode *create_node(RBTree *tree, int key, void *data) {
    RBNode *node = (RBNode *)malloc(sizeof(RBNode));
    if (!node) return NULL;
    
    node->key = key;
    node->data = data;
    node->color = RED;
    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;
    
    return node;
}

/* Left rotation */
static void rotate_left(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

/* Right rotation */
static void rotate_right(RBTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    
    if (x->right != tree->nil) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if (y->parent == tree->nil) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    
    x->right = y;
    y->parent = x;
}

/* Fix red-black tree properties after insertion */
static void insert_fixup(RBTree *tree, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            
            if (y->color == RED) {
                /* Case 1: Uncle is red */
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    /* Case 2: z is right child */
                    z = z->parent;
                    rotate_left(tree, z);
                }
                /* Case 3: z is left child */
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(tree, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            
            if (y->color == RED) {
                /* Case 1: Uncle is red */
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    /* Case 2: z is left child */
                    z = z->parent;
                    rotate_right(tree, z);
                }
                /* Case 3: z is right child */
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

/* Transplant subtrees during deletion */
static void transplant(RBTree *tree, RBNode *u, RBNode *v) {
    if (u->parent == tree->nil) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

/* Fix red-black tree properties after deletion */
static void delete_fixup(RBTree *tree, RBNode *x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            
            if (w->color == RED) {
                /* Case 1: Sibling is red */
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(tree, x->parent);
                w = x->parent->right;
            }
            
            if (w->left->color == BLACK && w->right->color == BLACK) {
                /* Case 2: Sibling's children are both black */
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    /* Case 3: Sibling's right child is black */
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(tree, w);
                    w = x->parent->right;
                }
                /* Case 4: Sibling's right child is red */
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotate_left(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode *w = x->parent->left;
            
            if (w->color == RED) {
                /* Case 1: Sibling is red */
                w->color = BLACK;
                x->parent->color = RED;
                rotate_right(tree, x->parent);
                w = x->parent->left;
            }
            
            if (w->right->color == BLACK && w->left->color == BLACK) {
                /* Case 2: Sibling's children are both black */
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    /* Case 3: Sibling's left child is black */
                    w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(tree, w);
                    w = x->parent->left;
                }
                /* Case 4: Sibling's left child is red */
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

/* Find node with given key */
static RBNode *find_node(RBTree *tree, int key) {
    RBNode *current = tree->root;
    
    while (current != tree->nil) {
        if (key == current->key) {
            return current;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return tree->nil;
}

/* Find minimum node in subtree */
RBNode *rbtree_minimum(RBTree *tree, RBNode *node) {
    while (node->left != tree->nil) {
        node = node->left;
    }
    return node;
}

/* Find maximum node in subtree */
RBNode *rbtree_maximum(RBTree *tree, RBNode *node) {
    while (node->right != tree->nil) {
        node = node->right;
    }
    return node;
}

/* Create a new red-black tree */
RBTree *rbtree_create(void) {
    RBTree *tree = (RBTree *)malloc(sizeof(RBTree));
    if (!tree) return NULL;
    
    /* Create sentinel nil node */
    tree->nil = (RBNode *)malloc(sizeof(RBNode));
    if (!tree->nil) {
        free(tree);
        return NULL;
    }
    
    tree->nil->color = BLACK;
    tree->nil->left = NULL;
    tree->nil->right = NULL;
    tree->nil->parent = NULL;
    tree->nil->data = NULL;
    
    tree->root = tree->nil;
    tree->size = 0;
    
    return tree;
}

/* Recursively destroy nodes */
static void destroy_nodes(RBTree *tree, RBNode *node) {
    if (node == tree->nil) return;
    
    destroy_nodes(tree, node->left);
    destroy_nodes(tree, node->right);
    free(node);
}

/* Destroy the tree and free all memory */
void rbtree_destroy(RBTree *tree) {
    if (!tree) return;
    
    destroy_nodes(tree, tree->root);
    free(tree->nil);
    free(tree);
}

/* Insert a key-value pair into the tree */
bool rbtree_insert(RBTree *tree, int key, void *data) {
    if (!tree) return false;
    
    /* Check if key already exists */
    if (find_node(tree, key) != tree->nil) {
        return false;  /* Duplicate keys not allowed */
    }
    
    RBNode *z = create_node(tree, key, data);
    if (!z) return false;
    
    RBNode *y = tree->nil;
    RBNode *x = tree->root;
    
    /* Find position for new node */
    while (x != tree->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    z->parent = y;
    
    if (y == tree->nil) {
        tree->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    /* Fix red-black properties */
    insert_fixup(tree, z);
    tree->size++;
    
    return true;
}

/* Delete a node with given key */
bool rbtree_delete(RBTree *tree, int key) {
    if (!tree) return false;
    
    RBNode *z = find_node(tree, key);
    if (z == tree->nil) return false;
    
    RBNode *y = z;
    RBNode *x;
    NodeColor y_original_color = y->color;
    
    if (z->left == tree->nil) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = rbtree_minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;
        
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    free(z);
    tree->size--;
    
    if (y_original_color == BLACK) {
        delete_fixup(tree, x);
    }
    
    return true;
}

/* Search for a value by key */
void *rbtree_search(RBTree *tree, int key) {
    if (!tree) return NULL;
    
    RBNode *node = find_node(tree, key);
    if (node == tree->nil) return NULL;
    
    return node->data;
}

/* Check if tree contains a key */
bool rbtree_contains(RBTree *tree, int key) {
    if (!tree) return false;
    return find_node(tree, key) != tree->nil;
}

/* Get the size of the tree */
size_t rbtree_size(RBTree *tree) {
    return tree ? tree->size : 0;
}

/* Inorder traversal helper */
static void inorder_helper(RBTree *tree, RBNode *node, void (*callback)(int key, void *data)) {
    if (node == tree->nil) return;
    
    inorder_helper(tree, node->left, callback);
    callback(node->key, node->data);
    inorder_helper(tree, node->right, callback);
}

/* Inorder traversal */
void rbtree_inorder(RBTree *tree, void (*callback)(int key, void *data)) {
    if (!tree || !callback) return;
    inorder_helper(tree, tree->root, callback);
}

/* Preorder traversal helper */
static void preorder_helper(RBTree *tree, RBNode *node, void (*callback)(int key, void *data)) {
    if (node == tree->nil) return;
    
    callback(node->key, node->data);
    preorder_helper(tree, node->left, callback);
    preorder_helper(tree, node->right, callback);
}

/* Preorder traversal */
void rbtree_preorder(RBTree *tree, void (*callback)(int key, void *data)) {
    if (!tree || !callback) return;
    preorder_helper(tree, tree->root, callback);
}

/* Postorder traversal helper */
static void postorder_helper(RBTree *tree, RBNode *node, void (*callback)(int key, void *data)) {
    if (node == tree->nil) return;
    
    postorder_helper(tree, node->left, callback);
    postorder_helper(tree, node->right, callback);
    callback(node->key, node->data);
}

/* Postorder traversal */
void rbtree_postorder(RBTree *tree, void (*callback)(int key, void *data)) {
    if (!tree || !callback) return;
    postorder_helper(tree, tree->root, callback);
}
