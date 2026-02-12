# py-rbtree

A high-performance red-black tree implementation in C with Python bindings.

## Features

- **Fast**: O(log n) insert, search, and delete operations
- **Self-balancing**: Automatically maintains index ordered

## Installation

```bash
pip install py-red-black-tree
```

## Quick Start

```python
import rbtree

# Create a tree
tree = rbtree.RBTree()

# Insert key-value pairs
tree.insert(10, "ten")
tree.insert(5, "five")
tree.insert(15, "fifteen")

# Search
value = tree.search(10)  # Returns "ten"

# Delete
tree.delete(5)

# Verify key
if 10 in tree:
    print("Found!")

# Size
print(len(tree))  # 3

# Sorted traversal
items = tree.inorder()  # [(5, 'five'), (10, 'ten'), (15, 'fifteen')]

```