// Developed since: Feb 2021

#include "image.h"


struct PyModuleDef modelmodule = {
    PyModuleDef_HEAD_INIT,
};
void Init_modelmodule() {
    modelmodule.m_name = "model";
    modelmodule.m_size = -1;
}

PyMODINIT_FUNC
PyInit_model() {
    PyObject* module;

    /* Initialise the Image extension type: */
    Init_ImageType();
    if (PyType_Ready(&ImageType) < 0) { return NULL; }

    /* Initialise the module: */
    Init_modelmodule();
    module = PyModule_Create(&modelmodule);
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
