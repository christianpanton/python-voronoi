/*
* The author of this software is Shane O'Sullivan.  
* Permission to use, copy, modify, and distribute this software for any
* purpose without fee is hereby granted, provided that this entire notice
* is included in all copies of any software which is or includes a copy
* or modification of this software and in all copies of the supporting
* documentation for such software.
* THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR AT&T MAKE ANY
* REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
* OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
*/


#include <Python.h>
#include <stdio.h>
#include <search.h>
#include <malloc.h>
#include "VoronoiDiagramGenerator.h"


static PyObject *
voronoi_voronoi(PyObject *self, PyObject *args)
{

    PyObject *a, *b, *xs, *ys, *x, *y;
    float *xValues, *yValues, xv, yv, xmin, xmax, ymin, ymax, mindist, x1, y1, x2, y2;
    int alen, blen, listlen, outlen = 0;
	VoronoiDiagramGenerator vdg;
    
    if (!PyArg_ParseTuple(args, "OOf", &a, &b, &mindist))
        Py_RETURN_NONE;
        
    alen = PyObject_Size(a);
    blen = PyObject_Size(b);
    
    if(alen > blen)
        listlen = blen;
    else
        listlen = alen;
        
    xValues = (float*) malloc(sizeof(float)*listlen);
    yValues = (float*) malloc(sizeof(float)*listlen);
    
    xs = PyObject_GetIter(a);
    ys = PyObject_GetIter(b);
    
    if (xs == NULL || ys == NULL){
        Py_XDECREF(xs);
        Py_XDECREF(ys);
        Py_RETURN_NONE;
    }
    
    x = PyIter_Next(xs);
    y = PyIter_Next(ys);
    
    xv = (float) PyFloat_AsDouble(x);
    yv = (float) PyFloat_AsDouble(y);
    
    xmax = xv;
    xmin = xv;
    ymin = yv;
    ymax = yv;
    
    for(int i = 0; i < listlen; i++) {

        xv = (float) PyFloat_AsDouble(x);
        yv = (float) PyFloat_AsDouble(y);
        
        xValues[i] = xv;
        yValues[i] = yv;
        
        if(xv < xmin)
            xmin = xv;
        else if(xv > xmax)
            xmax = xv;

        if(yv < ymin)
            ymin = yv;
        else if(yv > ymax)
            ymax = yv;           
            
        Py_DECREF(x);
        Py_DECREF(y);
        x = PyIter_Next(xs);
        y = PyIter_Next(ys);
    }    
    
    if(x != NULL) Py_DECREF(x);
    if(y != NULL) Py_DECREF(y);
    
    Py_DECREF(xs);
    Py_DECREF(ys);
        
    vdg.generateVoronoi(xValues,yValues,listlen, xmin-1,xmax+1,ymin-1,ymax+1, mindist);
	vdg.resetIterator();

	while(vdg.getNext(x1, y1, x2, y2)) { outlen++; }
	
	PyObject *lst = PyList_New(outlen); 
	
	vdg.resetIterator();
    for(int i = 0; i < outlen; i++) {
        vdg.getNext(x1, y1, x2, y2);
 		PyList_SET_ITEM(lst, i, Py_BuildValue("((ff),(ff))", x1, y1, x2, y2));
    }
    
	free(xValues);
	free(yValues);
        
    return lst;
	
}


static PyMethodDef VoronoiMethods[] =
{
     {"diagram", voronoi_voronoi, METH_VARARGS, "Voronoi Diagram"},
     {NULL, NULL, 0, NULL}
};
 
PyMODINIT_FUNC
initvoronoi(void)
{
     (void) Py_InitModule("voronoi", VoronoiMethods);
}

