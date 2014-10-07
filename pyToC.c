#include <Python.h>
#include "xlib_utils.h"

// Function to be called from Python
static PyObject* py_testWindow(PyObject* self, PyObject* args)
{
    char *s = "Hello from C!";
    displayTestWindow();
    return Py_BuildValue("s", s);
}

static PyObject* py_testPerformance(PyObject* self, PyObject* args)
{
    char *s = "Michael";
    testPerformance();
    return Py_BuildValue("s", s);
}

static PyObject* py_setupWindow(PyObject* self, PyObject* args)
{
    int retVal;
    retVal = setupWindow("Mines");
    return Py_BuildValue("d", retVal);
}

static PyObject* py_updateScreenshot(PyObject* self, PyObject* args)
{
    int retVal;
    retVal = updateScreenshot();
    return Py_BuildValue("d", retVal);
}

/////////////////////// METHOD TABLE /////////////////////////////////////// 
static PyMethodDef pyToC_methods[] = 
{
    {"testWindow", py_testWindow, METH_VARARGS, "Execute a test function."},
    {"testPerformance", py_testPerformance, METH_VARARGS, "Performance test."},
    {"setupWindow", py_setupWindow, METH_VARARGS, "Setup the window."},
    {"updateScreenshot", py_updateScreenshot, METH_VARARGS, "Update the screen shot."},
    {NULL, NULL, 0, NULL}
};

/////////////////////// MODULE DEFINITION ////////////////////////////////// 
static struct PyModuleDef pyToC = 
{
    PyModuleDef_HEAD_INIT,
    "pyToC",
    NULL,
    -1,
    pyToC_methods
};

/////////////////////// METHOD TABLE /////////////////////////////////////// 
PyMODINIT_FUNC
PyInit_pyToC(void)
{
    return PyModule_Create(&pyToC);
}
