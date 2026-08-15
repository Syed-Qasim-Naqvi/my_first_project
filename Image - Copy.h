#ifndef IMAGE_H
#define IMAGE_H

#include "Pixel.h"
#include <string>

using namespace std;

class FilterSession; // forward declaration to tell C++ this class exists and it has call it

class Image {
    
    friend class FilterSession; 

private:
    Pixel** pixels;
    int width;
    int height;
    int channels;

public:
    Image(const string& path); 
    Image(const Image& other);      
    virtual ~Image();               

    int getWidth() const; 
    int getHeight() const; 
    Pixel& at(int row, int col); 

    void savePNG(const string& path); 
    void displayASCII() const; 
};

#endif