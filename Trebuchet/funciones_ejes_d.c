#include <windows.h>	//Inclusion de variables de windows

#include <glut.h>	//Inclusion de GLUT
#include <GL/gl.h>		//Inclusion de GL		 	
#include <GL/glu.h>	
#include <stdio.h>
#include <math.h>	//Inclusion de librerias auxiliares	
#define INCREMENTO .05
#define DISTANCIA 2000
#define PI 3.1416

float alpha=0;
float beta =0;

int camara = 3;


void myCamara() {
		
		
	//Configuración de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//La ponemos auno
	glLoadIdentity();
	glOrtho(-2000.0,2000.0f,-2000.0f,2000.0f,-8000.0f,8000.0f);  
	gluLookAt(((float)DISTANCIA*(float) sin(alpha)*cos(beta)),((float)DISTANCIA*(float) sin(beta)), ((float)DISTANCIA*cos(alpha)*cos(beta)), 0,0,0,0,1,0);  


}

/**************************************
FUNCION DE CONTROL DE TECLAS ESPECIAIS:
**************************************
*/
 void myTeclasespeciales(int cursor,int x,int y) 
{
	switch(cursor) 
	{
//Traslaciones:
		case GLUT_KEY_F1:
			camara = 1;
	   		break;
		case GLUT_KEY_F2:
			camara = 2;
		break;
		case GLUT_KEY_F3:
			camara = 3;
		break;
		case GLUT_KEY_F4:
						break;
		case GLUT_KEY_F5:
			
		break;
		case GLUT_KEY_F6:
		
		break;
//Giros:
		case GLUT_KEY_UP:
			beta+=INCREMENTO;
			break;
		case GLUT_KEY_DOWN:
			beta-=INCREMENTO;
			break;
		case GLUT_KEY_RIGHT:
			alpha-=INCREMENTO;
			break;
		case GLUT_KEY_LEFT:
			alpha+=INCREMENTO;
			break;
		default:
			break;
	}

	if(alpha>=PI*2.0 && alpha<=0 ) alpha=0;
	if(beta>=PI*2.0 && beta<=0 ) beta=0; //hay que repasarlo para evitar el salto


		glutPostRedisplay();
}

 void myEjes() {



	 glColor3f(0.0f, 0.0f, 1.0f);
	 glBegin(GL_LINES);
	 glVertex3f(0.0f, 0.0f, 0.0f);
	 glVertex3f(2.0f, 0.0f, 0.0f);
	 glEnd();


	 glColor3f(1.0f, 0.0f, 0.0f);
	 glBegin(GL_LINES);
	 glVertex3f(0.0f, 0.0f, 0.0f);
	 glVertex3f(0.0f, 2.0f, 0.0f);
	 glEnd();


	 glColor3f(0.0f, 1.0f, 0.0f);
	 glBegin(GL_LINES);
	 glVertex3f(0.0f, 0.0f, 0.0f);
	 glVertex3f(0.0f, 0.0, 2.0f);
	 glEnd();

	 //Diagonal
	 glColor3f(1.0f, 1.0f, 1.0f);
	 glBegin(GL_LINES);
	 glVertex3f(0.0f, 0.0f, 0.0f);
	 glVertex3f(1.0f, 1.0f, 1.0f);
	 glEnd();

 }