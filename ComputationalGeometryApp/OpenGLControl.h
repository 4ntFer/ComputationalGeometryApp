#pragma once
#include <GL/glew.h>
#include <GL/glut.h>

#include"Polygon.h"
#include "config.h"

//Values for control flags
const int DRAW_POINTS_FROM_SIMPLE_BUFFER = 0;
const int DRAW_POINTS_FROM_DCEL = 1;
const int DRAW_ALL_EDGES_FROM_DCEL = 2;
const int OP_DRAW_EDGES_OF_A_FACE = 3;
const int INT_ADD_VERTEX_IN_SIMPLE_POL = 0;
const int INT_NONE = 1;
const int INT_ADD_VERTEX_DCEL = 2;
const int INT_ADD_EDGE_DCEL = 3;
const int INT_SELECT_FACE_DCEL = 4;

void setControlFlag(int val);
void setInteractionControlFlag(int val);
void setPolygon(Poligono* pol);
void onClick(int button, int state, int x, int y);
void render();
void createVertexBuffer(VertexList* list);
void createVertexBuffer();
void updateVertexBuffer();
void updateFaceBuffer(float x, float y);
void createVertexBuffer(float vertexArray[], int bufflen);
void updateVertexBuffer(float vertexArray[], int bufflen);
void renderPoints(int vertexNumber);
void renderPolygon(int vertexNumber);
void renderTriangles(int vertexNumber);
void drawTriangle(float p[], float q[], float r[]);
void renderTriangle();
void renderPointsFromDcel();
void renderEdgesFromDcel();
void renderOpFaceFromDcel();
void printDcelEdges();
void printDcelVertices();

