#include "DCEL.h"
#include "cglib.h"

DCEL::DCEL(Coordinates v[], int n)
{	

	Vertex* auxOrigin = new Vertex(v[0], 0, (HalfEdge*)NULL);
	Vertex* auxEnd;
	Vertex * head_vertex = auxOrigin;

	HalfEdge* auxHalfEdge = new HalfEdge(); //Ponteiro que será modificado a cada Iteração
	HalfEdge* prevHalfEdge = NULL;
	HalfEdge* head_half_edge = auxHalfEdge; //Poteiro para a primeira aresta criada
	HalfEdge* auxTwin; //Ponteiro que será modificado a cada Iteração

	Face* face = new Face(auxHalfEdge);
	
	faces->push_back(face);

	vertices->push_back(auxOrigin);
	for (int i =1; i < n; i++) {
		edges->push_back(auxHalfEdge);

		auxEnd = new Vertex(v[i], i, auxHalfEdge);

		vertices->push_back(auxEnd);

		auxHalfEdge->setIncidentFace(face);

		auxHalfEdge->setEnd(auxEnd);
		auxHalfEdge->setPrev(prevHalfEdge);
		
		prevHalfEdge = auxHalfEdge;


		auxHalfEdge = new HalfEdge();

		prevHalfEdge->setNext(auxHalfEdge);
		auxOrigin = auxEnd;
		
	}

	edges->push_back(auxHalfEdge);
	auxHalfEdge->setPrev(prevHalfEdge);

	//Tratando a primeira aresta, já que é formado um ciclo
	head_vertex->setIncidentEdge(auxHalfEdge);
	auxHalfEdge->setEnd(head_vertex);
	head_half_edge->setPrev(auxHalfEdge);
	auxHalfEdge->setNext(head_half_edge);


	//Criando os twins
	prevHalfEdge = NULL;
	auxHalfEdge = head_half_edge;
	auxTwin = new HalfEdge();
	head_half_edge = auxTwin;
	for (int i = 0; i < n; i++) {
		auxTwin->setEnd(auxHalfEdge->getPrev()->getEnd());
		auxTwin->setTwin(auxHalfEdge);
		auxHalfEdge->setTwin(auxTwin);

		auxTwin->setPrev(prevHalfEdge);

		prevHalfEdge = auxTwin;
		auxTwin = new HalfEdge();
		auxTwin->setPrev(prevHalfEdge);
		prevHalfEdge->setNext(auxTwin);

		auxHalfEdge = auxHalfEdge->getPrev();
	}

	head_half_edge->setPrev(prevHalfEdge);
	prevHalfEdge->setNext(head_half_edge);

	//inicializando incident edge

	for (int i = 0; i < edges->getSize(); i++) {
		edges->get(i)->getOrigin()->setIncidentEdge(edges->get(i));
	}
}



VertexList* DCEL::getVertexList()
{
	return vertices;
}

EdgeList* DCEL::getHalfEdgeList()
{
	return edges;
}

void DCEL::addVertex(float x, float y)
{
	Vertex* v = new Vertex(
		x, y,
		getVertexList()->getSize(),
		(HalfEdge*)NULL
		);

	vertices->push_back(v);

	printf("\nnovo vétice criado!\n");
}

void DCEL::connect(float v1[], float v2[]){

	int indexOfVertex;

	Vertex* vo;
	Vertex* ve;


	indexOfVertex = searchVertex(v1[0], v1[1]);

	if (indexOfVertex >= 0) {
		vo = vertices->get(indexOfVertex);
		printf("\nOrigem selecionada: \n");
		vo->print();
		printf("\n");
	}
	else {
		printf("\nSelecione um vertice valido!\n");
		return (void)0;
	}

	indexOfVertex = searchVertex(v2[0], v2[1]);

	if (indexOfVertex >= 0) {
		ve = vertices->get(indexOfVertex);
		printf("\nDestino selecionado: \n");
		ve->print();
		printf("\n");
	}
	else {
		printf("\nSelecione um vertice valido!\n");
		return (void)0;
	}

	HalfEdge* prev;
	HalfEdge* aux;

	HalfEdge* newEdge = nullptr;

	if (ve->getIncidentEdge() == nullptr) {
		printf("\n\nentrou\n");

		newEdge = new HalfEdge();
		newEdge->setTwin(new HalfEdge());
		newEdge->getTwin()->setTwin(newEdge);
		edges->push_back(newEdge);
		edges->push_back(newEdge->getTwin());

		prev = searchFaceThatContains(ve)->searchHalfEdgeWithEnd(vo);
	
		aux = prev->getNext();

		newEdge->setEnd(ve);

		newEdge->getTwin()->setEnd(vo);

		newEdge->setPrev(prev);
		newEdge->setNext(newEdge->getTwin());
		newEdge->getTwin()->setPrev(newEdge);
		newEdge->getTwin()->setNext(aux);
		
		prev->setNext(newEdge);
		aux->setPrev(newEdge);

		ve->setIncidentEdge(newEdge->getTwin());
		newEdge->setIncidentFace(prev->getIncidentFace());
	}
	else if (vo->getIncidentEdge() == nullptr) {
		printf("\n\ninverteu\n");
		connect(v2, v1);
	}
	else {
		//encontra aresta com origem em Ve
		int found = 0;
		HalfEdge* next;

		prev = nullptr;
		next = nullptr;
		for (int i = 0; i < edges->getSize() && found == 0 ; i++) {
			aux = edges->get(i);

			if (aux->getOrigin() == ve) {
				prev = aux->searchHalfEdgeWithEnd(vo);
				if (prev != nullptr) {
					next = aux;
					found = 1;
				}
			}
		}

		if (found == 1) {
			newEdge = new HalfEdge();
			newEdge->setTwin(new HalfEdge());
			newEdge->getTwin()->setTwin(newEdge);
			edges->push_back(newEdge);

			if (prev->getNext() == prev->getTwin()) {
				newEdge->getTwin()->setNext(prev->getTwin());
				prev->getTwin()->setPrev(newEdge->getTwin());
			}
			else {
				newEdge->getTwin()->setNext(prev->getNext());
				prev->getNext()->setPrev(newEdge->getTwin());
			}

			if (next->getPrev() == next->getTwin()) {
				newEdge->getTwin()->setPrev(next->getTwin());
				next->getTwin()->setNext(newEdge->getTwin());
			}
			else {
				newEdge->getTwin()->setPrev(next->getPrev());
				next->getPrev()->setNext(newEdge->getTwin());
			}

			next->setPrev(newEdge);
			prev->setNext(newEdge);

			newEdge->setEnd(ve);
			newEdge->getTwin()->setEnd(vo);

			newEdge->setPrev(prev);
			newEdge->setNext(next);

			//atualizando faces
			HalfEdge* InnerOfNewFace = nullptr;
			for (int i = 0; i < faces->getSize(); i++) {

				InnerOfNewFace = nullptr;
				aux = faces->get(i)->getInnerCommponent();

				aux = aux->getNext();

				while (aux != faces->get(i)->getInnerCommponent()) {
					if (aux == newEdge) {
						faces->get(i)->setInnerCommponent(newEdge);
						InnerOfNewFace = faces->get(i)->getInnerCommponent()->getTwin(); //Nova face a ser incerida
					}

					aux = aux->getNext();
				}

				if (InnerOfNewFace != nullptr) {
					faces->push_back(new Face(InnerOfNewFace));
				}
			}
		}
		else {
			printf("\nArestas não encontradas!\n");
		}
	}


}

int DCEL::searchVertex(float x, float y)
{
	float interval = 0.04;
	for (int i = 0; i < vertices->getSize(); i++) {
		if (
			vertices->get(i)->getX() < x + interval
			&& vertices->get(i)->getX() > x - interval

			&& vertices->get(i)->getY() < y + interval
			&& vertices->get(i)->getY() > y - interval
			) {

			return i;
		}
	}

	return -1;
}

HalfEdge* DCEL::searchFaceThatContains(Vertex* v)
{
	int i;

	HalfEdge* aux;

	for (i = 0; i < faces->getSize(); i++) {
		printf("teste");
		aux = faces->get(i)->getInnerCommponent();
		
		if (faceContains(aux, v) == 1) {
			
			return aux;
		}
	}

	printf("RETORNA NULL");

	return nullptr;
}

HalfEdge* DCEL::searchFaceThatContains(float x0, float y0)
{
	int i;

	HalfEdge* aux;

	for (i = 0; i < faces->getSize(); i++) {
		aux = faces->get(i)->getInnerCommponent();

		if (faceContains(aux, x0, y0) == 1) {

			return aux;
		}
	}

	printf("RETORNA NULL");

	return nullptr;
}

HalfEdge::HalfEdge()
{
	
}

HalfEdge::HalfEdge(Vertex* v1, Vertex* v2)
{
	if (v1->getIncidentEdge() == NULL) {
	
	}
}

void HalfEdge::setEnd(Vertex* v)
{
	end = v;
}

void HalfEdge::setTwin(HalfEdge* e)
{
	twin = e;
}

void HalfEdge::setNext(HalfEdge* e)
{
	next = e;
}

void HalfEdge::setPrev(HalfEdge* e)
{
	prev = e;
}

void HalfEdge::setIncidentFace(Face* f)
{
	incidentFace = f;
}

Vertex* HalfEdge::getEnd()
{
	return end;
}

Vertex* HalfEdge::getOrigin()
{
	return getTwin()->getEnd();
}

HalfEdge* HalfEdge::getTwin()
{
	return twin;
}

HalfEdge* HalfEdge::getNext()
{
	return next;
}

HalfEdge* HalfEdge::getPrev()
{
	return prev;
}

Face* HalfEdge::getIncidentFace()
{
	return incidentFace;
}

float* HalfEdge::getVertexPairArray()
{
	float* array = (float*)malloc(sizeof(float) * 4);

	printf("%d - %d\n", getOrigin()->getKey(), getEnd()->getKey());

	array[0] = getOrigin()->getX();
	array[1] = getOrigin()->getY();
	array[2] = getEnd()->getX();
	array[3] = getEnd()->getY();

	return array;
}

HalfEdge* HalfEdge::searchHalfEdgeWithEnd(Vertex* v)
{
	if (this->getEnd() == v) {
		return this;
	}
	else {
		HalfEdge* aux = getNext();
		HalfEdge* origin = this;
		
		while (aux != origin) {
			if (aux->getEnd() == v) {
				return aux;
			}

			aux = aux->getNext();
		}

		if (aux->getEnd() == v) {
			return aux;
		}
	}

	return nullptr;
}

Vertex::Vertex(Coordinates coord, int identifier, HalfEdge* incident)
{
	coordinates = coord;
	key = identifier;
	incidentEdge = incident;
}

Vertex::Vertex(float x, float y, int identifier, HalfEdge* incident)
{
	Coordinates coord;
	coord.x = x;
	coord.y = y;

	coordinates = coord;
	key = identifier;
	incidentEdge = incident;
}

void Vertex::setIncidentEdge(HalfEdge* e)
{
	incidentEdge = e;
}

HalfEdge* Vertex::getIncidentEdge()
{	
	return incidentEdge;
}

float Vertex::getX()
{
	return coordinates.x;
}

float Vertex::getY()
{
	return coordinates.y;
}

int Vertex::getKey()
{
	return key;
}

void Vertex::print()
{
	printf("v%d: x = %f,  y = %f\n", key, getX(), getY());
}

VertexList::VertexList()
{
}

void VertexList::push_back(Vertex* v)
{
	list[size] = v;
	size++;
}

Vertex* VertexList::get(int i)
{
	return list[i];
}

int VertexList::getSize()
{
	return size;
}

void VertexList::print()
{
	for (int i = 0; i < size; i++) {
		printf(
			"v%d:\n\t: x = %f, y = %f\n\n",
			i, list[i]->getX(), list[i]->getY()
		);

	}
}

EdgeList::EdgeList()
{
}

void EdgeList::push_back(HalfEdge* E)
{
	list[size] = E;
	size++;
}

HalfEdge* EdgeList::get(int i)
{
	return list[i];
}

int EdgeList::getSize()
{
	return size;
}

float* EdgeList::getVertexPairEdgesArray()
{
	float* array = (float*)malloc(sizeof(float) * (size * 2) * 3);
	Vertex* aux;


	for (int i = 0; i < size; i++) {
		aux = get(i)->getOrigin();
		array[(i*2) * 3] = aux->getX();
		array[(i * 2) * 3 + 1] = aux->getY();
		array[(i * 2) * 3 + 2] = 0.0;
		aux = get(i)->getEnd();
		array[(i * 2)*3 + 3] = aux->getX();
		array[(i * 2) * 3 + 4] = aux->getY();
		array[(i * 2) * 3 + 5] = 0.0;
	}

	/*
	for (int i = 0; i <= size * 3; i += 3) {
		printf("x: %f, y: %f /", array[(i)], array[(i + 1)]);
	}
	printf("\n");
	*/

	return array;
}

void EdgeList::print()
{
	for (int i = 0; i < size; i++) {
		printf(
			"e%d:\n\torigin: x = %f, y = %f\n\tend: x = %f, y = %f\n\n", i,
		list[i]->getTwin()->getEnd()->getX(), list[i]->getTwin()->getEnd()->getY(),
		list[i]->getEnd()->getX(), list[i]->getEnd()->getY()
		);

	}
}

Face::Face(HalfEdge* innerComponent)
{
	this->innerComponent = innerComponent;
}

void Face::setInnerCommponent(HalfEdge* e)
{
	innerComponent = e;
}

HalfEdge* Face::getInnerCommponent()
{
	return innerComponent;
}

FaceList::FaceList()
{
}

void FaceList::push_back(Face* f)
{
	list[size] = f;
	size++;
}

Face* FaceList::get(int i)
{
	return list[i];
}

int FaceList::getSize()
{
	return size;
}
