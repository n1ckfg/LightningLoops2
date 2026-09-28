#pragma once
#include "ofMain.h"
#include "Stroke.h"

// A snapshot of a projector's strokes, held on screen until the next frame tick.
class Frame {

    public:
        Frame();
        Frame(const vector<Stroke>& _strokes, int alpha);

        void draw(float lineWidth);
        void drawScope(XYScope& scope) const;

        vector<Stroke> strokes;
        uint64_t timestamp;
        ofMesh mesh;

};
