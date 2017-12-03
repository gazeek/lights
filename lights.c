#include <Python.h>
#include <wiringPi.h>
#include <softPwm.h>


static PyObject *LightsError;

int pin, value, range;

static PyObject *
lights_init(PyObject *self, PyObject *args)
{
    static int initialized = 0; 
    //Default values
    pin = 17;
    range = 100;
    value = 50;

    if (initialized)
    {
        PyErr_SetString(LightsError,
                        "The lights module has already been initialized!");
        return NULL;
    }

    if (!PyArg_ParseTuple(args, "|iii", &pin, &value, &range))  
        return NULL;

    // Setup wiringPi and the software PWM
    wiringPiSetupGpio();
    if (softPwmCreate(pin, value, range) != 0)
    {
        PyErr_SetFromErrno(LightsError);
        return NULL;
    }

    initialized = 1;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
lights_set(PyObject *self, PyObject *args)
{
    int newval;
    if (!PyArg_ParseTuple(args, "i", &newval))
        return NULL;
    // printf ("newval: %d\n", newval);
    softPwmWrite(pin, newval);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef LightsMethods[] = {
    {"init", lights_init, METH_VARARGS, "Initiate Lights Module."},
    {"set", lights_set, METH_VARARGS, "Set PWM value."},
    {NULL, NULL, 0 ,NULL} /*Sentinel*/
};

PyMODINIT_FUNC
initlights(void)
{
    PyObject *m;

    m = Py_InitModule("lights", LightsMethods);
    if(m == NULL)
        return;

    LightsError = PyErr_NewException("lights.error", NULL, NULL);
    Py_INCREF(LightsError);
    PyModule_AddObject(m, "error", LightsError);
}