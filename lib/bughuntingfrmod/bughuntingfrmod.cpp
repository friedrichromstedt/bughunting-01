// Developed since: Feb 2021

#define PY_SSIZE_T_CLEAN
#include <Python.h>


typedef struct {
    PyObject_HEAD
    double* data;
    int nRows, nColumns;
} ImageObject;

int Image_getbuffer(PyObject* exporter, Py_buffer* view, int flags) {
    ImageObject* imobj = (ImageObject*)exporter;

    if (PyBUF_FORMAT & ~flags) {
        PyErr_SetString(PyExc_BufferError,
                "Consumer does not accept the FORMAT field");
        view->obj = NULL;
        return -1;
    }
    if (PyBUF_STRIDES & ~flags) {
        PyErr_SetString(PyExc_BufferError,
                "Consumer does not accept multidimensional data");
        view->obj = NULL;
        return -1;
    }
    if ((PyBUF_F_CONTIGUOUS & ~flags) == 0) {
        PyErr_SetString(PyExc_BufferError,
                "Cunsumer expects fortran-aligned data");
        view->obj = NULL;
        return -1;
    }

    view->obj = exporter;
    view->buf = imobj->data;
    view->itemsize = sizeof(double);
    view->format = (char*)"d";
    view->ndim = 2;
    view->len = sizeof(double) * imobj->nRows * imobj->nColumns;

    Py_ssize_t* shape = new Py_ssize_t[2];
    shape[0] = imobj->nRows;
    shape[1] = imobj->nColumns;
    view->shape = shape;

    Py_ssize_t* strides = new Py_ssize_t[2];
    strides[0] = sizeof(double) * imobj->nColumns;
    strides[1] = sizeof(double);
    view->strides = strides;

    view->readonly = false;
    Py_INCREF(exporter);
    return 0;
}

void Image_releasebuffer(PyObject* exporter, Py_buffer* view) {
    delete[] view->shape;
    delete[] view->strides;
}

PyBufferProcs ImageBufferProcs = {
    (getbufferproc)Image_getbuffer,
    (releasebufferproc)Image_releasebuffer
};

int Image_init(ImageObject* self, PyObject* args, PyObject* kwargs) {
    int nRows, nColumns;
    if (!PyArg_ParseTuple(args, "ii", &nRows, &nColumns)) { return -1; }

    self->data = new double[nRows * nColumns];
    self->nRows = nRows;
    self->nColumns = nColumns;
    int counter = 0;
    for (int yidx = 0; yidx < nRows; ++yidx) {
        for (int xidx = 0; xidx < nColumns; ++xidx) {
            self->data[yidx * nColumns + xidx] = counter++;
        }
    }
    return 0;
}

void Image_dealloc(ImageObject* self) {
    delete[] self->data;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

PyTypeObject ImageType = {
    PyVarObject_HEAD_INIT(NULL, 0)
};

void Init_ImageType() {
    ImageType.tp_name = "bughuntingfrmod.Image";
    ImageType.tp_basicsize = sizeof(ImageObject);
    ImageType.tp_flags = Py_TPFLAGS_DEFAULT;
    ImageType.tp_new = PyType_GenericNew;
    ImageType.tp_init = (initproc)Image_init;
    ImageType.tp_dealloc = (destructor)Image_dealloc;
    ImageType.tp_as_buffer = &ImageBufferProcs;
}

struct PyModuleDef bughuntingfrmodule = {
    PyModuleDef_HEAD_INIT,
};

void Init_bughuntingfrmodule() {
    bughuntingfrmodule.m_name = "bughuntingfrmod";
    bughuntingfrmodule.m_size = -1;
}

PyMODINIT_FUNC
PyInit_bughuntingfrmod() {
    PyObject* module;

    /* Initialise the Image extension type: */
    Init_ImageType();
    if (PyType_Ready(&ImageType) < 0) { return NULL; }

    /* Initialise the module: */
    Init_bughuntingfrmodule();
    module = PyModule_Create(&bughuntingfrmodule);
    if (module == NULL) { return NULL; }

    /* Create module attributes: */

    Py_INCREF(&ImageType);
    if (PyModule_AddObject(module, "Image", (PyObject*)&ImageType) < 0) {
        Py_DECREF(&ImageType);
        Py_DECREF(module);
        return NULL;
    }

    /* The module is complete: */
    return module;
}
