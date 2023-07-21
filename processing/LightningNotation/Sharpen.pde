PShader shader_sharpen;

void sharpenSetup() {
  shader_sharpen = loadShader("shaders/sharpen.glsl");
  shader_sharpen.set("iResolution", float(width), float(height), 1.0);
}

void sharpenDraw() {
  shader_sharpen.set("tex0", tex);
  tex.filter(shader_sharpen);
}
