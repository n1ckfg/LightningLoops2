#include "Stroke.h"
#include "XYScope.h"

Stroke::Stroke(int idx, ofColor c, const vector<glm::vec3>& pts, int life) {
    index = idx;
    col = c;
    points = pts;
    timestamp = ofGetElapsedTimeMillis();
    lifespan = int(ofRandom(life / 10, life * 10));
}

// Appends this stroke's polyline to a frame's batched line mesh.
void Stroke::draw(ofMesh& mesh, int alpha) const {
    ofColor c(col, alpha);
    for (size_t i = 1; i < points.size(); i++) {
        mesh.addVertex(points[i-1]);
        mesh.addColor(c);
        mesh.addVertex(points[i]);
        mesh.addColor(c);
    }
}

// Only the newest segment of each stroke goes to the scope.
void Stroke::drawScope(XYScope& scope) const {
    size_t len = points.size();
    if (len > 1) {
        const glm::vec3& p = points[len-1];
        const glm::vec3& pp = points[len-2];
        scope.line(p.x, p.y, pp.x, pp.y);
    }
}

bool Stroke::isExpired(uint64_t time) const {
    return time > timestamp + lifespan;
}
