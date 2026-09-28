#include "FrameProjector.h"
#include "Cam.h"

// Port of the Processing sketch's Quaternion.calculateMatrix() + useMatrix(), so
// rotations saved in settings.txt produce the same orientation. glm matrices are
// indexed [column][row]; Processing's applyMatrix() takes rows.
static glm::mat4 rotationMatrix(const glm::quat& q) {
    float q0q0 = q.w * q.w;
    float q0q1 = q.w * q.x;
    float q0q2 = q.w * q.y;
    float q0q3 = q.w * q.z;
    float q1q1 = q.x * q.x;
    float q1q2 = q.x * q.y;
    float q1q3 = q.x * q.z;
    float q2q2 = q.y * q.y;
    float q2q3 = q.y * q.z;
    float q3q3 = q.z * q.z;

    float matrix[3][3];
    matrix[0][0] = 2.0 * (q0q0 + q1q1) - 1.0;
    matrix[1][0] = 2.0 * (q1q2 - q0q3);
    matrix[2][0] = 2.0 * (q1q3 + q0q2);

    matrix[0][1] = 2.0 * (q1q2 + q0q3);
    matrix[1][1] = 2.0 * (q0q0 + q2q2) - 1.0;
    matrix[2][1] = 2.0 * (q2q3 - q0q1);

    matrix[0][2] = 2.0 * (q1q3 - q0q2);
    matrix[1][2] = 2.0 * (q2q3 + q0q1);
    matrix[2][2] = 2.0 * (q0q0 + q3q3) - 1.0;

    glm::mat4 m(1.0);
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            m[col][row] = matrix[row][col];
        }
    }
    return m;
}

FrameProjector::FrameProjector() {
    reset();
}

// Takes a snapshot of the live strokes; it stays on screen until the next tick.
void FrameProjector::newFrame(int alpha) {
    removeExpiredStrokes();
    frame = Frame(strokesBuffer, alpha);
}

void FrameProjector::update() {
    velocity *= friction;
    pos += velocity;
}

void FrameProjector::draw(float lineWidth) {
    ofPushMatrix();
    ofTranslate(pos);
    ofMultMatrix(rotationMatrix(q));
    frame.draw(lineWidth);
    ofPopMatrix();
}

// A stroke with the same index as a buffered one replaces it.
void FrameProjector::createStroke(int index, ofColor c, const vector<glm::vec3>& points, int lifespan) {
    Stroke newStroke(index, c, points, lifespan);

    auto it = std::find_if(strokesBuffer.begin(), strokesBuffer.end(), [index](const Stroke& s) {
        return s.index == index;
    });

    if (it != strokesBuffer.end()) {
        *it = newStroke;
    } else {
        strokesBuffer.push_back(newStroke);
    }

    removeExpiredStrokes();
}

void FrameProjector::addStroke(const Stroke& stroke) {
    strokesBuffer.push_back(stroke);
    removeExpiredStrokes();
}

void FrameProjector::removeExpiredStrokes() {
    uint64_t time = ofGetElapsedTimeMillis();
    ofRemove(strokesBuffer, [time](const Stroke& s) {
        return s.isExpired(time);
    });
}

// Movement follows the camera's axes, so the controls match what is on screen.
void FrameProjector::moveForward(const Cam& cam) {
    velocity += cam.forward * speed;
}

void FrameProjector::moveBack(const Cam& cam) {
    velocity -= cam.forward * speed;
}

void FrameProjector::moveLeft(const Cam& cam) {
    velocity -= cam.right * speed;
}

void FrameProjector::moveRight(const Cam& cam) {
    velocity += cam.right * speed;
}

void FrameProjector::moveUp(const Cam& cam) {
    velocity -= cam.up * speed;
}

void FrameProjector::moveDown(const Cam& cam) {
    velocity += cam.up * speed;
}

void FrameProjector::rollUp() {
    rotateAxis(rotDelta, glm::vec3(1, 0, 0));
}

void FrameProjector::rollDown() {
    rotateAxis(-rotDelta, glm::vec3(1, 0, 0));
}

void FrameProjector::pitchUp() {
    rotateAxis(rotDelta, glm::vec3(0, 1, 0));
}

void FrameProjector::pitchDown() {
    rotateAxis(-rotDelta, glm::vec3(0, 1, 0));
}

void FrameProjector::yawUp() {
    rotateAxis(rotDelta, glm::vec3(0, 0, 1));
}

void FrameProjector::yawDown() {
    rotateAxis(-rotDelta, glm::vec3(0, 0, 1));
}

// Same convention as the Processing Quaternion.rotateAxis(): the step rotation
// is built from -angle, then multiplied on the right.
void FrameProjector::rotateAxis(float angle, const glm::vec3& axis) {
    q = q * glm::angleAxis(-angle, axis);
}

void FrameProjector::reset() {
    velocity = glm::vec3(0);
    pos = glm::vec3(0);
    q = glm::quat(1, 0, 0, 0);
}
