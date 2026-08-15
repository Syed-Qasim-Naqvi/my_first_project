#include "Image.h"
#include <iostream>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

// constructor: load from file
Image::Image(const string& path) {
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 3);
    
    if (!data) {
        // satisfies the "try/catch around file I/O" requirement
        throw runtime_error("File I/O Error: Could not load image file '" + path + "'");
    }

    pixels = new Pixel*[height];
    for (int i = 0; i < height; ++i) {
        pixels[i] = new Pixel[width];
        for (int j = 0; j < width; ++j) {
            int index = 3 * (i * width + j);
            pixels[i][j].setRGB(data[index], data[index + 1], data[index + 2]);
        }
    }
    stbi_image_free(data);
}

// deep copy constructor 
Image::Image(const Image& other) : width(other.width), height(other.height), channels(other.channels) {
    pixels = new Pixel*[height];
    for (int i = 0; i < height; ++i) {
        pixels[i] = new Pixel[width];
        for (int j = 0; j < width; ++j) {
            pixels[i][j] = other.pixels[i][j]; 
        }
    }
}

// destructor
Image::~Image() {
    if (pixels) {
        for (int i = 0; i < height; ++i) {
            delete[] pixels[i];
        }
        delete[] pixels;
    }
}

Pixel& Image::at(int row, int col) {
    return pixels[row][col];
}

int Image::getWidth() const { return width; }
int Image::getHeight() const { return height; }

void Image::savePNG(const string& path) {
    unsigned char* data = new unsigned char[width * height * 3];
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = 3 * (i * width + j);
            data[index] = pixels[i][j].getR();
            data[index + 1] = pixels[i][j].getG();
            data[index + 2] = pixels[i][j].getB();
        }
    }
    stbi_write_png(path.c_str(), width, height, 3, data, width * 3);
    delete[] data;
}

void Image::displayASCII() const {
    for (int i = 0; i < height; i += 10) { 
        for (int j = 0; j < width; j += 5) {
            cout << pixels[i][j]; 
        }
        cout << "\n";
    }
}