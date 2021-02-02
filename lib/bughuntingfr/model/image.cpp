// Developed since: Feb 2021

#include "image.h"

PyBufferProcs ImageBufferProcs = {
    (getbufferproc)Image_getbuffer,
    (releasebufferproc)Image_releasebuffer
};

PyTypeObject ImageType = {
    PyVarObject_HEAD_INIT(NULL, 0)
};

void Init_ImageType() {
    ImageType.tp_name = "bughuntingfr.model.Image";
    ImageType.tp_basicsize = sizeof(ImageObject);
    ImageType.tp_flags = Py_TPFLAGS_DEFAULT;
    ImageType.tp_new = PyType_GenericNew;
    ImageType.tp_init = (initproc)Image_init;
    ImageType.tp_dealloc = (destructor)Image_dealloc;
    ImageType.tp_as_buffer = &ImageBufferProcs;
}


int Image_getbuffer(PyObject* exporter, Py_buffer* view, int flags) {
    ImageObject* imobj = (ImageObject*)exporter;

    if (PyBUF_FORMAT & ~flags) {
        PyErr_SetString(PyExc_BufferError,
            "Consumer deos not accept the FORMAT field");
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
            "Consumer expects fortran-aligned data");
        view->obj = NULL;
        return -1;
    }
    
    view->obj = exporter;
    view->buf = imobj->data;
    view->itemsize = sizeof(double);
    view->format = "d";
    view->ndim = 2;
    view->len = sizeof(double) * \
        imobj->nColumns * imobj->nRows;

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


int Image_init(ImageObject* self, PyObject* args, PyObject* kwargs) {
    int nColumns, nRows;
    if (!PyArg_ParseTuple(args, "ii", &nRows, &nColumns)) { return -1; }

    self->data = new double[nColumns * nRows];
    self->nColumns = nColumns;
    self->nRows = nRows;
    int counter = 0;
    for (int yidx = 0; yidx < nRows; ++yidx) {
        for (int xidx = 0; xidx < nColumns; ++xidx) {
            self->data[yidx * nColumns + xidx] = counter;
            ++counter;
        }
    }
    return 0;
}

void Image_dealloc(ImageObject* self) {
    delete[] self->data;
    Py_TYPE(self)->tp_free((PyObject*)self);
}
