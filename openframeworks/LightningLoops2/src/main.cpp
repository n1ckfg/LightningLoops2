#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {

    // setup the GL context
#ifdef TARGET_OPENGLES
    ofGLESWindowSettings settings;
    settings.glesVersion = 2;
#else
    ofGLFWWindowSettings settings;
    settings.setGLVersion(2, 1);
#endif
    settings.windowMode = OF_FULLSCREEN;
    ofCreateWindow(settings);

    // this kicks off the running of my app
    ofRunApp(new ofApp());

}
