// Bibliotecas utilizadas pelo OpenGL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Biblioteca com funcoes matematicas
#include <algorithm> 
#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstdlib>

# define PI 3.14159265359

using namespace std;

// Variaveis Globais
bool click1 = false, click2 = false, click3 = false, auxPoli=false;

double x_1,y_1,x_2,y_2,x_3,y_3,poli_x1,poli_y1;

int contClicks = 2, contPoli=0; 
int mode = 0;
/*
    Modos de uso:
    0 - Linhas
    1 - Triangulos
    2 - Quadrilateros
    3 - Circunferências
    4 - Poligono

*/

int width = 512, height = 512; //Largura e altura da janela

// Estrututa de dados para o armazenamento dinamico dos pontos
// selecionados pelos algoritmos de rasterizacao
struct ponto{
    int x;
    int y;
    ponto * prox;
};

// Lista encadeada de pontos
// indica o primeiro elemento da lista
ponto * pontos = NULL;

// Funcao para armazenar um ponto na lista
// Armazena como uma Pilha (empilha)
ponto * pushPonto(int x, int y){
	ponto * pnt;
	pnt = new ponto;
	pnt->x = x;
	pnt->y = y;
	pnt->prox = pontos;
	pontos = pnt;
	return pnt;
}

// Funcao para desarmazenar um ponto na lista
// Desarmazena como uma Pilha (desempilha)
ponto * popPonto(){
	ponto * pnt;
	pnt = NULL;
	if(pontos != NULL){
		pnt = pontos->prox;
		delete pontos;
		pontos = pnt;
	}
	return pnt;
}

// Declaracoes forward das funcoes utilizadas
void init(void);
void reshape(int w, int h);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

// Funcao que implementa o Algoritmo Imediato para rasterizacao de retas
void retaImediata(double x1,double y1,double x2,double y2);

// Função do breseham
void bresenham(double x1,double y1,double x2,double y2);
void bresenhamCircle(double x1,double y1,double x2,double y2);

// Funcao que percorre a lista de pontos desenhando-os na tela
void drawPontos();

// Funcoes de desenhos
void drawLine();
void drawTriangle();
void drawSquare();
void drawCircle();
void drawPoli();

// Transformações geométricas
void scale(double sx, double sy);
void shear(double cx, double cy);
void reflection(bool x, bool y);
void rotation(double angulo);
// Menu botao direito
void menuOptions(int op) {
    switch (op){
    case 0:
        cout << "Modo linha" << endl;
        contClicks = 2;
        mode = 0;
        break;
    case 1:
        cout << "Modo triangulo" << endl;
        contClicks = 3;
        mode = 1;
        break;
    case 2:
        cout << "Modo quadrado" << endl;
        contClicks = 2;
        mode = 2;
        break;
    case 3: 
        cout << "Modo circulo" << endl;
        contClicks = 2;
        mode = 3;
        break;
    case 4: 
        cout << "Modo poligono" << endl;
        cout << "Pressione F e clique com o botão esquerdo do mouse para fechar" << endl;
        contClicks = 2;
        contPoli=0; 
        mode = 4;
        break;
    case 5:
        cout << "Escala" << endl;
        contClicks = 1;
        mode = 5;
        break;
    case 6:
        cout << "Cisalhamento" << endl;
        contClicks = 1;
        mode = 6;
        break;
    case 7:
        cout << "Reflexao" << endl;
        contClicks = 1;
        mode = 7;
        break;
    case 8:
        cout << "Rotacao" << endl;
        contClicks = 1;
        mode = 8;
        break;
    }
    drawPontos();
}

// Funcao Principal do C
int main(int argc, char** argv){
    int menu;
    glutInit(&argc, argv); // Passagens de parametro C para o glut
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); // Selecao do Modo do Display e do Sistema de cor utilizado
    glutInitWindowSize (width, height);  // Tamanho da janela do OpenGL
    glutInitWindowPosition (100, 100); //Posicao inicial da janela do OpenGL
    glutCreateWindow ("Rasterizacao"); // Da nome para uma janela OpenGL
    init(); // Chama funcao init();
    glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
    glutKeyboardFunc(keyboard); //funcao callback do teclado
    glutMouseFunc(mouse); //funcao callback do mouse
    glutDisplayFunc(display); //funcao callback de desenho
    menu = glutCreateMenu(menuOptions);
    glutAddMenuEntry("Linha", 0);
    glutAddMenuEntry("Triangulo", 1);
    glutAddMenuEntry("Quadrado", 2);
    glutAddMenuEntry("Circulo", 3);
    glutAddMenuEntry("Poligono", 4);
    glutAddMenuEntry("Escala", 5);
    glutAddMenuEntry("Cisalhamento", 6);
    glutAddMenuEntry("Reflexao", 7);
    glutAddMenuEntry("Rotacao", 8);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop(); // executa o loop do OpenGL
    return 0; // retorna 0 para o tipo inteiro da funcao main();
}

// Funcao com alguns comandos para a inicializacao do OpenGL;
void init(void){
    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
}

void reshape(int w, int h)
{
	// Reinicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Definindo o Viewport para o tamanho da janela
	glViewport(0, 0, w, h);
	
	width = w;
	height = h;
    glOrtho (0, w, 0, h, -1 ,1);  

   // muda para o modo GL_MODELVIEW (n�o pretendemos alterar a projec��o
   // quando estivermos a desenhar na tela)
	glMatrixMode(GL_MODELVIEW);
    click1 = true; //Para redesenhar os pixels selecionados
    click2 = true;
    click3 = true;
}

// Funcao usada na funcao callback para utilizacao das teclas normais do teclado
void keyboard(unsigned char key, int x, int y){
    switch (key) { // key - variavel que possui valor ASCII da tecla precionada
        case 27: // codigo ASCII da tecla ESC
            exit(0); // comando pra finalizacao do programa
            break;
        case 'f':
            auxPoli = true;
            break;
        default:
            break;
    }
}

//Funcao usada na funcao callback para a utilizacao do mouse
void mouse(int button, int state, int x, int y){
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            if(click1 && !click2){
                click2 = true;
                x_2 = x;
                y_2 = height - y;
                printf("x2y2(%.0f, %.0f)\n",x_2,y_2);
                if(contClicks == 2)
                    glutPostRedisplay();
            }else if(click2){
                click3 = true;
                x_3 = x;
                y_3 = height - y;
                printf("x3y3(%.0f, %.0f)\n",x_3,y_3);
                if(contClicks == 3)
                    glutPostRedisplay();
            }else {
                click1 = true;
                x_1 = x;
                y_1 = height - y;
                printf("x1y1(%.0f, %.0f)\n",x_1,y_1);
                if(contClicks == 1)
                    glutPostRedisplay();
            }
         }
         break;
      default:
         break;
   }
}

// Funcao usada na funcao callback para desenhar na tela
void display(void){
    glClear(GL_COLOR_BUFFER_BIT); //Limpa o buffer de cores e reinicia a matriz
    glLoadIdentity();
    
    glColor3f (0.0, 0.0, 0.0); // Seleciona a cor default como preto
    
    cout << "mode: " << mode << endl;
    switch (mode){
        case 0:
            drawLine();
            break;
        case 1: 
            drawTriangle();
            break;
        case 2:
            drawSquare();
            break;
        case 3:
            drawCircle();
            break;
        case 4:
            drawPoli();
            break;
        case 5:
            scale(1.1, 1.1);
            break;
        case 6:
            shear(1.2, 0);
            break;
        case 7:
            reflection(true, false);
            break;
        case 8:
            rotation(10); 
            break;
        default:
            break;
    }
    
    glutSwapBuffers(); // manda o OpenGl renderizar as primitivas
}

void drawLine() {
    if(click1 && click2){
        bresenham(x_1,y_1,x_2,y_2);
        click1 = false;
        click2 = false;
    }
    drawPontos();
}

void drawTriangle() {
    if(click1 && click2 && click3){
        bresenham(x_1,y_1,x_2,y_2);
        bresenham(x_1,y_1,x_3,y_3);
        bresenham(x_2,y_2,x_3,y_3);
        click1 = false;
        click2 = false;
        click3 = false;
    }
    drawPontos();
}

void drawPontos(){
    ponto * pnt;
    pnt = pontos;
    glBegin(GL_POINTS); // Seleciona a primitiva GL_POINTS para desenhar
        while(pnt != NULL){
            glVertex2i(pnt->x,pnt->y);
            pnt = pnt->prox;
        }
    glEnd();  // indica o fim do desenho
}

void retaImediata(double x1,double y1,double x2,double y2){
    double m, b, yd, xd;
    double xmin, xmax,ymin,ymax;
    //Armazenando os extremos para desenho
    pontos = pushPonto((int)x1,(int)y1);
    pontos = pushPonto((int)x2,(int)y2);

    if(x2-x1 != 0){ //Evita a divis�o por zero
        m = (y2-y1)/(x2-x1);
        b = y1 - (m*x1);

        if(m>=-1 && m <= 1){ // Verifica se o declive da reta tem tg de -1 a 1, se verdadeira calcula incrementando x
            xmin = (x1 < x2)? x1 : x2;
            xmax = (x1 > x2)? x1 : x2;

            for(int x = (int)xmin+1; x < xmax; x++){
                yd = (m*x)+b;
                yd = floor(0.5+yd);
                pontos = pushPonto(x,(int)yd);
            }
        }else{ // Se menor que -1 ou maior que 1, calcula incrementado os valores de y
            ymin = (y1 < y2)? y1 : y2;
            ymax = (y1 > y2)? y1 : y2;

            for(int y = (int)ymin + 1; y < ymax; y++){
                xd = (y - b)/m;
                xd = floor(0.5+xd);
                pontos = pushPonto((int)xd,y);
            }
        }

    }else{ // se x2-x1 == 0, reta perpendicular ao eixo x
        ymin = (y1 < y2)? y1 : y2;
        ymax = (y1 > y2)? y1 : y2;
        for(int y = (int)ymin + 1; y < ymax; y++){
            pontos = pushPonto((int)x1,y);
        }
    }
}

void bresenham(double x1,double y1,double x2,double y2){
    double dx, dy, d, m, yd, xd, D=false, S=false;
    double xmin, xmax, ymin, ymax, incE, incNE;
    //Armazenando os extremos para desenho

    if(x2-x1 != 0){ //Evita a divis�o por zero
        dx = x2-x1;
        dy = y2-y1;

        if(dx*dy < 0){
            y1 *= -1;
            y2 *= -1;
            dx = x2-x1;
            dy = y2-y1;
            S = true;
        }

        if(abs(dy) > abs(dx)){
            swap(x1, y1);
            swap(x2, y2);
            dx = x2-x1;
            dy = y2-y1;
            D = true;
        }

        if(x1 > x2){
            swap(x1, x2);
            swap(y1, y2);
            dx = x2-x1;
            dy = y2-y1;
        }

        d = 2*dy - dx;

        xmin = (x1 < x2)? x1 : x2;
        xmax = (x1 > x2)? x1 : x2;
        ymin = (y1 < y2)? y1 : y2;
        ymax = (y1 > y2)? y1 : y2;
        
        incE = 2*dy;
        incNE = 2*(dy-dx);

        yd = floor(ymin);
        xd = floor(x1);
        int auxX, auxY;

        for(int x=(int)xmin; x <= xmax; x++){
            auxX = x;
            auxY = (int) yd;
            if(d <= 0){
                d += incE;
                if(D){
                    swap(auxX, auxY);
                }
                if(S){
                    auxY *= -1;
                }
                pontos = pushPonto(auxX,auxY);
            } else {
                d += incNE;
                yd++;
                if(D){
                    swap(auxX, auxY);
                }
                if(S){
                    auxY *= -1;
                }
                pontos = pushPonto(auxX,auxY);
            }
        }

    }else{ // se x2-x1 == 0, reta perpendicular ao eixo x
        ymin = (y1 < y2)? y1 : y2;
        ymax = (y1 > y2)? y1 : y2;
        for(int y = (int)ymin + 1; y < ymax; y++){
            pontos = pushPonto((int)x1,y);
        }
    }
}

void drawSquare(){
    if(click1 && click2){
        bresenham(x_1,y_1, x_1,y_2);
        bresenham(x_1,y_2, x_2,y_2);
        bresenham(x_2,y_2, x_2,y_1);
        bresenham(x_1,y_1, x_2,y_1);
        click1 = false;
        click2 = false;
    }
    drawPontos();
}

void drawCircle(){
    if(click1 && click2){
        bresenhamCircle(x_1,y_1, x_2,y_2);
        click1 = false;
        click2 = false;
    }
    drawPontos();
}

void bresenhamCircle(double x1,double y1,double x2,double y2){
    double dx, dy, d, m, yd, xd, r;
    double deltaE = 3, deltaSE;

    r = sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
    cout << r << " Raio " << y1-y2 << " " << x1-x2 << endl;

    deltaSE = -2*r + 5;

    dx = x2-x1;
    dy = y2-y1;

    d = 1 - r;

    int auxX, auxY;
    yd = r;
    for(int x = 0; x <= (int) r*0.72; x++){
        cout << "X: " << x <<" Y: " << yd << endl;
        auxX = x;
        auxY = yd;
        if(d < 0){
            d += deltaE;
            deltaE += 2; 
            deltaSE += 2; 
            pontos = pushPonto(auxX+x1,auxY+y1);
            pontos = pushPonto((int)auxY+x1, (int)auxX+y1);

            pontos = pushPonto(-auxX+x1,auxY+y1);
            pontos = pushPonto((int)-auxY+x1, (int)auxX+y1);

            pontos = pushPonto(auxX+x1,-auxY+y1);
            pontos = pushPonto((int)auxY+x1, (int)-auxX+y1);

            pontos = pushPonto(-auxX+x1,-auxY+y1);
            pontos = pushPonto((int)-auxY+x1, (int)-auxX+y1);
        } else {
            d += deltaSE;
            deltaE += 2; 
            deltaSE += 4; 
            yd--;
            pontos = pushPonto(auxX+x1,auxY+y1);
            pontos = pushPonto((int)auxY+x1, (int)auxX+y1);

            pontos = pushPonto(-auxX+x1,auxY+y1);
            pontos = pushPonto((int)-auxY+x1, (int)auxX+y1);

            pontos = pushPonto(auxX+x1,-auxY+y1);
            pontos = pushPonto((int)auxY+x1, (int)-auxX+y1);

            pontos = pushPonto(-auxX+x1,-auxY+y1);
            pontos = pushPonto((int)-auxY+x1, (int)-auxX+y1);
        }
    }
}

void drawPoli() {
    if(click1 && click2){
        if(contPoli == 0){
            poli_x1 = x_1;
            poli_y1 = y_1;
            cout << "ContPoli = " << poli_x1 << " " << poli_y1 << endl;
        }
        if(auxPoli){
            bresenham(poli_x1,poli_y1,x_1,y_1);
            click2 = false;
            click1 = false;
            contPoli = 0;
            auxPoli = false;
        }else{
            cout << "ContPoli = " << contPoli << endl;
            bresenham(x_1,y_1,x_2,y_2);
            swap(x_1, x_2);
            swap(y_1, y_2);
            click2 = false;
            contPoli++;
        }
    }
    drawPontos();
}

void scale(double sx, double sy) {
    ponto *pnt = pontos;
    if(click1){
        while(pnt != NULL){
            pnt->x = pnt->x * sx;
            pnt->y = pnt->y * sy;
            pnt = pnt->prox;
        }
        click1 = false;
    }
    drawPontos();
}

void shear(double cx, double cy) {
    ponto *pnt = pontos;
    if(click1){
        while(pnt != NULL){
            pnt->x = pnt->x + (cx* pnt->x);
            pnt->y = pnt->y + (cy* pnt->y);
            pnt = pnt->prox;
        }
        click1 = false;
    }
    drawPontos();
}

void reflection(bool x, bool y) {
    ponto *pnt = pontos;
    if(click1){
        while(pnt != NULL){
            if(x) {
                pnt->x = -pnt->x;
            }
            if(y) {
                pnt->y = -pnt->y;
            }
            pnt = pnt->prox;
        }
        click1 = false;
    }
    drawPontos();
}

void rotation(double angulo) {
    ponto *pnt = pontos;
    while(pnt != NULL){
        pnt->x = (pnt->x * cos(angulo*PI/180)) - (pnt->y * sin(angulo*PI/180));
        pnt->y = (pnt->x * sin(angulo*PI/180)) + (pnt->y * cos(angulo*PI/180));
        pnt = pnt->prox;
    }
    click1 = false;
    drawPontos();
}