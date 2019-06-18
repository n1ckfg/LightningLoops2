import peasy.PeasyCam;

PeasyCam cam;

float dotSize = 1;
int globalAlpha = 63;
ArrayList<Stroke> strokesBuffer;
boolean clicked = false;
boolean isDrawing = false;
int fps = 12;
int markTime = 0;
Frame frame;

void setup() {
  fullScreen(P3D);
  noCursor();
  frameRate(60);
  cam = new PeasyCam(this, 200);
  strokesBuffer = new ArrayList<Stroke>();
  frame = new Frame(strokesBuffer);
  oscSetup();
  bloomSetup();
  soundOutSetup();
  //opticalFlowSetup();
  fps = int((1.0/float(fps)) * 1000);
}

void draw() {
  background(0);
  
  bloomMatrixStart();
  tex.beginDraw();
  tex.setMatrix(getMatrix());
  tex.background(0);

  int time = millis();
  if (time > markTime + fps) {
    markTime = time;
    frame = new Frame(strokesBuffer);
  }
  
  frame.draw();
  
  tex.endDraw();
  soundOutDraw();
  
  //opticalFlowDraw();
  bloomMatrixEnd();

  surface.setTitle("" + frameRate);
}
