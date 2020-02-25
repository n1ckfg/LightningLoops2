// https://github.com/jrc03c/queasycam
// https://processing.org/reference/camMatera_.html

class FrameProjector {
  
  Frame frame;
  ArrayList<Stroke> strokesBuffer;
  String hostName;
  
  boolean controllable;
  boolean enablePosition = true;
  boolean enableRotation = true;
  boolean enableMouse = true;
  float speed;
  float sensitivity;
  PVector pos = new PVector(0,0,0);
  float pan;
  float tilt;
  PVector velocity;
  float friction;

  PVector up;
  PVector right;
  PVector forward;
  //HashMap<Character, Boolean> keys;

  PGraphics3D p3d;
  PMatrix3D proj, camMat, modvw, modvwInv, screen2Model;    
  String displayText = "";
  PFont font;
  int fontSize = 12;
  
  FrameProjector() {
    strokesBuffer = new ArrayList<Stroke>();
    controllable = true;
    speed = 3f;
    sensitivity = 2f;
    reset();
    friction = 0.75f;  
    initMats(); 
  }
  
  void newFrame() {
    frame = new Frame(strokesBuffer);
  }
  
  void initMats() {
    p3d = (PGraphics3D) g;
    //proj = new PMatrix3D();
    camMat = new PMatrix3D();
    //modvw = new PMatrix3D();
    modvwInv = new PMatrix3D();
    screen2Model = new PMatrix3D();    
  }

 void calcPanTilt() {
    //pan += map((width-rotMouse.x) - (width-pRotMouse.x), 0, width, 0, TWO_PI) * sensitivity;
    //tilt += map((height-rotMouse.y) - (height-pRotMouse.y), 0, height, 0, PI) * sensitivity;
    
    tilt = clamp(tilt, -PI/2.01f, PI/2.01f);  
    if (tilt == PI/2) tilt += 0.001f;
  }
  
  void calcDirections() {
    forward = new PVector(cos(pan), tan(tilt), sin(pan));
    forward.normalize();
    right = new PVector(cos(pan - PI/2), 0, sin(pan - PI/2));
    right.normalize();
    up = right.cross(forward);
    up.normalize();
  }
  
  void updateRotation() {
    if (enableRotation) {
      calcPanTilt();
    }
    calcDirections();
  }
  
  void updatePosition() {
    if (enablePosition) {  
      velocity.mult(friction);
      pos.add(velocity);
    }
  }
  
  void update() {
    up = cam.up;
    right = cam.right;
    forward = cam.forward;
      
    if (!controllable) return;
    updateRotation();
    updatePosition();
  }
  
  void draw(){
    //tex.camera(pos.x, pos.y, pos.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);
    //drawText();
    tex.pushMatrix();
    tex.translate(pos.x, pos.y, pos.z);
    frame.draw();
    tex.popMatrix();
  }
  
  void run() {
    update();
    draw();
  }

  float clamp(float x, float min, float max){
    if (x > max) return max;
    if (x < min) return min;
    return x;
  }
  
  PVector getForward(){
    return forward;
  }
  
  PVector getUp(){
    return up;
  }
  
  PVector getRight(){
    return right;
  }
  
  void moveForward() {
    velocity.add(PVector.mult(forward, speed));
  }
  
  void moveBack() {
    velocity.sub(PVector.mult(forward, speed));
  }
  
  void moveLeft() {
    velocity.sub(PVector.mult(right, speed));
  }
  
  void moveRight() {
    velocity.add(PVector.mult(right, speed));
  }
  
  void moveUp() {
    velocity.sub(PVector.mult(up, speed));
  }
  
  void moveDown() {
    velocity.add(PVector.mult(up, speed));
  }
  
  void reset() {
    velocity = new PVector(0, 0, 0);
    pan = 0f;
    tilt = 0f;
    pos = new PVector(0, 0, 0);
  }
  
}
