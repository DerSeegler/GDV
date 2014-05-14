// GD-Praktikum:   teil_1.cpp  (Teil 1: Start-Programm)
// Hergenroether / Groch    Last Update: 29.07.2011

#define _USE_MATH_DEFINES

#include <iostream> 
#include <GL/freeglut.h>         //lädt alles für OpenGL
#include <math.h>

//using namespace std;

float fRotation = 0.0;
float winkelGeschwindigekeit = 0.0;
float winkel = 0.0;
float winkel2 = 0.0;
float x,y,z = 0.0;
int zylinderGenauigkeit = 100;

void Init()	
{
   // Hier finden jene Aktionen statt, die zum Programmstart einmalig 
   // durchgeführt werden müssen
	glClearColor(1.0, 0.6, 0.0, 1.0);

	// Licht
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    GLfloat light_position [] = {1.0, 1.0, 1., 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position ); // Licht Nr. 0 rechts oben
    glEnable(GL_COLOR_MATERIAL);
    // z-Buffer
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    // Normalen fuer korrekte Beleuchtungs-Berechnung normalisieren
    glEnable(GL_NORMALIZE);

}

void Zylinder(float height, float resolution, float radius, float color [4])
{
	resolution = 1 / resolution;
	float i = 0;
	float zero = 0;
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
	glVertex3f((radius * cos(zero)), height, radius * sin(zero));
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
	glNormal3f(0., 0., -1);
	glVertex3f(0, 0, 0);
	glVertex3f(-1, 2, 0);
	glVertex3f(-0.45, 3, 0);
	glVertex3f(0.45, 3, 0);
	glVertex3f(1, 2, 0);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.1);
	glNormal3f(-0.2, -0.1, 0);
	glVertex3f(-1, 2, 0);
	glVertex3f(-1, 2, 0.1);
	glNormal3f(1., -0.55, 0);
	glVertex3f(-0.45, 3, 0);
	glVertex3f(-0.45, 3, 0.1);
	glNormal3f(0., 1., 0);
	glVertex3f(0.45, 3, 0);
	glVertex3f(0.45, 3, 0.1);
	glNormal3f(1., 0.55, 0);
	glVertex3f(1, 2, 0);
	glVertex3f(1, 2, 0.1);
	glNormal3f(-0.2, 0.1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.1);
	glEnd();
	
	glBegin(GL_POLYGON);
	glNormal3f(0., 0., 1);
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
   //glClearColor(1.0, 0.6, 0.0, 1.0);
   //glTranslatef(0., 0., -1);
   gluLookAt(0.+x, 1.+y, 6.+z, 0., 0., 0., 0., 1., 0.);

   float braun [4] = { 0.4, 0.3, 0.3, 1. };
   float weiss[4] = { 1, 1, 1, 1. };
   float blau[4] = { 0, 0, 1, 1. };
   float silber[4] = { 0.75, 0.75, 0.75, 1. };
   float rot[4] = { 1., 0., 0., 1. };

   // Tragestange
   glPushMatrix();
   glTranslatef(-1.25, 2., 0.);
   glRotatef(90, 0., 1., 0.);
   glColor4f(0.75, 0.75, 0.75, 1.);
   glutSolidCylinder(0.05, 2.5, zylinderGenauigkeit, zylinderGenauigkeit);;
   glPopMatrix();

   // Standfuss oben links
   glPushMatrix();
   glTranslatef(-1.2, -0.584, -0.941);
   glRotatef(20, 1., 0., 0.);
   glRotatef(90, -1., 0., 0.);
   glColor4f(0.75, 0.75, 0.75, 1.);
   glutSolidCylinder(0.05, 2.75, zylinderGenauigkeit, zylinderGenauigkeit);;
   glPopMatrix();

   // Standfuss unten links
   glPushMatrix();
   glTranslatef(-1.2, -0.584, 0.941);
   glRotatef(-20, 1., 0., 0.);
   glRotatef(90, -1., 0., 0.);
   glColor4f(0.75, 0.75, 0.75, 1.);
   glutSolidCylinder(0.05, 2.75, zylinderGenauigkeit, zylinderGenauigkeit);;
   glPopMatrix();

   // Standfuss oben rechts
   glPushMatrix();
   glTranslatef(1.2, -0.584, -0.941);
   glRotatef(20, 1., 0., 0.);
   glRotatef(90, -1., 0., 0.);
   glColor4f(0.75, 0.75, 0.75, 1.);
   glutSolidCylinder(0.05, 2.75, zylinderGenauigkeit, zylinderGenauigkeit);;
   glPopMatrix();

   // Standfuss unten rechts
   glPushMatrix();
   glTranslatef(1.2, -0.584, 0.941);
   glRotatef(-20, 1., 0., 0.);
   glRotatef(90, -1., 0., 0.);
   glColor4f(0.75, 0.75, 0.75, 1.);
   glutSolidCylinder(0.05, 2.75, zylinderGenauigkeit, zylinderGenauigkeit);;
   glPopMatrix();

   glTranslatef(0, 2.0175, 0);
   glRotatef(winkel, 1., 0., 0.);
   glTranslatef(0, -2.0175, 0);

   // Sitzfläche
   glPushMatrix();
   glScalef(0.75, 0.025, 0.25);
   glColor4f(1.,1.,1.,1.);	
   glutSolidCube(1);
   glPopMatrix();

   // Lehne
   glPushMatrix();
   glTranslatef(0, 0.05, -0.15);
   glScalef(0.05, 0.125, 0.05);
   glColor4f(1., 0., 0., 1.);
   glutSolidCube(1); // rot
   glPopMatrix();
   glPushMatrix();
   glTranslatef(0, 0.2375, -0.15);
   glScalef(0.6, 0.25, 0.02);
   glColor4f(0., 0., 1., 1.);
   glutSolidCube(1); // blau
   glPopMatrix();

   // Seilbefestigung links
   glPushMatrix();
   glTranslatef(-0.375, 0., 0.);
   glRotatef(90, 0., -1., 0.);
   glColor4f(0.4, 0.3, 0.3, 1.);
   glutSolidCylinder(0.035, 0.1, zylinderGenauigkeit, zylinderGenauigkeit);
   glPopMatrix();

   // Seilbefestigung rechts
   glPushMatrix();
   glTranslatef(0.475, 0., 0.);
   glRotatef(90, 0., -1., 0.);
   glColor4f(0.4, 0.3, 0.3, 1.);
   glutSolidCylinder(0.035, 0.1, zylinderGenauigkeit, zylinderGenauigkeit);
   glPopMatrix();

   // Seil links
   glPushMatrix();
   glTranslatef(0.425, 0., 0.);
   glRotatef(90, -1., 0., 0.);
   glColor4f(0, 0, 1, 1.);
   glutSolidCylinder(0.02, 2, zylinderGenauigkeit, zylinderGenauigkeit);
   glPopMatrix();

   // Seil rechts
   glPushMatrix();
   glTranslatef(-0.425, 0., 0.);
   glRotatef(90, -1., 0., 0.);
   glColor4f(0, 0, 1, 1.);
   glutSolidCylinder(0.02, 2, zylinderGenauigkeit, zylinderGenauigkeit);
   glPopMatrix();

   glTranslatef(0, 0.2365, -0.185);
   glRotatef(winkel2, 0., 0., 1.);
   glTranslatef(0, -0.2365, 0.185);

   // Ventilator Befestigung
   glPushMatrix();
   glTranslatef(0, 0.2375, -0.17);
   glRotatef(180, 0., 1., 0.);
   glColor4f(0.4, 0.3, 0.3, 1.);
   glutSolidCylinder(0.05, 0.1, zylinderGenauigkeit, zylinderGenauigkeit);
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
  // std::cout << "value=" << value << std::endl;
   // RenderScene aufrufen
   glutPostRedisplay();
  
   fRotation = fRotation - 0.1;  // Rotationswinkel aendern 
   if (fRotation <= 0.0) {
	   fRotation = fRotation + 6.3;
   }

   winkel = sin(fRotation) * 60;

   winkel2 = winkel2 - 10;  // Rotationswinkel aendern 
   if (winkel2 <= 0.0) {
	   winkel2 = winkel2 + 360;
   }

  // winkelGeschwindigekeit = - g/2 * sin(

  // std::cout << fRotation << std::endl;
   // Timer wieder registrieren; Animate wird so nach 100 msec mit value+=1 aufgerufen
   glutTimerFunc(50, Animate, ++value);          
}

void KeyboardFunc ( unsigned char key, int x, int y )
{
	switch (key){
	case 'w': z--; break;
	case 'a': x--; break;
	case 's': z++; break;
	case 'd': x++; break;
	}

    // RenderScene aufrufen.
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
   glutInit( &argc, argv );                // GLUT initialisieren
   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );        
   glutInitWindowSize( 600, 600 );         // Fenster-Konfiguration
   glutCreateWindow( "Tom Mongan; Yannik Seegel" );   // Fenster-Erzeugung
   glutDisplayFunc( RenderScene );         // Zeichenfunktion bekannt machen
   glutReshapeFunc( Reshape );
   glutKeyboardFunc( KeyboardFunc );
   // TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
   glutTimerFunc( 10, Animate, 0);
   Init();
   glutMainLoop();
   return 0;
}
