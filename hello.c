#https://docs.python.org/2/extending/newtypes.html
#include <Python.h>
static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "helloworld",     /* m_name */
        "Extension module example!",  /* m_doc */
        -1,                  /* m_size */
        helloworld_funcs,    /* m_methods */
        NULL,                /* m_reload */
        NULL,                /* m_traverse */
        NULL,                /* m_clear */
        NULL,                /* m_free */
    };

static PyObject* helloworld(PyObject* self)
{
    return Py_BuildValue("s", "Hello, Python extensions!!");
}

static char helloworld_docs[] =
    "helloworld( ): Any message you want to put here!!\n";

static PyMethodDef helloworld_funcs[] = {
    {"helloworld", (PyCFunction)helloworld, 
     METH_NOARGS, helloworld_docs},
    {NULL}
};

void PyInit_helloworld(void)
{
    Py_InitModule3("helloworld", helloworld_funcs,
                   "Extension module example!");
}