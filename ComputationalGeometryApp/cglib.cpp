#include "cglib.h"

int left(float p[], float q[], float r[]) {
	float val = (
			p[0] * q[1] + p[1] * r[0] + q[0] * r[1]
			- q[1] * r[0] - p[1] * q[0] - p[0] * r[1]
		);

	if (val >= 0) {
		return 1;
	}
	else {
		return -1;
	}
}

int faceContains(HalfEdge* e, Vertex* v)
{
	float point[2] = { v->getX(), v->getY() };
	int faceSize = 1; //quantidade de pontos da face, *2 = qntd de posições do vetor

	HalfEdge* origin = e;
	HalfEdge* aux = e->getNext();
	
	int N = 0;
	//contruindo a face
	while (aux != origin) {
		printf("aa\n");
		//Testando intersecção
		if (aux->getEnd()->getX() > point[0]) {
			if (aux->getEnd()->getY() == point[1]) //Se o vertice da face tem a mesma altura do ponto
			{
				if (aux->getNext()->getEnd()->getY() == point[1]) /* /------\ */
				{
					if (aux->getNext()->getNext()->getEnd()->getY() < point[1]) {
						N++;
					}

					if (aux->getPrev()->getEnd()->getY() < point[1]) {
						N++;
					}
				}
				else /* /\ */
				{
					if (aux->getNext()->getEnd()->getY() < point[1]) {
						N++;
					}

					if (aux->getPrev()->getEnd()->getY() < point[1]) {
						N++;
					}
				}
			}

			if (aux->getEnd()->getY() > point[1] && aux->getOrigin()->getY() < point[1]
				|| aux->getEnd()->getY() < point[1] && aux->getOrigin()->getY() > point[1])
			{
				N++;
			}
		}

		aux = aux->getNext();
	}

	if (aux->getEnd()->getX() > point[0]) {
		if (aux->getEnd()->getY() == point[1]) //Se o vertice da face tem a mesma altura do ponto
		{
			if (aux->getNext()->getEnd()->getY() == point[1]) /* /------\ */
			{
				if (aux->getNext()->getNext()->getEnd()->getY() < point[1]) {
					N++;
				}

				if (aux->getPrev()->getEnd()->getY() < point[1]) {
					N++;
				}
			}
			else /* /\ */
			{
				if (aux->getNext()->getEnd()->getY() < point[1]) {
					N++;
				}

				if (aux->getPrev()->getEnd()->getY() < point[1]) {
					N++;
				}
			}
		}

		if (aux->getEnd()->getY() > point[1] && aux->getOrigin()->getY() < point[1]
			|| aux->getEnd()->getY() < point[1] && aux->getOrigin()->getY() > point[1])
		{
			N++;
		}
	}


	if (N % 2 == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

int faceContains(HalfEdge* e, float x0, float y0)
{
	float point[3] = { x0, y0, 0 };
	float pontoForaDaTela[3] = { 3,y0, 0 };
	int faceSize = 1; //quantidade de pontos da face, *2 = qntd de posições do vetor

	HalfEdge* origine = e;
	HalfEdge* aux = e->getNext();

	int N = 0;
	//contruindo a face
	while (aux != origine) {
		float origin[3] = { aux->getOrigin()->getX(), aux->getOrigin()->getY(), 0 };
		float end[3] = { aux->getEnd()->getX(), aux->getEnd()->getY(), 0 };
		//Testando intersecção

			if (left(origin, end, point) == 1 && left(end, origin, pontoForaDaTela) == 1||
				left(origin, end, pontoForaDaTela) == 1 && left(end, origin, point) == 1) {
				
				if (left(point, pontoForaDaTela, origin) == 1 && left(pontoForaDaTela, point, end) ||
					left(pontoForaDaTela, point, origin) == 1 && left(point, pontoForaDaTela, end) == 1) {
					N++;
				}
			}


		//
		if (aux->getEnd()->getX() >= point[0]) {

			printf("WHILE!\n");
			if (aux->getEnd()->getY() == point[1]) //Se o vertice da face tem a mesma altura do ponto
			{
				if (aux->getNext()->getEnd()->getY() == point[1]) /* /------\ */
				{
					if (aux->getNext()->getNext()->getEnd()->getY() < point[1]) {
						N++;
					}

					if (aux->getPrev()->getEnd()->getY() < point[1]) {
						N++;
					}
				}
				else /* /\ */
				{
					if (aux->getNext()->getEnd()->getY() < point[1]) {
						N++;
					}

					if (aux->getPrev()->getEnd()->getY() < point[1]) {
						N++;
					}
				}
			}
		}
		else {

			printf("%f < %f \n", aux->getEnd()->getX(), point[0]);
		}

		aux = aux->getNext();
	}

	float origin[3] = { aux->getOrigin()->getX(), aux->getOrigin()->getY(), 0 };
		float end[3] = { aux->getEnd()->getX(), aux->getEnd()->getY(), 0 };
		//Testando intersecção

		if (left(origin, end, point) == 1 && left(end, origin, pontoForaDaTela) == 1 ||
			left(origin, end, pontoForaDaTela) == 1 && left(end, origin, point) == 1) {

			if (left(point, pontoForaDaTela, origin) == 1 && left(pontoForaDaTela, point, end) ||
				left(pontoForaDaTela, point, origin) == 1 && left(point, pontoForaDaTela, end) == 1) {
				N++;
			}
		}

	if (aux->getEnd()->getX() >= point[0]) {
		printf("%f > %f \n", aux->getEnd()->getX(), point[0]);
		if (aux->getEnd()->getY() == point[1]) //Se o vertice da face tem a mesma altura do ponto
		{
			if (aux->getNext()->getEnd()->getY() == point[1]) /* /------\ */
			{
				if (aux->getNext()->getNext()->getEnd()->getY() < point[1]) {
					N++;
				}

				if (aux->getPrev()->getEnd()->getY() < point[1]) {
					N++;
				}
			}
			else /* /\ */
			{
				if (aux->getNext()->getEnd()->getY() < point[1]) {
					N++;
				}

				if (aux->getPrev()->getEnd()->getY() < point[1]) {
					N++;
				}
			}
		}
	}

	printf("[N: %d]", N);

	if (N % 2 == 0) {
		return 0;
	}
	else {
		return 1;
	}
}
