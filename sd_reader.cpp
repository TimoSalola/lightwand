#include "sd_reader.h"
#include <SdFat.h>
#include <SD.h>
#include <SPI.h>


SdFat sd;
SdFile myFile;

const int chipSelect = BUILTIN_SDCARD; 



sd_reader::sd_reader(){
  SD.begin(chipSelect);
}


void sd_reader::openfile(String path){
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("Opening file " + path);



  FsFile myFile = sd.open(path);

  if (myFile == 0){
    Serial.println("file '" +  path +   "' not found");
  }

  myFile.close();

}

void sd_reader::serial_print_files(){

  File root = SD.open("/");

  while (true) {

    File entry =  root.openNextFile();

    if (! entry) {

      // no more files

      break;

    }
    Serial.print(entry.name());
  }

}

int sd_reader::get_file_count(){
  File root = SD.open("/");
  int filecount = 0;
  while (true) {
    File entry =  root.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    filecount += 1;
  }
  return filecount;
}

// was const char*
const char* sd_reader::file_n_name(int n){
  // returns file name at index n

  File root = SD.open("/");
  int filecount = 0;
  while (true) {
    File entry =  root.openNextFile();


    // this does not return file name with null determinator, has to be added
    const char* name_at_n = entry.name();

    size_t length = strlen(name_at_n);
    // Allocate memory with +1 for null terminator
    char* copy = new char[length + 1];
    // Copy the content and add null terminator
    strcpy(copy, name_at_n);

    const char* filename = copy;

    if (filecount == n){

      Serial.print("SD READER: returning file '");
      Serial.print(filename);
      Serial.print("' from index: ");
      Serial.println(filecount);

      return filename;
    }
    if (! entry) {
      // no more files
      break;
    }
    filecount += 1;
  }
  return "none";
}

bool sd_reader::loadBMP(const char *filename, uint8_t*& imgArray, int*& widthpointer, int*& heightpointer) {

  Serial.print("Filename:");
  Serial.println(filename);
  // Try to open the file
  File file = SD.open(filename);

  // Check if the file opened successfully
  if (!file) {
    Serial.println("Error opening file");
    return false;
  }

  // BMP files have a 54-byte header, read and discard it
  uint8_t header[54];
  file.read(header, sizeof(header));

  /*
  // Check if the file is a valid BMP file
  if (header[0] != 'B' || header[1] != 'M') {
    Serial.println("Not a valid BMP file");
    file.close();
    return false;
  }
  */

  // this is supposed to read image dimensions from file
  int width= *(uint16_t*)&header[18];
  int height = *(uint16_t*)&header[22];

  Serial.println("Updating image dimension pointers...");
  *widthpointer = width;
  *heightpointer = height;

  Serial.print("Pointer values should now be:");
  Serial.print(width);
  Serial.print(" x ");
  Serial.println(height);

  // Calculate padding for each row
  int padding = (4 - (width * 3) % 4) % 4;

  // Read the image data into the array
  //uint32_t dataSize = width * height;
  //uint32_t *pixelData = new uint32_t[dataSize];

  Serial.println("Creating image array...");


  uint8_t* newImgArray= new uint8_t[width*height*3];

  for (uint16_t y = height - 1; y < height; y--) {
    for (uint16_t x = 0; x < width; x++) {

      // assuming blue green red order, same as in wikipedia
      uint8_t blue = file.read();
      uint8_t green = file.read();
      uint8_t red = file.read();


      int arrayPos = ((width*y) + x)*3;;

      newImgArray[arrayPos+0] = red;
      newImgArray[arrayPos+1] = green;
      newImgArray[arrayPos+2] = blue;

      

      // Combine the RGB values into a single 32-bit pixel
      //pixelData[y * width + x] = (red << 16) | (green << 8) | blue;
    }

    // Skip over any padding bytes
    file.seek(file.position() + padding);
  }

  Serial.println("Image array created. Now changing pointer to point to it");

  imgArray = newImgArray;


  // Close the file
  file.close();


  // Free the dynamically allocated memory
  //delete[] pixelData;

  Serial.print("Reached function end");

  return true;
}