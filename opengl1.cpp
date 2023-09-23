
#include <iostream>
#include <stdio.h>
#include <math.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GL/glut.h>

const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 600;

const int BUFFLEN = 20 * 3;

unsigned int VBO;

float calculaAreaParalelogramo(float u[], float v[]) {
    //A raiz quadrada do quadrado do produto vetorial dos 2 velores/27
    return sqrt(
        (u[1] * v[2] - u[2] * v[1]) * (u[1] * v[2] - u[2] * v[1]) +
        (u[2] * v[0] - u[0] * v[2]) * (u[2] * v[0] - u[0] * v[2]) +
        (u[0] * v[1] - u[1] * v[0]) * (u[0] * v[1] - u[1] * v[0])
    );
}

float calculaArealTriangulo(float p[], float q[], float r[]) {

    float v[3] = { q[0] - p[0],  q[1] - p[1], q[2] - p[2] };
    float u[3] = { r[0] - p[0],  r[1] - p[1], r[2] - p[2] };

    return calculaAreaParalelogramo(v, u) / 2;

}

float calculaAreaTrianguloDet(float p[], float q[], float r[]) {
    return(
        p[0] * q[1] + p[1] * r[0] + q[0] * r[1]
        - q[1] * r[0] - p[1] * q[0] - p[0] * r[1]
        ) / 2;
}


float calculaProdutoInterno(float u[], float v[]) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

float calculaNorma(float u[]) {
    return sqrt(
        calculaProdutoInterno(u, u)
    );
}

//existem erros nessa função, mas  não sei qual é
double calculaAngulo(float u[], float v[]) {
    return acos(
        (double)(calculaProdutoInterno(u, v) / calculaNorma(u) * calculaNorma(v))
    );
}

int orientation(float p[], float q[], float r[]) {
    float val = (
        p[0] * q[1] + p[1] * r[0] + q[0] * r[1]
        - q[1] * r[0] - p[1] * q[0] - p[0] * r[1]
        );


    if (val > 0) {
        return 1;
    }
    else {
        return -1;
    }

    return 0;
}

class Dots {
private:
    float vertices[BUFFLEN];
    int len = 0; //numero de vertices

    void createVertexBuffer() {

        /*
            Não sei se está correto substituir o buffer
            desta forma. A memória usada pelos buffers anteriores
            será liberada?
        */

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

public:

    Dots() {
    }

    void addVertice(float x, float y, float z) {
        vertices[len * 3] = x;
        vertices[len * 3 + 1] = y;
        vertices[len * 3 + 2] = z;
        len++;
        

        createVertexBuffer();

        mostra_area_e_orientacao();
        if (len == 3) {
            float p[3] = {vertices[0], vertices[1], vertices[2]};
            float q[3] = { vertices[3], vertices[4], vertices[5] };
            float r[3] = { vertices[6], vertices[7], vertices[8] };
            printf("area do triangulo: %f\n", calculaAreaTrianguloDet(p,q,r));
        }

        if (len >= 3) {

            int orelha = verificaOrelha(len - 1);
            if (orelha == 1) {
                printf("vertice adicionado eh uma orelhas\n");
            }
            else {
                printf("vertice adicionado nao eh uma orelha\n");
            }
            convexidadeDoVertice(len-1);
        }

    }

    void renderDots() {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glEnableVertexAttribArray(0);

        glDrawArrays(GL_POINTS, 0, len);

        glDisableVertexAttribArray(0);

        glutSwapBuffers();
    }

    void renderTriangles() {

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glEnableVertexAttribArray(0);


        glDrawArrays(GL_TRIANGLES, 0, len);


        glDisableVertexAttribArray(0);

        glutSwapBuffers();
    }

    void renderPolygon() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glEnableVertexAttribArray(0);

        glDrawArrays(GL_LINE_LOOP, 0, len);

        glDisableVertexAttribArray(0);
        glutSwapBuffers();
    }

    float calculaAreaPoligono() {

        //Essa função não está fazendo o calculo correto, não como corrigir

        float externalDOT[] = { 0,-2,0 };
        float area = 0;

        for (int i = 0; i < len - 1; i += 1) {
            float q[3] = { vertices[i*3], vertices[i*3 + 1], vertices[i*3 + 2] };
            float r[3] = { vertices[i*3 + 3], vertices[i * 3 + 4 ], vertices[i * 3 + 5]};

            area += calculaAreaTrianguloDet(externalDOT, q, r);

        }


        float q[3] = { vertices[((len -1)*3)], vertices[((len-1)*3) +1], vertices[((len-1)*3) +2]};
        float r[3] = { vertices[0], vertices[1], vertices[2] };

        area += calculaAreaTrianguloDet(externalDOT, q, r);

        return area;

    }

    void mostra_area_e_orientacao() {
        float area = calculaAreaPoligono();
        float areaNormalizada = area;

        if (areaNormalizada < 0) {

            printf("poligono horario\n");

            areaNormalizada *= -1;
        }
        else {
            printf("poligono anti-horario\n");
        }

        printf("area  do poligono %f\n", areaNormalizada);

    }

    //Recebe qualquer verice no intervalo entre 0 e len-1
    void convexidadeDoVertice(int i) {
        if (i < len) {
            int ori;
            
            if (i == 0) {
                float p[3] = { vertices[(len - 1) * 3], vertices[(len - 1) * 3 + 1], vertices[(len - 1) * 3 + 2] };
                float q[3] = { vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2] };
                float r[3] = { vertices[(i + 1) * 3], vertices[(i + 1) * 3 + 1], vertices[(i + 1) * 3 + 2] };

                ori = orientation(p, q, r);
            }
            else if (i == len - 1) {

                float p[3] = { vertices[(i - 1) * 3], vertices[(i - 1) * 3 + 1], vertices[(i - 1) * 3 + 2] };
                float q[3] = { vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2] };
                float r[3] = { vertices[0], vertices[1], vertices[2] };

                ori = orientation(p, q, r);
            }
            else {
                float p[3] = { vertices[(i - 1) * 3], vertices[(i - 1) * 3 + 1], vertices[(i - 1) * 3 + 2] };
                float q[3] = { vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2] };
                float r[3] = { vertices[(i + 1) * 3], vertices[(i + 1) * 3 + 1], vertices[(i + 1) * 3 + 2] };

                ori = orientation(p, q, r);
            }

            //se a orientação é horária
            if (ori == 1) {

                if (calculaAreaPoligono() > 0) {
                    printf("vertice convexo\n");
                }
                else {

                    printf("vertice concavo\n");
                }
            
            }
            //se a orintação é anti-horária
            else {

                if (calculaAreaPoligono() > 0) {
                    printf("vertice concavo\n");
                }
                else {

                    printf("vertice convexo\n");
                }
            }
            
        }
    }

    int verificaDiagonal(float* p, float* q) {
        float p1[3];
        float q1[3];
     
        for (int i = 0; i < len; i++) {
            p1[0] = vertices[i * 3]; p1[1] = vertices[i * 3 + 1]; p1[2] = vertices[i*3 + 2];

            if (i + 1 < len) {
                
                q1[0] = vertices[(i + 1) * 3]; q1[1] = vertices[(i + 1) * 3 + 1]; q1[2] = vertices[(i + 1) * 3 + 2];
            }
            else {
                q1[0] = vertices[0]; q1[1] = vertices[1]; q1[2] = vertices[2];
            }

            if (orientation(p, q, p1) != orientation(p, q, q1)){
                if (orientation(p1, q1, p) != orientation(p1, q1, q)) {
                    return -1;
                }   
            }
        }

        return 1;
    }

    int verificaOrelha(int i) {
        if (i < len) {
            int ori;

            float pDiagonal[3];
            float qDiagonal[3];

            if (i == 0) {
                float p[3] = { vertices[(len - 1) * 3], vertices[(len - 1) * 3 + 1], vertices[(len - 1) * 3 + 2] };
                float q[3] = { vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2] };
                float r[3] = { vertices[(i + 1) * 3], vertices[(i + 1) * 3 + 1], vertices[(i + 1) * 3 + 2] };

                pDiagonal[0] = p[0]; pDiagonal[1] = p[1]; pDiagonal[2] = p[2];
                qDiagonal[0] = r[0]; qDiagonal[1] = r[1]; qDiagonal[2] = r[2];

                ori = orientation(p, q, r);

                if (ori == 1) {
                    for (int j = 0; j < len; j++) {
                        float vertice[3] = {vertices[j*3], vertices[j*3+1], vertices[j*3 + 2]};
                 
                        if (
                            orientation(p, q, vertice) == 1 &&
                            orientation(q, r, vertice) == 1 &&
                            orientation(r, p, vertice) == 1
                            ) {

                            

                            printf("(encontrou um ponto interno %f, %f, %f)   ", vertice[0], vertice[1], vertice[2]);
                            return -1;
                        }
                    }
                }
                else {
                    return -1;
                }
            }
            else if (i == len - 1) {

                float p[3] = { vertices[(i - 1) * 3], vertices[(i - 1) * 3 + 1], vertices[(i - 1) * 3 + 2] };
                float q[3] = { vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2] };
                float r[3] = { vertices[0], vertices[1], vertices[2] };

                pDiagonal[0] = p[0]; pDiagonal[1] = p[1]; pDiagonal[2] = p[2];
                qDiagonal[0] = r[0]; qDiagonal[1] = r[1]; qDiagonal[2] = r[2];

                ori = orientation(p, q, r);

                if (ori == 1) {
                    for (int j = 0; j < len; j++) {
                        float vertice[3] = { vertices[j * 3], vertices[j * 3 + 1], vertices[j * 3 + 2] };

                        if (
                            orientation(p, q, vertice) == 1 &&
                            orientation(q, r, vertice) == 1 &&
                            orientation(r, p, vfertice) == 1
                            ) {

                            printf("(encontrou um ponto interno)   ");
                            return -1;
                        }
                    }
                }
                else {
                    return -1;
                }
            }
            else {
                float p[3] = { vertices[(i - 1) * 3], vertices[(i - 1) * 3 + 1], vertices[(i - 1) * 3 + 2] };
                float q[3] = { vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2] };
                float r[3] = { vertices[(i + 1) * 3], vertices[(i + 1) * 3 + 1], vertices[(i + 1) * 3 + 2] };

                pDiagonal[0] = p[0]; pDiagonal[1] = p[1]; pDiagonal[2] = p[2];
                qDiagonal[0] = r[0]; qDiagonal[1] = r[1]; qDiagonal[2] = r[2];

                ori = orientation(p, q, r);

                if (ori == 1) {
                    for (int j = 0; j < len; j++) {
                        float vertice[3] = { vertices[j * 3], vertices[j * 3 + 1], vertices[j * 3 + 2] };

                        if (
                            orientation(p, q, vertice) == 1 &&
                            orientation(q, r, vertice) == 1 &&
                            orientation(r, p, vertice) == 1
                            ) {
                            return -1;
                        }
                    }
                }
                else {
                    return -1;
                }
            }

            return 1;
        }
    }

};

Dots* DOTS;



void onClick(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {

        float normalizedX = (float)x / (float)WIN_WIDTH * 2 - 1;
        float normalizedY = ((float)y / (float)WIN_HEIGHT * 2 - 1) * (-1);

        DOTS->addVertice(normalizedX, normalizedY, 0);
    }
}

void render() {
    DOTS->renderDots();
    DOTS->renderPolygon();
}

int main(int argc, char* argv[])
{

    Dots* dots = new Dots();

    /*
        Estou usando uma variavel global para
        que as funções de callback possam acessar
        as funções da classe, porem isso não me parece correto.
    */
    DOTS = dots;

    glutInit(&argc, argv);

    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);

    glutCreateWindow("OpenGL me da medo");

    glewInit();



    glutDisplayFunc(render);

    glutMouseFunc(onClick);

    glutMainLoop();


    return 0;
}
