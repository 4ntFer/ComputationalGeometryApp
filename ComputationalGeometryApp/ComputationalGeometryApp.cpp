// ComputationalGeometryApp.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <stdio.h>

#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "config.h"
#include "OpenGLControl.h"
#include "Polygon.h"


char userInput[20] = "";

void handleUserInput() {
    while (true) {
        cin >> userInput;

        if (strcmp(userInput, "draw/dcel") == 0) {
            printf("\n");
            setControlFlag(DRAW_POINTS_FROM_DCEL);
        }
        else if (strcmp(userInput, "draw/dcel/edges") == 0) {
            printf("\n");
            setControlFlag(DRAW_ALL_EDGES_FROM_DCEL);
        }
        else if (strcmp(userInput, "print/dcel/edges") == 0) {
            printDcelEdges();
        }
        else if (strcmp(userInput, "print/dcel/vertices") == 0) {
            printDcelVertices();
        }
        else if(strcmp(userInput, "add/dcel/vertex")== 0)
        {
            setInteractionControlFlag(INT_ADD_VERTEX_DCEL);
            printf("clique em qualquer lugar da tela para gerar o vétice!");
        }
        else if (strcmp(userInput, "add/dcel/edge") == 0)
        {
            setInteractionControlFlag(INT_ADD_EDGE_DCEL);
        }
        else if (strcmp(userInput, "op/dcel/face") == 0)
        {
            setInteractionControlFlag(INT_SELECT_FACE_DCEL);
        }

        glutPostRedisplay();
    }
}

int main(int argc, char* argv[])
{   
    //string userInput = "";
    Poligono* pol = new Poligono();
    thread userInputThread(handleUserInput);
    setPolygon(pol);


    //userInputThread.join();


    glutInit(&argc, argv);

    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);

    glutCreateWindow("OpenGL me da medo");

    glewInit();

    glutDisplayFunc(render);

    glutMouseFunc(onClick);

    glutMainLoop();

    createVertexBuffer();
}