#include <Python.h>

// static PyObject *LightsError;

// PyMODINIT_FUNC
// initlights(void)
// {
//     PyObject *m;

//     m = Py_InitModule("lights", LightsMethods);
//     if(m == NULL)
//         return;

//     SpamError = PyErr_NewException("lights.error", NULL, NULL);
//     Py_INCREF(LightsError);
//     PyModule_AddObject(m, "error", LightsError);
// }



static PyObject *
lights_system(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return Py_BuildValue("i", sts);
}


static PyMethodDef LightsMethods[] = {
    {"system", lights_system, METH_VARARGS, "Execute shell command."},
    {NULL, NULL, 0 ,NULL} /*Sentinel*/
};

PyMODINIT_FUNC
initlights(void)
{
    (void) Py_InitModule("lights", LightsMethods);
}