#pragma once
#include <cstddef>
#include <list>

class Coordinates {
public:
	float x;
	float y;
};

class Vertex {
private:
	Coordinates coordinates;
	int key;
	class HalfEdge* incidentEdge = nullptr;
public:
	Vertex(Coordinates coord, int identifier, HalfEdge* incident);
	Vertex(float x, float y, int identifier, HalfEdge* incident);
	void setIncidentEdge(HalfEdge* e);
	HalfEdge* getIncidentEdge();
	float getX();
	float getY();
	int getKey();
	void print();
};

class Face;

class HalfEdge {
private:
	Vertex* end = NULL;
	HalfEdge* twin = NULL;
	HalfEdge* next = NULL, * prev = NULL;
	Face* incidentFace = NULL;
public:
	HalfEdge();
	HalfEdge(Vertex* v1, Vertex* v2);
	void setEnd(Vertex* v);
	void setTwin(HalfEdge* e);
	void setNext(HalfEdge* e);
	void setPrev(HalfEdge* e);
	void setIncidentFace(Face* f);

	Vertex* getEnd();
	Vertex* getOrigin();
	HalfEdge* getTwin();
	HalfEdge* getNext();
	HalfEdge* getPrev();
	Face* getIncidentFace();

	float* getVertexPairArray();

	HalfEdge* searchHalfEdgeWithEnd(Vertex* v); //envontra half edge com final em v
};

class VertexList
{
private:
	class Vertex* list[100];
	int size = 0;
public:
	VertexList();
	void push_back(Vertex* v);
	Vertex* get(int i);
	int getSize();
	void print();
};

class Face {
private:
	HalfEdge* innerComponent = NULL, * outerComponent = NULL;
public:
	Face(HalfEdge* innerComponent);
	void setInnerCommponent(HalfEdge* e);
	HalfEdge* getInnerCommponent();
};

class EdgeList
{
private:
	class HalfEdge* list[100];
	int size = 0;
public:
	EdgeList();
	void push_back(HalfEdge* E);
	HalfEdge* get(int i);
	int getSize();
	float* getVertexPairEdgesArray();
	void print();
};

class FaceList {
private:
	Face* list[100];
	int size = 0;
public:
	FaceList();
	void push_back(Face* f);
	Face* get(int i);
	int getSize();
};

class DCEL
{
private:
	VertexList* vertices = new VertexList();
	EdgeList* edges = new EdgeList();
	FaceList* faces = new FaceList();

public:
	DCEL(Coordinates vertices[], int n);
	VertexList* getVertexList();
	EdgeList* getHalfEdgeList();
	void addVertex(float x, float y);
	void connect(float v1[], float v2[]);
	int searchVertex(float x, float y);
	HalfEdge* searchFaceThatContains(Vertex* v); /* Enconta a face que contem v */
	HalfEdge* searchFaceThatContains(float x0, float y0);
	HalfEdge* searchFaceThatContains(Vertex* v1, Vertex* v2); /* Enconta a face que orbita v1 e contem v2 */
};