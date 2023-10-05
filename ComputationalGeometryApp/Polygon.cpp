#include "OpenGLControl.h"
#include "Polygon.h"
#include <stdio.h>

void Poligono::addVertex(float x, float y, float z)
{
    vertexArray[len] = x;
    vertexArray[len + 1] = y;
    vertexArray[len + 2] = z;
    len += 3;
    vertexNumber++;

    float p[3] = { 0.5, 0.5, 0 };
    float q[3] = { 0, -0.5, 0 };
    float r[3] = { 0,0,0 };

    updateVertexBuffer(vertexArray, len);
}

float* Poligono::getVertexArray()
{
    return vertexArray;
}

int Poligono::getLen()
{
    return len;
}

int Poligono::getVertexNumber()
{
    return vertexNumber;
}

DCEL* Poligono::getDCEL() {
    Coordinates aux[100];

    for (int i = 0; i < vertexNumber; i++) {
        aux[i].x = vertexArray[i * 3];
        aux[i].y = vertexArray[i * 3 + 1];
    }

    return new DCEL(aux, vertexNumber);
}
