#ifndef FILTERSESSION_H
#define FILTERSESSION_H
#include <vector>
#include <iostream>
#include "Filter.h"
#include "Image.h"

using namespace std;

class FilterSession {
private:
    vector<Filter*> pipeline;

public:
    FilterSession() {}
    ~FilterSession() {
        for (Filter* f : pipeline) {
            delete f;
        }
    }

    FilterSession& addFilter(Filter* f) {
        this->pipeline.push_back(f);
        return *this; 
    }

    bool isEmpty() const { return pipeline.empty(); }

    void applyPipeline(Image& img) {
        cout << "Applying pipeline to image size: " << img.width << "x" << img.height << "\n";
        for (Filter* f : pipeline) {
            cout << " -> Applying " << f->getName() << "...\n";
            f->apply(img);
        }
    }
};
#endif