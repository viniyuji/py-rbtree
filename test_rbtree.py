#!/usr/bin/env python3
"""Test script for Red-Black Tree Python module."""

def test_basic_operations():
    """Test basic insert, search, and delete operations."""
    import rbtree
    
    print("Creating red-black tree...")
    tree = rbtree.RBTree()
    
    print("\nTesting insert operations...")
    tree.insert(10, "ten")
    tree.insert(5, "five")
    tree.insert(15, "fifteen")
    tree.insert(3, "three")
    tree.insert(7, "seven")
    tree.insert(12, "twelve")
    tree.insert(17, "seventeen")
    
    print(f"Tree size: {len(tree)}")
    assert len(tree) == 7, "Tree size should be 7"
    
    print("\nTesting search operations...")
    assert tree.search(10) == "ten"
    assert tree.search(5) == "five"
    assert tree.search(15) == "fifteen"
    print("Search operations passed!")
    
    print("\nTesting __contains__ operations...")
    assert 10 in tree
    assert 5 in tree
    assert 100 not in tree
    print("Contains operations passed!")
    
    print("\nTesting inorder traversal...")
    inorder = tree.inorder()
    print(f"Inorder traversal: {inorder}")
    keys = [item[0] for item in inorder]
    assert keys == [3, 5, 7, 10, 12, 15, 17], "Inorder should be sorted"
    print("Inorder traversal is correct!")
    
    print("\nTesting delete operations...")
    tree.delete(5)
    assert len(tree) == 6
    assert 5 not in tree
    print("Delete operation passed!")
    
    print("\n✓ All basic tests passed!")

def test_error_handling():
    """Test error handling."""
    import rbtree
    
    print("\nTesting error handling...")
    tree = rbtree.RBTree()
    tree.insert(10, "ten")
    
    # Test duplicate key
    try:
        tree.insert(10, "another ten")
        assert False, "Should raise ValueError for duplicate key"
    except ValueError as e:
        print(f"✓ Correctly raised ValueError: {e}")
    
    # Test search for non-existent key
    try:
        tree.search(999)
        assert False, "Should raise KeyError for non-existent key"
    except KeyError as e:
        print(f"✓ Correctly raised KeyError: {e}")
    
    # Test delete for non-existent key
    try:
        tree.delete(999)
        assert False, "Should raise KeyError for non-existent key"
    except KeyError as e:
        print(f"✓ Correctly raised KeyError: {e}")
    
    print("✓ All error handling tests passed!")

def test_complex_values():
    """Test storing complex Python objects."""
    import rbtree
    
    print("\nTesting with complex Python objects...")
    tree = rbtree.RBTree()
    
    # Store different types
    tree.insert(1, [1, 2, 3])
    tree.insert(2, {"key": "value"})
    tree.insert(3, (4, 5, 6))
    tree.insert(4, "string value")
    
    assert tree.search(1) == [1, 2, 3]
    assert tree.search(2) == {"key": "value"}
    assert tree.search(3) == (4, 5, 6)
    assert tree.search(4) == "string value"
    
    print("✓ Complex value tests passed!")

def test_large_dataset():
    """Test with a larger dataset."""
    import rbtree
    
    print("\nTesting with large dataset...")
    tree = rbtree.RBTree()
    
    # Insert 1000 items
    for i in range(1000):
        tree.insert(i, f"value_{i}")
    
    assert len(tree) == 1000
    
    # Verify some random values
    assert tree.search(0) == "value_0"
    assert tree.search(500) == "value_500"
    assert tree.search(999) == "value_999"
    
    # Delete some items
    for i in range(0, 100):
        tree.delete(i)
    
    assert len(tree) == 900
    
    print("✓ Large dataset test passed!")

def main():
    """Run all tests."""
    print("=" * 60)
    print("Red-Black Tree Python Module Tests")
    print("=" * 60)
    
    test_basic_operations()
    test_error_handling()
    test_complex_values()
    test_large_dataset()
    
    print("\n" + "=" * 60)
    print("All tests passed successfully! ✓")
    print("=" * 60)

if __name__ == "__main__":
    main()
