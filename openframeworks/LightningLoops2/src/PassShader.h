#pragma once
#include "ofMain.h"

// Loads a full-screen pass fragment shader from bin/data/shaders. The files are
// written in the GLSL 1.20 / GLSL ES 1.00 subset (varying, texture2D, gl_FragColor)
// and read `varying vec2 vTexCoord`; this adds the version header and vertex
// shader the current renderer needs, so they run on GL2, GL3 and GLES.
inline bool loadPassShader(ofShader& shader, const string& fragPath) {
    string header;
    string vertBody;
    string fragDefines;

    if (!ofIsGLProgrammableRenderer()) {
        header = "#version 120\n";
        vertBody =
            "varying vec2 vTexCoord;\n"
            "void main() {\n"
            "    vTexCoord = gl_MultiTexCoord0.xy;\n"
            "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
            "}\n";
    } else {
        int major = ofGetGLRenderer()->getGLVersionMajor();
        int minor = ofGetGLRenderer()->getGLVersionMinor();
#ifdef TARGET_OPENGLES
        bool modern = major >= 3;
        header = modern ? "#version 300 es\n" : "#version 100\n";
        header += "#ifdef GL_FRAGMENT_PRECISION_HIGH\nprecision highp float;\n#else\nprecision mediump float;\n#endif\n";
#else
        bool modern = true;
        header = "#version " + ofToString(ofGLSLVersionFromGL(major, minor)) + "\n";
#endif
        if (modern) {
            header += "#define texture2D texture\n";
            fragDefines = "#define varying in\nout vec4 fragColor;\n#define gl_FragColor fragColor\n";
        }
        vertBody = string(modern ? "#define attribute in\n#define varying out\n" : "") +
            "attribute vec4 position;\n"
            "attribute vec2 texcoord;\n"
            "uniform mat4 modelViewProjectionMatrix;\n"
            "varying vec2 vTexCoord;\n"
            "void main() {\n"
            "    vTexCoord = texcoord;\n"
            "    gl_Position = modelViewProjectionMatrix * position;\n"
            "}\n";
    }

    ofBuffer frag = ofBufferFromFile(fragPath);
    if (frag.size() == 0) {
        ofLogError("loadPassShader") << "Couldn't load " << fragPath;
        return false;
    }

    shader.setupShaderFromSource(GL_VERTEX_SHADER, header + vertBody);
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, header + fragDefines + frag.getText());
    if (ofIsGLProgrammableRenderer()) shader.bindDefaults();
    return shader.linkProgram();
}
