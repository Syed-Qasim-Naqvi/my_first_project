#ifndef FILTER_H
#define FILTER_H

#include "Image.h"
#include <string>
#include <algorithm> // For min and max
#include <utility>   // For swap

using namespace std;

// abstract base class
class Filter {
protected:
    string name;
public:
    Filter(string n) : name(n) {}
    virtual ~Filter() {} 
    virtual void apply(Image& img) = 0; 
    string getName() const { return name; }
};

// 01:Grayscale
class GrayscaleFilter : public Filter {
public:
    GrayscaleFilter() : Filter("Grayscale") {}
    void apply(Image& img) override {
        for (int i = 0; i < img.getHeight(); ++i) {
            for (int j = 0; j < img.getWidth(); ++j) {
                Pixel& p = img.at(i, j);
                int avg = (p.getR() + p.getG() + p.getB()) / 3;
                p.setRGB(avg, avg, avg);
            }
        }
    }
};

// 02:invert/negative
class InvertFilter : public Filter {
public:
    InvertFilter() : Filter("Invert") {}
    void apply(Image& img) override {
        for (int i = 0; i < img.getHeight(); ++i) {
            for (int j = 0; j < img.getWidth(); ++j) {
                Pixel& p = img.at(i, j);
                p.setRGB(255 - p.getR(), 255 - p.getG(), 255 - p.getB());
            }
        }
    }
};

// 03:brightness adjust
class BrightnessAdjustFilter : public Filter {
private:
    int amount;
public:
    BrightnessAdjustFilter(int amt) : Filter("Brightness Adjust"), amount(amt) {}
    void apply(Image& img) override {
        for (int i = 0; i < img.getHeight(); ++i) {
            for (int j = 0; j < img.getWidth(); ++j) {
                Pixel& p = img.at(i, j);
                p.setRGB(p.getR() + amount, p.getG() + amount, p.getB() + amount);
            }
        }
    }
};

// 04:contrast stretch
class ContrastStretchFilter : public Filter {
public:
    ContrastStretchFilter() : Filter("Contrast Stretch") {}
    void apply(Image& img) override {
        int minR = 255, maxR = 0, minG = 255, maxG = 0, minB = 255, maxB = 0;

        for (int i = 0; i < img.getHeight(); ++i) {
            for (int j = 0; j < img.getWidth(); ++j) {
                Pixel& p = img.at(i, j);
                minR = min(minR, p.getR()); maxR = max(maxR, p.getR());
                minG = min(minG, p.getG()); maxG = max(maxG, p.getG());
                minB = min(minB, p.getB()); maxB = max(maxB, p.getB());
            }
        }

        for (int i = 0; i < img.getHeight(); ++i) {
            for (int j = 0; j < img.getWidth(); ++j) {
                Pixel& p = img.at(i, j);
                int r = (maxR == minR) ? p.getR() : ((p.getR() - minR) * 255) / (maxR - minR);
                int g = (maxG == minG) ? p.getG() : ((p.getG() - minG) * 255) / (maxG - minG);
                int b = (maxB == minB) ? p.getB() : ((p.getB() - minB) * 255) / (maxB - minB);
                p.setRGB(r, g, b);
            }
        }
    }
};

// 05:red channel only
class RedChannelFilter : public Filter {
public:
    RedChannelFilter() : Filter("Red Channel Only") {}
    void apply(Image& img) override {
        for (int i = 0; i < img.getHeight(); ++i) {
            for (int j = 0; j < img.getWidth(); ++j) {
                Pixel& p = img.at(i, j);
                p.setRGB(p.getR(), 0, 0);
            }
        }
    }
};

// 06:green channel only
class GreenChannelFilter : public Filter {
public:
    GreenChannelFilter() : Filter("Green Channel Only") {}
    void apply(Image& img) override {
        for (int i = 0; i < img.getHeight(); ++i) {
            for (int j = 0; j < img.getWidth(); ++j) {
                Pixel& p = img.at(i, j);
                p.setRGB(0, p.getG(), 0);
            }
        }
    }
};

// 07:blue channel only
class BlueChannelFilter : public Filter {
public:
    BlueChannelFilter() : Filter("Blue Channel Only") {}
    void apply(Image& img) override {
        for (int i = 0; i < img.getHeight(); ++i) {
            for (int j = 0; j < img.getWidth(); ++j) {
                Pixel& p = img.at(i, j);
                p.setRGB(0, 0, p.getB());
            }
        }
    }
};

// 08:box blur (3x3)
class BoxBlurFilter : public Filter {
public:
    BoxBlurFilter() : Filter("Box Blur") {}
    void apply(Image& img) override {
        Image copyImg(img); 

        for (int r = 0; r < img.getHeight(); ++r) {
            for (int c = 0; c < img.getWidth(); ++c) {
                int sumR = 0, sumG = 0, sumB = 0, count = 0;

                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        int nr = r + dr;
                        int nc = c + dc;
                        
                        if (nr >= 0 && nr < img.getHeight() && nc >= 0 && nc < img.getWidth()) {
                            Pixel& neighbor = copyImg.at(nr, nc);
                            sumR += neighbor.getR();
                            sumG += neighbor.getG();
                            sumB += neighbor.getB();
                            count++;
                        }
                    }
                }
                img.at(r, c).setRGB(sumR / count, sumG / count, sumB / count);
            }
        }
    }
};

// 09: flip horizontal
class FlipHorizontalFilter : public Filter {
public:
    FlipHorizontalFilter() : Filter("Flip Horizontal") {}
    void apply(Image& img) override {
        for (int r = 0; r < img.getHeight(); ++r) {
            for (int c = 0; c < img.getWidth() / 2; ++c) {
                Pixel temp = img.at(r, c);
                img.at(r, c) = img.at(r, img.getWidth() - 1 - c);
                img.at(r, img.getWidth() - 1 - c) = temp;
            }
        }
    }
};

// 10: flip vertical
class FlipVerticalFilter : public Filter {
public:
    FlipVerticalFilter() : Filter("Flip Vertical") {}
    void apply(Image& img) override {
        for (int r = 0; r < img.getHeight() / 2; ++r) {
            for (int c = 0; c < img.getWidth(); ++c) {
                Pixel temp = img.at(r, c);
                img.at(r, c) = img.at(img.getHeight() - 1 - r, c);
                img.at(img.getHeight() - 1 - r, c) = temp;
            }
        }
    }
};

#endif