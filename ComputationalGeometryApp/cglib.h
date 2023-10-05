#pragma once
#include "DCEL.h"

int left(float p[], float q[], float r[]);

int faceContains(HalfEdge* e, Vertex* v);
int faceContains(HalfEdge* e, float x0, float y0);

