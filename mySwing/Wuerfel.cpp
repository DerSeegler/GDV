/*
    wuerfel.cpp definiert einen Würfel über seine 6 Seiten
    Last Update:  13.10.2011   W.-D. Groch
*/ 
#include <GL/freeglut.h>
#include "Wuerfel.h"


void Wuerfel(GLfloat fSeitenL, GLuint texture, GLenum filterMode, float numberOfTiles)
{ 
	 glEnable(GL_TEXTURE_2D);              //Textur-Mapping anschalten
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	

	//glColor4f(color[0], color[1], color[2], color[3]);
	glBegin(GL_POLYGON);   //Vorderseite
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-fSeitenL/2.0f,-fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(0.0,0.0);
	glVertex3f(+fSeitenL/2.0f,-fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,0.0);
	glVertex3f(+fSeitenL/2.0f,+fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(0.0,numberOfTiles);
	glVertex3f(-fSeitenL/2.0f,+fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,numberOfTiles);
	glEnd();


	glBegin(GL_POLYGON);   //Rechte Seite
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(+fSeitenL/2.0f,-fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(0.0,0.0);
	glVertex3f(+fSeitenL/2.0f,-fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,0.0);
	glVertex3f(+fSeitenL/2.0f,+fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(0.0,numberOfTiles);
	glVertex3f(+fSeitenL/2.0f,+fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,numberOfTiles);
	glEnd();


	glBegin(GL_POLYGON);   //Rueckseite
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(+fSeitenL/2.0f,+fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(0.0,0.0);
	glVertex3f(+fSeitenL/2.0f,-fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,0.0);
	glVertex3f(-fSeitenL/2.0f,-fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(0.0,numberOfTiles);
	glVertex3f(-fSeitenL/2.0f,+fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,numberOfTiles);
	glEnd();


	glBegin(GL_POLYGON);   //Linke Seite
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-fSeitenL/2.0f,+fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(0.0,0.0);
	glVertex3f(-fSeitenL/2.0f,-fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,0.0);
	glVertex3f(-fSeitenL/2.0f,-fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(0.0,numberOfTiles);
	glVertex3f(-fSeitenL/2.0f,+fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,numberOfTiles);
	glEnd();

	glBegin(GL_POLYGON);   //Deckflaeche
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-fSeitenL/2.0f,+fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(0.0,0.0);
	glVertex3f(+fSeitenL/2.0f,+fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,0.0);
	glVertex3f(+fSeitenL/2.0f,+fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(0.0,numberOfTiles);
	glVertex3f(-fSeitenL/2.0f,+fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,numberOfTiles);
	glEnd();

	glBegin(GL_POLYGON);   //Bodenflaeche
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-fSeitenL/2.0f,-fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(0.0,0.0);
	glVertex3f(+fSeitenL/2.0f,-fSeitenL/2.0f,-fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,0.0);
	glVertex3f(+fSeitenL/2.0f,-fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(0.0,numberOfTiles);
	glVertex3f(-fSeitenL/2.0f,-fSeitenL/2.0f,+fSeitenL/2.0f);
	glTexCoord2d(numberOfTiles,numberOfTiles);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	return;
}