#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "rbtree.h"

/* Python object wrapper for RBTree */
typedef struct {
    PyObject_HEAD
    RBTree *tree;
} RBTreeObject;

/* Forward declarations */
static PyTypeObject RBTreeType;

/* Helper to decref stored Python objects */
static void RBTree_decref_data(void *data) {
    Py_XDECREF((PyObject *)data);
}

/* Destructor */
static void RBTree_dealloc(RBTreeObject *self) {
    if (self->tree) {
        rbtree_destroy_with_callback(self->tree, RBTree_decref_data);
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

/* Constructor */
static PyObject *RBTree_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    RBTreeObject *self;
    self = (RBTreeObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->tree = rbtree_create();
        if (self->tree == NULL) {
            Py_DECREF(self);
            PyErr_SetString(PyExc_MemoryError, "Failed to create red-black tree");
            return NULL;
        }
    }
    return (PyObject *)self;
}

/* Initialization */
static int RBTree_init(RBTreeObject *self, PyObject *args, PyObject *kwds) {
    return 0;
}

/* Insert method: insert(key, value) */
static PyObject *RBTree_insert(RBTreeObject *self, PyObject *args) {
    int key;
    PyObject *value;
    
    if (!PyArg_ParseTuple(args, "iO", &key, &value)) {
        return NULL;
    }
    
    /* Increment reference count to keep the Python object alive */
    Py_INCREF(value);
    
    if (!rbtree_insert(self->tree, key, (void *)value)) {
        Py_DECREF(value);
        PyErr_SetString(PyExc_ValueError, "Key already exists in tree");
        return NULL;
    }
    
    Py_RETURN_NONE;
}

/* Search method: search(key) */
static PyObject *RBTree_search(RBTreeObject *self, PyObject *args) {
    int key;
    
    if (!PyArg_ParseTuple(args, "i", &key)) {
        return NULL;
    }
    
    void *data = rbtree_search(self->tree, key);
    
    if (data == NULL) {
        PyErr_SetString(PyExc_KeyError, "Key not found in tree");
        return NULL;
    }
    
    PyObject *value = (PyObject *)data;
    Py_INCREF(value);
    return value;
}

/* Delete method: delete(key) */
static PyObject *RBTree_delete(RBTreeObject *self, PyObject *args) {
    int key;
    
    if (!PyArg_ParseTuple(args, "i", &key)) {
        return NULL;
    }
    
    /* Get the data before deleting to decrement reference count */
    void *data = rbtree_search(self->tree, key);
    
    if (data == NULL) {
        PyErr_SetString(PyExc_KeyError, "Key not found in tree");
        return NULL;
    }
    
    if (!rbtree_delete(self->tree, key)) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to delete key");
        return NULL;
    }
    
    /* Decrement reference count */
    Py_DECREF((PyObject *)data);
    
    Py_RETURN_NONE;
}

/* Contains method for sequence protocol */
static int RBTree_sq_contains(RBTreeObject *self, PyObject *key) {
    if (!PyLong_Check(key)) {
        PyErr_SetString(PyExc_TypeError, "Key must be an integer");
        return -1;
    }
    
    int k = PyLong_AsLong(key);
    if (k == -1 && PyErr_Occurred()) {
        return -1;
    }
    
    return rbtree_contains(self->tree, k) ? 1 : 0;
}

/* Contains method: __contains__(key) - for explicit calls */
static PyObject *RBTree_contains(RBTreeObject *self, PyObject *args) {
    int key;
    
    if (!PyArg_ParseTuple(args, "i", &key)) {
        return NULL;
    }
    
    if (rbtree_contains(self->tree, key)) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

/* Helper for actual inorder traversal */
static void inorder_traversal_helper(RBNode *node, RBNode *nil, PyObject *list) {
    if (node == nil) return;
    
    inorder_traversal_helper(node->left, nil, list);
    
    PyObject *tuple = Py_BuildValue("(iO)", node->key, (PyObject *)node->data);
    if (tuple) {
        PyList_Append(list, tuple);
        Py_DECREF(tuple);
    }
    
    inorder_traversal_helper(node->right, nil, list);
}

/* Inorder method: inorder() - returns list of (key, value) tuples */
static PyObject *RBTree_inorder(RBTreeObject *self, PyObject *Py_UNUSED(ignored)) {
    PyObject *list = PyList_New(0);
    if (!list) {
        return NULL;
    }
    
    inorder_traversal_helper(self->tree->root, self->tree->nil, list);
    
    return list;
}

/* Validate method: validate() */
static PyObject *RBTree_validate(RBTreeObject *self, PyObject *Py_UNUSED(ignored)) {
    if (rbtree_validate(self->tree)) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

/* Size method: __len__() */
static Py_ssize_t RBTree_length(RBTreeObject *self) {
    return (Py_ssize_t)rbtree_size(self->tree);
}

/* Method definitions */
static PyMethodDef RBTree_methods[] = {
    {"insert", (PyCFunction)RBTree_insert, METH_VARARGS,
     "Insert a key-value pair into the tree"},
    {"search", (PyCFunction)RBTree_search, METH_VARARGS,
     "Search for a value by key"},
    {"delete", (PyCFunction)RBTree_delete, METH_VARARGS,
     "Delete a key from the tree"},
    {"__contains__", (PyCFunction)RBTree_contains, METH_VARARGS,
     "Check if a key exists in the tree"},
    {"inorder", (PyCFunction)RBTree_inorder, METH_NOARGS,
     "Return inorder traversal as list of (key, value) tuples"},
    {"validate", (PyCFunction)RBTree_validate, METH_NOARGS,
     "Validate the Red-Black Tree properties"},
    {NULL}  /* Sentinel */
};

/* Sequence methods for len() and 'in' operator support */
static PySequenceMethods RBTree_as_sequence = {
    .sq_length = (lenfunc)RBTree_length,
    .sq_contains = (objobjproc)RBTree_sq_contains,
};

/* Type definition */
static PyTypeObject RBTreeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "rbtree.RBTree",
    .tp_doc = "Red-Black Tree data structure",
    .tp_basicsize = sizeof(RBTreeObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = RBTree_new,
    .tp_init = (initproc)RBTree_init,
    .tp_dealloc = (destructor)RBTree_dealloc,
    .tp_methods = RBTree_methods,
    .tp_as_sequence = &RBTree_as_sequence,
};

/* Module definition */
static PyModuleDef rbtreemodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "rbtree",
    .m_doc = "Red-Black Tree module implemented in C",
    .m_size = -1,
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_rbtree(void) {
    PyObject *m;
    
    if (PyType_Ready(&RBTreeType) < 0) {
        return NULL;
    }
    
    m = PyModule_Create(&rbtreemodule);
    if (m == NULL) {
        return NULL;
    }
    
    Py_INCREF(&RBTreeType);
    if (PyModule_AddObject(m, "RBTree", (PyObject *)&RBTreeType) < 0) {
        Py_DECREF(&RBTreeType);
        Py_DECREF(m);
        return NULL;
    }
    
    return m;
}
