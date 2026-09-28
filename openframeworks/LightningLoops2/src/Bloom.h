#pragma once
#include "ofMain.h"

// Glow effect in place of PixelFlow's DwFilter.bloom: the scene is downsampled
// into a pyramid of blurred layers, which are weighted and added back on top.
class Bloom {

    public:
        static const int LAYERS = 5;

        void setup(int width, int height);
        void apply(const ofTexture& src);
        void draw(float x, float y, float w, float h);

        float mult = 3.5; // 0.0-10.0
        float radius = 0.5; // 0.0-1.0, how much the widest layers count

    private:
        void blurPass(ofFbo& src, ofFbo& dst, const glm::vec2& direction);

        ofShader blurShader;
        ofShader mergeShader;
        ofFbo layers[LAYERS];
        ofFbo temp[LAYERS];
        ofFbo result;

};
