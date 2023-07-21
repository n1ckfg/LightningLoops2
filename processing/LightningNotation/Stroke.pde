class Stroke {

  ArrayList<PVector> points;
  int index;
  int timestamp;
  int lifespan = 1000;
  color col;
  
  Stroke() {
    index = 0;
    col = color(255);
    points = new ArrayList<PVector>();
    timestamp = millis();
  }
  
  Stroke(int idx) {
    index = idx;
    col = color(255);
    points = new ArrayList<PVector>();
    timestamp = millis();
  }
  
  Stroke(int idx, ArrayList<PVector> pts) {
    index = idx;
    col = color(255);
    points = pts;
    timestamp = millis();
  }

  Stroke(int idx, color c, ArrayList<PVector> pts) {
    index = idx;
    col = c;
    points = pts;
    timestamp = millis();
  }
  
  Stroke(int idx, color c, ArrayList<PVector> pts, int life) {
    index = idx;
    col = c;
    points = pts;
    timestamp = millis();
    lifespan = int(random(life/10, life*10));
  }
  
  void draw() {
    tex.noFill();
    tex.stroke(col, globalAlpha);
    tex.strokeWeight(dotSize);
    tex.beginShape();
    
    int len = points.size();
    if (len > 1) {
      PVector p = points.get(len-1);
      PVector pp = points.get(len-2);
      xy1.line(p.x, p.y, pp.x, pp.y);
    }
    
    for (PVector p : points) {
      tex.vertex(p.x, p.y, p.z);
    }
    tex.endShape();
  }
}
