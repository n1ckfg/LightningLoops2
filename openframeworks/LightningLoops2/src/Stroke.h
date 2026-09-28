#pragma once
#include "ofMain.h"

class XYScope;

class Stroke {

    public:
        Stroke(int idx, ofColor c, const vector<glm::vec3>& pts, int life);

        void draw(ofMesh& mesh, int alpha) const;
        void drawScope(XYScope& scope) const;
        bool isExpired(uint64_t time) const;

        vector<glm::vec3> points;
        int index;
        uint64_t timestamp;
        int lifespan = 1000;
        ofColor col;

};
