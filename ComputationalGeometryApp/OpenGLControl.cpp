//#include <stdio.h>
#include "OpenGLControl.h"
#include "Polygon.h"
#include "Lists.h"

unsigned int VBO;
int ELEMENT_SIZE = 3;

const int BUFFER_VERTEX_TRIANGLE_POINTER = 30;
const int BUFFER_EDGES_POINTER = 100;
const int BUFFER_POINTS_POINTER = 0;
const int BUFFER_FACE_POINTER = 200;

Poligono* MY_POL;
DCEL* MY_DCEL = NULL;

int RENDER_TRIANGLE = 0;
const int BUFFERLEN = 1000;
float BUFFER_ARRAY[BUFFERLEN];
int BUFFER_IS_CREATED = 0;

int FACELEN = 0;

float V1[2];
float V2[2];


// Control Flags
int CONTROL_FLAG = 0;
int INTERACTION_CONTROL_FLAG = 0;
int INTERACTION_COUNT = 0;

void setPolygon(Poligono* pol) {
	MY_POL = pol;
}

void setControlFlag(int val) {
	CONTROL_FLAG = val;
}

void setInteractionControlFlag(int val)
{
	INTERACTION_CONTROL_FLAG = val;
}

void onClick(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		float normalizedX = (float)x / (float)WIN_WIDTH * 2 - 1;
		float normalizedY = ((float)y / (float)WIN_HEIGHT * 2 - 1) * (-1);

		if (INTERACTION_CONTROL_FLAG == 0) {
			MY_POL->addVertex(normalizedX, normalizedY, 0);
		}
		else if (INTERACTION_CONTROL_FLAG == INT_ADD_VERTEX_DCEL) {
			MY_DCEL->addVertex(normalizedX, normalizedY);
			INTERACTION_CONTROL_FLAG = INT_NONE;
		}
		else if (INTERACTION_CONTROL_FLAG == INT_ADD_EDGE_DCEL) {
			INTERACTION_COUNT++;

				if (INTERACTION_COUNT == 2) {
					V2[0] = normalizedX;
					V2[1] = normalizedY;

					INTERACTION_COUNT = 0;
					INTERACTION_CONTROL_FLAG = INT_NONE;

					MY_DCEL->connect(V1, V2);
				}
				else {
					V1[0] = normalizedX;
					V1[1] = normalizedY;
				}
		}
		else if (INTERACTION_CONTROL_FLAG == INT_SELECT_FACE_DCEL) {
			updateFaceBuffer(normalizedX, normalizedY);
			CONTROL_FLAG = OP_DRAW_EDGES_OF_A_FACE;
			INTERACTION_CONTROL_FLAG = INT_NONE;
		}


		//printf("clck x: %f, y: %f\n", normalizedX, normalizedY);
	}
}

void render() {

	glClear(GL_COLOR_BUFFER_BIT);

	if (CONTROL_FLAG == DRAW_POINTS_FROM_SIMPLE_BUFFER) {
		renderPoints(MY_POL->getVertexNumber());
	}
	else if (CONTROL_FLAG == DRAW_POINTS_FROM_DCEL) {
		renderPointsFromDcel();
		renderEdgesFromDcel();
	}
	else if (CONTROL_FLAG == DRAW_ALL_EDGES_FROM_DCEL) {
		renderEdgesFromDcel();
	}
	else if (CONTROL_FLAG == OP_DRAW_EDGES_OF_A_FACE) {
		renderOpFaceFromDcel();
	}
}

void createVertexBuffer(VertexList* list)
{
	
}

void createVertexBuffer()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BUFFER_ARRAY), BUFFER_ARRAY, GL_STATIC_DRAW);
}

void updateVertexBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BUFFER_ARRAY), BUFFER_ARRAY, GL_STATIC_DRAW);
}

void updateFaceBuffer(float x, float y)
{	
	HalfEdge* inner = MY_DCEL->searchFaceThatContains(x, y);
	HalfEdge* aux = inner;
	float* vertexPair = inner->getVertexPairArray();
	int i =0;

	//origem de edge
	BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = vertexPair[0];
	i++;
	BUFFER_ARRAY[ELEMENT_SIZE *BUFFER_FACE_POINTER + i] = vertexPair[1];
	i++;
	BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = 0;
	i++;

	//end de edge
	BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = vertexPair[2];
	i++;
	BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = vertexPair[3];
	i++;
	BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = 0;
	i++;
	
	free(vertexPair);

	aux = aux->getNext();

	FACELEN = 2;
	while (aux != inner) {
		vertexPair = aux->getVertexPairArray();

		//origem de edge
		BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = vertexPair[0];
		i++;
		BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = vertexPair[1];
		i++;
		BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = 0;
		i++;

		//end de edge
		BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = vertexPair[2];
		i++;
		BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = vertexPair[3];
		i++;
		BUFFER_ARRAY[ELEMENT_SIZE * BUFFER_FACE_POINTER + i] = 0;
		i++;

		aux = aux->getNext();
		free(vertexPair);

		FACELEN+=2;
	}
}


void createVertexBuffer(float vertexArray[]) {

}

void createVertexBuffer(float vertexArray[], int bufflen) {

	for (int i = 0; i < bufflen; i++) {
		BUFFER_ARRAY[i] = vertexArray[i];
	}

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * BUFFERLEN, BUFFER_ARRAY, GL_STATIC_DRAW);
}

void updateVertexBuffer(float vertexArray[], int bufflen) {

	if (BUFFER_IS_CREATED == 0) {
		createVertexBuffer(vertexArray, bufflen);
	}
	else {
		for (int i = 0; i < bufflen; i++) {
			BUFFER_ARRAY[i] = vertexArray[i];
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * BUFFERLEN, BUFFER_ARRAY, GL_STATIC_DRAW);
	}
}


void renderPoints(int vertexNumber) {

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, ELEMENT_SIZE, GL_FLOAT, GL_FALSE, ELEMENT_SIZE * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	glDrawArrays(GL_POINTS, 0, vertexNumber);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void renderPolygon(int vertexNumber) {

	glVertexAttribPointer(0, ELEMENT_SIZE, GL_FLOAT, GL_FALSE, ELEMENT_SIZE * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	glDrawArrays(GL_LINE_LOOP, 0, vertexNumber);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void renderTriangles(int vertexNumber) {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, ELEMENT_SIZE, GL_FLOAT, GL_FALSE, ELEMENT_SIZE * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, vertexNumber);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void renderTriangle() {
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, ELEMENT_SIZE, GL_FLOAT, GL_FALSE, ELEMENT_SIZE * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, BUFFER_VERTEX_TRIANGLE_POINTER, 3);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();

}

void renderPointsFromDcel()
{
	if (MY_DCEL == NULL) {
		MY_DCEL = MY_POL->getDCEL();
		printf("DCEL GERADA A PARTIR DE UM BUFFER SIMPLES!\n");

	}

	VertexList* list = MY_DCEL->getVertexList();
	float vertexArray[BUFFERLEN];

	for (int i = 0; i < list->getSize(); i++) {
		vertexArray[i * 3] = list->get(i)->getX();
		vertexArray[i * 3 + 1] = list->get(i)->getY();
		vertexArray[i * 3 + 2] = 0;
	}

	updateVertexBuffer(vertexArray, list->getSize()*3);

	
	renderPoints(MY_DCEL->getVertexList()->getSize());
	//printf("TELA DESENHADA COM OS VERTICES DE DCEL!\n");
}

void renderEdgesFromDcel()
{
	float* array = MY_DCEL->getHalfEdgeList()->getVertexPairEdgesArray();

	for (int i = 0; i < MY_DCEL->getHalfEdgeList()->getSize() * ELEMENT_SIZE *2; i++) {
		BUFFER_ARRAY[BUFFER_EDGES_POINTER * ELEMENT_SIZE + i ] = array[i];
	}
	
	free(array);

	updateVertexBuffer();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, ELEMENT_SIZE, GL_FLOAT, GL_FALSE, ELEMENT_SIZE * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	for (int i = 0; i < MY_DCEL->getHalfEdgeList()->getSize()*2; i+=2) {
		glDrawArrays(GL_LINES, BUFFER_EDGES_POINTER+i, 2);
	}


	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void renderOpFaceFromDcel()
{
	updateVertexBuffer();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, ELEMENT_SIZE, GL_FLOAT, GL_FALSE, ELEMENT_SIZE * sizeof(float), 0);

	glEnableVertexAttribArray(0);

	for (int i = 0; i < FACELEN; i += 2) {
		glDrawArrays(GL_LINES, BUFFER_FACE_POINTER + i, 2);
	}

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void printDcelEdges()
{
	MY_DCEL->getHalfEdgeList()->print();
}

void printDcelVertices()
{
	MY_DCEL->getVertexList()->print();
}

void drawTriangle(float p[], float q[], float r[]) {

	BUFFER_ARRAY[90] = p[0];
	BUFFER_ARRAY[91] = p[1];
	BUFFER_ARRAY[92] = p[2];
	BUFFER_ARRAY[93] = q[0];
	BUFFER_ARRAY[94] = q[1];
	BUFFER_ARRAY[95] = q[2];
	BUFFER_ARRAY[97] = r[0];
	BUFFER_ARRAY[98] = r[1];
	BUFFER_ARRAY[99] = r[2];

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * BUFFERLEN, BUFFER_ARRAY, GL_STATIC_DRAW);

	RENDER_TRIANGLE = 1;
}
