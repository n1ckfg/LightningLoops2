#pragma once
#include "ofMain.h"
#include "Frame.h"

class Cam;

// A virtual projector: buffers the strokes from one client and shows them as
// frames, with its own position and rotation in the scene.
class FrameProjector {

    public:
        FrameProjector();

        void newFrame(int alpha);
        void update();
        void draw(float lineWidth);
        void createStroke(int index, ofColor c, const vector<glm::vec3>& points, int lifespan);
        void addStroke(const Stroke& stroke);
        void removeExpiredStrokes();

        void moveForward(const Cam& cam);
        void moveBack(const Cam& cam);
        void moveLeft(const Cam& cam);
        void moveRight(const Cam& cam);
        void moveUp(const Cam& cam);
        void moveDown(const Cam& cam);
        void rollUp();
        void rollDown();
        void pitchUp();
        void pitchDown();
        void yawUp();
        void yawDown();
        void reset();

        Frame frame;
        vector<Stroke> strokesBuffer;
        string hostname = "";
        bool hostnameConfirmed = false;

        float speed = 3;
        float friction = 0.75;
        float rotDelta = 0.05;
        glm::vec3 pos;
        glm::vec3 velocity;
        glm::quat q;

    private:
        void rotateAxis(float angle, const glm::vec3& axis);

};
