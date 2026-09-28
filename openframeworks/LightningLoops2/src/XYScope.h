#pragma once
#include "ofMain.h"

// Draws lines on an oscilloscope or laser by playing them as audio: the left
// channel is x and the right is y. Replaces the XYscope Processing library
// (https://github.com/ffd8/xyscope), following its buildWaves() algorithm.
class XYScope : public ofBaseSoundOutput {

    public:
        ~XYScope();

        bool setup(int sampleRate = 44100, int bufferSize = 512);
        void close();
        void setCanvasSize(float w, float h);

        void clearWaves();
        void line(float x1, float y1, float x2, float y2);
        void buildWaves();
        void amp(float newAmp);

        void audioOut(ofSoundBuffer& buffer) override;

    private:
        vector<vector<glm::vec2>> shapes;
        float xyWidth = 1;
        float xyHeight = 1;
        int waveSize = 512;
        int steps = 24;
        float freq = 50;

        std::mutex waveMutex;
        vector<float> waveX;
        vector<float> waveY;
        std::atomic<float> ampValue{1};
        double phase = 0;

        ofSoundStream soundStream;

};
