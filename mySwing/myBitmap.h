#ifndef _MYBITMAP_H_
#define _MYBITMAP_H_
/*
  Version: 0.4, 23.07.05 13:54
  Author: Florian Mücke (flo_AT_mueckeimnetz.de)
  Copyright: Florian Mücke, Uni-Augsburg, 2005
  Description: Bibliothek zum Laden von BMP Bildern.
               Als Vorlage diente eine Funktion von Richard Campbell.
               Unterstützt werden alle BMP Bilder mit entweder 8 oder 24 Bit
               Farbtiefe (bei 8 Bit auch mit RLE-Kodierung).
*/

/* 
  Informationen zum Windows Bitmap:
  Die Bitmaps sind einfach und direkt (ohne großartige Kodierung) gespeichert.
  Am Anfang der Datei steht der sog. BitmapHeader. Darin steht wesentlichen im 
  Klartext die Bildgröße (Dimension), die Farbtiefe, die Anzahl der Ebenen, die
  verwendete Speicherungsart/Kompression (z.B. Run Length Encoding), der Offset
  der Farbtabelle und deren Größe bzw. den Offset der Pixeldaten.
  - 24 Bit: hier ist die Farbtabelle 0 Byte groß, d.h. die Farbwerte der 
            einzelnen Pixel kommen direkt nach dem Header. Dabei nimmt jedes
            Pixel 3 Byte (steht allerdings verkehrt herum also BGR statt RGB in
            der Datei, da aus dem Speicher anders herum gelesen wird) ein.
  - 8 Bit: hier wird zuerst die Farbpalette angegeben. Diese besteht aus 256
           3 Byte großen Farbfeldern, jeweils getrennt durch ein Nullbyte 
           (möglicherweise Alphawert?), wieder nach obigem Schema gespeichert
           (BGR statt RGB). Zur Beschreibung der einzelnen Pixel reicht jetzt
           jeweils ein einziges Byte aus, das einfach den Index in der Farb-
           tabelle angibt.

  Compression: Im Normalfall wird keine verwendet. Es gibt aber die Möglichkeit
               die Bilder mittels Run Length Encoding (RLE) abzuspeichern.
               Dabei wird im Prizip einfach gesagt, wieviele Pixel der selben
               Farbe nacheinander auftreten und nicht für jeden Pixel explizit
               die Farbe angegeben. (für 8bit implementiert)
*/            
   


#include <stdio.h>      //Standard Dateioperationen
#include <stdlib.h>     //malloc/free

struct myRGBImage {
    int sizeX, sizeY;
    unsigned char *data;  //Daten: für jeden Pixel 3 Byte (RGB)
};
typedef struct myRGBImage myRGBImage;
#define BMPHEADER_DATA_OFFSET        0x0A
#define BMPHEADER_DIMENSIONS         0x12
#define BMPHEADER_COMPRESSION        0x1E
#define BMPHEADER_COLOR_TABLE        0x36
#define BI_RGB                       0x0
#define BI_RLE8                      0x1
#define BI_RLE4                      0x2


/**
 * BITMAP Loader Prozedur
 * ======================
 * fuer 8 und 24bit Farbtiefe Bilder (auch 8Bit RLE!)
 * Liest eine .BMP Datei ein und gibt ein myRGBImage zurück. Dieses enthält die
 * Farbdaten der einzelnen Pixel unkomprimiert als RGB-Werte (=> 3 Byte/Pixel)
 * sowie deren Dimension.
 */
int LoadBMP(char *filename, myRGBImage *bitmap) {
    FILE *file;
    unsigned long size;                 // Dateigröße
    unsigned long i;                    // Zähler
    unsigned long dataOffset;           // Offset der tatsächlichen Bitmap-Daten
    unsigned short int planes;          // #Ebenen (muss 1 sein) 
    unsigned short int bpp;             // BPP - muss (noch) 8 oder 24 sein
    unsigned char colorTable[256][4];   // Farbtabelle für 8-Bit Bilder
    unsigned char temp;                 // temporary color storage for bgr-rgb conversion.
    unsigned char compression;          // Kompression (BI_RGB, BI_RLE8, oder BI_RLE4)

    // überprüfen ob Datei vorhanden
    if ((file = fopen(filename, "rb"))==NULL) {
       printf("Datei nicht gefunden: %s.\n",filename);
    	 return 0;
    }

    // Lese Datenoffset aus dem Header
    fseek(file, BMPHEADER_DATA_OFFSET, SEEK_SET);
    if ((i = fread(&dataOffset, 4, 1, file)) != 1) {
	     printf("Fehler beim Lesen von %s.\n", filename);
	     return 0;
    }

    // gehe zur width/height Stelle

    // Lese width, height 
    fseek(file, BMPHEADER_DIMENSIONS, SEEK_SET);
    if (((i = fread(&bitmap->sizeX, 4, 1, file)) != 1) | 
       ((i = fread(&bitmap->sizeY, 4, 1, file)) != 1)) {
	     printf("Fehler beim Lesen von %s.\n", filename);
	     return 0;
    }

    // Ebenen einlesen
    if ((fread(&planes, 2, 1, file)) != 1) {
	     printf("Fehler beim Lesen der Ebenenanzahl von %s.\n", filename);
	     return 0;
    }
    if (planes != 1) {
    	 printf("Ebenenanzahl von %s ist nicht 1 sondern %u.\n", filename, planes);
       return 0;
    }

    // Bittiefe lesen
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
	     printf("Fehler beim Lesen der Bittiefe von %s.\n", filename);
	     return 0;
    }
    if ((bpp != 24) && (bpp != 8)) {
	     printf("Bittiefe von %s muss 8 oder 24 bpp sein! Ist aber: %u\n", filename, bpp);
	     return 0;
    }
    
    // Kompression ermitteln
    if (fread(&compression, 1, 1, file) != 1) {
	     printf("Fehler beim Ermitteln der Kompression von %s.\n", filename);
	     return 0;
    }
    if (!(compression == BI_RGB || compression == BI_RLE8)) {
	     printf("Diese RLE Kompression wird nicht unterstützt!\n");
	     return 0;
    }
    
    // Farbtabelle einlesen
    if (bpp == 8) {
       fseek(file, BMPHEADER_COLOR_TABLE, SEEK_SET);
       if ((i = fread(&colorTable, 1024, 1, file)) != 1) {
	        printf("Fehler beim Lesen der Farbtabelle aus %s.\n", filename);
	        return 0;
       }
    }

    // Größe berechnen (24 Bit bzw. 3 Byte per Pixel).
    size = bitmap->sizeX * bitmap->sizeY * 3;

    // Gehe zu den Bilddaten
    fseek(file, dataOffset, SEEK_SET);

    // Datenbereich lesen
    bitmap->data = (unsigned char *) malloc(size);
    if (bitmap->data == NULL) {
	     printf("Fehler beim Zuweisen des Speichers für den Bitmap Datenbereich");
	     return 0;	
    }
    if (bpp == 8) {
       if(compression == BI_RLE8) {
          unsigned char rledata, rletemp= 0;
          unsigned int pos = 0;
          while(pos < bitmap->sizeX*bitmap->sizeY){
             //read 1st byte
             if (fread(&rledata, 1, 1, file) != 1) {
                printf("Fehler beim Lesen des Datenbereichs von %s.\n", filename);
      	        return 0;
             }
             //read 2nd byte
             if (fread(&rletemp, 1, 1, file) != 1) {
                printf("Fehler beim Lesen des Datenbereichs von %s.\n", filename);
   	           return 0;
             }
             if (rledata == 0x00) {
                if (rletemp == 0x01) return 1; //end of bitmap
                if (rletemp == 0x00) continue; //end of line
                //if (rletemp == 0x02) //delta mode...
                if (rletemp > 0x02) { //abolute pixels
                   char align = rletemp & 1; //is pattern word aligned?
                   for(rletemp; rletemp>0; rletemp--) {
                      if (fread(&rledata, 1, 1, file) != 1) {
                         printf("Fehler beim Lesen des Datenbereichs von %s.\n", filename);
       	                 return 0;
                      }
                      bitmap->data[pos*3] = colorTable[rledata][2];//Rot
                      bitmap->data[pos*3+1] = colorTable[rledata][1];//Grün
                      bitmap->data[pos*3+2] = colorTable[rledata][0];//Blau
                      pos++;                      
                   }
                   if (align) { //not word aligned, so read one more byte
                      if (fread(&rledata, 1, 1, file) != 1) {
                         printf("Fehler beim Lesen des Datenbereichs von %s.\n", filename);
                         return 0;
                      }
                   }
                }//if (rletemp > 0x02)
             }//if (rledata == 0x00)
             else {//rledata != 0x00
                for(rledata; rledata>0; rledata--) {
                   bitmap->data[pos*3] = colorTable[rletemp][2];//Rot
                   bitmap->data[pos*3+1] = colorTable[rletemp][1];//Grün
                   bitmap->data[pos*3+2] = colorTable[rletemp][0];//Blau
                   pos++;                   
                }
             }//rledata != 0x00
          }//while(pos < bitmap->sizeX*bitmap->sizeY)
       }//if(compression == BI_RLE8)
       else {
          // 8Bit Bild, daher müssen die Farbinformationen erst aus der Farbtabelle
          // gelesen werden
          for (i=0; i<bitmap->sizeX*bitmap->sizeY; i++) {
             if (fread(&temp, 1, 1, file) != 1) {
                printf("Fehler beim Lesen des Datenbereichs von %s.\n", filename);
      	         return 0;
             }
             bitmap->data[i*3] = colorTable[temp][2];//Rot
             bitmap->data[i*3+1] = colorTable[temp][1];//Grün
             bitmap->data[i*3+2] = colorTable[temp][0];//Blau
          }
       }
    }
    else { //24Bit Bild, Werte können direkt übernommen werden
       if ((i = fread(bitmap->data, size, 1, file)) != 1) {
	        printf("Fehler beim Lesen des Datenbereichs von %s.\n", filename);
	        return 0;
       }
       for (i=0;i<size;i+=3) { //Farbwerte umdrehen (bgr -> rgb)
          temp = bitmap->data[i];
	        bitmap->data[i] = bitmap->data[i+2];
	        bitmap->data[i+2] = temp;
       }
    }
    return 1;  //fertig
} 

#endif 
