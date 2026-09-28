#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxLibwebsockets.h"
#include "Bloom.h"
#include "Cam.h"
#include "FrameProjector.h"
#include "GameMode.h"
#include "Settings.h"
#include "Sharpen.h"
#include "XYScope.h"

class ofApp : public ofBaseApp {

    public:
        void setup();
        void update();
        void draw();
        void exit();

        void keyPressed(int key);
        void keyReleased(int key);
        void windowResized(int w, int h);

        // Websocket events arrive on the client's own thread.
        void onConnect(ofxLibwebsockets::Event& args);
        void onClose(ofxLibwebsockets::Event& args);
        void onIdle(ofxLibwebsockets::Event& args);
        void onMessage(ofxLibwebsockets::Event& args);

        float dotSize = 0.5;
        int globalAlpha = 63;
        int globalLifespan = 1000;
        int fps = 12; // how often projectors take a new frame
        int realFps = 60; // render framerate
        int frameInterval;
        uint64_t markTime = 0;
        vector<FrameProjector> frameProjector;
        Settings settings;
        Cam cam;
        GameMode gameMode = GameMode::OFF;

        // ~ ~ ~ Controls ~ ~ ~
        bool keyW = false;
        bool keyA = false;
        bool keyS = false;
        bool keyD = false;
        bool keyQ = false;
        bool keyE = false;
        bool keySpace = false;

        // ~ ~ ~ Osc ~ ~ ~
        int receivePort = 7110;
        ofxOscReceiver oscReceiver;

        // ~ ~ ~ Websocket ~ ~ ~
        uint64_t wsNow = 0;
        int wsInterval = 83;
        string wsUrl = "ws://vr.fox-gieg.com:8080";
        float wsGlobalScale = 1;
        glm::vec3 wsGlobalOffset = glm::vec3(0, 0, 0);
        bool useWebsockets = false;
        int targetProjector = 0;

        // ~ ~ ~ Render texture and post-processing ~ ~ ~
        int texWidth = 960;
        int texHeight = 720;
        ofFbo tex;
        Bloom bloom;
        Sharpen sharpen;
        bool useSharpen = false;

        // ~ ~ ~ SoundOut ~ ~ ~
        bool useSound = false;
        int activityThreshold = 1;
        float xy1amp = 0.01;
        XYScope xy1;

    private:
        void loadSettings();
        void saveSettings();

        void setGameMode(GameMode mode);
        bool checkKeyChar(int k, bool b);
        void updateControls();
        void moveProjector(FrameProjector& p);
        void rotateProjector(FrameProjector& p);

        void oscSetup();
        void oscUpdate();
        void oscEvent(const ofxOscMessage& msg);

        void wsSetup();
        void wsUpdate();
        void webSocketEvent(const ofJson& json);

        void soundOutUpdate();

        // Declared last so the client (and its thread) is destroyed first.
        std::mutex wsMutex;
        vector<ofJson> wsMessages;
        ofxLibwebsockets::Client wsc;

};
