#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>

using namespace std;

class Pixel {
private:
    int r, g, b; //encapsulation 

public:
    Pixel(); //default constructor 
    Pixel(int red, int green, int blue); //parameterised constructor 

    // getters and Setters
    int getR() const;
    int getG() const;
    int getB() const;
    void setRGB(int red, int green, int blue);

    static int clamp(int value); // static utility function 
    
    // operator overloads 
    Pixel operator+(const Pixel& other) const;
    friend ostream& operator<<(ostream& os, const Pixel& p);
};

#endif