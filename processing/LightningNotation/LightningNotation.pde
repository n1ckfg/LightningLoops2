float dotSize = 0.5;
int globalAlpha = 63;
int globalLifespan = 1000;
ArrayList<Stroke> strokesBuffer;
boolean clicked = false;
boolean isDrawing = false;
int fps = 12;
int realFps = 60;
int markTime = 0;
FrameProjector frameProjector1, frameProjector2;
Settings settings;
Cam cam;
float camShake = 5.0;
float camStable = 0.2;
GameMode gameMode;

PVector posOrig;
PVector poiOrig;

void setup() {
  size(900, 600, P3D);
  cam = new Cam();
  frameProjector1 = new FrameProjector();
  frameProjector2 = new FrameProjector();

  settings = new Settings("settings.txt");
  gameMode = GameMode.OFF;
  
  noCursor();
  frameRate(realFps);
  //cam = new PeasyCam(this, 400);
  frameProjector1.newFrame();
  frameProjector2.newFrame();
  oscSetup();
  if (useWebsockets) wsSetup();
  bloomSetup();
  soundOutSetup();
  sharpenSetup();
  //opticalFlowSetup();
  
  fps = int((1.0/float(fps)) * 1000);
  
  //cam.pos = new PVector(0,0,0);
  //cam.poi = new PVector(0,0,0);

  posOrig = cam.pos.copy();
  poiOrig = cam.poi.copy();
}

void draw() { 
  background(0);
  if (useWebsockets) wsUpdate();
  
  tex.beginDraw();
  
  updateControls();
  cam.controllable = gameMode == GameMode.CAM;
  cam.run();
  //tex.setMatrix(getMatrix());  // used with camera
  tex.background(0);

  int time = millis();
  if (time > markTime + fps) {
    markTime = time;
    frameProjector1.newFrame();
    frameProjector2.newFrame();
  }
  
  frameProjector1.run();
  frameProjector2.run();
  
  sharpenDraw();
  //opticalFlowDraw();
  
  tex.endDraw();
  
  soundOutDraw();
  
  
  bloomDraw();

  surface.setTitle("" + frameRate);
}
