#include "Cam.h"

// Same platforms ofAppRunner uses a GLFW window on.
#if !defined(TARGET_NODISPLAY) && !defined(TARGET_OF_IOS) && !defined(TARGET_ANDROID) && !defined(TARGET_EMSCRIPTEN) && !defined(TARGET_RASPBERRY_PI_LEGACY)
#define CAM_USE_GLFW
#include "ofAppGLFWWindow.h"
#include <GLFW/glfw3.h>
#endif

// Matches Processing's default P3D perspective for the render texture: a 60 degree
// field of view, clip planes at cameraZ/10 and cameraZ*10, and +y pointing down.
void Cam::setupProjection(int texWidth, int texHeight) {
    float fov = 60;
    float cameraZ = (texHeight / 2.0) / tan(ofDegToRad(fov / 2.0));
    camera.setVFlip(true);
    camera.setFov(fov);
    camera.setNearClip(cameraZ / 10.0);
    camera.setFarClip(cameraZ * 10.0);
}

void Cam::setControllable(bool b) {
    if (b == controllable) return;
    controllable = b;
    setPointerCaptured(b);
    pRotMouse = glm::vec2(ofGetMouseX(), ofGetMouseY());
    ignoreNextMouseMove = true;
}

// Stands in for the Processing sketch's java.awt.Robot edge-wrapping: a disabled
// GLFW cursor is hidden and locked to the window but keeps reporting motion.
void Cam::setPointerCaptured(bool captured) {
#ifdef CAM_USE_GLFW
    auto window = dynamic_cast<ofAppGLFWWindow*>(ofGetWindowPtr());
    if (window) {
        glfwSetInputMode(window->getGLFWWindow(), GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_HIDDEN);
    }
#endif
}

void Cam::updateRotation() {
    glm::vec2 rotMouse(ofGetMouseX(), ofGetMouseY());

    // GLFW can measure the first move after capture from a stale pointer
    // position, which would jerk the view, so that move only sets the baseline.
    if (ignoreNextMouseMove && rotMouse != pRotMouse) {
        ignoreNextMouseMove = false;
        pRotMouse = rotMouse;
    }

    pan += ofMap(pRotMouse.x - rotMouse.x, 0, ofGetWidth(), 0, TWO_PI) * sensitivity;
    tilt += ofMap(pRotMouse.y - rotMouse.y, 0, ofGetHeight(), 0, PI) * sensitivity;
    tilt = ofClamp(tilt, -PI / 2.01, PI / 2.01);

    forward = glm::normalize(glm::vec3(cos(pan), tan(tilt), sin(pan)));
    right = glm::normalize(glm::vec3(cos(pan - HALF_PI), 0, sin(pan - HALF_PI)));
    up = glm::normalize(glm::cross(right, forward));

    pRotMouse = rotMouse;
}

void Cam::updatePosition() {
    velocity *= friction;
    pos += velocity;
    poi = pos + forward;
}

void Cam::update() {
    if (!controllable) return;
    updateRotation();
    updatePosition();
}

void Cam::begin() {
    camera.setPosition(pos);
    camera.lookAt(poi, up);
    camera.begin();
}

void Cam::end() {
    camera.end();
}

void Cam::moveForward() {
    velocity += forward * speed;
}

void Cam::moveBack() {
    velocity -= forward * speed;
}

void Cam::moveLeft() {
    velocity -= right * speed;
}

void Cam::moveRight() {
    velocity += right * speed;
}

void Cam::moveUp() {
    velocity -= up * speed;
}

void Cam::moveDown() {
    velocity += up * speed;
}

void Cam::reset() {
    pos = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, (ofGetHeight() / 2.0) / tan(PI * 30.0 / 180.0));
    poi = glm::vec3(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 0);
    up = glm::vec3(0, 1, 0);
    right = glm::vec3(1, 0, 0);
    forward = glm::vec3(0, 0, 1);
    velocity = glm::vec3(0);
    pan = 0;
    tilt = 0;
}
