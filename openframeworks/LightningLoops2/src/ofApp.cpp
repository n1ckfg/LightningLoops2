#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofDisableArbTex();

    cam.reset();
    frameProjector.resize(2);

    settings.load("settings.txt");
    loadSettings();

    ofHideCursor();
    ofSetFrameRate(realFps);
    for (FrameProjector& p : frameProjector) {
        p.newFrame(globalAlpha);
    }
    oscSetup();
    if (useWebsockets) wsSetup();

    ofFboSettings texSettings;
    texSettings.width = texWidth;
    texSettings.height = texHeight;
    texSettings.internalformat = GL_RGBA;
    texSettings.useDepth = true;
#ifndef TARGET_OPENGLES
    texSettings.numSamples = 4; // Processing's P3D renderer is multisampled too
#endif
    tex.allocate(texSettings);
    cam.setupProjection(texWidth, texHeight);
    bloom.setup(texWidth, texHeight);
    sharpen.setup(texWidth, texHeight);

    if (useSound) {
        xy1.setCanvasSize(ofGetWidth(), ofGetHeight());
        xy1.setup();
    }

    frameInterval = int((1.0 / fps) * 1000);
}

//--------------------------------------------------------------
void ofApp::update() {
    oscUpdate();
    if (useWebsockets) wsUpdate();

    updateControls();
    cam.setControllable(gameMode == GameMode::CAM);
    cam.update();

    uint64_t time = ofGetElapsedTimeMillis();
    if (time > markTime + frameInterval) {
        markTime = time;
        for (FrameProjector& p : frameProjector) {
            p.newFrame(globalAlpha);
        }
    }

    for (FrameProjector& p : frameProjector) {
        p.update();
    }

    if (useSound) soundOutUpdate();
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);

    tex.begin();
    ofClear(0, 0, 0, 255);
    cam.begin();
    ofEnableDepthTest();
    glDepthFunc(GL_LEQUAL); // Processing's depth test, so coincident strokes still blend
    for (FrameProjector& p : frameProjector) {
        p.draw(dotSize);
    }
    ofDisableDepthTest();
    cam.end();
    tex.end();

    if (useSharpen) sharpen.apply(tex);
    bloom.apply(tex.getTexture());

    // A 4:3 frame, centered and full height.
    ofSetColor(255);
    int w = (ofGetHeight() / 3) * 4;
    bloom.draw((ofGetWidth() - w) / 2, 0, w, ofGetHeight());

    if (!cam.displayText.empty()) {
        ofDrawBitmapString(cam.displayText, 6, 12);
    }

    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::exit() {
    wsc.close();
    xy1.close();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    xy1.setCanvasSize(w, h);
}

// ~ ~ ~ Settings ~ ~ ~

void ofApp::loadSettings() {
    texWidth = settings.getInt("Render Width", texWidth);
    texHeight = settings.getInt("Render Height", texHeight);
    realFps = settings.getInt("Framerate", realFps);
    bloom.mult = settings.getFloat("Bloom Amount", bloom.mult);
    activityThreshold = settings.getInt("Activity Threshold", activityThreshold);

    xy1amp = settings.getFloat("Volume", xy1amp);
    useSound = settings.getBoolean("Use Sound", useSound);

    globalLifespan = settings.getInt("Stroke Lifetime", globalLifespan);
    dotSize = settings.getFloat("Stroke Weight", dotSize);
    globalAlpha = settings.getInt("Stroke Alpha", globalAlpha);

    useWebsockets = settings.getBoolean("Use Website", useWebsockets);
    wsUrl = settings.getString("Website Url", wsUrl);
    wsGlobalScale = settings.getFloat("Website Scale", wsGlobalScale);
    wsGlobalOffset = settings.getVector("Website Offset", wsGlobalOffset);

    cam.pos = settings.getVector("Cam Position", cam.pos);
    cam.poi = settings.getVector("Cam Point of Interest", cam.poi);
    cam.up = settings.getVector("Cam Up", cam.up);
    cam.pan = settings.getFloat("Cam Pan", cam.pan);
    cam.tilt = settings.getFloat("Cam Tilt", cam.tilt);

    for (size_t i = 0; i < frameProjector.size(); i++) {
        FrameProjector& p = frameProjector[i];
        string depth = "Depth" + ofToString(i + 1);
        p.hostname = settings.getString(depth + " Name", p.hostname);
        p.pos = settings.getVector(depth + " Position", p.pos);
        p.q = settings.getQuaternion(depth + " Rotation", p.q);
    }
}

void ofApp::saveSettings() {
    settings.setVector("Cam Position", cam.pos);
    settings.setVector("Cam Point of Interest", cam.poi);
    settings.setVector("Cam Up", cam.up);
    settings.setFloat("Cam Pan", cam.pan);
    settings.setFloat("Cam Tilt", cam.tilt);

    for (size_t i = 0; i < frameProjector.size(); i++) {
        FrameProjector& p = frameProjector[i];
        string depth = "Depth" + ofToString(i + 1);
        settings.setString(depth + " Name", p.hostname);
        settings.setVector(depth + " Position", p.pos);
        settings.setQuaternion(depth + " Rotation", p.q);
    }

    settings.save();
}

// ~ ~ ~ Controls ~ ~ ~

void ofApp::setGameMode(GameMode mode) {
    gameMode = mode;
    cam.displayText = mode == GameMode::OFF ? "" : gameModeName(mode);
    ofLogNotice() << gameModeName(mode);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    checkKeyChar(key, true);

    if (key == OF_KEY_TAB) {
        setGameMode(nextGameMode(gameMode));
    } else if (key == 'c') {
        setGameMode(gameMode == GameMode::CAM ? GameMode::OFF : GameMode::CAM);
    } else if (key == '1') {
        setGameMode(gameMode == GameMode::DEPTH1_POS ? GameMode::DEPTH1_ROT : GameMode::DEPTH1_POS);
    } else if (key == '2') {
        setGameMode(gameMode == GameMode::DEPTH2_POS ? GameMode::DEPTH2_ROT : GameMode::DEPTH2_POS);
    } else if (key == 'o') {
        saveSettings();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    checkKeyChar(key, false);
}

bool ofApp::checkKeyChar(int k, bool b) {
    switch (k) {
        case 'w':
            return keyW = b;
        case 'a':
            return keyA = b;
        case 's':
            return keyS = b;
        case 'd':
            return keyD = b;
        case 'q':
            return keyQ = b;
        case 'e':
            return keyE = b;
        case ' ':
            return keySpace = b;
        default:
            return b;
    }
}

void ofApp::updateControls() {
    switch (gameMode) {
        case GameMode::CAM:
            if (keyW) cam.moveForward();
            if (keyS) cam.moveBack();
            if (keyA) cam.moveLeft();
            if (keyD) cam.moveRight();
            if (keyQ) cam.moveDown();
            if (keyE) cam.moveUp();
            if (keySpace) cam.reset();
            break;
        case GameMode::DEPTH1_POS:
            moveProjector(frameProjector[0]);
            break;
        case GameMode::DEPTH1_ROT:
            rotateProjector(frameProjector[0]);
            break;
        case GameMode::DEPTH2_POS:
            moveProjector(frameProjector[1]);
            break;
        case GameMode::DEPTH2_ROT:
            rotateProjector(frameProjector[1]);
            break;
        case GameMode::OFF:
            break;
    }
}

void ofApp::moveProjector(FrameProjector& p) {
    if (keyW) p.moveForward(cam);
    if (keyS) p.moveBack(cam);
    if (keyA) p.moveLeft(cam);
    if (keyD) p.moveRight(cam);
    if (keyQ) p.moveDown(cam);
    if (keyE) p.moveUp(cam);
    if (keySpace) p.reset();
}

void ofApp::rotateProjector(FrameProjector& p) {
    if (keyW) p.rollUp();
    if (keyS) p.rollDown();
    if (keyA) p.pitchUp();
    if (keyD) p.pitchDown();
    if (keyQ) p.yawUp();
    if (keyE) p.yawDown();
    if (keySpace) p.reset();
}

// ~ ~ ~ Osc ~ ~ ~

void ofApp::oscSetup() {
    oscReceiver.setup(receivePort);
}

void ofApp::oscUpdate() {
    while (oscReceiver.hasWaitingMessages()) {
        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);
        oscEvent(msg);
    }
}

// Blobs hold little-endian float32s.
static float asFloat(const char* bytes) {
    uint32_t bits = uint32_t(uint8_t(bytes[0]))
                  | (uint32_t(uint8_t(bytes[1])) << 8)
                  | (uint32_t(uint8_t(bytes[2])) << 16)
                  | (uint32_t(uint8_t(bytes[3])) << 24);
    float f;
    memcpy(&f, &bits, sizeof(f));
    return f;
}

// Args: hostname, session id, stroke index, color blob (r, g, b floats, 0-255),
// points blob (x, y, z floats), timestamp.
void ofApp::oscEvent(const ofxOscMessage& msg) {
    string address = msg.getAddress();
    if ((address != "/contour" && address != "/scanline") || msg.getTypeString() != "ssibbi") return;

    string hostname = msg.getArgAsString(0);
    int index = msg.getArgAsInt32(2);
    ofBuffer readColorBytes = msg.getArgAsBlob(3);
    ofBuffer readLNPointsBytes = msg.getArgAsBlob(4);
    if (readColorBytes.size() < 12) return;

    float r = asFloat(readColorBytes.getData());
    float g = asFloat(readColorBytes.getData() + 4);
    float b = asFloat(readColorBytes.getData() + 8);
    ofColor c(255);
    if (!std::isnan(r) && !std::isnan(g) && !std::isnan(b)) {
        c = ofColor(ofClamp(r, 0, 255), ofClamp(g, 0, 255), ofClamp(b, 0, 255));
    }

    vector<glm::vec3> points;
    const char* pointBytes = readLNPointsBytes.getData();
    for (size_t i = 0; i + 12 <= readLNPointsBytes.size(); i += 12) {
        float x = asFloat(pointBytes + i);
        float y = asFloat(pointBytes + i + 4);
        float z = asFloat(pointBytes + i + 8) * 10;
        if (!std::isnan(x) && !std::isnan(y)) {
            points.push_back(glm::vec3(x, y, z));
        }
    }

    // A known hostname goes back to its projector; a new one claims the first unclaimed projector.
    for (FrameProjector& p : frameProjector) {
        if (p.hostname == hostname) {
            p.createStroke(index, c, points, globalLifespan);
            p.hostnameConfirmed = true;
            return;
        }
    }

    for (FrameProjector& p : frameProjector) {
        if (!p.hostnameConfirmed) {
            p.hostname = hostname;
            p.createStroke(index, c, points, globalLifespan);
            p.hostnameConfirmed = true;
            return;
        }
    }
}

// ~ ~ ~ Websocket ~ ~ ~

// wsUrl looks like ws://host:port/path.
void ofApp::wsSetup() {
    ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();

    string address = wsUrl;
    options.bUseSSL = ofIsStringInString(address, "wss://");
    size_t scheme = address.find("://");
    if (scheme != string::npos) address = address.substr(scheme + 3);

    options.path = "/";
    size_t slash = address.find('/');
    if (slash != string::npos) {
        options.path = address.substr(slash);
        address = address.substr(0, slash);
    }

    options.port = options.bUseSSL ? 443 : 80;
    size_t colon = address.rfind(':');
    if (colon != string::npos) {
        options.port = ofToInt(address.substr(colon + 1));
        address = address.substr(0, colon);
    }

    options.host = address;
    options.reconnect = false;

    wsc.addListener(this);
    wsc.connect(options);
    wsNow = ofGetElapsedTimeMillis();
}

void ofApp::wsUpdate() {
    if (ofGetElapsedTimeMillis() > wsNow + wsInterval) {
        if (wsc.isConnected()) wsc.send("clientRequestFrame");
        wsNow = ofGetElapsedTimeMillis();
    }

    vector<ofJson> messages;
    {
        std::lock_guard<std::mutex> lock(wsMutex);
        messages.swap(wsMessages);
    }
    for (const ofJson& json : messages) {
        webSocketEvent(json);
    }
}

void ofApp::onConnect(ofxLibwebsockets::Event& args) {
    ofLogNotice("Websocket") << "Connected to " << wsUrl;
}

void ofApp::onClose(ofxLibwebsockets::Event& args) {
    ofLogNotice("Websocket") << "Disconnected from " << wsUrl;
}

void ofApp::onIdle(ofxLibwebsockets::Event& args) {
}

// The client has already parsed the JSON on its thread; the strokes are built on the main thread.
void ofApp::onMessage(ofxLibwebsockets::Event& args) {
    std::lock_guard<std::mutex> lock(wsMutex);
    wsMessages.push_back(args.json);
}

// A frame is an array of strokes: { "color": [r, g, b] (0-1), "points": [{ "co": [x, y, z] }, ...] }
void ofApp::webSocketEvent(const ofJson& json) {
    try {
        if (!json.is_array()) throw std::runtime_error("expected an array of strokes");

        for (size_t i = 0; i < json.size(); i++) {
            const ofJson& jsonStroke = json.at(i);

            const ofJson& color = jsonStroke.at("color");
            int r = int(255.0 * color.at(0).get<float>());
            int g = int(255.0 * color.at(1).get<float>());
            int b = int(255.0 * color.at(2).get<float>());
            ofColor c(ofClamp(r, 0, 255), ofClamp(g, 0, 255), ofClamp(b, 0, 255));

            vector<glm::vec3> points;
            for (const ofJson& jsonLNPoint : jsonStroke.at("points")) {
                const ofJson& co = jsonLNPoint.at("co");
                float x = co.at(0).get<float>();
                float y = co.at(1).get<float>();
                float z = co.at(2).get<float>();
                points.push_back(glm::vec3(x, y, -z) * wsGlobalScale + wsGlobalOffset);
            }

            frameProjector[targetProjector].addStroke(Stroke(i, c, points, globalLifespan));
        }
    } catch (const std::exception& e) {
        ofLogError("Websocket") << "Error receiving ws message: " << e.what();
    }
}

// ~ ~ ~ SoundOut ~ ~ ~

// Rebuilds the scope's waveform from the strokes on screen, once the first
// projector has enough strokes; otherwise the last waveform keeps playing.
void ofApp::soundOutUpdate() {
    xy1.clearWaves();
    for (const FrameProjector& p : frameProjector) {
        p.frame.drawScope(xy1);
    }

    if (int(frameProjector[0].frame.strokes.size()) > activityThreshold) {
        xy1.amp(xy1amp);
        xy1.buildWaves();
    }
}
