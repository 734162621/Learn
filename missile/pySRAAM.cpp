#include <Python.h>
#include <missilesim.h>
#include <omp.h>

typedef struct {
    PyObject_HEAD
    missilesim* missile;
} SRAAM; 

static PyObject *SRAAM_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
   SRAAM *self;
   self = (SRAAM *)type->tp_alloc(type, 0); 
   if (self != NULL){
        self->missile = new missilesim();
   }
   return (PyObject *) self;                                                                                             
}

static PyObject *SRAAM_Init(SRAAM *self, PyObject *args, PyObject *kwds) {
   double min_dist, max_run_time;
   int missile_type;
   if(!PyArg_ParseTuple(args, "idd", &missile_type, &min_dist, &max_run_time))
   {
        cout << "parse init parameter error!" << endl;
   }
   self->missile->missile_type = missile_type;
   self->missile->min_target_dist = min_dist;
   self->missile->max_run_time = max_run_time;    
   Py_INCREF(Py_None);
   return Py_None;                                                                     
}

static void SRAAM_dealloc(SRAAM *self)
{
    delete self->missile;
    Py_TYPE(self)->tp_free((PyObject *)self); 
}


static PyObject *SRAAM_missileInit(SRAAM *self, PyObject *args, PyObject *kwds){
    double sbel1, sbel2, sbel3, psiblx, thtblx, phiblx, alpha0x, beta0x, dvbe, maut;                        
    if (!PyArg_ParseTuple(args, "dddddddddd", &sbel1, &sbel2, &sbel3, &psiblx, &thtblx, &phiblx, &alpha0x, &beta0x, &dvbe, &maut)) return NULL;
    self->missile->missile_init(sbel1, sbel2, sbel3, psiblx, thtblx, phiblx, alpha0x, beta0x, dvbe, maut);
    self->missile->SimInit();
    self->missile->state = Running;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *SRAAM_set_target_input(SRAAM *self, PyObject *args, PyObject *kwds){
    double sbel1, sbel2, sbel3, psiblx, thtblx, dvbe, iftarget;                               
    if (!PyArg_ParseTuple(args,"ddddddd", &sbel1, &sbel2, &sbel3, &psiblx, &thtblx, &dvbe, &iftarget)) return NULL;
    self->missile->target_input(sbel1, sbel2, sbel3, psiblx, thtblx,  dvbe, iftarget);
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *SRAAM_run(SRAAM *self, PyObject *args, PyObject *kwds){
    double t;
    double dbt;
    if (!PyArg_ParseTuple(args,"d", &t)) return NULL;
    if(self->missile->state == Running || self->missile->state == Miss){
//        std::cout<<"!!! Running || Miss <-------------------------------------------------"<<endl;
        if (self->missile->sim_time > self->missile->max_run_time)
        {
            self->missile->state = Unactive;
//            std::cout<<"!!! Unactive <-------------------------------------------------"<<endl;
        }
        else
        {
            for(int i=0;i<(int)(t/self->missile->int_step);i++){
                int ans = self->missile->Run();
                dbt = self->missile->vehicle_type->missile[209].real(); // dbt
                if(dbt < self->missile->min_target_dist){
                    self->missile->state = Hit;
//                    std::cout<<"!!! Hit <-------------------------------------------------"<<endl;
                    break;
                }
                if (ans == 1){
                    self->missile->state = Miss;
//                    std::cout<<"!!! Miss <-------------------------------------------------"<<endl;
                }
                if (ans == 2){
                    self->missile->state = Unactive;
//                    std::cout<<"!!! Unactive <-------------------------------------------------"<<endl;
                    break;
                }
            }
        }
    }
//    else{
//        cout << "hello" << endl;
//    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *SRAAM_GetSRAAMValue(SRAAM *self, PyObject *args, PyObject *kwds){
    char* name;
    if (!PyArg_ParseTuple(args,"s", &name)) return NULL;
    return PyFloat_FromDouble(self->missile->GetMissileValue(name));
}

static PyObject *SRAAM_SetSRAAMValue(SRAAM *self, PyObject *args, PyObject *kwds){
    char* name;
    double* data;
    if (!PyArg_ParseTuple(args,"sd", &name, &data)) return NULL;
    return PyFloat_FromDouble(self->missile->GetMissileValue(name));
}
static PyObject *SRAAM_simTime(SRAAM *self, void *closure){return PyFloat_FromDouble(self->missile->SimTime());}
static PyObject *SRAAM_state(SRAAM *self, void *closure){return Py_BuildValue("i",self->missile->state);}
static PyObject *PySRAAM_G_min_target_dist(SRAAM *self, void *closure) { return PyFloat_FromDouble(self->missile->min_target_dist); }
static PyObject *PySRAAM_G_dist2target(SRAAM *self, void *closure) { return PyFloat_FromDouble(self->missile->vehicle_type->missile[209].real()); } // dbt
static PyObject *PySRAAM_G_max_run_time(SRAAM *self, void *closure) { return PyFloat_FromDouble(self->missile->max_run_time); }
static PyObject *PySRAAM_G_pos(SRAAM *self, void *closure) { 
    return Py_BuildValue("[ddd]",
                        self->missile->vehicle_type->flat6[220].real(), // sbel1
                        self->missile->vehicle_type->flat6[221].real(), // sbel2
                        -self->missile->vehicle_type->flat6[222].real());  // sbel3
                        }
static PyObject *PySRAAM_G_rot(SRAAM *self, void *closure) { 
    return Py_BuildValue("[ddd]",
                        self->missile->vehicle_type->flat6[137].real(), // psiblx
                        self->missile->vehicle_type->flat6[138].real(), // thtblx
                        self->missile->vehicle_type->flat6[139].real());  // phiblx
                        }
static int PySRAAM_S_min_target_dist(SRAAM *self, PyObject *value, void *closure) { self->missile->min_target_dist = PyFloat_AS_DOUBLE(value); return 0;}
static int PySRAAM_S_max_run_time(SRAAM *self, PyObject *value, void *closure) { self->missile->max_run_time = PyFloat_AS_DOUBLE(value); return 0;}

static PyGetSetDef PySRAAM_getseters[] = {
    {"sim_time", (getter) SRAAM_simTime, NULL, "sim_time", NULL},
    {"state", (getter) SRAAM_state, NULL, "sim_state", NULL},
    {"min_target_dist", (getter) PySRAAM_G_min_target_dist, (setter) PySRAAM_S_min_target_dist, 
      "min_target_dist", NULL},
    {"max_run_time", (getter) PySRAAM_G_max_run_time, (setter) PySRAAM_S_max_run_time, 
    "max_run_time", NULL},
    {"pos", (getter) PySRAAM_G_pos, NULL,"postion", NULL},
    {"rot", (getter) PySRAAM_G_rot, NULL,"rotation", NULL},
    {"dist", (getter) PySRAAM_G_dist2target, NULL,"dist to target", NULL},
    {NULL}  
};

static PyMethodDef PySRAAM_methods[] = {
        {"InitParamter",(PyCFunction) SRAAM_Init,METH_VARARGS,
         "Init missile data"
        },
        {"missileInit", (PyCFunction) SRAAM_missileInit, METH_VARARGS,
         "Init missile data"
        },
        {"run", (PyCFunction) SRAAM_run, METH_VARARGS,
         "Run Sim"
        },
        {"getValue", (PyCFunction) SRAAM_GetSRAAMValue, METH_VARARGS,
         "Get missile value"
        },
        {"setValue", (PyCFunction) SRAAM_SetSRAAMValue, METH_VARARGS,
         "Set missile value"
        },
        {"setTarget", (PyCFunction) SRAAM_set_target_input, METH_VARARGS,
         "Set missile target"
        },
        {NULL}  /* Sentinel */
};

static PyTypeObject PySRAAM_Type = {
    PyVarObject_HEAD_INIT(NULL,0)
    "SRAAM.SRAAM",                 /* tp_name */
    sizeof(SRAAM)   ,           /* tp_basicsize */
    0,                           /* tp_itemsize */
    (destructor) SRAAM_dealloc, /* tp_dealloc */
    0,                           /* tp_print */
    0,                           /* tp_getattr */
    0,                           /* tp_setattr */
    0,                           /* tp_compare */
    0,                           /* tp_repr */
    0,                           /* tp_as_number */
    0,                           /* tp_as_sequence */
    0,                           /* tp_as_mapping */
    0,                           /* tp_hash */
    0,                           /* tp_call */
    0,                           /* tp_str */
    0,                           /* tp_getattro */
    0,                           /* tp_setattro */
    0,                           /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,          /* tp_flags */
    "This class encapsulates an LWPR model for learning regression functions\n",
    0,		                     /* tp_traverse */
    0,		                     /* tp_clear */
    0,      		               /* tp_richcompare */
    0,		                     /* tp_weaklistoffset */
    0,		                     /* tp_iter */
    0,		                     /* tp_iternext */
    PySRAAM_methods,              /* tp_methods */
    0,                           /* tp_members */
    PySRAAM_getseters,           /* tp_getset */
    0,                           /* tp_base */
    0,                           /* tp_dict */
    0,                           /* tp_descr_get */
    0,                           /* tp_descr_set */
    0,                           /* tp_dictoffset */
    0,        /* tp_init */
    0,                           /* tp_alloc */
    SRAAM_new,                  /* tp_new */
}; 

static PyMethodDef SRAAM_methods[] = {{NULL, NULL, 0, NULL}};

static struct PyModuleDef SRAAMModule = 
{ 
    PyModuleDef_HEAD_INIT, 
    "SRAAM", /* name of module */ 
    "Python wrapper for the C implementation of SRAAM.", /* module documentation, may be NULL */ 
    -1, /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */ 
    SRAAM_methods
}; 

PyMODINIT_FUNC PyInit_SRAAM(void)
{
    PyObject *m;
    if (PyType_Ready(&PySRAAM_Type) < 0)
        return NULL;

    m = PyModule_Create(&SRAAMModule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PySRAAM_Type);
    if (PyModule_AddObject(m, "SRAAM", (PyObject *) &PySRAAM_Type) < 0) {
        Py_DECREF(&PySRAAM_Type);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}