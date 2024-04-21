#ifndef sd_reader_h
#define sd_reader_h


#include <SD.h>
#include <SPI.h>

class sd_reader{
  public:
    sd_reader();

    void openfile(String);
    
    bool loadBMP(const char *, uint8_t*&, int*&, int*&);

    void serial_print_files();

    int get_file_count();

    // was const char
    const char* file_n_name(int);

    //bool modify3DArray(int (*array)[Y_SIZE][Z_SIZE], int xSize, int ySize, int zSize);

  private:

};


#endif