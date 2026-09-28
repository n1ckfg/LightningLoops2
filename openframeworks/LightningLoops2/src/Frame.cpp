#include "Frame.h"

Frame::Frame() {
    timestamp = ofGetElapsedTimeMillis();
    mesh.setMode(OF_PRIMITIVE_LINES);
}

// All strokes are batched into one line mesh, so a frame is a single draw call.
Frame::Frame(const vector<Stroke>& _strokes, int alpha) : Frame() {
    strokes = _strokes;
    for (const Stroke& s : strokes) {
        s.draw(mesh, alpha);
    }
}

void Frame::draw(float lineWidth) {
    ofSetLineWidth(lineWidth);
    mesh.draw();
}

void Frame::drawScope(XYScope& scope) const {
    for (const Stroke& s : strokes) {
        s.drawScope(scope);
    }
}
