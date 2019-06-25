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
Cam cam;
float camShake = 5.0;
float camStable = 0.2;

PVector posOrig;
PVector poiOrig;

void setup() {
  fullScreen(P3D);
  settings = new Settings("settings.txt");
  noCursor();
  frameRate(realFps);
  //cam = new PeasyCam(this, 400);
  cam = new Cam();
  strokesBuffer = new ArrayList<Stroke>();
  frame = new Frame(strokesBuffer);
  oscSetup();
  bloomSetup();
  soundOutSetup();
  sharpenSetup();
  //opticalFlowSetup();
  
  fps = int((1.0/float(fps)) * 1000);
  
  cam.poi.y -= 200;
  cam.pos.z += -1200;
  cam.poi.z += -1200;

  posOrig = cam.pos.copy();
  poiOrig = cam.poi.copy();
}

void draw() { 
  background(0);
  
  tex.beginDraw();
  
  cam.poi.x += random(camShake) - random(camShake);
  cam.poi.y += random(camShake) - random(camShake);
  cam.poi.z += random(camShake) - random(camShake);

  cam.pos.x += random(camShake) - random(camShake);
  cam.pos.y += random(camShake) - random(camShake);
  cam.pos.z += random(camShake) - random(camShake);
  
  cam.pos = PVector.lerp(cam.pos, posOrig, camStable);
  cam.poi = PVector.lerp(cam.poi, poiOrig, camStable);
  
  cam.run();
  //tex.setMatrix(getMatrix());  // used with camera
  tex.background(0);

  int time = millis();
  if (time > markTime + fps) {
    markTime = time;
    frame = new Frame(strokesBuffer);
  }
  
  frame.draw();
  
  sharpenDraw();
  //opticalFlowDraw();
  
  tex.endDraw();
  
  soundOutDraw();
  
  
  bloomDraw();

  //surface.setTitle("" + frameRate);
}
