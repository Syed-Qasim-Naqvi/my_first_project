#include "Pixel.h"

using namespace std;

Pixel::Pixel() : r(0), g(0), b(0) {}
Pixel::Pixel(int red, int green, int blue) {
    setRGB(red, green, blue);
}

int Pixel::getR() const { return r; }
int Pixel::getG() const { return g; }
int Pixel::getB() const { return b; }

int Pixel::clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}

void Pixel::setRGB(int red, int green, int blue) {
    r = clamp(red);
    g = clamp(green);
    b = clamp(blue);
}

Pixel Pixel::operator+(const Pixel& other) const {
    return Pixel(clamp(r + other.r), clamp(g + other.g), clamp(b + other.b));
}

ostream& operator<<(ostream& os, const Pixel& p) {
    // ASCII preview logic:brightness 
    int brightness = (p.r + p.g + p.b) / 3;
    if (brightness < 64) os << " ";
    else if (brightness < 128) os << ".";
    else if (brightness < 192) os << "*";
    else os << "@";
    return os;
}