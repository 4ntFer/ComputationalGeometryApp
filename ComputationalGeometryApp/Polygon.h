#pragma once

#include "DCEL.h"

using namespace std;

const int BUFFLEN = 100;

class Poligono
{
	private:
		float vertexArray[BUFFLEN];
		int len = 0;
		int vertexNumber = 0;

	public:
		Poligono() {}
		void addVertex(float x, float y, float z);
		float* getVertexArray();
		int getLen();
		int getVertexNumber();
		DCEL* getDCEL();
};
