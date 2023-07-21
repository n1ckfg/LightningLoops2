import com.thomasdiewald.pixelflow.java.DwPixelFlow;
import com.thomasdiewald.pixelflow.java.imageprocessing.filter.DwFilter;
import processing.opengl.PGraphics2D;
import processing.opengl.PGraphics3D;

DwPixelFlow context;
DwFilter filter;
PGraphics3D tex;
PMatrix mat_scene;

float bloomMult = 3.5;
float bloomRadius = 0.5;
int texWidth = 960;
int texHeight = 720;

// This goes immediately after size().
void bloomSetup() {  
  mat_scene = getMatrix();
  tex = (PGraphics3D) createGraphics(texWidth, texHeight, P3D);
  context = new DwPixelFlow(this);
  filter = new DwFilter(context);
  //filter.bloom.setBlurLayers(10);
  filter.bloom.param.mult = bloomMult; // 0.0-10.0
  filter.bloom.param.radius = bloomRadius; // 0.0-1.0
}

// For a simple scene, just put this at the end of the draw loop.
void bloomDraw() {
  filter.bloom.apply(tex);
  int w = (height/3)*4;
  image(tex, (width - w)/2, 0, w, height);
}

// Or, for a more complex scene, this goes at the beginning of the draw loop...
void bloomMatrixStart() {
  pushMatrix();
  float fov = PI/3.0;
  float cameraZ = (tex.height/2.0) / tan(fov/2.0);
  tex.perspective(fov, float(tex.width)/float(tex.height), cameraZ/100.0, cameraZ*100.0);  
}

// ...and this goes at the end.
void bloomMatrixEnd() {
  setMatrix(mat_scene);
  bloomDraw();
  popMatrix();
}
