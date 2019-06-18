import peasy.PeasyCam;

PeasyCam cam;

float dotSize = 1;
int globalAlpha = 63;
int globalLifespan = 1000;
ArrayList<Stroke> strokesBuffer;
boolean clicked = false;
boolean isDrawing = false;
int fps = 12;
int realFps = 60;
int markTime = 0;
Frame frame;
Settings settings;

void setup() {
  fullScreen(P3D);
  settings = new Settings("settings.txt");
  noCursor();
  frameRate(realFps);
  cam = new PeasyCam(this, 400);
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

  //surface.setTitle("" + frameRate);
}
