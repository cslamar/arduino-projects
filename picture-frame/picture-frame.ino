#include <stdint.h>
#include <TouchScreen.h> 
#include <SPI.h>
#include <SD.h>
#include "TFT.h"


#define MEGA

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1

#define TS_MINX 140
#define TS_MAXX 900

#define TS_MINY 120
#define TS_MAXY 940

#define CS_PIN 53

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

File bmpFile;

// File root point
File root;

// Number of files in file array
int fileCount = 0;

// Index placement of the counter
int index = 0;

// information we extract about the bitmap file
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;

unsigned long previousMillis = 0;
const long interval = 7000;
int counter = 0;

String *fileArray;

bool paused = false;

void setup(void) {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  if (!SD.begin(CS_PIN)) { // chip select pin
      Serial.println("failed!");
      return;
  }
  Serial.println("SD OK!");

  Tft.init();
  Tft.setDisplayDirect(UP2DOWN);
  Tft.drawString("Loading...", 166, 100, 2, GREEN);
  
  root = SD.open("/");

  // Count the number of images
  countFiles(root);
  root.rewindDirectory();

  // Declare the new array size based on the file count
  fileArray = new String[fileCount];
  
  fillArray(fileCount, &fileArray[0], root);
  Serial.print("Image files found: ");
  Serial.println(fileCount);
  
  showNextSlide(millis());
}

void loop(void) {
  if(counter == fileCount) {
    counter = 0;
  }
  unsigned long currentMillis = millis();
  
  TSPoint p = ts.getPoint();

  p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
  p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);

  if(p.z > ts.pressureThreshhold) {
    if( p.y > 211 ){
      Serial.println("FORWARD!");
      showNextSlide(currentMillis);
    } else if ( p.y < 105 ) {
      Serial.println("BACK!");
      previousSlide(currentMillis);
    } else if ( (p.y > 105) && (p.y < 211) ){
      paused = !paused;
//      Tft.drawString("Paused", 166, 100, 2, GREEN);
      Serial.println("Paused toggled");
    }
  }
  
  // Default to next slide
  if( (currentMillis - previousMillis >= interval) && (paused == false) ) {
      showNextSlide(currentMillis);
  }

}

void previousSlide(unsigned long current) {
  previousMillis = current;

  counter -= 2;
  
  Serial.print("counter: ");
  Serial.println(counter);

  showNextSlide(current);
}

void showNextSlide(unsigned long current) {
  previousMillis = current;

  bmpFile = SD.open(fileArray[counter]);
  bmpReadHeader(bmpFile);
  bmpdraw(bmpFile, 0, 0);
/*
  // Debugging code
  Serial.print(bmpFile.name());
  Serial.print("\t");
  Serial.println(bmpFile.size(), DEC);
  Serial.print("image size ");
  Serial.print(bmpWidth, DEC);
  Serial.print(", ");
  Serial.println(bmpHeight, DEC);
 */
  
  bmpFile.close();
  counter++;
}

#define BUFFPIXEL 20

void bmpdraw(File f, int x, int y) {
    bmpFile.seek(bmpImageoffset);

    uint32_t time = millis();
    uint16_t p;
    uint8_t g, b;
    int i, j;

    uint8_t sdbuffer[3 * BUFFPIXEL];  // 3 * pixels to buffer
    uint8_t buffidx = 3*BUFFPIXEL;


    for (i=0; i< bmpHeight; i++) {

        Tft.setXY(x, y+bmpHeight-i);


        for (j=0; j<bmpWidth; j++) {
            // read more pixels
            if (buffidx >= 3*BUFFPIXEL) {
                bmpFile.read(sdbuffer, 3*BUFFPIXEL);
                buffidx = 0;
            }

            // convert pixel from 888 to 565
            b = sdbuffer[buffidx++];     // blue
            g = sdbuffer[buffidx++];     // green
            p = sdbuffer[buffidx++];     // red

            p >>= 3;
            p <<= 6;

            g >>= 2;
            p |= g;
            p <<= 5;

            b >>= 3;
            p |= b;

            // write out the 16 bits of color
            Tft.sendData(p);
        }
    }
    Serial.print(millis() - time, DEC);
    Serial.println(" ms");
}

boolean bmpReadHeader(File f) {
    // read header
    uint32_t tmp;

    if (read16(f) != 0x4D42) {
        // magic bytes missing
        return false;
    }

    // read file size
    tmp = read32(f);
    Serial.print("size 0x"); Serial.println(tmp, HEX);

    // read and ignore creator bytes
    read32(f);

    bmpImageoffset = read32(f);
    Serial.print("offset "); Serial.println(bmpImageoffset, DEC);

    // read DIB header
    tmp = read32(f);
    Serial.print("header size "); Serial.println(tmp, DEC);
    bmpWidth = read32(f);
    bmpHeight = read32(f);


    if (read16(f) != 1)
    return false;

    bmpDepth = read16(f);
    Serial.print("bitdepth "); Serial.println(bmpDepth, DEC);

    if (read32(f) != 0) {
        // compression not supported!
        return false;
    }

    Serial.print("compression "); Serial.println(tmp, DEC);

    return true;
}

/*********************************************/
// These read data from the SD card file and convert them to big endian
// (the data is stored in little endian format!)

// LITTLE ENDIAN!
uint16_t read16(File f) {
    uint16_t d;
    uint8_t b;
    b = f.read();
    d = f.read();
    d <<= 8;
    d |= b;
    return d;
}

// LITTLE ENDIAN!
uint32_t read32(File f) {
    uint32_t d;
    uint16_t b;

    b = read16(f);
    d = read16(f);
    d <<= 16;
    d |= b;
    return d;
}

void countFiles(File dir) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      return;
    }
    if (entry.isDirectory()) {
      // recurse through next directory
      countFiles(entry);
    } else {
      String filename = entry.name();
      if( filename.endsWith("BMP") ){
        // increment global file count
        fileCount++;
      }
    }
    entry.close();
  }
}

void fillArray(int numFiles, String *theArray, File dir) {
  String currentDir = dir.name();
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      return;
    }
    if (entry.isDirectory()) {
      // recurse through next directory
      fillArray(numFiles, theArray, entry);
    } else {
      String filename = entry.name();
      if( filename.endsWith("BMP") ){
        String fullPath = currentDir;
        if( currentDir != "/") {
          fullPath.concat("/");
        }
        fullPath.concat(filename);
        // Add file to global file array
        theArray[index] = fullPath;
        // increment global file count
        index++;
      }
    }
    entry.close();
  }
}

