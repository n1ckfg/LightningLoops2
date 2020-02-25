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
  PVector pos;
  Quaternion q;
  float rotDelta = 0.05;
  float pan;
  float tilt;
  PVector velocity;
  float friction;

  PVector up;
  PVector right;
  PVector forward;
  //HashMap<Character, Boolean> keys;

  PGraphics3D p3d;
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
    
    pos = new PVector();
    q = new Quaternion();
  }
  
  void newFrame() {
    frame = new Frame(strokesBuffer);
  }
   
  void updateRotation() {
    if (enableRotation) {
      q.run();
    }
  }
  
  void updatePosition() {
    if (enablePosition) {  
      velocity.mult(friction);
      pos.add(velocity);
    }
  }
  
  void update() {
    if (!controllable) return;
    updatePosition();
    updateRotation();
  }
  
  void draw(){
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
    velocity.add(PVector.mult(cam.forward, speed));
  }
  
  void moveBack() {
    velocity.sub(PVector.mult(cam.forward, speed));
  }
  
  void moveLeft() {
    velocity.sub(PVector.mult(cam.right, speed));
  }
  
  void moveRight() {
    velocity.add(PVector.mult(cam.right, speed));
  }
  
  void moveUp() {
    velocity.sub(PVector.mult(cam.up, speed));
  }
  
  void moveDown() {
    velocity.add(PVector.mult(cam.up, speed));
  }
  
  void rollUp() {
    q.rotateAxisX(rotDelta);
  }
  
  void rollDown() {
    q.rotateAxisX(-rotDelta);
  }
  
  void pitchUp() {
    q.rotateAxisY(rotDelta);
  }
  
  void pitchDown() {
    q.rotateAxisY(-rotDelta);
  }
  
  void yawUp() {
    q.rotateAxisZ(rotDelta);
  }
  
  void yawDown() {
    q.rotateAxisZ(-rotDelta);  
  }
   
  void reset() {
    velocity = new PVector();
    pan = 0f;
    tilt = 0f;
    pos = new PVector();
    q = new Quaternion();
  }
  
}
