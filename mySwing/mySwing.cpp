// GD-Praktikum:   teil_1.cpp  (Teil 1: Start-Programm)
// Hergenroether / Groch    Last Update: 29.07.2011

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_DEPRECATE

#include <iostream> 
#include <vector>
#include <iomanip>
#include <GL/freeglut.h>         //lädt alles für OpenGL
#include "wuerfel.h"
#include <math.h>
#include "myBitmap.h"

#include <io.h> 
#include <fcntl.h>


using namespace std;

float fRotation = 0.0;
float winkelGeschwindigekeit = 0.0;
float winkel = 0.0;
float winkel2 = 0.0;
float x,y,z = 0.0;
float CamRotation = 0.0;
int speed = 3;
int speedArray [] = { 1, 5, 10, 20, 30, 40, 50, 75, 100, 125, 150 };
int zylinderGenauigkeit = 100;

#define NUM_TEXTURES 8           //Anzahl der Texturen
GLuint texture[NUM_TEXTURES];    //Textur-Pointer-Array
GLenum filterMode = GL_LINEAR;
GLuint texNum = 0; 
float fSeitenL = 6;
GLuint numberOfTiles = 5;

static void LoadGLTextures()     //Bitmaps laden und in Texturen umwandeln
{
  myRGBImage *textureImage[NUM_TEXTURES];
  int i;
  for (i=0; i<NUM_TEXTURES; i++) {
      textureImage[i] = (myRGBImage *) malloc(sizeof(myRGBImage));
      if (textureImage[i] == NULL) {
         printf("Fehler bei der Speicherreservierung fuer die Bilddaten");
         exit(0);
      } 
  }  
  //memset(textureImage,0,sizeof(void *)*1); //sicherheitshalber den Inhalt loeschen
	if (
     (LoadBMP("Textures/wood.bmp", textureImage[0])) &&
     (LoadBMP("Textures/wood2.bmp", textureImage[1])) &&
	 (LoadBMP("Textures/grass.bmp", textureImage[2])) && 
	 (LoadBMP("Textures/siege/siege_front.bmp", textureImage[3])) &&
	 (LoadBMP("Textures/siege/siege_back.bmp", textureImage[4])) &&
	 (LoadBMP("Textures/siege/siege_left.bmp", textureImage[5])) &&
	 (LoadBMP("Textures/siege/siege_right.bmp", textureImage[6])) &&
	 (LoadBMP("Textures/siege/siege_top.bmp", textureImage[7]))
	){
	

     for (i=0; i<NUM_TEXTURES; i++) {
		     glGenTextures(1, &texture[i]);  //Textur erzeugen
		     // Textur Erstellung mit Daten vom Bitmap
		     glBindTexture(GL_TEXTURE_2D, texture[i]);
		     glTexImage2D(GL_TEXTURE_2D,            //target (Gibt an, welche Texture erreicht werden soll)
                      0,                          //level (Grad der Mipmap-Reduzierung, 0 ist das Basisbild)
                      3,                          //components (1 fuer R, 2 fuer R+A, 3 fuer RGB, 4 fuer RGBA)
					  textureImage[i]->sizeX,     //width (Bildbreite in Pixeln)
                      textureImage[i]->sizeY,     //height (Bildhoehe in Pixeln)
                      0,                          //border (Rand: 0 oder 1)
                      GL_RGB,                     //format (z.B. GL_COLOR_INDEX, GL_RGBA, GL_LUMINANCE, ...)
                      GL_UNSIGNED_BYTE,           //type (Typ der Daten; z.B. GL_BYTE, GL_INT, GL_FLOAT, ...)
                      textureImage[i]->data);     //pixels (eigentlichen Bilddaten)
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

		   //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
     }
     //Aufraeumen
     for (i=0; i<NUM_TEXTURES; i++) {
         if (textureImage[i]) {              //falls existiert
	         if (textureImage[i]->data) {     //falls Daten existieren
   	         free(textureImage[i]->data);  //Datenbereich freigeben
           }
		       free(textureImage[i]);          //Bitmap-Struktur freigeben
         }
     }
  }
}

void Init()	
{
   // Hier finden jene Aktionen statt, die zum Programmstart einmalig 
   // durchgeführt werden müssen
	glClearColor(1.0, 0.6, 0.0, 1.0);

	LoadGLTextures();            //Texturen Laden
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0f };
	GLfloat diffuseLight[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Licht
	GLfloat light_position[] = { -fSeitenL + 0.2, fSeitenL - 3.5, -fSeitenL + 2.5, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position ); // Licht Nr. 0 rechts oben
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_BACK, GL_AMBIENT);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    // z-Buffer
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    // Normalen fuer korrekte Beleuchtungs-Berechnung normalisieren
    glEnable(GL_NORMALIZE);

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

void bottom(){
	 glPushMatrix();
    glTranslatef(0., -0.584, 0.);
   	glEnable(GL_TEXTURE_2D);   
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	
	glBegin(GL_POLYGON);
	glNormal3f(0., 1., 0.);
	glVertex3f(-fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(0.0,0.0);
	glVertex3f(+fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(numberOfTiles,0.0);
	glVertex3f(+fSeitenL,0.0,+fSeitenL);
	glTexCoord2d(numberOfTiles,numberOfTiles);
	glVertex3f(-fSeitenL,0.0,+fSeitenL);
	glTexCoord2d(0.0,numberOfTiles);
	glEnd();

	glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void back(){
		 glPushMatrix();
	
    glTranslatef(0.0, -0.584,-fSeitenL);
    //glScalef(0.2, 0.2, 0.2);
	glRotatef(90, -1., 0., 0.);
	//glRotatef(180, 1., 0., 0.);

   	glEnable(GL_TEXTURE_2D);   
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);
	glVertex3f(-fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(1.0,0.0);
	glVertex3f(+fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(1.0,1.0);
	glVertex3f(+fSeitenL,0.0,+fSeitenL);
	glTexCoord2d(0.0,1.0);
	glVertex3f(-fSeitenL,0.0,+fSeitenL);
	glEnd();

	glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void front(){
	glPushMatrix();
	
    glTranslatef(0.0, -0.584,fSeitenL);
	glRotatef(180, 0., 1., 0.);
	glRotatef(90, -1., 0., 0.);
	

   	glEnable(GL_TEXTURE_2D);   
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);
	glVertex3f(-fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(1.0,0.0);
	glVertex3f(+fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(1.0,1.0);
	glVertex3f(+fSeitenL,0.0,+fSeitenL);
	glTexCoord2d(0.0,1.0);
	glVertex3f(-fSeitenL,0.0,+fSeitenL);
	glEnd();

	glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void left(){
	glPushMatrix();
	
    glTranslatef(-fSeitenL, -0.584,0.0);
		glRotatef(180, 0., 1., 0.);
	glRotatef(90, 0., -1., 0.);
	glRotatef(90, -1., 0., 0.);
	

   	glEnable(GL_TEXTURE_2D);   
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);
	glVertex3f(-fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(1.0,0.0);
	glVertex3f(+fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(1.0,1.0);
	glVertex3f(+fSeitenL,0.0,+fSeitenL);
	glTexCoord2d(0.0,1.0);
	glVertex3f(-fSeitenL,0.0,+fSeitenL);
	glEnd();

	glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void right(){

	glPushMatrix();
	
    glTranslatef(+fSeitenL, -0.584,0.0);
	glRotatef(90, 0., -1., 0.);
	glRotatef(90, -1., 0., 0.);
	

   	glEnable(GL_TEXTURE_2D);   
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);
	glVertex3f(-fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(1.0,0.0);
	glVertex3f(+fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(1.0,1.0);
	glVertex3f(+fSeitenL,0.0,+fSeitenL);
	glTexCoord2d(0.0,1.0);
	glVertex3f(-fSeitenL,0.0,+fSeitenL);
	glEnd();

	glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void top(){
	 glPushMatrix();
    glTranslatef(0., fSeitenL-0.6, 0.);
	glRotatef(90., 0., -1., 0.);
	//glRotatef(180, 1., 0., 0.);
   	glEnable(GL_TEXTURE_2D);   
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	
	glBegin(GL_POLYGON);
	glVertex3f(-fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(0.0,0.0);
	glVertex3f(+fSeitenL,0.0,-fSeitenL);
	glTexCoord2d(1.0,0.0);
	glVertex3f(+fSeitenL,0.0,+fSeitenL);
	glTexCoord2d(1.0,1.0);
	glVertex3f(-fSeitenL,0.0,+fSeitenL);
	glTexCoord2d(0.0,1.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void RenderScene() //Zeichenfunktion
{

   // Hier befindet sich der Code der in jedem Frame ausgefuehrt werden muss
   glLoadIdentity ();   // Aktuelle Model-/View-Transformations-Matrix zuruecksetzen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glClearColor(1.0, 0.6, 0.0, 1.0);
   //glTranslatef(0., 0., -1);
  // gluLookAt(0.+x, 0.+y, 6.+z, 0., 0., 0., 0., 1., 0.);
   gluLookAt(6*sin(CamRotation / 180 * M_PI), 1.0, 6*cos(CamRotation / 180 * M_PI), 0., 0., 0., 0., 1., 0.);
   GLfloat light_position[] = { -fSeitenL + 0.2, fSeitenL - 3.5, -fSeitenL + 2.5, 0.0 };
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glPushMatrix();
   glTranslatef(-fSeitenL + 0.2, fSeitenL - 3.5, -fSeitenL + 2.5);
   glutWireCube(0.2);
   glPopMatrix();

   float braun [4] = { 0.4, 0.3, 0.3, 1. };
   float weiss[4] = { 1, 1, 1, 1. };
   float blau[4] = { 0, 0, 1, 1. };
   float silber[4] = { 0.75, 0.75, 0.75, 1. };
   float rot[4] = { 1., 0., 0., 1. };

      // Grass
   bottom();
// Skybox
   glDisable(GL_LIGHTING);
   front();
   back();
   left();
   right();
   top();
   glEnable(GL_LIGHTING);

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
   Wuerfel(1,texture[0],filterMode,4); 
   glPopMatrix();

   // Lehne
   glPushMatrix();
   glTranslatef(0, 0.05, -0.15);
   glScalef(0.05, 0.125, 0.05);
   Wuerfel(1, texture[1],filterMode,4);
   glPopMatrix();
   glPushMatrix();
   glTranslatef(0, 0.2375, -0.15);
   glScalef(0.6, 0.25, 0.02);
   Wuerfel(1, texture[0],filterMode,4);
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
   glColor4f(0.45, 0.45, 0.45, 1.);
   glutSolidCylinder(0.02, 2, zylinderGenauigkeit, zylinderGenauigkeit);
   glPopMatrix();

   // Seil rechts
   glPushMatrix();
   glTranslatef(-0.425, 0., 0.);
   glRotatef(90, -1., 0., 0.);
   glColor4f(0.45, 0.45, 0.45, 1.);
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
	gluPerspective(45., 1., 0.1, 250.0);
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
   glutTimerFunc(speedArray[speed], Animate, ++value);          
}

void displaySpeed(){
	std::cout << " speed: " << speedArray[speed] << "ms" << std::endl;
}

void KeyboardFunc ( int key, int x, int y )
{
	switch (key){
	//case 'w': z--; break;
	case GLUT_KEY_LEFT: CamRotation = CamRotation-5; break;
	//case 's': CamRotation++; break;
	case GLUT_KEY_RIGHT: CamRotation = CamRotation+5; break;
	case GLUT_KEY_DOWN: if(speed != 10) speed++; displaySpeed();  break;
	case GLUT_KEY_UP: if(speed !=0) speed--; displaySpeed(); break;
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

 
   cout << setw(45) << "Willkommen bei MySwing"  << endl;
   cout << setw(54) << "Projekt von Tom Mongan und Yannik Seegel" << endl << endl;


 /*  char* locale = setlocale(LC_ALL, "English"); // Get the CRT's current locale.
    std::locale lollocale(locale);
    setlocale(LC_ALL, locale); // Restore the CRT.
    std::wcout.imbue(lollocale); // Now set the std::wcout to have the locale that we got from the CRT.
    std::wcout << L'\u0444';

	 SetConsoleOutputCP(65001); 
    printf("ф\n");*/

   cout << "Steuerung:" << endl;

   cout << "Kamera nach links drehen \t - \t" << "Pfeil nach links"  << endl;
   cout << "Kamera nach rechts drehen \t - \t" << "Pfeil nach rechts"  << endl;
   cout << "Animation beschleunigen \t - \t" << "Pfeil nach oben"  << endl;
   cout << "Animation verlangsamen \t\t - \t" << "Pfeil nach unten"  << endl;

   glutDisplayFunc( RenderScene );         // Zeichenfunktion bekannt machen
   glutReshapeFunc( Reshape );
  // glutKeyboardFunc( KeyboardFunc );
   glutSpecialFunc( &KeyboardFunc );
   // TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
   glutTimerFunc( 10, Animate, 0);
   Init();
   glutMainLoop();
   return 0;
}
