#include "Bloom.h"
#include "PassShader.h"

void Bloom::setup(int width, int height) {
    loadPassShader(blurShader, "shaders/bloom_blur.frag");
    loadPassShader(mergeShader, "shaders/bloom_merge.frag");

    result.allocate(width, height, GL_RGBA);

    int w = width;
    int h = height;
    for (int i = 0; i < LAYERS; i++) {
        w = max(w / 2, 1);
        h = max(h / 2, 1);
        layers[i].allocate(w, h, GL_RGBA);
        temp[i].allocate(w, h, GL_RGBA);
    }
}

void Bloom::blurPass(ofFbo& src, ofFbo& dst, const glm::vec2& direction) {
    dst.begin();
    blurShader.begin();
    blurShader.setUniformTexture("tex0", src.getTexture(), 0);
    blurShader.setUniform2f("direction", direction);
    src.draw(0, 0);
    blurShader.end();
    dst.end();
}

void Bloom::apply(const ofTexture& src) {
    ofPushStyle();
    ofDisableAlphaBlending();
    ofSetColor(255);

    // Each layer is half the size of the one before, so the blur widens with depth.
    for (int i = 0; i < LAYERS; i++) {
        const ofTexture& prev = i == 0 ? src : layers[i-1].getTexture();
        layers[i].begin();
        prev.draw(0, 0, layers[i].getWidth(), layers[i].getHeight());
        layers[i].end();

        blurPass(layers[i], temp[i], glm::vec2(1.0 / layers[i].getWidth(), 0));
        blurPass(temp[i], layers[i], glm::vec2(0, 1.0 / layers[i].getHeight()));
    }

    // radius 0 favours the finest layer, 1 the widest, 0.5 weights them evenly.
    float weights[LAYERS];
    float step = 1.0 / LAYERS;
    for (int i = 0; i < LAYERS; i++) {
        float fac = 1.0 - step * i;
        weights[i] = mult * ofLerp(fac, 1.0 + step - fac, radius);
    }

    result.begin();
    mergeShader.begin();
    mergeShader.setUniformTexture("tex0", src, 0);
    for (int i = 0; i < LAYERS; i++) {
        mergeShader.setUniformTexture("blur" + ofToString(i), layers[i].getTexture(), i + 1);
    }
    mergeShader.setUniform1fv("weights", weights, LAYERS);
    src.draw(0, 0, result.getWidth(), result.getHeight());
    mergeShader.end();
    result.end();

    ofPopStyle();
}

void Bloom::draw(float x, float y, float w, float h) {
    result.draw(x, y, w, h);
}
