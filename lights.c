#include <Python.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "queue.h"

static PyObject *LightsError;

int pin, value, range;

int getFillValue(double setting)
{
    const int settingCount = 82;
    const double settings[82] = {-1,13.403406,21.530804,28.454408,34.361889,39.286767,43.339989,46.942958,50.319221,53.511905,56.707125,59.663578,62.156546,64.478098,66.687709,68.57353,70.390296,71.91995,73.61323,74.860109,76.106988,77.261583,78.2837,79.176996,80.4164,81.234872,82.055779,82.752564,83.617061,83.995056,84.586033,85.1729862,85.7599394,86.3468926,86.9338458,87.520799,87.9798978,88.4389966,88.8980954,89.3571942,89.816293,90.2498612,90.6834294,91.1169976,91.5505658,91.984134,92.2605502,92.5369664,92.8133826,93.0897988,93.366215,93.6500124,93.9338098,94.2176072,94.5014046,94.785202,95.072776,95.36035,95.647924,95.935498,96.223072,96.4100892,96.5971064,96.7841236,96.9711408,97.158158,97.310116,97.462074,97.614032,97.76599,97.917948,97.9670674,98.0161868,98.0653062,98.1144256,98.163545,98.2597682,98.3559914,98.4522146,98.5484378,98.644661,100};
    int counter = 0;
    while(counter < settingCount)
    {
        if (settings[counter] > setting)
            break;
        counter++;
    }
    return counter-1;
}


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
lights_fill(PyObject *self, PyObject *args)
{
    int fillVal;
    if (!PyArg_ParseTuple(args, "i", &fillVal))
        return NULL;
    // printf ("fillVal: %d\n", fillVal);
    softPwmWrite(pin, fillVal);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
lights_set(PyObject *self, PyObject *args)
{
    int setting;
    if (!PyArg_ParseTuple(args, "i", &setting))
        return NULL;
    // Input validation
    if (setting < 0 || setting > 99)
    {
        PyErr_SetString(PyExc_ValueError,
                        "Setting must be an integer in range <0,99> inclusive.");
        return NULL;
    }
    // printf ("setting: %d\n", setting);
    int fillVal = getFillValue(setting);
    // printf("fillVal: %d\n", fillVal);
    softPwmWrite(pin, fillVal);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
lights_load_vals(PyObject *self, PyObject *args)
{
    char *wrongInputMsg = "Input must be a list of tuples of ints.";
    queue_t *queue;
    Py_ssize_t i;
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O:load_vals", &list))
        return NULL;
    if (!PyList_Check(list))
    {
        PyErr_SetString(PyExc_ValueError, wrongInputMsg);
        return NULL;
    }
    queue = create_queue(sizeof(element_t), PyList_Size(list));
    // Input validation
    for( i = 0; i < PyList_Size(list); i++)
    {
        PyObject *tmp = PyList_GetItem(list, i);
        unsigned int time_ms, brightness;
        element_t el;
        char msgBuffer [100];
        if(!tmp)
            return NULL;
        if(!PyTuple_Check(tmp))
        {
            PyErr_SetString(PyExc_ValueError, wrongInputMsg);
            queue_delete(queue);
            return NULL;
        }
        sprintf(msgBuffer,"I|I;item %d in input list is not tuple: (int, int).", i);
        if (!PyArg_ParseTuple(tmp, msgBuffer,
                              &el.time_ms, &el.brightness))
        {
            queue_delete(queue);
            return NULL;
        }
        queue_push_back(queue, &el);
    }
    queue_flush_and_print(queue);

    queue_delete(queue);
    Py_INCREF(Py_None);
    return Py_None;
}


static PyMethodDef LightsMethods[] = {
    {"init", lights_init, METH_VARARGS, "Initiate Lights Module."},
    {"fill", lights_fill, METH_VARARGS, "Set PWM fill value."},
    {"set", lights_set, METH_VARARGS, "Set setting (will be converted to PWM value)."},
    {"load_vals", lights_load_vals, METH_VARARGS,
        "Changes brightness using list of (time_ms, birghtness)"},
    {NULL, NULL, 0 ,NULL} /*Sentinel*/
};

PyMODINIT_FUNC
init_lights(void)
{
    PyObject *m;

    m = Py_InitModule("_lights", LightsMethods);
    if(m == NULL)
        return;

    LightsError = PyErr_NewException("lights.error", NULL, NULL);
    Py_INCREF(LightsError);
    PyModule_AddObject(m, "error", LightsError);
}