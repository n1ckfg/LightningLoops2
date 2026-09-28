#include "Sharpen.h"
#include "PassShader.h"

void Sharpen::setup(int width, int height) {
    loadPassShader(shader, "shaders/sharpen.frag");
    buffer.allocate(width, height, GL_RGBA);
}

void Sharpen::apply(ofFbo& fbo) {
    ofPushStyle();
    ofDisableAlphaBlending();
    ofSetColor(255);

    buffer.begin();
    shader.begin();
    shader.setUniformTexture("tex0", fbo.getTexture(), 0);
    shader.setUniform2f("texelSize", 1.0 / fbo.getWidth(), 1.0 / fbo.getHeight());
    fbo.draw(0, 0, buffer.getWidth(), buffer.getHeight());
    shader.end();
    buffer.end();

    fbo.begin();
    buffer.draw(0, 0, fbo.getWidth(), fbo.getHeight());
    fbo.end();

    ofPopStyle();
}
