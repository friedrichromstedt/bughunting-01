// Developed since: Feb 2021
#ifndef MODEL_H
#define MODEL_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>


typedef struct {
    PyObject_HEAD
    double* data;
    int nColumns, nRows;
} ImageObject;

extern PyBufferProcs ImageBufferProcs;
extern PyTypeObject ImageType;
void Init_ImageType();

int Image_getbuffer(PyObject* obj, Py_buffer* view, int flags);
void Image_releasebuffer(PyObject* exporter, Py_buffer* view);

int Image_init(ImageObject* self, PyObject* args, PyObject* kwargs);
void Image_dealloc(ImageObject* self);

#endif
