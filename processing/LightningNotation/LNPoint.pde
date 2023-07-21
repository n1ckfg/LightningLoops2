class LNPoint {
  
  PVector pos;
  PVector col;
  
  LNPoint() {
    pos = new PVector(0,0,0);
    col = new PVector(255, 255, 255);
  }
  
  LNPoint(PVector p) {
    pos = p;
    col = new PVector(255, 255, 255);
  }
  
  LNPoint(PVector p, PVector c) {
    pos = p;
    col = c;
  }
  
}
