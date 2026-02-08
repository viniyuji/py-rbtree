# py-rbtree

A high-performance red-black tree implementation in C with Python bindings.

## Features

- **Fast**: O(log n) insert, search, and delete operations
- **Self-balancing**: Automatically maintains optimal tree structure

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

# Check membership
if 10 in tree:
    print("Found!")

# Size
print(len(tree))  # 3

# Sorted traversal
items = tree.inorder()  # [(5, 'five'), (10, 'ten'), (15, 'fifteen')]

# Delete
tree.delete(5)
```

## API Reference

### `RBTree()`

Create a new red-black tree.

### `insert(key: int, value: Any) -> None`

Insert a key-value pair. Keys must be integers. Values can be any Python object.

**Raises**: `ValueError` if key already exists.

### `search(key: int) -> Any`

Search for a value by key.

**Raises**: `KeyError` if key not found.

### `delete(key: int) -> None`

Delete a key from the tree.

**Raises**: `KeyError` if key not found.

### `inorder() -> List[Tuple[int, Any]]`

Return inorder traversal as a sorted list of (key, value) tuples.

### `len(tree) -> int`

Return the number of items in the tree.

### `key in tree -> bool`

Check if a key exists in the tree.
