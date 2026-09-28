#pragma once
#include "ofMain.h"

// First-person camera: WASDQE movement plus mouselook while controllable.
// https://github.com/jrc03c/queasycam
class Cam {

    public:
        void setupProjection(int texWidth, int texHeight);
        void setControllable(bool b);
        void update();
        void begin();
        void end();
        void reset();

        void moveForward();
        void moveBack();
        void moveLeft();
        void moveRight();
        void moveUp();
        void moveDown();

        bool controllable = false;
        float speed = 3;
        float sensitivity = 2;
        float friction = 0.75;
        glm::vec3 pos;
        glm::vec3 poi;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 forward;
        glm::vec3 velocity;
        float pan = 0;
        float tilt = 0;
        string displayText = "";

    private:
        void updateRotation();
        void updatePosition();
        void setPointerCaptured(bool captured);

        ofCamera camera;
        glm::vec2 pRotMouse;
        bool ignoreNextMouseMove = false;

};
