// GD-Praktikum:   teil_1.cpp  (Teil 1: Start-Programm)
// Hergenroether / Groch    Last Update: 29.07.2011

#define _USE_MATH_DEFINES

#include <iostream> 
#include <GL/freeglut.h>         //lädt alles für OpenGL
#include "wuerfel.h"
#include <math.h>

float fRotation = 315.0;

void Init()	
{
   // Hier finden jene Aktionen statt, die zum Programmstart einmalig 
   // durchgeführt werden müssen
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

void Zylinder(float height, float resolution, float radius, float color [4])
{
	resolution = 1 / resolution;
	float i = 0;
	glColor4f(color[0], color[1], color[2], color[3]);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, height, 0);  /* center */
	for (i = 0; i <= 2 * M_PI; i += resolution)
		glVertex3f(radius * cos(i), height, radius * sin(i));
	glVertex3f(radius * cos(2 * M_PI), height, radius * sin(2 * M_PI));
	glEnd();

	/* bottom triangle: note: for is in reverse order */
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);  /* center */
	for (i = 2 * M_PI; i >= 0; i -= resolution)
		glVertex3f(radius * cos(i), 0, radius * sin(i));
	glVertex3f(radius * cos(0), height, radius * sin(0));
	/* close the loop back to 0 degrees */
	glVertex3f(radius, height, 0);
	glEnd();

	/* middle tube */
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= 2 * M_PI; i += resolution)
	{
		glVertex3f(radius * cos(i), 0, radius * sin(i));
		glVertex3f(radius * cos(i), height, radius * sin(i));
	}
	glVertex3f(radius * cos(2 * M_PI), 0, radius * sin(2 * M_PI));
	glVertex3f(radius * cos(2 * M_PI), height, radius * sin(2 * M_PI));
	/* close the loop back to zero degrees */
	glVertex3f(radius, 0, 0);
	glVertex3f(radius, height, 0);
	glEnd();
	
}

void Rotorblaetter()
{
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(-1, 2, 0);
	glVertex3f(-0.45, 3, 0);
	glVertex3f(0.45, 3, 0);
	glVertex3f(1, 2, 0);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.1);
	glVertex3f(-1, 2, 0);
	glVertex3f(-1, 2, 0.1);
	glVertex3f(-0.45, 3, 0);
	glVertex3f(-0.45, 3, 0.1);
	glVertex3f(0.45, 3, 0);
	glVertex3f(0.45, 3, 0.1);
	glVertex3f(1, 2, 0);
	glVertex3f(1, 2, 0.1);
	glEnd();
	
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0.1);
	glVertex3f(-1, 2, 0.1);
	glVertex3f(-0.45, 3, 0.1);
	glVertex3f(0.45, 3, 0.1);
	glVertex3f(1, 2, 0.1);
	glEnd();
}

void RenderScene() //Zeichenfunktion
{
   // Hier befindet sich der Code der in jedem Frame ausgefuehrt werden muss
   glLoadIdentity ();   // Aktuelle Model-/View-Transformations-Matrix zuruecksetzen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(1.0, 0.6, 0.0, 1.0);
   //glTranslatef(0., 0., -1);
   gluLookAt(0., 0., 6., 0., 0., 0., 0., 1., 0.);

   float braun [4] = { 0.4, 0.3, 0.3, 1. };
   float weiss[4] = { 1, 1, 1, 1. };
   float blau[4] = { 0, 0, 1, 1. };
   float silber[4] = { 0.75, 0.75, 0.75, 1. };
   float rot[4] = { 1., 0., 0., 1. };

   // Sitzfläche
   glPushMatrix();
   glScalef(0.75, 0.025, 0.25);
   Wuerfel(1, weiss);
   glPopMatrix();

   // Lehne
   glPushMatrix();
   glTranslatef(0, 0.05, -0.15);
   glScalef(0.05, 0.125, 0.05);
   Wuerfel(1, rot);
   glPopMatrix();
   glPushMatrix();
   glTranslatef(0, 0.2375, -0.15);
   glScalef(0.6, 0.25, 0.02);
   Wuerfel(1, blau);
   glPopMatrix();

   // Ventilator Befestigung
   glPushMatrix();
   glTranslatef(0, 0.2375, -0.17);
   glRotatef(90, -1., 0., 0.);
   Zylinder(0.1, 1000, 0.05, braun);
   glPopMatrix();

   glPushMatrix();
   glColor4f(silber[0], silber[1], silber[2], silber[3]);
   glTranslatef(0, 0.2375, -0.22);
   glPushMatrix();
   glRotatef(15, 0., 1., 0.);
   glScalef(0.07, 0.07, 0.07);
   Rotorblaetter();
   glPopMatrix();

   glPushMatrix();
   glRotatef(90, 0., 0., 1.);
   glRotatef(15, 0., 1., 0.);
   glScalef(0.07, 0.07, 0.07);
   Rotorblaetter();
   glPopMatrix();

   glPushMatrix();
   glRotatef(-90, 0., 0., 1.);
   glRotatef(15, 0., 1., 0.);
   glScalef(0.07, 0.07, 0.07);
   Rotorblaetter();
   glPopMatrix();

   glPushMatrix();
   glRotatef(180, 0., 0., 1.);
   glRotatef(15, 0., 1., 0.);
   glScalef(0.07, 0.07, 0.07);
   Rotorblaetter();
   glPopMatrix();
   glPopMatrix();

   // Seilbefestigung links
   glPushMatrix();
   glTranslatef(-0.375, 0., 0.);
   glRotatef(90, 0., 0., 1.);
   Zylinder(0.1, 1000, 0.035, braun);
   glPopMatrix();

   // Seilbefestigung rechts
   glPushMatrix();
   glTranslatef(0.475, 0., 0.);
   glRotatef(90, 0., 0., 1.);
   Zylinder(0.1, 1000, 0.035, braun);
   glPopMatrix();

   // Seil links
   glPushMatrix();
   glTranslatef(0.425, 0., 0.);
   Zylinder(2., 1000, 0.02, blau);
   glPopMatrix();

   // Seil rechts
   glPushMatrix();
   glTranslatef(-0.425, 0., 0.);
   Zylinder(2., 1000, 0.02, blau);
   glPopMatrix();

   // Tragestange
   glPushMatrix();
   glTranslatef(1.25, 2., 0.);
   glRotatef(90, 0., 0., 1.);
   Zylinder(2.5, 1000, 0.05, silber);
   glPopMatrix();

   // Standfuss oben links
   glPushMatrix();
   glTranslatef(-1.2, -0.584, -0.941);
   glRotatef(20, 1., 0., 0.);
   Zylinder(2.75, 1000, 0.05, silber);
   glPopMatrix();

   // Standfuss unten links
   glPushMatrix();
   glTranslatef(-1.2, -0.584, 0.941);
   glRotatef(-20, 1., 0., 0.);
   Zylinder(2.75, 1000, 0.05, silber);
   glPopMatrix();

   // Standfuss oben rechts
   glPushMatrix();
   glTranslatef(1.2, -0.584, -0.941);
   glRotatef(20, 1., 0., 0.);
   Zylinder(2.75, 1000, 0.05, silber);
   glPopMatrix();

   // Standfuss unten rechts
   glPushMatrix();
   glTranslatef(1.2, -0.584, 0.941);
   glRotatef(-20, 1., 0., 0.);
   Zylinder(2.75, 1000, 0.05, silber);
   glPopMatrix();

   glutSwapBuffers();
   glFlush(); //Buffer leeren   
}

void Reshape(int width,int height)
{
   // Hier finden die Reaktionen auf eine Veränderung der Größe des 
   // Graphikfensters statt
	// Matrix für Transformation: Frustum->viewport 
	glMatrixMode(GL_PROJECTION);
	// Aktuelle Transformations-Matrix zuruecksetzen 
	glLoadIdentity();
	// Viewport definieren 
	glViewport(0, 0, width, height);
	// Frustum definieren (siehe unten) 
	//glOrtho(-1., 1., -1., 1., 0.0, 2.0);
	gluPerspective(45., 1., 0.1, 10.0);
	// Matrix für Modellierung/Viewing 
	glMatrixMode(GL_MODELVIEW);
}

void Animate (int value)    
{
   // Hier werden Berechnungen durchgeführt, die zu einer Animation der Szene  
   // erforderlich sind. Dieser Prozess läuft im Hintergrund und wird alle 
   // 1000 msec aufgerufen. Der Parameter "value" wird einfach nur um eins 
   // inkrementiert und dem Callback wieder uebergeben. 
   std::cout << "value=" << value << std::endl;
   // RenderScene aufrufen
   glutPostRedisplay();
   fRotation = fRotation - 1.0;  // Rotationswinkel aendern 
   if (fRotation <= 0.0) {
	   fRotation = fRotation + 360.0;
   }
   // Timer wieder registrieren; Animate wird so nach 100 msec mit value+=1 aufgerufen
   glutTimerFunc(100, Animate, ++value);          
}

int main(int argc, char **argv)
{
   glutInit( &argc, argv );                // GLUT initialisieren
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );        
   glutInitWindowSize( 600, 600 );         // Fenster-Konfiguration
   glutCreateWindow( "Tom Mongan; Yannik Seegel" );   // Fenster-Erzeugung
   glutDisplayFunc( RenderScene );         // Zeichenfunktion bekannt machen
   glutReshapeFunc( Reshape );
   // TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
   glutTimerFunc( 10, Animate, 0);
   Init();
   glutMainLoop();
   return 0;
}
