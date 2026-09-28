#include "XYScope.h"

XYScope::~XYScope() {
    close();
}

// XYscope sizes its wavetables to the audio buffer.
bool XYScope::setup(int sampleRate, int bufferSize) {
    waveSize = bufferSize;
    waveX.assign(waveSize, 0);
    waveY.assign(waveSize, 0);

    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    return soundStream.setup(settings);
}

void XYScope::close() {
    soundStream.close();
}

// Line coordinates are normalized against this size.
void XYScope::setCanvasSize(float w, float h) {
    xyWidth = w;
    xyHeight = h;
}

void XYScope::clearWaves() {
    shapes.clear();
}

void XYScope::line(float x1, float y1, float x2, float y2) {
    shapes.push_back({
        glm::vec2(x1 / xyWidth, y1 / xyHeight),
        glm::vec2(x2 / xyWidth, y2 / xyHeight)
    });
}

// Traces every shape as one path, giving each segment a share of the path's
// samples in proportion to its length, then resamples the path to the wavetable.
void XYScope::buildWaves() {
    size_t totalPoints = 0;
    double totalDist = 0;
    for (const auto& shape : shapes) {
        totalPoints += shape.size();
        for (size_t j = 1; j < shape.size(); j++) {
            totalDist += glm::distance(shape[j-1], shape[j]);
        }
    }
    double pathSize = totalPoints * steps;

    vector<glm::vec2> path;
    for (const auto& shape : shapes) {
        for (size_t j = 1; j < shape.size(); j++) {
            const glm::vec2& p1 = shape[j-1];
            const glm::vec2& p2 = shape[j];
            double lineDist = glm::distance(p1, p2);
            int sections = round(1 + (totalDist > 0 ? lineDist / totalDist : 0) * pathSize);
            for (int k = 0; k <= sections; k++) {
                path.push_back(glm::mix(p1, p2, float(k) / sections));
            }
        }
    }

    vector<float> newX;
    vector<float> newY;
    if (!path.empty()) {
        newX.resize(waveSize);
        newY.resize(waveSize);
        for (int i = 0; i < waveSize; i++) {
            const glm::vec2& p = path[size_t(i) * path.size() / waveSize];
            newX[i] = p.x * 2 - 1;
            newY[i] = p.y * -2 + 1;
        }
    }

    std::lock_guard<std::mutex> lock(waveMutex);
    waveX.swap(newX);
    waveY.swap(newY);
}

void XYScope::amp(float newAmp) {
    ampValue = ofClamp(newAmp, 0, 1);
}

// Linear interpolation between wavetable samples, wrapping at the end.
static float waveValue(const vector<float>& wave, double at) {
    if (wave.empty()) return 0;
    double whichSample = wave.size() * at;
    size_t lowSamp = size_t(whichSample) % wave.size();
    size_t hiSamp = (lowSamp + 1) % wave.size();
    float rem = whichSample - floor(whichSample);
    return wave[lowSamp] + rem * (wave[hiSamp] - wave[lowSamp]);
}

void XYScope::audioOut(ofSoundBuffer& buffer) {
    double phaseStep = freq / buffer.getSampleRate();
    float a = ampValue;
    size_t channels = buffer.getNumChannels();

    std::lock_guard<std::mutex> lock(waveMutex);
    for (size_t i = 0; i < buffer.getNumFrames(); i++) {
        buffer[i * channels] = waveValue(waveX, phase) * a;
        if (channels > 1) buffer[i * channels + 1] = waveValue(waveY, phase) * a;
        phase = fmod(phase + phaseStep, 1.0);
    }
}
