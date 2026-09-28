#pragma once
#include "ofMain.h"

// Sharpens an fbo in place, like PGraphics.filter(shader) in the Processing sketch.
class Sharpen {

    public:
        void setup(int width, int height);
        void apply(ofFbo& fbo);

    private:
        ofShader shader;
        ofFbo buffer;

};
