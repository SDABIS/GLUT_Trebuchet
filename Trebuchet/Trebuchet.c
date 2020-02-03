
#include <windows.h>	//Inclusion de variables de windows

#include <glut.h>	//Inclusion de GLUT mira que la tienes ahi
#include <GL/gl.h>		//Inclusion de GL		 	
#include <GL/glu.h>	
#include <stdio.h>
#include <math.h>	//Inclusion de librerias auxiliares	

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int W_WIDTH = 500;	 //Ancho de la ventana
const int W_HEIGHT = 500;		//Alto de la ventana

#define GL_PI 3.14f
#define VELOCIDAD 0.25
#define FPS 60
double VELOCIDADMEDIDOR = 0.03;

// Ángulos de rotación para la cámara
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

GLfloat fAngulo;

GLuint texturaHierba;
GLuint texturaOtono;
GLuint texturaNieve;
GLuint texturaFlores;
GLuint texturaYo;

float tiempoInicial, tiempoFinal;

//Funciones externas
extern myCamara();

extern myEjes();

extern myEsfera();

extern void myTeclasespeciales(int cursor,int x,int y);

extern int camara;

//Definición de los estados en los que puede estar un trebuchet
#define INICIAL 0
#define GIRODER 1
#define GIROIZQ 2
#define CARGANDO 3
#define LANZADO 4
#define PARADO 5

int estado = INICIAL;

void dibujarSuelo(int inicio, int fin, GLenum luz);
void dibujarPosteLuz(int fin, GLenum luz);
//Funciones de movimiento
void girar();
void moverPiedra();
void aumentarMedidor();

void reiniciar();

//Creación de la estructura "estructura"
typedef struct {

	float posX;
	float posY;
	float posZ;
	float angRotZ;
	float aumentoRotZ;
	float tamX;
	float tamY;
	float tamZ;
	GLuint indiceLista;
	GLuint textura;

}estructura;

typedef struct {

	float inicioX;
	float inicioY;
	float inicioZ;
	float posX;
	float posY;
	float posZ;
	float anteriorX;
	float anteriorY;
	float direccionX;
	float direccionY;
	float angRotZ;
	float aumentoRotZ;
	float tamX;
	float tamY;
	float tamZ;
	float gravedad;
	float rozamiento;
	GLuint indiceLista;
	GLuint textura;

}municion;

typedef struct {

	float posX;
	float posY;
	float posZ;
	float incremento;
	float tamX;
	float tamY;
	GLuint indiceLista;

}medidor;

//Constantes
#define GR 0.0175
#define MYTIEMPO 41

//estructuras para dibujar el suelo
GLuint cuadrado;
GLuint cubo;
GLuint esfera;
int tamCuadSuelo = 1000;
int record = 0;

//Declaración de los estructuras

estructura base1 = { 0, 0, 20, 0, 0, 50, 100, 10, 0, 0 };
estructura base2 = { 0, 0, -20, 0, 0, 50, 100, 10, 0 };
estructura cuerdaAguante = { -60, 25, 0, 0, 0, 2, 50, 2, 0, 0 };
estructura unionBases = { 0, 50, 0, 0, 0, 5, 5, 39, 0, 0 };
estructura palanca = { 0, 0, 0, 0, 0, 60, 10, 10, 0, 0 };
estructura exPalanca = { -80, 0, 0, 0, 0, 100, 10, 10, 0, 0 };
estructura contrapeso = { 30, 0, 0, 0, 0, 20, 20, 20, 0, 0 };
estructura artCuerda = { -50, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
estructura cuerda = { 0, -20, 0, 0, 0, 2, 40, 2, 0, 0 };
//					ix  iy    iz px  py    pz ax ay dx dy aR vR tx ty tz gr     roz   li tx
municion piedra = { 0, -22.5, 0, 0, -22.5, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0.2, 0.35, 0, 0 };

medidor hueco = { 200, 20, 250, 0, 20, 150, 0 };
medidor barra = { -2, 0, 10, 0, 20, 150, 0 };

//Matriz de modelo
GLfloat matriz[16];
//Definición de cosas de la luz

GLfloat Ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat Diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat SpecRef[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat Specular[] = { 1.0f,1.0f,1.0f,1.0f };

GLfloat LuzPos[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat SpotDir[] = { 0.0f,-1.0f,0.0f };

void camaraLateral(estructura el) {
	//Matriz de proyección para cambiar la cámara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Propiedades de la cámara
	float ancho = (float)glutGet(GLUT_WINDOW_WIDTH);
	float largo = (float)glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(75.0f, ancho / largo, 0.01f, 5000.0f);
	//Posición y orientación
	//Está situada en la misma posición que el objeto (un poco más alto, porque se le suma 5 a la coordenada Z)
	//La dirección en la que mira depende del GIRODER del objeto, en los ejes X e Y
	gluLookAt(el.posX + 120, el.posY + 50, el.posZ + 410, el.posX + 120, el.posY, 0, 0, 1, 0);
}

void camaraLateralMunicion(municion el) {
	//Matriz de proyección para cambiar la cámara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Propiedades de la cámara
	float ancho = (float)glutGet(GLUT_WINDOW_WIDTH);
	float largo = (float)glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(75.0f, ancho / largo, 0.01f, 5000.0f);
	//Posición y orientación
	//Está situada en la misma posición que el objeto (un poco más alto, porque se le suma 5 a la coordenada Z)
	//La dirección en la que mira depende del GIRODER del objeto, en los ejes X e Y
	gluLookAt(el.posX + 120, el.posY + 50, el.posZ + 410, el.posX + 120, el.posY, 0, 0, 1, 0);
}

void camaraTrasera(estructura el) {
	//Matriz de proyección para cambiar la cámara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Propiedades de la cámara
	float ancho = (float)glutGet(GLUT_WINDOW_WIDTH);
	float largo = (float)glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(75.0f, ancho / largo, 0.01f, 5000.0f);

	//Posición y orientación
	//Está situada en la misma posición que el objeto (un poco más alto, porque se le suma 5 a la coordenada Z)
	//La dirección en la que mira depende del GIRODER del objeto, en los ejes X e Y
	gluLookAt(el.posX - 200, el.posY + 150, el.posZ, el.posX, 50, el.posZ, 0, 1, 0);
}

void camaraTraseraMunicion(municion el) {
	//Matriz de proyección para cambiar la cámara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Propiedades de la cámara
	float ancho = (float)glutGet(GLUT_WINDOW_WIDTH);
	float largo = (float)glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(75.0f, ancho / largo, 0.01f, 5000.0f);

	//Posición y orientación
	//Está situada en la misma posición que el objeto (un poco más alto, porque se le suma 5 a la coordenada Z)
	//La dirección en la que mira depende del GIRODER del objeto, en los ejes X e Y
	gluLookAt(el.posX - 200, el.posY + 150, el.posZ, el.posX, el.posY - 50, el.posZ, 0, 1, 0);
}

void primerPlanoMunicion(municion el) {
	//Matriz de proyección para cambiar la cámara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Propiedades de la cámara
	float ancho = (float)glutGet(GLUT_WINDOW_WIDTH);
	float largo = (float)glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(75.0f, ancho / largo, 0.01f, 5000.0f);

	//Posición y orientación
	//Está situada en la misma posición que el objeto (un poco más alto, porque se le suma 5 a la coordenada Z)
	//La dirección en la que mira depende del GIRODER del objeto, en los ejes X e Y
	gluLookAt(el.posX, el.posY, el.posZ + 50, el.posX, el.posY, el.posZ, 0, 1, 0);
}

// Funcion de dibuk
void myDisplay(void) {


	// Clear the window with current clearing color


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tiempoInicial = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	//Elegir qué cámara se va a usar
	switch (camara) {
	case 1:
		if ((estado == INICIAL) || (estado == GIRODER) || (estado == GIROIZQ) || (estado == CARGANDO)) {
			camaraLateral(unionBases);
		}
		else {
			camaraLateralMunicion(piedra);
		}
		break;
	case 2:

		if ((estado == INICIAL) || (estado == GIRODER) || (estado == GIROIZQ) || (estado == CARGANDO)) {
			camaraTrasera(unionBases);
		}
		else {
			camaraTraseraMunicion(piedra);
		}
		break;
	case 3:

		if ((estado == INICIAL) || (estado == GIRODER) || (estado == GIROIZQ) || (estado == CARGANDO)) {
			camaraLateral(unionBases);
		}
		else {
			primerPlanoMunicion(piedra);
		}
		
		break;
	}

	//myEjes();
	glColor3f(255, 255, 255);

	glMatrixMode(GL_MODELVIEW); //Matriz del Modelo
	glLoadIdentity(); // Inicializamos la matriz del modelo a la identidad

	//SUELO
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	dibujarPosteLuz(-200, GL_LIGHT0);

	dibujarSuelo(-2000, 4000, texturaHierba, GL_LIGHT1);

	dibujarSuelo(4000, 8000, texturaOtono, GL_LIGHT2);

	dibujarSuelo(8000, 12000, texturaNieve, GL_LIGHT3);

	dibujarSuelo(12000, 16000, texturaFlores, GL_LIGHT4);

	dibujarSuelo(16000, 20000, texturaYo, GL_LIGHT5);

	glColor3f(255, 255, 255);

	if (record != 0) {

		glPushMatrix();
		glColor3f(255, 0, 0);
		glTranslatef(record, 0, -800);
		glScalef(10, 1, 1600);
		glCallList(cuadrado);
		glPopMatrix();
	}
	glColor3f(255, 255, 255);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	/*glPushMatrix();
		glColor3f(0, 0, 255);
		glScalef(3000, 3000, 3000);
		glutSolidSphere(1, 80, 80);

	glPopMatrix();*/

	//BASE 1
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, base1.textura);
		glTranslatef(base1.posX, base1.posY, base1.posZ);

		glRotatef(base1.angRotZ, 0.0, 0.0, 1.0);

		glPushMatrix();
			glScalef(base1.tamX, base1.tamY, base1.tamZ);
			glCallList(base1.indiceLista);
		glPopMatrix();
	glPopMatrix();

	//BASE 2
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, base2.textura);
		glTranslatef(base2.posX, base2.posY, base2.posZ);

		glRotatef(base2.angRotZ, 0.0, 0.0, 1.0);

		glPushMatrix();
			glScalef(base2.tamX, base2.tamY, base2.tamZ);
			glCallList(base2.indiceLista);
		glPopMatrix();
	glPopMatrix();

	if (estado == INICIAL) {
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, cuerdaAguante.textura);
			glTranslatef(cuerdaAguante.posX, cuerdaAguante.posY, cuerdaAguante.posZ);

			glRotatef(cuerdaAguante.angRotZ, 0.0, 0.0, 1.0);

			glPushMatrix();
				glScalef(cuerdaAguante.tamX, cuerdaAguante.tamY, cuerdaAguante.tamZ);
				glCallList(cuerdaAguante.indiceLista);
			glPopMatrix();
		glPopMatrix();
	}
	//UNIÓN BASES
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, unionBases.textura);
		glTranslatef(unionBases.posX, unionBases.posY, unionBases.posZ);

		glRotatef(unionBases.angRotZ, 0.0, 0.0, 1.0);

		glPushMatrix();
			glScalef(unionBases.tamX, unionBases.tamY, unionBases.tamZ);
			glCallList(unionBases.indiceLista);
		glPopMatrix();

	//PALANCA
		glBindTexture(GL_TEXTURE_2D, palanca.textura);
		glTranslatef(palanca.posX, palanca.posY, palanca.posZ);

		glRotatef(palanca.angRotZ, 0.0, 0.0, 1.0);

		glPushMatrix();
			glScalef(palanca.tamX, palanca.tamY, palanca.tamZ);
			glCallList(palanca.indiceLista);
		glPopMatrix();

	//CONTRAPESO

		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, contrapeso.textura);
			glTranslatef(contrapeso.posX, contrapeso.posY, contrapeso.posZ);

			glRotatef(contrapeso.angRotZ, 0.0, 0.0, 1.0);

			glPushMatrix();
				glScalef(contrapeso.tamX, contrapeso.tamY, contrapeso.tamZ);
				glCallList(contrapeso.indiceLista);
			glPopMatrix();
		glPopMatrix();

		//EXTENSIÓN PALANCA

		glBindTexture(GL_TEXTURE_2D, exPalanca.textura);
		glTranslatef(exPalanca.posX, exPalanca.posY, exPalanca.posZ);

		glRotatef(exPalanca.angRotZ, 0.0, 0.0, 1.0);

		glPushMatrix();
			glScalef(exPalanca.tamX, exPalanca.tamY, exPalanca.tamZ);
			glCallList(exPalanca.indiceLista);
		glPopMatrix();

		//ARTICULACIÓN DE LA CUERDA
		//glBindTexture(GL_TEXTURE_2D, artCuerda.textura);
		glTranslatef(artCuerda.posX, artCuerda.posY, artCuerda.posZ);

		glRotatef(artCuerda.angRotZ, 0.0, 0.0, 1.0);

		if ((estado != LANZADO) && (estado != PARADO)) {
			glPushMatrix();
				glScalef(artCuerda.tamX, artCuerda.tamY, artCuerda.tamZ);
				glCallList(artCuerda.indiceLista);
			glPopMatrix();
		}

		//CUERDA
		glBindTexture(GL_TEXTURE_2D, cuerda.textura);
		glTranslatef(cuerda.posX, cuerda.posY, cuerda.posZ);

		glRotatef(cuerda.angRotZ, 0.0, 0.0, 1.0);

		if ((estado != LANZADO) && (estado != PARADO)) {
			glPushMatrix();
				glScalef(cuerda.tamX, cuerda.tamY, cuerda.tamZ);
				glCallList(cuerda.indiceLista);
			glPopMatrix();
		}

		//PIEDRA
		
		
		//Mientras la piedra no esté lanzada, la piedra hereda el movimiento del Trebuchet
		
		glBindTexture(GL_TEXTURE_2D, piedra.textura);
		//glColor3f(0, 0, 0);
		glTranslatef(piedra.inicioX, piedra.inicioY, piedra.inicioZ);

		glRotatef(piedra.angRotZ, 0.0, 0.0, 1.0);

		//Conseguir la posición de la piedra
		glGetFloatv(GL_MODELVIEW_MATRIX, matriz);

		if ((estado != LANZADO) && (estado != PARADO)) {
			glPushMatrix();
				glScalef(piedra.tamX, piedra.tamY, piedra.tamZ);
				glCallList(piedra.indiceLista);
			glPopMatrix();
		}

	glPopMatrix();

	//Una vez que se rompe la cuerda, la piedra se independiza del giro del Trebuchet
	if ((estado == LANZADO) || (estado == PARADO)) {
		glBindTexture(GL_TEXTURE_2D, piedra.textura);
		//glColor3f(0, 0, 0);
		glTranslatef(piedra.posX, piedra.posY, piedra.posZ);

		glRotatef(piedra.angRotZ, 0.0, 0.0, 1.0);

		glPushMatrix();
			glScalef(piedra.tamX, piedra.tamY, piedra.tamZ);
			glCallList(piedra.indiceLista);
		glPopMatrix();
	}

	if (estado == CARGANDO) {
		if (camara != 2) {

			//Dibujar el medidor: una barra dentro de un hueco
			glPushMatrix();
				//glColor3f(0, 0, 0);
				glBindTexture(GL_TEXTURE_2D, 0);

				glTranslatef(hueco.posX, hueco.posY, hueco.posZ);
				//glPushMatrix();

				//glScalef(hueco.tamX, hueco.tamY, 1);
				//glCallList(hueco.indiceLista);
				//glPopMatrix();

				//Para cuadrar ambas barras
				glTranslatef(0, 0, 1);

				glColor3f(0, 0, 255);
				glPushMatrix();
					glScalef(barra.tamX, barra.tamY * barra.incremento, 1);
					glCallList(barra.indiceLista);
				glPopMatrix();
			glPopMatrix();
		}
		
	}

	tiempoFinal = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	if ((tiempoFinal - tiempoInicial) < (1000 / FPS)) {
		Sleep((1000 / FPS) - (tiempoFinal - tiempoInicial));
	}

	//Llamadas a las funciones de movimiento
	switch (estado) {
	case GIRODER:
		girar();
		break;

	case CARGANDO:
		aumentarMedidor();
		break;

	case LANZADO:
		moverPiedra();
		girar();
		break;

	case GIROIZQ:
		girar();
	}

	glFlush();
	glutSwapBuffers();
		
}	

void dibujarSuelo(int inicio, int fin, GLuint textura, GLenum luz) {
	glBindTexture(GL_TEXTURE_2D, textura);
	
	for (int i = inicio; i < fin; i += tamCuadSuelo) {
		for (int j = -1000; j < 1000; j += tamCuadSuelo) {

			glPushMatrix();
			glTranslatef(i, -5, j);
			glScalef(tamCuadSuelo, 10, tamCuadSuelo);
			glCallList(cuadrado);
			glPopMatrix();

		}
	}

	dibujarPosteLuz(fin, luz);
}

void dibujarPosteLuz(int posX, GLenum luz) {
	glBindTexture(GL_TEXTURE_2D, base1.textura);
	glPushMatrix();
		glTranslatef(posX, 75, 50);
		glPushMatrix();
			glScalef(20, 150, 20);
			glCallList(cubo);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
		//glColor3f(255, 255, 0);
		glTranslatef(0, 85, 0);
		glPushMatrix();
			glScalef(20, 20, 20);
			glCallList(esfera);
		glPopMatrix();

		glLightfv(luz, GL_AMBIENT, Ambient);
		glLightfv(luz, GL_DIFFUSE, Diffuse);
		glLightfv(luz, GL_SPECULAR, Specular);
		glLightfv(luz, GL_POSITION, LuzPos);
		glLightfv(luz, GL_SPOT_DIRECTION, SpotDir);

		glLightf(luz, GL_SPOT_CUTOFF, 90.0f);
		glLightf(luz, GL_SPOT_EXPONENT, 1.0f);

		glEnable(luz);
		glEnable(GL_LIGHTING);

		glMaterialfv(GL_FRONT, GL_SPECULAR, SpecRef);
		glMateriali(GL_FRONT, GL_SHININESS, 1);

	glPopMatrix();
	glColor3f(255, 255, 255);

}

//Teclas de movimiento. WASD mueven la articulación inferior y IJKL la articulación central
//TFGH mueven la base
void myTeclado(unsigned char tecla, int x, int y)
{
	switch (tecla) {
	case ' ':
		switch(estado) {
		
		case INICIAL:
			estado = GIRODER;
			break;

		case GIRODER:
			estado = CARGANDO;
			piedra.direccionX = (piedra.posX - piedra.anteriorX) * 1.75;
			piedra.direccionY = (piedra.posY - piedra.anteriorY) * 1.75;

			piedra.angRotZ = palanca.angRotZ;

			break;

		case CARGANDO:
			estado = LANZADO;

			piedra.direccionX = piedra.direccionX * barra.incremento * 2.25;
			break;

		case PARADO:
			estado = INICIAL;

			reiniciar();
			break;
		}


		break;
	default:
		break;
	}

}

void reiniciar() {
	piedra.posX = piedra.inicioX;
	piedra.posY = piedra.inicioY;

	palanca.angRotZ = 0;
	palanca.aumentoRotZ = 0;

	artCuerda.angRotZ = 0;
	artCuerda.aumentoRotZ = 0;

	piedra.angRotZ = 0;
}

void girar() {
	if ((estado == GIRODER) || (estado == LANZADO)) {
		palanca.aumentoRotZ = sin(fabs(palanca.angRotZ) * GR) * 5 + VELOCIDAD;
		palanca.angRotZ -= palanca.aumentoRotZ;

		artCuerda.aumentoRotZ = sin(fabs(palanca.angRotZ) * GR) * 5 + VELOCIDAD;
		artCuerda.angRotZ -= artCuerda.aumentoRotZ;
	}

	else if (estado == GIROIZQ) {
		palanca.aumentoRotZ = sin(fabs(palanca.angRotZ) * GR) * 5 + VELOCIDAD;
		palanca.angRotZ += palanca.aumentoRotZ;

		artCuerda.aumentoRotZ = sin(fabs(palanca.angRotZ) * GR) * 5 + VELOCIDAD;
		artCuerda.angRotZ += artCuerda.aumentoRotZ;
	}

	//Se guarda la posición de la piedra para cuando se lance
	if (estado == GIRODER) {
		piedra.anteriorX = piedra.posX;
		piedra.anteriorY = piedra.posY;
		piedra.posX = matriz[12];
		piedra.posY = matriz[13];
	}

	if (palanca.angRotZ <= -180) {
		palanca.angRotZ = -180;
		if (estado == GIRODER) {
			estado = GIROIZQ;

			palanca.aumentoRotZ = 0;
			artCuerda.aumentoRotZ = 0;
		}
	}

	if (palanca.angRotZ >= 0) {
		palanca.angRotZ = 0;
		if (estado == GIROIZQ) {
			estado = GIRODER;

			palanca.aumentoRotZ = 0;
			artCuerda.aumentoRotZ = 0;
		}
	}
}

void moverPiedra() {
	piedra.posX += piedra.direccionX;
	piedra.posY += piedra.direccionY;

	piedra.angRotZ -= piedra.direccionX;

	if (piedra.posY > (piedra.tamY/2)) {
		piedra.direccionY -= piedra.gravedad;
	}
	else {
		piedra.posY = piedra.tamY/2;
		if (piedra.direccionX > 0) {
			piedra.direccionX -= piedra.rozamiento;
		}
		else {
			estado = PARADO;

			if(piedra.posX >= record) {
				record = piedra.posX;
			}
		}
	}
}

void aumentarMedidor() {
	barra.incremento += VELOCIDADMEDIDOR;
	if (barra.incremento >= 1) VELOCIDADMEDIDOR = -0.03;
		
	if (barra.incremento <= 0) VELOCIDADMEDIDOR = 0.03;
}

void Idle() {
	glutPostRedisplay();
}

GLuint myCuadrado() {

	GLuint indice;

	indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);
	glBegin(GL_TRIANGLES);

	glTexCoord2f(1.0f, 1.00000f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.00000f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.00000f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glTexCoord2f(0.0f, 1.00000f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.00000f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.00000f);
	glVertex3f(0.0f, 0.0f, 1.0f);



	glEnd();
	glEndList();
	return indice;
}

GLuint myCuadradoVert() {

	GLuint indice;

	indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);
	glBegin(GL_TRIANGLES);

	glTexCoord2f(1.0f, 1.00000f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.00000f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.00000f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.00000f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.00000f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.00000f);
	glVertex3f(1.0f, 0.0f, 0.0f);



	glEnd();
	glEndList();
	return indice;
}

GLuint myTrianguloEq() {
	GLuint indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);
	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 0.00000f);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.00000f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.00000f);
	glVertex3f(0.0f, 0.866f, 0.0f);

	glEnd();
	glEndList();

	return indice;
}

GLuint myCubo() {
	GLuint indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);
		glBegin(GL_TRIANGLES);


		//Cara izquierda
		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(.5f, -.5f, .5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(.5f, .5f, .5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(-.5f, -.5f, .5f);

		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(.5f, .5f, .5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(-.5f, .5f, .5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(-.5f, -.5f, .5f);

		//Cara frontal
		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(.5f, .5f, -.5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(.5f, -.5f, .5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(.5f, -.5f, -.5f);

		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(.5f, .5f, -.5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(.5f, .5f, .5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(.5f, -.5f, .5f);

		//Cara derecha
		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(.5f, -.5f, -.5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(-.5f, -.5f, -.5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(-.5f, .5f, -.5f);

		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(-.5f, .5f, -.5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(.5f, .5f, -.5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(.5f, -.5f, -.5f);

		//Cara trasera
		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(-.5f, -.5f, -.5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(-.5f, -.5f, .5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(-.5f, .5f, .5f);

		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(-.5f, .5f, .5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(-.5f, .5f, -.5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(-.5f, -.5f, -.5f);

		//Cara superior
		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(.5f, .5f, .5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(.5f, .5f, -.5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(-.5f, .5f, -.5f);

		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(-.5f, .5f, -.5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(-.5f, .5f, .5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(.5f, .5f, .5f);

		//Cara inferior
		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(.5f, -.5f, .5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(-.5f, -.5f, .5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(-.5f, -.5f, -.5f);

		glTexCoord2f(0.0f, 0.00000f);
		glVertex3f(-.5f, -.5f, -.5f);
		glTexCoord2f(1.0f, 1.00000f);
		glVertex3f(.5f, -.5f, -.5f);
		glTexCoord2f(1.0f, 0.00000f);
		glVertex3f(.5f, -.5f, .5f);

		glEnd();
	glEndList();

	return indice;
}

void Carga_Texturas(GLuint *textura, char* nombreImagen) {
	// load and create a texture 
	// -------------------------

	glGenTextures(1, textura);
	glBindTexture(GL_TEXTURE_2D, *textura); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load(nombreImagen, &width, &height, &nrChannels, 3);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); //con mimap 

	}
	else
	{
		//std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);



}


int main(int argc, char **argv) {

	glutInit(&argc, argv);
	
	glutInitWindowPosition(50,50);  
	glutInitWindowSize (W_WIDTH, W_HEIGHT);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("EL TREBUCHET");
	
	glClearDepth( 1.0f );
    glClearColor(0.4f,0.4f,1.0f,1.0f);
	
	glEnable(GL_CULL_FACE); // Habilita la ocultacion de caras
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	//Inicializar texturas

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	GLuint texturaTreb;
	GLuint texturaCuerda;
	GLuint texturaPiedra;

	Carga_Texturas(&texturaHierba, "hierba.jpg");
	Carga_Texturas(&texturaTreb, "madera.jpg");
	Carga_Texturas(&texturaCuerda, "cuerda.jpg");
	Carga_Texturas(&texturaPiedra, "piedra.jpg");
	Carga_Texturas(&texturaOtono, "otoño.jpg");
	Carga_Texturas(&texturaNieve, "nieve.jpg");
	Carga_Texturas(&texturaFlores, "flores.jpg");
	Carga_Texturas(&texturaYo, "yo.jpg");


	//Eventos comentado hasta la siguiente practica
	glutKeyboardFunc(myTeclado);
	glutSpecialFunc(myTeclasespeciales);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(Idle);
	
	cuadrado = myCuadrado();
	cubo = myCubo();
	esfera = myEsfera();

	base1.indiceLista = myTrianguloEq();
	base2.indiceLista = myTrianguloEq();
	cuerdaAguante.indiceLista = myCubo();
	unionBases.indiceLista = myCubo();
	palanca.indiceLista = myCubo();
	exPalanca.indiceLista = myCubo();
	contrapeso.indiceLista = myCubo();
	artCuerda.indiceLista = myEsfera();
	cuerda.indiceLista = myCubo();
	piedra.indiceLista = myEsfera();
	hueco.indiceLista = myCuadradoVert();
	barra.indiceLista = myCuadradoVert();

	base1.textura = texturaTreb;
	base2.textura = texturaTreb;
	cuerdaAguante.textura = texturaCuerda;
	unionBases.textura = texturaTreb;
	palanca.textura = texturaTreb;
	exPalanca.textura = texturaTreb;
	contrapeso.textura = texturaTreb; 
	artCuerda.textura = texturaCuerda;
	cuerda.textura = texturaCuerda;
	piedra.textura = texturaPiedra;

	// Empieza en bucle principal
	glutMainLoop();
	return 0; 
}